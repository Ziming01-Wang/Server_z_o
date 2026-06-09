cmake_minimum_required(VERSION 3.10)
project(SingleThread_CS CXX)

# 强制开启 C++17 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 编译服务端可执行文件
add_executable(game_server server.cpp)

# 编译客户端可执行文件
add_executable(game_client client.cpp)