#include "socket.h"
#include "inetaddress.h"
#include "util.h"
#include <sys/socket.h>
#include <unistd.h>

Socket::Socket(){
    fd=socket(AF_INET, SOCK_STREAM, 0);
    errif(fd==-1, "built error");
}

Socket::Socket(const int &m_fd){
    fd=m_fd;
    errif(fd==-1, "built error");
}

Socket::~Socket(){
    fd==-1?:close(fd);
    fd=-1;
}


void Socket::bind(Inetaddress &A){
    errif(::bind(fd,(sockaddr*)&A.addr,A.addrlen)==-1,"bind error");
}

void Socket::listen(){
    errif(::listen(fd,SOMAXCONN)==-1,"listen error");
}

int Socket::accept(){
    Inetaddress tem;
    int temfd=::accept(fd, (sockaddr*)&tem.addr,&tem.addrlen);
    errif(temfd==-1, "accept error");
    return temfd;
}

void Socket::setnonblocking(){
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

void Socket::connect(Inetaddress &A){
    errif(::connect(fd, (sockaddr*)&A.addr, A.addrlen)==-1,"connect error");
}

