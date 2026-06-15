#include "channel.h"

Channel::Channel(Epoll *ep, int fd){
    this->ep=ep;
    this->fd=fd;
    this->events=0;
    this->revents=0;
    this->inEpoll=false;
}

Channel::~Channel(){
    ep=nullptr;
    fd=-1;
    events=0;
    revents=0;
    inEpoll=false;
}

void Channel::enableReading(){
    events = EPOLLIN | EPOLLET;
    ep->updateChannel(this);
}

int Channel::getfd(){
    return fd;
}

uint32_t Channel::getEvents(){
    return events;
}

uint32_t Channel::getRevents(){
    return revents;
}

bool Channel::getInpoll(){
    return inEpoll;
}

void Channel::setInpoll(){
    inEpoll=true;
}

void Channel::setRevents(uint32_t m_ev){
    revents=m_ev;
}