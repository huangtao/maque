#ifndef _MAQUE_SERVER_H_
#define _MAQUE_SERVER_H_

#include "common/define.h"

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
     * @brief 初始化
     * @return =-1 输出帮助信息,结束
     * @return <0 失败
     * @return =0 成功
     */
    int Init(int argc, char * argv[]);

protected:
    /**
     * @brief 加载命令行和配置文件
     */
    int LoadOptions(int argc, char* argv[], fs::path& configFile, std::string& configService);

private:
    Options             _options;
    AppEventHandler*    _event_handler;
};

} // namespace maque
#endif // _MAQUE_SERVER_H_
