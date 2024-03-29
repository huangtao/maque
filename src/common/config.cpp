﻿/*
 * Copyright (C) 2018 huangtao117@yeah.net QQ:409577078
 * Licensed under the MIT License (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of the License
 * at http://opensource.org/licenses/MIT Unless required by applicable law or
 * agreed to in writing, software distributed under the License is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#include "config.h"
#include "Log.h"
#include <boost/property_tree/ini_parser.hpp>
#include <algorithm>
#include <memory>
#include <mutex>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace pt = boost::property_tree;

// 无名命名空间,类似static
namespace
{
    std::string _filename;
    std::vector<std::string> _args;
    pt::ptree _config;
    std::mutex _configLock;
}

namespace maque
{

bool ConfigMgr::LoadInitial(std::string const& file, std::vector<std::string> args,
                            std::string& error)
{
    std::lock_guard<std::mutex> lock(_configLock);

    _filename = file;
    _args = args;

    try
    {
        pt::ptree tree;
        pt::ini_parser::read_ini(file, tree);

        if (tree.empty())
        {
            error = "empty file (" + file + ")";
            return false;
        }

        // 约定我们的配置文件只有一节,所以直接访问属性就可以了
        _config = tree.begin()->second;

        // 确保一些配置项必须存在
    }
    catch (pt::ini_parser::ini_parser_error const& e)
    {
        if (e.line() == 0)
            error = e.message() + " (" + e.filename() + ")";
        else
            error = e.message() + " (" + e.filename() + ":" + std::to_string(e.line()) + ")";
        return false;
    }

    return true;
}

ConfigMgr* ConfigMgr::Instance()
{
    static ConfigMgr instance;
    return &instance;
}

bool ConfigMgr::Reload(std::string& error)
{
    return LoadInitial(_filename, std::move(_args), error);
}

template<class T>
T ConfigMgr::GetValueDefault(std::string const& name, T def) const
{
    try
    {
        return _config.get<T>(pt::ptree::path_type(name, '/'));
    }
    catch (pt::ptree_bad_path)
    {
        LOG_WARN("Missing name %s in config file %s, add \"%s = %s\" to this file",
            name.c_str(), _filename.c_str(), name.c_str(), std::to_string(def).c_str());
    }
    catch (pt::ptree_bad_data)
    {
        LOG_ERROR("Bad value defined for name %s in config file %s, going to use %s instead",
            name.c_str(), _filename.c_str(), std::to_string(def).c_str());
    }

    return def;
}

template<>
std::string ConfigMgr::GetValueDefault<std::string>(std::string const& name, std::string def) const
{
    try
    {
        return _config.get<std::string>(pt::ptree::path_type(name, '/'));
    }
    catch (pt::ptree_bad_path)
    {
        LOG_WARN("Missing name %s in config file %s, add \"%s = %s\" to this file",
            name.c_str(), _filename.c_str(), name.c_str(), def.c_str());
    }
    catch (pt::ptree_bad_data)
    {
        LOG_ERROR("Bad value defined for name %s in config file %s, going to use %s instead",
            name.c_str(), _filename.c_str(), def.c_str());
    }

    return def;
}

std::string ConfigMgr::GetStringDefault(std::string const& name, const std::string& def) const
{
    std::string val = GetValueDefault(name, def);
    val.erase(std::remove(val.begin(), val.end(), '"'), val.end());
    return val;
}

bool ConfigMgr::GetBoolDefault(std::string const& name, bool def) const
{
    std::string val = GetValueDefault(name, std::string(def ? "1" : "0"));
    val.erase(std::remove(val.begin(), val.end(), '"'), val.end());
    return (val == "1" || val == "true" || val == "TRUE" || val == "yes" || val == "YES");
}

int ConfigMgr::GetIntDefault(std::string const& name, int def) const
{
    return GetValueDefault(name, def);
}

float ConfigMgr::GetFloatDefault(std::string const& name, float def) const
{
    return GetValueDefault(name, def);
}

std::string const& ConfigMgr::GetFilename()
{
    std::lock_guard<std::mutex> lock(_configLock);
    return _filename;
}

std::vector<std::string> const& ConfigMgr::GetArguments() const
{
    return _args;
}

std::vector<std::string> ConfigMgr::GetKeysByString(std::string const& name)
{
    std::lock_guard<std::mutex> lock(_configLock);

    std::vector<std::string> keys;

    for (pt::ptree::value_type const& child : _config)
        if (child.first.compare(0, name.length(), name) == 0)
            keys.push_back(child.first);

    return keys;
}

}
