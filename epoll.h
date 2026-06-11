#ifndef EPOLL_H
#define EPOLL_H

#define MAX_EVENTS 1024
#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <functional>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>

class Epoll{
    int epfd=-1;
    struct epoll_event *ep_events;

public:
    Epoll();
    ~Epoll();

    void addfd(int fd);
    std::vector<epoll_event> poll(int timeout=-1);
};

#endif