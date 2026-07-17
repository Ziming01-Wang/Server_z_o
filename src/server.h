#ifndef SERVER_H
#define SERVER_H
#include "connection.h"
#include "epoll.h"
#include "eventloop.h"
#include "inetaddress.h"
#include "socket.h"
#include "util.h"
#include "channel.h"
#include "acceptor.h"


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
    Acceptor *m_acc;
    //Epoll *m_ep;
    std::map<int,Connection*> m_connections;
public:

    Server(Eventloop *eventl);
    ~Server();
    void newconnect(Socket *server_socket);
    void disconnect(Socket *clsock);
    void dosomething(int clfd);
};
#endif