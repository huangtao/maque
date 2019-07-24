/*
 * Copyright (C) 2018 huangtao117@yeah.net QQ:409577078
 * Licensed under the MIT License (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 * http://opensource.org/licenses/MIT
 * Unless required by applicable law or agreed to in writing, software distributed under the License
 * is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the License for the specific language governing permissions and limitations under
 * the License.
 */

#ifndef _MAQUE_COMMON_CONFIG_H_
#define _MAQUE_COMMON_CONFIG_H_

#include <string>
#include <vector>

namespace maque {

class ConfigMgr {
    ConfigMgr() = default;
    ConfigMgr(ConfigMgr const&) = delete;
    ConfigMgr& operator=(ConfigMgr const&) = delete;
    ~ConfigMgr() = default;

public:
    // 加载主配置文件
    bool LoadInitial(std::string const& file, std::vector<std::string> args, std::string& error);

    static ConfigMgr* Instance();

    bool Reload(std::string& error);

    std::string GetStringDefault(std::string const& name, const std::string& def) const;
    bool GetBoolDefault(std::string const& name, bool def) const;
    int GetIntDefault(std::string const& name, int def) const;
    float GetFloatDefault(std::string const& name, float def) const;

    std::string const& GetFilename();
    std::vector<std::string> const& GetArguments() const;
    std::vector<std::string> GetKeysByString(std::string const& name);

private:
    template <class T>
    T GetValueDefault(std::string const& name, T def) const;
};

#define sConfigMgr ConfigMgr::Instance()

} // namespace maque
#endif // _MAQUE_COMMON_CONFIG_H_
