#include <iostream>
#include<arpa/inet.h>
#include <netinet/in.h>
#include <ostream>
#include <strings.h>
#include<sys/socket.h>
#include <string.h>

class Server {
public:
    Server(){
        int sockfd=socket(AF_INET,SOCK_STREAM,0);

        struct sockaddr_in serv_addr;
        bzero(&serv_addr,sizeof(serv_addr));//initialize
        serv_addr.sin_family=AF_INET;
        serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
        serv_addr.sin_port=htons(8888);

        bind(sockfd,(sockaddr*)&serv_addr,sizeof(serv_addr));

        listen(sockfd,SOMAXCONN);


            struct sockaddr_in clin_addr;
            bzero(&clin_addr, sizeof(clin_addr));
            socklen_t clin_length=sizeof(clin_addr);
            int clin_sockfd=accept(sockfd, (sockaddr*)&clin_addr, &clin_length);
            std::cout<<"qwertyuiopasdfghjkl;zxcvbnm,"<<std::endl;

    }

    

};