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

        errif(connect(clinfd, (sockaddr*)&cl_addr, sizeof(cl_addr))==-1,"connect faliure");
        while (true) {
            char buf[1024];//buffer
            bzero(&buf,sizeof(buf));

            std::cin>>buf;
            int isw=write(clinfd, &buf, sizeof(buf));
            if(isw==-1){
                printf("socket already disconnected, can't write any more!\n");
                break;
            }

            char bufread[1024];//buffer
            bzero(&bufread,sizeof(bufread));
            ssize_t readbyte=read(clinfd,&bufread,sizeof(bufread));

            if(readbyte>0){
                std::cout<<bufread<<std::endl;
            }else if(readbyte==0){
                std::cout<<"safe close"<<std::endl;
                close(clinfd);
                break;
            }else if(readbyte==-1){
                close(clinfd);
                errif(true, "read error");
                break;
            }
        }
        close(clinfd);
    }
};
#endif