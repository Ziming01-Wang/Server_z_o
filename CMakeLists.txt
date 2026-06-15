cmake_minimum_required(VERSION 3.10)
project(SingleThread_CS CXX)

# 强制开启 C++17 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES1 server.cpp util.cpp socket.cpp inetaddress.cpp epoll.cpp channel.cpp)
set(SOURCES2 client.cpp util.cpp socket.cpp inetaddress.cpp channel.h) 

set(HEADERS server.h client.h socket.h inetaddress.h epoll.h util.h)
# 编译服务端可执行文件
add_executable(game_server ${SOURCES1} ${HEADERS})

# 编译客户端可执行文件
add_executable(game_client ${SOURCES2} ${HEADERS})