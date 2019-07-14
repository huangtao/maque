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

#ifndef _MAQUE_COMMON_ERROR_H_
#define _MAQUE_COMMON_ERROR_H_

#include "define.h"

namespace maque {

/**
 * @brief 错误码定义
 */
enum ERROR_CODE_BASE {
    NO_ERROR = 0,
    SESSION_ERROR_CODE_BASE = -1000,
    MESSAGE_ERROR_CODE_BASE = -2000,
    USER_ERROR_CODE_BASE = -100000,
};

/**
 * @brief 获取错误描述
 */
const char *GetErrorString(int error_code);

/**
 * @brief 设置错误描述
 */
void SetErrorString(int error_code, const char *error_string);

} // namespace maque
#endif // _MAQUE_COMMON_CONFIG_H_
