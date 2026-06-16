#ifndef SERVER_H
#define SERVER_H
#include "epoll.h"
#include "eventloop.h"
#include "inetaddress.h"
#include "socket.h"
#include "util.h"
#include "channel.h"


#include <iostream>
#include<arpa/inet.h>
#include <netinet/in.h>
#include <ostream>
#include <strings.h>
#include<sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>

class Server {
    Eventloop *m_loop;
    //Epoll *m_ep;
    Socket *server_sock;//监听套接字提升为成员，避免局部对象析构时提前 close 掉监听 fd
public:

    Server(Eventloop *eventl);
    ~Server();
    void newconnect(Socket *server_socket);
    void dosomething(int clfd);
};
#endif