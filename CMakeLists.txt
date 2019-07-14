# Copyright (C) 2018-2020 huangtao117@yeah.net
# 与vcpkg集成
# cmake .. -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake
#

# Minimum CMake required
cmake_minimum_required (VERSION 3.2)

project (maque)

# c++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 第三方依赖库
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
#find_package(protobuf CONFIG REQUIRED)

find_package(Boost REQUIRED COMPONENTS
date_time
filesystem
system
program_options
regex
)

#find_package(OpenSSL REQUIRED)
#find_package(MySQL REQUIRED)
#find_package(RapidJSON REQUIRED)

# 源代码
add_subdirectory(src)
