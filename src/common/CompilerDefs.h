/*
 * Copyright (C) 2018 HANGHZOU YUNPAI NETWORK TECH CO.
 * huangtao117@yeah.net QQ:409577078
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

#ifndef YUNPAI_COMPILERDEFS_H_
#define YUNPAI_COMPILERDEFS_H_

// 运行平台定义
#define YUNPAI_PLATFORM_WINDOWS 0
#define YUNPAI_PLATFORM_LINUX   1

// must be first (win 64 also define _WIN32)
#if defined( _WIN64 )
#  define YUNPAI_PLATFORM YUNPAI_PLATFORM_WINDOWS
#elif defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#  define YUNPAI_PLATFORM YUNPAI_PLATFORM_WINDOWS
#else
#  define YUNPAI_PLATFORM YUNPAI_PLATFORM_LINUX
#endif

// 编译器定义
#define YUNPAI_COMPILER_MICROSOFT 0
#define YUNPAI_COMPILER_GNU       1
#define YUNPAI_COMPILER_CLANG     2

#ifdef _MSC_VER
#  define YUNPAI_COMPILER YUNPAI_COMPILER_MICROSOFT
#elif defined( __clang__ )
#  define YUNPAI_COMPILER YUNPAI_COMPILER_CLANG
#elif defined( __GNUC__ )
#  define YUNPAI_COMPILER YUNPAI_COMPILER_GNU
#  define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#else
#  error "FATAL ERROR: Unknown compiler."
#endif

#endif
