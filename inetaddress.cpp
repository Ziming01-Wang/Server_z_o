#include"inetaddress.h"
#include <arpa/inet.h>
#include <cstdint>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>

Inetaddress::Inetaddress():addrlen(sizeof(addr)){
    bzero(&addr, addrlen);
}
Inetaddress::Inetaddress(const char* ip,uint16_t port):addrlen(sizeof(addr)){
    bzero(&addr, addrlen);
    addr.sin_addr.s_addr=inet_addr(ip);
    addr.sin_port=port;
    addr.sin_family=AF_INET;
}
Inetaddress::~Inetaddress(){
}