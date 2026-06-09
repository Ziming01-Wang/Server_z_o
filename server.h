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

class Server {
public:
    Server(){
        int sockfd=socket(AF_INET,SOCK_STREAM,0);

        struct sockaddr_in serv_addr;
        bzero(&serv_addr,sizeof(serv_addr));//initialize
        serv_addr.sin_family=AF_INET;
        serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
        serv_addr.sin_port=htons(8888);

        errif(bind(sockfd,(sockaddr*)&serv_addr,sizeof(serv_addr))==-1,"bind faliure");

        errif(listen(sockfd,SOMAXCONN)==-1,"listen faliure");


            struct sockaddr_in clin_addr;
            bzero(&clin_addr, sizeof(clin_addr));
            socklen_t clin_length=sizeof(clin_addr);
            int clin_sockfd=accept(sockfd, (sockaddr*)&clin_addr, &clin_length);//客户端套接字
            std::cout<<"qwertyuiopasdfghjkl;zxcvbnm,"<<std::endl;

        while(true){
            char buf[1024];//buffer
            bzero(&buf,sizeof(buf));
            ssize_t readbyte=read(clin_sockfd,&buf,sizeof(buf));

            if(readbyte>0){
                std::cout<<buf<<std::endl;
                write(clin_sockfd, &buf, sizeof(buf));
            }else if(readbyte==0){
                std::cout<<"safe close"<<std::endl;
                close(clin_sockfd);
                break;
            }else if(readbyte==-1){
                close(clin_sockfd);
                errif(true, "read error");
                break;
            }

        }
        close(clin_sockfd);
    }
};
#endif