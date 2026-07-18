#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include "buffer.h"
#include "channel.h"
#include "eventloop.h"
#include "socket.h"
#include <functional>
class Connection{
private:
    Eventloop *loop;
    Channel *m_channel;
    Socket *cl_socket;
    std::function<void(Socket*)>disconnectCallback;

    Buffer m_buffer;
public:
    Connection(Eventloop *loop,int clfd);
    ~Connection();

    void dosomething_echo(int clfd);
    void setDisconnectCallback(std::function<void(Socket*)> cb);
};

#endif