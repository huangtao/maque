#include "server.h"
#include <glog/logging.h>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

using namespace boost::program_options;
namespace fs = boost::filesystem;

namespace maque {

Server::Server() {
}

int Server::Init(int argc, char * argv[]) {
    signal(SIGABRT, &yunpai::AbortHandler);

    LoadOptions(argc, argv);

    auto configFile = fs::absolute(_YUNPAI_GATE_CONFIG);
    std::string configService;
    auto vm = GetConsoleArguments(argc, argv, configFile, configService);
    // 如果是帮助和版本参数就退出
    if (vm.count("help") || vm.count("version"))
        return 0;

    _event_handler = event_handler;

    InitLog();

    LOG(INFO) << _options.ToString();

    int ret = InitTimer();

    return 0;
}

int Server::LoadOptions(int argc, char* argv[]) {
    std::string configName = argv[0] + ".conf";
    auto configFile = fs::absolute(configName);

    // 命令行
    options_description generic("Generic options");
    generic.add_options()
        ("version,v", "print version string")
        ("help,h", "produce help message")
        ("config,c", value<fs::path>(&configFile)->default_value(fs::absolute(configName)),
            "name of a file of a configuration.")
        ;

#if defined(BOOST_WINDOWS)
    std::string configService;
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
        // 硬编码版本号
        std::cout << "version 1.0.0\n";
    }
    // 如果是帮助和版本参数就退出
    if (vm.count("help") || vm.count("version"))
        return 0;

#if defined(BOOST_WINDOWS)
    if (configService.compare("install") == 0)
        return WinServiceInstall() ? 0 : 1;
    else if (configService.compare("uninstall") == 0)
        return WinServiceUninstall() ? 0 : 1;
    else if (configService.compare("run") == 0)
        return WinServiceRun() ? 0 : 1;
#endif

    // 继续
    return -1;
}

void Server::InitLog() {
    FLAGS_log_dir = _options._log_path;
    // glog默认输出到文件
    google::InitGoogleLoggint(_options._log_path);
    if (_options._log_std) {
        // 写到std
        FLAGS_alsologtostderr = true;
    }
    FLAGS_minloglevel = _options._log_level;            // 日志级别
    FLAGS_max_log_size = _options._log_file_size_MB;    // 日志文件最大10M
    FLAGS_log_prefix = true;
    FLAGS_colorlogtostderr = true;
}

int Server::InitTimer() {
    return 0;
}

}
