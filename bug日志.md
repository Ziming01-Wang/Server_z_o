1.服务端套接字阻塞，用EPOLLET

2.客户端getline不判断EOF

3.客户端的read会一直等待服务端的write，所以会阻塞，后续要处理

4为什么引入channel啊

5.epoll_event的成员变量：
data：
void *ptr（最常用，工业级网络库的选择）
作用：存放一个内存指针。
场景：在 Reactor 模式中（如 Muduo 库），我们通常把封装了该 fd 的 Channel 对象、或者 Connection 对象的指针赋给 ptr（例如 ev.data.ptr = my_channel_ptr;）。
好处：当 epoll_wait 返回时，我们不需要拿着 fd 去红黑树或哈希表里查对应的对象，而是直接强转 Channel* ch = (Channel*)events[i].data.ptr; ch->handleEvent();。极其高效。


6.epoll_event_data的fd和ptr引用的是同一块内存，用哪个方式读就会转换为那个方式的类型；

7.epoll *ev指针里边的update会改变内核给events的ptr，对应的，ev的poll读取events的ptr的时候，相应的强转为channel ※类型

8。channel类作用是让每一个channel绑定总体的epoll类指针，channel的某些操作可以修改ep的update

8.1：eventloop是事件处理器：在main中创建，生命周期是整个程序

8.2：acceptor是时间接收器，在server中创建，生命周期=server=main



9.分离Acceptor:
    1.std：：bind的函数如果不需要传参，则可以将参数设置为占位符：std：：placeholders：：——1
    2.std：：function《A（B）》A是返回类型，B是参数列表

