#ifndef _EVENTLOOP_H_
#define _EVENTLOOP_H_


#include "epoll.h"

class Channel;
class Eventloop{
private:
    Epoll *ep;
    bool quit;
public:
    Eventloop();
    ~Eventloop();
    void loop();
    void updatechannel(Channel*m_ch);
};

#endif