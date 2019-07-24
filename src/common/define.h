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

#ifndef _MAQUE_COMMON_DEFINE_H_
#define _MAQUE_COMMON_DEFINE_H_

#include <iostream>
#include <cstddef>
#include <cinttypes>
#include <climits>
#include <memory>
#include <functional>
#include <unordered_map>
#include <tuple>

/**
 * @brief 日志输出设备，支持控制台、文件、网络
 */
typedef enum {
    DEV_NONE = 0,
    DEV_STDOUT = 0x01,
    DEV_FILE = 0x02,
    DEV_NET = 0x04,
} DEVICE_TYPE;

#endif // _MAQUE_COMMON_DEFINE_H_
