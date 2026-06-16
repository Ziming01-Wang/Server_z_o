#ifndef SOCKET_H
#define SOCKET_H

#include "inetaddress.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

class Socket{
public:
    Socket();
    Socket(const int &m_fd);
    ~Socket();

    void bind(Inetaddress &A);
    void listen();
    void setnonblocking();
    int accept();
    void connect(Inetaddress &A);

    int getfd(){return fd;}
private:
    int fd=-1;
};


#endif