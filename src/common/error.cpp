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

#include "error.h"

namespace maque {

// 避免全局变量构造、析构问题
static std::unordered_map<int, const char*>* g_error_string_map = nullptr;
struct ErrorInfo {
    ErrorInfo() { g_error_string_map = &_error_info; }
    ~ErrorInfo() { g_error_string_map = nullptr; }
    std::unordered_map<int, const char*> _error_info;
};

const char* GetErrorString(int error_code)
{
    if (0 == error_code)
        return "no error";

    return "unregister error description";
}

void SetErrorString(int error_code, const char* error_string)
{
    static ErrorInfo s_error_info;
    (*g_error_string_map)[error_code] = error_string;
}

void AbortHandler(int sigval)
{
    printf("call AbortHandler.\n");
    *((volatile int*)NULL) = 0;
    exit(1);
}

} // namespace maque
