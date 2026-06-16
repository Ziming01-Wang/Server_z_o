#include "epoll.h"
#include "util.h"
#include <strings.h>
#include <sys/epoll.h>
#include <vector>
#include "channel.h"
Epoll::Epoll(){
    epfd=epoll_create1(0);
    errif(epfd==-1, "epoll create error");
    ep_events=new epoll_event[MAX_EVENTS];
    bzero(ep_events, sizeof(epoll_event*)*MAX_EVENTS);
}

Epoll::~Epoll(){
    epfd=-1;
    delete[] ep_events;
}

void Epoll::addfd(int fd ,uint32_t m_event){
    struct epoll_event epev;
    epev.events=m_event;
    epev.data.fd=fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &epev);

}

std::vector<Channel*> Epoll::poll(int timeout){
    int nfds=epoll_wait(epfd, ep_events, MAX_EVENTS,  timeout);
    errif(nfds==-1, "wait error");
    std::vector<Channel*> on_channels;
    for(int i=0;i<nfds;i++){
        Channel* ch=(Channel*)ep_events[i].data.ptr;
        ch->setRevents(ep_events[i].events);//修改现有事件
        on_channels.push_back(ch);
    }
    return on_channels;
}

void Epoll::updateChannel(Channel *m_channel){
    int temfd=m_channel->getfd();
    struct epoll_event temev;
    bzero(&temev, sizeof(temev));
    temev.events=m_channel->getEvents();
    temev.data.ptr=m_channel;//必须存 Channel 指针，poll() 里才能用 data.ptr 取回；存 data.fd 会导致解引用非法地址而段错误
    if(!m_channel->getInpoll()){
        epoll_ctl(epfd, EPOLL_CTL_ADD, temfd, &temev);
        m_channel->setInpoll();
    }else{
        epoll_ctl(epfd, EPOLL_CTL_MOD, temfd, &temev);
    }
}
