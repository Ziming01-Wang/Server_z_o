#ifndef CLIENT_H
#define CLIENT_H
#include <iostream>
#include<arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include<sys/socket.h>
#include <string.h>
#include <unistd.h>
#include "util.h"
#include "socket.h"
#include "inetaddress.h"
class Client{
public:
    Client();
};
#endif