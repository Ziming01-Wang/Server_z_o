#include "connection.h"
#include "channel.h"
#include "eventloop.h"
#include "socket.h"
#include "util.h"
#include <functional>
#include <iostream>
#include <unistd.h>
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
            m_buffer.append(buf, sizeof(buf));
        }else if(bytes_read==-1&&errno==EINTR){//客户端正常中断

            std::cout<<"continue reading"<<std::endl;
            continue;
        }else if(bytes_read==-1&&(errno==EAGAIN||errno==EWOULDBLOCK)){//读完了

            errif(write(clfd, m_buffer.c_str(), m_buffer.getsize())==-1,"数据回复失败");
            m_buffer.clear();
            break;
        }else if(bytes_read==0){//EOF，客户端断开连接，对应disconnect

            std::cout<<"client:"<<clfd <<"disconnect"<<std::endl;
            m_buffer.clear();
            disconnectCallback(cl_socket);
            break;

        }
    }
}

void Connection::setDisconnectCallback(std::function<void(Socket*)> cb){
    disconnectCallback=cb;
}