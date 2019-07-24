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

#ifndef _MAQUE_SERVER_H_
#define _MAQUE_SERVER_H_

#include "common/error.h"

namespace maque {

// 前置声明
class Server;

// 事件处理基类
class AppEventHandler {
public:
    AppEventHandler() {}
    virtual ~AppEventHandler() {}

    /**
     * @brief Init事件回调处理
     * @return <0 失败
     * @return =0 成功
     */
    virtual int OnInit(Server* server) { return 0; }
};

class Server {
public:
    Server();
    ~Server();

public:
    /**
     * @brief 初始化,加载应用同名配置文件
     * @return =-1 输出帮助信息,结束
     * @return <0 失败
     * @return =0 成功
     */
    int Init(int argc, char* argv[]);

protected:
    /**
     * @brief 加载命令行和配置文件
     */
    int LoadOptions(int argc, char* argv[]);

    void InitLog();

private:
    Options _options;
    AppEventHandler* _event_handler;
};

} // namespace maque
#endif // _MAQUE_SERVER_H_
