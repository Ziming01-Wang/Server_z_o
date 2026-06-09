#include <iostream>
#include<arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include<sys/socket.h>
#include <string.h>

class Client{
public:
    Client(){
        int clinfd=socket(AF_INET,SOCK_STREAM,0);
        struct sockaddr_in cl_addr;
        bzero(&cl_addr, sizeof(cl_addr));
        cl_addr.sin_port=htons(8888);
        cl_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
        cl_addr.sin_family=AF_INET;
        //bind(clinfd,(sockaddr*)&cl_addr,sizeof(cl_addr));

        connect(clinfd, (sockaddr*)&cl_addr, sizeof(cl_addr));
    }
};
