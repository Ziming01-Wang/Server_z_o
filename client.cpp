#include "client.h"
#include "socket.h"
#include "inetaddress.h"
#include "util.h"
#include <sys/socket.h>
Client::Client(){
    /*int clinfd=socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in cl_addr;
    bzero(&cl_addr, sizeof(cl_addr));
    cl_addr.sin_port=htons(8888);
    cl_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    cl_addr.sin_family=AF_INET;
    //bind(clinfd,(sockaddr*)&cl_addr,sizeof(cl_addr));

    errif(connect(clinfd, (sockaddr*)&cl_addr, sizeof(cl_addr))==-1,"connect faliure");*/

    Socket clsock;
    Inetaddress soaddr("127.0.0.1",8888);
    clsock.connect(soaddr);

    while (true) {
        char buf[1024];//buffer
        bzero(&buf,sizeof(buf));

        //std::cin>>buf;
        if(!std::cin.getline(buf, sizeof(buf))){//输入结束(EOF/Ctrl+D)，退出循环，避免空数据死循环
            std::cout<<"input closed, bye"<<std::endl;
            break;
        }
        int isw=write(clsock.getfd(), &buf, sizeof(buf));
        if(isw==-1){
            printf("socket already disconnected, can't write any more!\n");
            break;
        }

        char bufread[1024];//buffer
        bzero(&bufread,sizeof(bufread));
        ssize_t readbyte=read(clsock.getfd(),&bufread,sizeof(bufread));

        if(readbyte>0){
            std::cout<<bufread<<std::endl;
        }else if(readbyte==0){
            std::cout<<"safe close"<<std::endl;
            close(clsock.getfd());
            break;
        }else if(readbyte==-1){
            close(clsock.getfd());
            errif(true, "read error");
            break;
        }
    }
    //close(clinfd);
}


int main(){
    Client A;
}
