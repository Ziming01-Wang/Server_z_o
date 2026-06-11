#include "server.h"
#include "epoll.h"
#include "inetaddress.h"
#include "socket.h"
#include "util.h"
#include <cerrno>
#include <cstdio>
#include <netinet/in.h>
#include <strings.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>

#define MAX_EVENTS 1024


void setnonblocking(int fd){
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

Server::Server(){
    /*int sockfd=socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in serv_addr;
    bzero(&serv_addr,sizeof(serv_addr));//initialize
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serv_addr.sin_port=htons(8888);

    errif(bind(sockfd,(sockaddr*)&serv_addr,sizeof(serv_addr))==-1,"bind faliure");
    errif(listen(sockfd,SOMAXCONN)==-1,"listen faliure");

    setnonblocking(sockfd);*/

    Socket server_socket;
    Inetaddress server_addr("127.0.0.1",8888);
    server_socket.bind(server_addr);
    server_socket.listen();
    server_socket.setnonblocking();

    /*int epfd=epoll_create1(0);
    struct epoll_event ep_events[MAX_EVENTS];
    struct epoll_event epev;//只初始化一次，以后新客户端连接可以复用
    epev.events=EPOLLIN;
    epev.data.fd=sockfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &epev);//添加服务端监听套接字*/
    Epoll m_ep;
    m_ep.addfd(server_socket.getfd());

    while(1){
        //int nfds=epoll_wait(epfd, ep_events, MAX_EVENTS, -1);
        std::vector<epoll_event> onevents=m_ep.poll();
        for(auto it:onevents){
            if(it.data.fd==server_socket.getfd()){
                /*struct sockaddr_in cl_addr;
                bzero(&cl_addr, sizeof(cl_addr));
                socklen_t cl_addr_len=sizeof(cl_addr);
                int cl_sockfd=accept(sockfd,(sockaddr*)&cl_addr, &cl_addr_len);
                errif(cl_sockfd==-1, "connect error!");
                std::cout<<"a new client has connected"<<std::endl;

                setnonblocking(cl_sockfd);
                bzero(&epev, sizeof(epev));
                epev.data.fd=cl_sockfd;
                epev.events=EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cl_sockfd, &epev);*/
                int clfd=server_socket.accept();
                std::cout<<"a new client has connected"<<std::endl;
                m_ep.addfd(clfd);


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