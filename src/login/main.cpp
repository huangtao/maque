/*
* Copyright (C) 2018 HANGHZOU YUNPAI NETWORK TECH CO.
* huangtao117@yeah.net QQ:409577078
*/

#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <csignal>

#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
//#include <boost/beast/core.hpp>
//#include <boost/beast/http.hpp>
//#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/signal_set.hpp>
#include <google/protobuf/stubs/common.h>
//#include <mysql.h>

//#include "YPErrors.h"
#include "Banner.h"
#include "Config.h"
#include "Log.h"
//#include "SessionManager.h"
//#include "SslContext.h"
//#include "Util.h"
//#include "BigNumber.h"

using namespace boost::program_options;
namespace fs = boost::filesystem;
using tcp = boost::asio::ip::tcp;
//namespace http = boost::beast::http;

using namespace yunpai;

#ifndef _YUNPAI_GATE_CONFIG
#define _YUNPAI_GATE_CONFIG "GateServer.conf"
#endif

#if defined(BOOST_WINDOWS)
#include "WinService.h"
char serviceName[] = "GateServer";
char serviceLongName[] = "yunpai gate service";
char serviceDescription[] = "yunpai gate service";
/*
* -1 - not in service mode
*  0 - stopped
*  1 - running
*  2 - paused
*/
int g_service_status = -1;

void ServiceStatusWatcher(std::weak_ptr<boost::asio::deadline_timer> serviceStatusWatchTimerRef, std::weak_ptr<boost::asio::io_context> ioContextRef, boost::system::error_code const& error);
#endif // defined(BOOST_WINDOWS)

bool StartDB();
void StopDB();
void SignalHandler(std::weak_ptr<boost::asio::io_context> ioContextRef, boost::system::error_code const& error, int signalNumber);
variables_map GetConsoleArguments(int argc, char* argv[], fs::path& configFile, std::string& configService);

int main(int argc, char * argv[])
{
    signal(SIGABRT, &yunpai::AbortHandler);

    auto configFile = fs::absolute(_YUNPAI_GATE_CONFIG);
    std::string configService;
    auto vm = GetConsoleArguments(argc, argv, configFile, configService);
    // 如果是帮助和版本参数就退出
    if (vm.count("help") || vm.count("version"))
        return 0;

    //GOOGLE_PROTOBUF_VERIFY_VERSION;
    //std::shared_ptr<void> protobufHandle(nullptr, [](void*) {
    //    google::protobuf::ShutdownProtobufLibrary();
    //});

#if defined(BOOST_WINDOWS)
    if (configService.compare("install") == 0)
        return WinServiceInstall() ? 0 : 1;
    else if (configService.compare("uninstall") == 0)
        return WinServiceUninstall() ? 0 : 1;
    else if (configService.compare("run") == 0)
        return WinServiceRun() ? 0 : 1;
#endif

    std::string configError;
    if (!sConfigMgr->LoadInitial(configFile.generic_string(),
        std::vector<std::string>(argv, argv + argc),
        configError)) {
        printf("Error in config file: %s\n", configError.c_str());
        return 1;
    }

    // 初始化日志
    sLog->Initialize();

    // Banner
    yunpai::Banner::Show("authserver",
        [](char const* text) {
        LOG_INFO("%s", text);
    },
        []() {
        LOG_INFO("Using configuration file %s.", sConfigMgr->GetFilename().c_str());
        //YP_LOG_INFO("server.authserver", "Using SSL version: %s (library: %s)", OPENSSL_VERSION_TEXT, SSLeay_version(SSLEAY_VERSION));
    });

    //// 初始化OpenSSL's PRNG随机因子
    //BigNumber seed;
    //seed.SetRand(16 * 8);

    // PID file
    std::string pidFile = sConfigMgr->GetStringDefault("PidFile", "");
    if (!pidFile.empty()) {
        if (uint32_t pid = CreatePIDFile(pidFile)) {
            LOG_INFO("Daemon PID: %u\n", pid);
        } else {
            LOG_ERROR("Cannot create PID file %s.\n", pidFile.c_str());
            return 1;
        }
    }

    //if (!Authserver::SslContext::Initialize()) {
    //    YP_LOG_ERROR("server.authserver", "Failed to initialize SSL context.");
    //    return 1;
    //}

    //// 数据库类型
    //// MySQL or Lua(其他数据库通过脚本语言提供)
    //std::string dbType = sConfigMgr->GetStringDefault("Database.type", "MySQL");
    //bool use_inside_db = true;
    //if (boost::iequals(dbType, "MYSQL")) {
    //    // 连接数据库
    //    if (!StartDB()) {
    //        return 1;
    //    }
    //    std::shared_ptr<void> dbHandler(nullptr, [](void*) { StopDB(); });
    //} else if (boost::iequals(dbType, "LUA")) {
    //    use_inside_db = false;
    //} else {
    //    YP_LOG_ERROR("server.authserver", "Unsupport database type %s.\n", dbType.c_str());
    //    return 1;
    //}

    //
    // 网络
    //

    // asio
    std::shared_ptr<boost::asio::io_context> ioContext = std::make_shared<boost::asio::io_context>();

    // 开始侦听端口
    int32_t port = sConfigMgr->GetIntDefault("ServerPort", 6610);
    if (port < 0 || port > 0xFFFF) {
        YP_LOG_ERROR("server.authserver", "Specified authserver port (%d) out of allowed range (1-65535)", port);
        return 1;
    }
    std::string bingIp = sConfigMgr->GetStringDefault("BindIP", "0.0.0.0");
    if (!sSessionMgr.StartNetwork(*ioContext, bingIp, port)) {
        YP_LOG_ERROR("server.authserver", "Failed to initialize network");
        return 1;
    }
    std::shared_ptr<void> sSessionMgrHandle(nullptr, [](void*) { sSessionMgr.StopNetwork(); });

    // 设置信号处理
    boost::asio::signal_set signals(*ioContext, SIGINT, SIGTERM);
#if YUNPAI_PLATFORM == YUNPAI_PLATFROM_WINDOWS
    signals.add(SIGBREAK);
#endif
    signals.async_wait(std::bind(&SignalHandler, std::weak_ptr<boost::asio::io_context>(ioContext), std::placeholders::_1, std::placeholders::_2));

#if YUNPAI_PLATFORM == YUNPAI_PLATFORM_WINDOWS
    std::shared_ptr<boost::asio::deadline_timer> serviceStatusWatchTimer;
    if (g_service_status != -1) {
        serviceStatusWatchTimer = std::make_shared<boost::asio::deadline_timer>(*ioContext);
        serviceStatusWatchTimer->expires_from_now(boost::posix_time::seconds(1));
        serviceStatusWatchTimer->async_wait(std::bind(&ServiceStatusWatcher,
            std::weak_ptr<boost::asio::deadline_timer>(serviceStatusWatchTimer),
            std::weak_ptr<boost::asio::io_context>(ioContext),
            std::placeholders::_1));
    }
#endif

    // io service loop
    ioContext->run();

    if (use_inside_db) {
        //banExpiryCheckTimer->cancel();
        //dbPingTimer->cancel();
    }

    YP_LOG_INFO("server.authserver", "Halting precess...");

    signals.cancel();

    return 0;
}

