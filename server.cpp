#include "server.h"
#include "util.h"
#include <cerrno>
#include <cstdio>
#include <netinet/in.h>
#include <strings.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_EVENTS 1024
Server::Server(){
    int sockfd=socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in serv_addr;
    bzero(&serv_addr,sizeof(serv_addr));//initialize
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serv_addr.sin_port=htons(8888);

    errif(bind(sockfd,(sockaddr*)&serv_addr,sizeof(serv_addr))==-1,"bind faliure");
    errif(listen(sockfd,SOMAXCONN)==-1,"listen faliure");


    /*  struct sockaddr_in clin_addr;
        bzero(&clin_addr, sizeof(clin_addr));
        socklen_t clin_length=sizeof(clin_addr);
        int clin_sockfd=accept(sockfd, (sockaddr*)&clin_addr, &clin_length);//客户端套接字
        std::cout<<"qwertyuiopasdfghjkl;zxcvbnm,"<<std::endl;*/  

    /*while(true){
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
    */
    int epfd=epoll_create1(0);
    struct epoll_event ep_events[MAX_EVENTS];
    struct epoll_event epev;//只初始化一次，以后新客户端连接可以复用
    epev.events=EPOLLIN;
    epev.data.fd=sockfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &epev);//添加服务端监听套接字

    while(1){
        int nfds=epoll_wait(epfd, ep_events, MAX_EVENTS, -1);
        for(int i=0;i<nfds;i++){
            if(ep_events[i].data.fd==sockfd){
                struct sockaddr_in cl_addr;
                bzero(&cl_addr, sizeof(cl_addr));
                socklen_t cl_addr_len=sizeof(cl_addr);
                int cl_sockfd=accept(sockfd,(sockaddr*)&cl_addr, &cl_addr_len);
                errif(cl_sockfd==-1, "connect error!");
                std::cout<<"a new client has connected"<<std::endl;

                bzero(&epev, sizeof(epev));
                epev.data.fd=cl_sockfd;
                epev.events=EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cl_sockfd, &epev);

            }else if(ep_events[i].events&EPOLLIN){
                char buf[1024];
                while(1){
                    bzero(&buf, sizeof(buf));
                    ssize_t bytes_read=read(ep_events[i].data.fd,&buf,sizeof(buf));
                    if(bytes_read>0){
                        std::cout<<"[client:"<<ep_events[i].data.fd<<"]has written something:"<<buf<<std::endl;
                    }else if(bytes_read==-1&&errno==EINTR){//客户端正常中断
                        continue;
                    }else if(bytes_read==-1&&(errno==EAGAIN||errno==EWOULDBLOCK)){//读完了
                        break;
                    }else if(bytes_read==0){//EOF
                        close(ep_events[i].data.fd);
                        break;
                    }
                }
            }else{
                printf("something else coming son....");
            }
        }

    }
    close(sockfd);
}

int main(){
    Server A;
}