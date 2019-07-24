/*
 * Copyright (C) 2018 huangtao117@yeah.net QQ:409577078
 * Licensed under the MIT License (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of the License
 * at http://opensource.org/licenses/MIT Unless required by applicable law or
 * agreed to in writing, software distributed under the License is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#include "server.h"
#include "log.h"
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

using namespace boost::program_options;
namespace fs = boost::filesystem;

namespace maque {

Server::Server()
{
}

int Server::Init(int argc, char* argv[])
{
    signal(SIGABRT, &maque::AbortHandler);

    if (LoadOptions(argc, argv) == DO_EXIST) {
        // 版本信息，安装等
        return DO_EXIST;
    }

    // 初始化日志
    Log::Instance()->Initialize();

    int ret = InitTimer();
    if (ret == DO_EXIST) {
        return DO_EXIST;
    }

    return 0;
}

int Server::LoadOptions(int argc, char* argv[])
{
    std::string configName = argv[0] + ".conf";
    auto configFile = fs::absolute(configName);

    // 命令行
    options_description generic("Generic options");
    generic.add_options()("version,v", "print version string");
    generic.add_options()("help,h", "produce help message");
    generic.add_options()("config,c",
        value<fs::path>(&configFile)->default_value(fs::absolute(configName)),
        "name of a file of a configuration.");

#if defined(BOOST_WINDOWS)
    std::string configService;
    // 加服务命令行
    options_description win("Windows platform options");
    win.add_options()("service,s",
        value<std::string>(&configService)->default_value(""),
        "Windows service options: [install | uninstall]");
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
        return DO_EXIST;

#if defined(BOOST_WINDOWS)
    if (configService.compare("install") == 0)
        return WinServiceInstall() ? 0 : 1;
    else if (configService.compare("uninstall") == 0)
        return WinServiceUninstall() ? 0 : 1;
    else if (configService.compare("run") == 0)
        return WinServiceRun() ? 0 : 1;
#endif

    // 加载配置文件
    std::string configError;
    bool ret = sConfigMgr->LoadInitial(configFile.generic_string(),
        std::vector<std::string>(argv, argv + argc),
        configError);
    if (!ret) {
        return DO_EXIST;
    }

    return 0;
}

int Server::InitTimer()
{
    // 初始化一个一秒的定时器
    std::share_ptr<boost::asio::deadline_timer>
    return 0;
}

} // namespace maque
