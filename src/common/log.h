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

#ifndef _MAQUE_COMMON_LOG_H_
#define _MAQUE_COMMON_LOG_H_

#include <algorithm>
#include <functional>
#include <memory>
#include <stdarg.h>
#include <string>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "config.h"

namespace maque {

/**
 * @brief 默认同时输出控制台和文件(每天一个)
 */
class Log : public std::enable_shared_from_this<Log> {
public:
    ~Log() { spdlog::drop_all(); }

    Log()
    {
        // 日志格式
        spdlog::set_pattern(
            "[%Y-%m-%d %H:%M:%S.%f] [logname:%n] [level:%l] [thread:%t] %v");

        // 控制台日志
        auto console = spdlog::stdout_color_mt("console");
        console->set_level(spdlog::level::trace);
    }

    static Log* Instance()
    {
        static Log _instance;
        return &_instance;
    }

    void Initialize()
    {
        _dir = sConfigMgr->GetStringDefault("LogsDir", "./logs");
        if (!_dir.empty()) {
            if ((_dir.at(_dir.length() - 1) != '/') && (_dir.at(_dir.length() - 1) != '\\')) {
                _dir.push_back('/');
            }
        }
        std::string filepath = _dir + "log.txt";
        // 文件日志
        auto common = spdlog::daily_logger_st("common", filepath);
        common->flush_on(spdlog::level::trace);
    }

private:
    std::string _dir;
};

#define sLog Log::Instance()

#define LOG_TRACE(fmt, ...)                                                \
    {                                                                      \
        spdlog::get("common")->trace("[file:#{} func:#{} line:#{}] " fmt,  \
            __FILE__,                                                      \
            __func__,                                                      \
            __LINE__,                                                      \
            ##__VA_ARGS__);                                                \
        spdlog::get("console")->trace("[file:#{} func:#{} line:#{}] " fmt, \
            __FILE__,                                                      \
            __func__,                                                      \
            __LINE__,                                                      \
            ##__VA_ARGS__);                                                \
    }

#define LOG_DEBUG(fmt, ...)                                                \
    {                                                                      \
        spdlog::get("common")->debug("[file:#{} func:#{} line:#{}] " fmt,  \
            __FILE__,                                                      \
            __func__,                                                      \
            __LINE__,                                                      \
            ##__VA_ARGS__);                                                \
        spdlog::get("console")->debug("[file:#{} func:#{} line:#{}] " fmt, \
            __FILE__,                                                      \
            __func__,                                                      \
            __LINE__,                                                      \
            ##__VA_ARGS__);                                                \
    }

#define LOG_INFO(fmt, ...)                                                \
    {                                                                     \
        spdlog::get("common")->info("[file:#{} func:#{} line:#{}] " fmt,  \
            __FILE__,                                                     \
            __func__,                                                     \
            __LINE__,                                                     \
            ##__VA_ARGS__);                                               \
        spdlog::get("console")->info("[file:#{} func:#{} line:#{}] " fmt, \
            __FILE__,                                                     \
            __func__,                                                     \
            __LINE__,                                                     \
            ##__VA_ARGS__);                                               \
    }

#define LOG_ACTION(fmt, ...)                                             \
    {                                                                    \
        spdlog::get("common")->info("[file:#{} func:#{} line:#{}] " fmt, \
            __FILE__,                                                    \
            __func__,                                                    \
            __LINE__,                                                    \
            ##__VA_ARGS__);                                              \
    }

#define LOG_WARN(fmt, ...)                                                \
    {                                                                     \
        spdlog::get("common")->warn("[file:#{} func:#{} line:#{}] " fmt,  \
            __FILE__,                                                     \
            __func__,                                                     \
            __LINE__,                                                     \
            ##__VA_ARGS__);                                               \
        spdlog::get("console")->warn("[file:#{} func:#{} line:#{}] " fmt, \
            __FILE__,                                                     \
            __func__,                                                     \
            __LINE__,                                                     \
            ##__VA_ARGS__);                                               \
    }

#define LOG_ERROR(fmt, ...)                                                \
    {                                                                      \
        spdlog::get("common")->error("[file:#{} func:#{} line:#{}] " fmt,  \
            __FILE__,                                                      \
            __func__,                                                      \
            __LINE__,                                                      \
            ##__VA_ARGS__);                                                \
        spdlog::get("console")->error("[file:#{} func:#{} line:#{}] " fmt, \
            __FILE__,                                                      \
            __func__,                                                      \
            __LINE__,                                                      \
            ##__VA_ARGS__);                                                \
    }

#define LOG_ERR(fmt, ...)                                                  \
    {                                                                      \
        spdlog::get("common")->error("[file:#{} func:#{} line:#{}] " fmt,  \
            __FILE__,                                                      \
            __func__,                                                      \
            __LINE__,                                                      \
            ##__VA_ARGS__);                                                \
        spdlog::get("console")->error("[file:#{} func:#{} line:#{}] " fmt, \
            __FILE__,                                                      \
            __func__,                                                      \
            __LINE__,                                                      \
            ##__VA_ARGS__);                                                \
    }

#define LOG_CRITICAL(fmt, ...)                                               \
    {                                                                        \
        spdlog::get("common")->critical("[file:#{} func:#{} line:#{}] " fmt, \
            __FILE__,                                                        \
            __func__,                                                        \
            __LINE__,                                                        \
            ##__VA_ARGS__);                                                  \
    }

#define LOG(level, fmt, ...) LOG_##level(fmt, ##__VA_ARGS__);

} // namespace maque
#endif // _MAQUE_COMMON_LOG_H_
