#include "src/eventloop.h"
#include "src/server.h"

int main(){
    Eventloop *m_eventloop=new Eventloop();
    Server m_server(m_eventloop);
    m_eventloop->loop();
}