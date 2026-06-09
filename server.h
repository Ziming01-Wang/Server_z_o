#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include<arpa/inet.h>
#include <netinet/in.h>
#include <ostream>
#include <strings.h>
#include<sys/socket.h>
#include <string.h>
#include "util.h"
#include <unistd.h>
#include <sys/epoll.h>

class Server {
public:
    Server();
};
#endif