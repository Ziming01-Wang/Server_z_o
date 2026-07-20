#include "eventloop.h"
#include "channel.h"
#include "epoll.h"
#include "threadpool.h"
#include <functional>
#include <vector>

Eventloop::Eventloop(){
    quit=false;
    ep=new Epoll();
    m_pool=new Threadpool(10);
}

Eventloop::~Eventloop(){
    quit=true;
    delete ep;
    delete m_pool;
}

void Eventloop::updatechannel(Channel* m_channel){
    ep->updateChannel(m_channel);
}

void Eventloop::loop(){
    while(!quit){
        std::vector<Channel*>activechannels=ep->poll();
        for(auto it:activechannels){
            it->handleEvent();
        }
    }
}

void Eventloop::addthread(std::function<void()> task){
    m_pool->add_task(task);
}