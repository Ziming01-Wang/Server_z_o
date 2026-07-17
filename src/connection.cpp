#include "connection.h"
#include "channel.h"
#include "eventloop.h"
#include "socket.h"
#include <functional>
Connection::Connection(Eventloop *evloop,int clfd):loop(evloop){
    cl_socket=new Socket(clfd);
    m_channel=new Channel(loop,cl_socket->getfd());
    std::function<void()> cb=std::bind(&Connection::dosomething_echo,this,cl_socket->getfd());
    m_channel->setcallback(cb);
    m_channel->enableReading();
}

Connection::~Connection(){
    delete m_channel;
    delete cl_socket;
    loop=nullptr;
    
}

void Connection::dosomething_echo(int clfd){
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
        }else if(bytes_read==0){//EOF，客户端断开连接，对应disconnect
            close(clfd);
            disconnectCallback(cl_socket);
            break;

        }
    }
}

void Connection::setDisconnectCallback(std::function<void(Socket*)> cb){
    disconnectCallback=cb;
}