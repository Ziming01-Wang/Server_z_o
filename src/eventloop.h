#ifndef _EVENTLOOP_H_
#define _EVENTLOOP_H_


#include "epoll.h"
#include "threadpool.h"
#include <functional>

class Channel;
class Eventloop{
private:
    Epoll *ep;
    bool quit;
    Threadpool *m_pool;
public:
    Eventloop();
    ~Eventloop();
    void loop();
    void updatechannel(Channel*m_ch);
    void addthread(std::function<void()> task);
};

#endif