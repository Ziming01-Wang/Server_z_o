#include "epoll.h"
#include "util.h"
#include <strings.h>
#include <sys/epoll.h>
#include <vector>

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

void Epoll::addfd(int fd){
    struct epoll_event epev;
    epev.events=EPOLLIN|EPOLLET;
    epev.data.fd=fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &epev);

}

std::vector<epoll_event> Epoll::poll(int timeout){
    int nfds=epoll_wait(epfd, ep_events, MAX_EVENTS,  timeout);
    errif(nfds==-1, "wait error");
    std::vector<epoll_event> on_events;
    for(int i=0;i<nfds;i++){
        on_events.push_back(ep_events[i]);
    }
    return on_events;
}