#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include "channel.h"
#include "eventloop.h"
#include "inetaddress.h"
#include "socket.h"
#include <functional>
class Acceptor{
private:
    Eventloop *loop;
    Channel *m_channel;
    Socket *m_socket;
    Inetaddress *m_addr;
    std::function<void(Socket*)> newconnectionCallback;

public:
    Acceptor(Eventloop *loop);
    ~Acceptor();
    void acceptConnection();
    void setcallback(std::function<void(Socket*)> cb);
};

#endif