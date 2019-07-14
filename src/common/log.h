/*
 * Copyright (C) 2018 huangtao117@yeah.net QQ:409577078
 * Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MAQUE_COMMON_LOG_H_
#define _MAQUE_COMMON_LOG_H_

#include <string>
#include <memory>
#include <functional>
#include <algorithm>
#include <string>
#include <stdarg.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>

#include "config.h"

namespace maque
{

class Log : public std::enable_shared_from_this<Log>
{
public:
	~Log() {
        spdlog::drop_all();
	}

	Log()
    {
        // 日志格式
		spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%f] [logname:%n] [level:%l] [thread:%t] %v");

        // 控制台日志
        auto console = spdlog::stdout_color_mt("console");
		console->set_level(spdlog::level::trace);

        // 文件日志
        auto common = spdlog::daily_logger_st("common", "logs/log.txt");
        common->flush_on(spdlog::level::trace);

        // 玩家日志
		auto player = spdlog::daily_logger_st("player", "logs/player.txt");
		player->flush_on(spdlog::level::trace);

        // 账号数据日志
		auto account = spdlog::daily_logger_st("account", "logs/account.txt");
		account->flush_on(spdlog::level::trace);
	}

	static Log* Instance()
	{
		static Log _instance;
		return &_instance;
	}

	void Initialize()
	{
		_dir = sConfigMgr->GetStringDefault("LogDirectory", "logs");

		if (!_dir.empty())
		{
			if ((_dir.at(_dir.length() - 1) != '/') && (_dir.at(_dir.length() - 1) != '\\'))
			{
				_dir.push_back('/');
			}
		}
	}

private:
    std::string _dir;
};

#define sLog Log::Instance()

#define LOG_TRACE(fmt, ...) { \
		spdlog::get("common")->trace("[file:#{} func:#{} line:#{}] " fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__); \
		spdlog::get("console")->trace("[file:#{} func:#{} line:#{}] " fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__); \
}\

#define LOG_DEBUG(fmt, ...) { \
		spdlog::get("common")->debug("[file:#{} func:#{} line:#{}] " fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__); \
		spdlog::get("console")->debug("[file:#{} func:#{} line:#{}] " fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__); \
}\

#define LOG_INFO(fmt, ...) { \
		spdlog::get("common")->info("[file:#{} func:#{} line:#{}] " fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__); \
		spdlog::get("console")->info("[file:#{} func:#{} line:#{}] " fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__); \
}\

#define LOG_ACTION(fmt, ...) { \
		spdlog::get("common")->info("[file:#{} func:#{} line:#{}] " fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__); \
}\

#define LOG_WARN(fmt, ...) { \
		spdlog::get("common")->warn("[file:#{} func:#{} line:#{}] " fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__); \
		spdlog::get("console")->warn("[file:#{} func:#{} line:#{}] " fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__); \
}\

#define LOG_ERROR(fmt, ...) { \
		spdlog::get("common")->error("[file:#{} func:#{} line:#{}] " fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__); \
		spdlog::get("console")->error("[file:#{} func:#{} line:#{}] " fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__); \
}\

#define LOG_ERR(fmt, ...) { \
		spdlog::get("common")->error("[file:#{} func:#{} line:#{}] " fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__); \
		spdlog::get("console")->error("[file:#{} func:#{} line:#{}] " fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__); \
}\

#define LOG_CRITICAL(fmt, ...) { \
		spdlog::get("common")->critical("[file:#{} func:#{} line:#{}] " fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__); \
}\

#define LOG(level, fmt, ...) LOG_##level(fmt, ##__VA_ARGS__);

} // namespace maque
#endif // _MAQUE_COMMON_LOG_H_
