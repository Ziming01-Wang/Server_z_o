#include "server.h"
#include "epoll.h"
#include "inetaddress.h"
#include "socket.h"
#include "util.h"
#include "channel.h"

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

void Server::dosomething(int clfd){
    char buf[1024];
    while(1){
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read=read(clfd,&buf,sizeof(buf));
        if(bytes_read>0){
            std::cout<<"[client:"<<clfd<<"]has written something:"<<buf<<std::endl;
            write(clfd, buf, bytes_read);//回写给客户端，否则客户端的 read 会一直阻塞等待回复
        }else if(bytes_read==-1&&errno==EINTR){//客户端正常中断
            continue;
        }else if(bytes_read==-1&&(errno==EAGAIN||errno==EWOULDBLOCK)){//读完了
            break;
        }else if(bytes_read==0){//EOF
            close(clfd);
            break;
        }
    }
}

Server::Server(){
   
    Socket server_socket;
    Inetaddress server_addr("127.0.0.1",8888);
    server_socket.bind(server_addr);
    server_socket.listen();
    server_socket.setnonblocking();
    Epoll *m_ep=new Epoll();
    Channel *servch=new Channel(m_ep,server_socket.getfd());
    servch->enableReading();
    //m_ep.addfd(server_socket.getfd(),EPOLLIN);

    while(1){
        std::vector<Channel*> onchannels=m_ep->poll();
        for(auto it:onchannels){
            if(it->getfd()==server_socket.getfd()){
        
                int clfd=server_socket.accept();
                std::cout<<"a new client has connected"<<std::endl;
                setnonblocking(clfd);//accept 返回的 fd 默认是阻塞的，EPOLLET 必须配合非阻塞，否则 while 循环里第二次 read 会卡死
                
                //m_ep.addfd(clfd);
                Channel *temcl=new Channel(m_ep,clfd);
                temcl->enableReading();


            }else if(it->getEvents()&EPOLLIN){
                dosomething(it->getfd());
            }else{
                printf("something else coming son....");
            }
        }

    }
}

int main(){
    Server A;
}