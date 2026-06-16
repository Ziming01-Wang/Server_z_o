#ifndef _CHANNEL_H_
#define _CHANNEL_H_
#include "epoll.h"
#include "eventloop.h"
#include <cstdint>
#include <functional>
#include <sys/types.h>
class Channel{
private:
//    Epoll *ep;
    Eventloop *loop;
    int fd;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;
    std::function<void()> callback;
public:
    void enableReading();
    Channel(Eventloop *loop, int fd);
    ~Channel();
    int getfd();
    uint32_t getEvents();
    uint32_t getRevents();
    bool getInpoll();
    void setInpoll();

    void setRevents(uint32_t m_ev);
    void setcallback(std::function<void()> m_call);
    void handleEvent();
};
#endif