/// 连接数据库
bool StartDB()
{
    mysql_library_init(-1, NULL, NULL);

    // 加载数据库

    return true;
}

/// 关闭数据库连接
void StopDB()
{
    mysql_library_end();
}

void SignalHandler(std::weak_ptr<boost::asio::io_context> ioContextRef, boost::system::error_code const& error, int signalNumber)
{
    if (!error) {
        if (std::shared_ptr<boost::asio::io_context> ioContext = ioContextRef.lock())
            ioContext->stop();
    }
}

#if YUNPAI_PLATFORM == YUNPAI_PLATFORM_WINDOWS
void ServiceStatusWatcher(std::weak_ptr<boost::asio::deadline_timer> serviceStatusWatchTimerRef, std::weak_ptr<boost::asio::io_context> ioContextRef, boost::system::error_code const& error)
{
    if (!error)
    {
        if (std::shared_ptr<boost::asio::io_context> ioContext = ioContextRef.lock())
        {
            if (g_service_status == 0)
            {
                ioContext->stop();
            } else if (std::shared_ptr<boost::asio::deadline_timer> serviceStatusWatchTimer = serviceStatusWatchTimerRef.lock())
            {
                serviceStatusWatchTimer->expires_from_now(boost::posix_time::seconds(1));
                serviceStatusWatchTimer->async_wait(std::bind(&ServiceStatusWatcher, serviceStatusWatchTimerRef, ioContext, std::placeholders::_1));
            }
        }
    }
}
#endif

variables_map GetConsoleArguments(int argc, char* argv[], fs::path& configFile, std::string& configService)
{
    (void)configService;

    // 命令行
    options_description generic("Generic options");
    generic.add_options()
        ("version,v", "print version string")
        ("help,h", "produce help message")
        ("config,c", value<fs::path>(&configFile)->default_value(fs::absolute(_YUNPAI_AUTH_CONFIG)),
            "name of a file of a configuration.")
        ;

#if YUNPAI_PLATFORM == YUNPAI_PLATFORM_WINDOWS
    // 加服务命令行
    options_description win("Windows platform options");
    win.add_options()
        ("service,s", value<std::string>(&configService)->default_value(""), "Windows service options: [install | uninstall]")
        ;
    generic.add(win);
#endif

    variables_map vm;
    try {
        store(command_line_parser(argc, argv).options(generic).allow_unregistered().run(), vm);
        notify(vm);
    } catch (std::exception& e) {
        std::cerr << e.what() << "\n";
    }

    if (vm.count("help")) {
        std::cout << generic << "\n";
    } else if (vm.count("version")) {
        std::cout << "version " << Authserver_VERSION_MAJOR << "." << Authserver_VERSION_MINOR << "\n";
    }

    return vm;
}
