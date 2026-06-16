#include "eventloop.h"
#include "channel.h"
#include "epoll.h"
#include <vector>

Eventloop::Eventloop(){
    quit=false;
    ep=new Epoll();
}

Eventloop::~Eventloop(){
    quit=true;
    delete ep;
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