# Copyright (C) 2018-2020 Yunpai <http://www.yunpai.cn>
#
################## common ###################

file(GLOB PRIVATE_SOURCES *.cpp *.h)
file(GLOB PRIVATE_HEADERS *.h)

# 调试版加后缀
#set(CMAKE_DEBUG_POSTFIX -d)

# UTFCPP库是头文件库,我们需要用find_path
find_path(UTFCPP_INCLUDE_DIR NAMES utf8.h PATH_SUFFIXES utf8)

include_directories(${PROJECT_SOURCE_DIR})

# 输出
add_library(common STATIC ${PRIVATE_SOURCES})

# 链接库
target_link_libraries(common
  PUBLIC
    fmt::fmt fmt::fmt-header-only
#  PRIVATE
#   ${OPENSSL_LIBRARIES} 
)

# 包含目录
target_include_directories(common
  PUBLIC
    # Provide the binary dir for all child targets
    ${CMAKE_BINARY_DIR}
    ${PUBLIC_INCLUDES}
    ${Boost_INCLUDE_DIRS}
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${UTFCPP_INCLUDE_DIR}
    #${OPENSSL_INCLUDE_DIR}
)
