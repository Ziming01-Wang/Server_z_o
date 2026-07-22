#include "acceptor.h"
#include "channel.h"
#include "eventloop.h"
#include "inetaddress.h"
#include "socket.h"
#include <cstddef>
#include <functional>
Acceptor::Acceptor(Eventloop *loop){
    this->loop=loop;
    m_socket=new Socket();
    m_addr=new Inetaddress("127.0.0.1",8888);
    m_socket->bind(*m_addr);
    m_socket->listen();
//    m_socket->setnonblocking();

    m_channel=new Channel(loop,m_socket->getfd());
    auto acclink=std::bind(&Acceptor::acceptConnection,this);
    m_channel->setcallback(acclink);
    m_channel->enableReadinglt();
    m_channel->nouseThreadpool();
}

Acceptor::~Acceptor(){
    loop=nullptr;
    delete m_socket;
    delete m_addr;
    delete m_channel;
    newconnectionCallback=NULL;
}

void Acceptor::acceptConnection(){
    newconnectionCallback(m_socket);
}

void Acceptor::setcallback(std::function<void(Socket*)> cb){
    newconnectionCallback=cb;
}
