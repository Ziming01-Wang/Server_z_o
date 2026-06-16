#ifndef _INETADDRESS_H_
#define  _INETADDRESS_H_
#include<arpa/inet.h>
#include <cstdint>
#include <netinet/in.h>
#include <sys/socket.h>

class Inetaddress{
public:
    struct sockaddr_in addr;
    socklen_t addrlen=0;
public:
    Inetaddress();
    Inetaddress(const char* ip,uint16_t port);
    ~Inetaddress();
};

#endif