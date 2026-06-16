#include "src/server.h"
#include "src/eventloop.h"
#include "src/socket.h"

#include <cerrno>
#include <functional>
#include <netinet/in.h>
#include <strings.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
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

Server::Server(Eventloop *eventl):m_loop(eventl){

    server_sock=new Socket();//用 new 让监听套接字活到 Server 析构，且只此一份，避免拷贝/提前 close
    Inetaddress server_addr("127.0.0.1",8888);
    server_sock->bind(server_addr);
    server_sock->listen();
    server_sock->setnonblocking();

    Channel *servch=new Channel(m_loop,server_sock->getfd());
    auto connectMask=std::bind(&Server::newconnect,this,server_sock);//传指针而非拷贝 Socket
    servch->setcallback(connectMask);
    servch->enableReading();
    //m_ep.addfd(server_socket.getfd(),EPOLLIN);

    /*while(1){
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

    }*/

}
Server::~Server(){
    delete server_sock;
    m_loop=nullptr;
}


void Server::newconnect(Socket *server_socket){
    int clfd=server_socket->accept();
    std::cout<<"a new client has connected"<<std::endl;
    setnonblocking(clfd);//accept 返回的 fd 默认是阻塞的，EPOLLET 必须配合非阻塞，否则 while 循环里第二次 read 会卡死
                
    //m_ep.addfd(clfd);
    Channel *temcl=new Channel(m_loop,clfd);
    auto handlereadmask=std::bind(&Server::dosomething,this,clfd);
    temcl->setcallback(handlereadmask);
    temcl->enableReading();
}