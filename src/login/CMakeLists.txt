# Copyright (C) 2018-2020 Yunpai <http://www.yunpai.cn>
#
################## GateServer ###################

file(GLOB PRIVATE_SOURCES *.cpp *.h)
file(GLOB PRIVATE_HEADERS *.h)

# 加入头文件路径
include_directories(
  ${CMAKE_SOURCE_DIR}/src/common
  ${PROJECT_BINARY_DIR}
  ${Boost_INCLUDE_DIRS}
#  ${OPENSSL_INCLUDE_DIR}
#  ${MARIADB_INCLUDE_DIR}
)

# 输出可执行文件
add_executable(GateServer
  ${PRIVATE_SOURCES}
)

# 库目录
link_directories(${CMAKE_LIBRARY_PATH})
#link_directories("d:/vcpkg/installed/x86-windows/lib/")

# 链接库
target_link_libraries(GateServer
  #protobuf::libprotoc protobuf::libprotobuf
  ${Boost_LIBRARIES}
  spdlog::spdlog
  ${OPENSSL_LIBRARIES}
  ${MARIADB_LIBRARIES}
)

#target_link_libraries(GateServer optimized libuv)
#target_link_libraries(GateServer optimized apr-1)

target_include_directories(GateServer
  PUBLIC
    ${PUBLIC_INCLUDES}
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

if(MSVC)
# 忽略LIBCMTD.lib
#target_link_libraries(GateServer ws2_32 Iphlpapi Psapi Userenv Rpcrt4)
endif()
    
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
#target_link_libraries(GateServer m)
endif(CMAKE_SYSTEM_NAME MATCHES "Linux")
