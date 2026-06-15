#ifndef _CHANNEL_H_
#define _CHANNEL_H_
#include "epoll.h"
#include <cstdint>
#include <sys/types.h>
class Channel{
private:
    Epoll *ep;
    int fd;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;
public:
    void enableReading();
    Channel(Epoll *ep, int fd);
    ~Channel();
    int getfd();
    uint32_t getEvents();
    uint32_t getRevents();
    bool getInpoll();
    void setInpoll();
    void setRevents(uint32_t m_ev);

};
#endif