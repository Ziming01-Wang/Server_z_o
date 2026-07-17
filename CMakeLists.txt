cmake_minimum_required(VERSION 3.10)
project(SingleThread_CS CXX)

# 强制开启 C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 让根目录的 #include "src/xxx.h" 和 src 内部的 #include "xxx.h" 都能被找到
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src)

# src 目录下的公共库源文件
set(COMMON_SRC
    src/socket.cpp
    src/inetaddress.cpp
    src/util.cpp
    src/channel.cpp
    src/epoll.cpp
    src/eventloop.cpp
    src/acceptor.cpp
    src/connection.cpp
)

# 编译服务端可执行文件（servermain.cpp 里有 main）
add_executable(game_server
    servermain.cpp
    server.cpp
    ${COMMON_SRC}
)

# 编译客户端可执行文件（client.cpp 里有 main，只依赖 socket/inetaddress/util）
add_executable(game_client
    client.cpp
    src/socket.cpp
    src/inetaddress.cpp
    src/util.cpp
)
