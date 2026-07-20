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

10.线程池：
    bool的stop标志修改也需要上锁
    ~threadpool需要点join
    条件变量的wait后边是判断继续的标志
    Bug 2 到底错在哪
{
channel.cpp
Lines 52-55
void Channel::handleEvent(){
    //callback();
    loop->addthread(callback);   // 把回调丢给线程池
}
配合 EPOLLET，有两个致命点：

同一连接被并发执行：主线程投递任务后立刻回到 epoll_wait。同一个 fd 只要再来数据就会再次触发，于是同一个 Connection::dosomething_echo 可能同时在两个 worker 上跑 → 两个线程同时 read/write 同一 fd、同时改 m_buffer。
use-after-free：EOF 分支里 disconnectCallback → delete Connection。若同 fd 两个事件在两个 worker，一个已经 delete 了 Connection，另一个还在用 this/m_buffer → 崩溃。
根因：每个连接的读写、以及 accept、epoll_ctl，本应在主线程串行做；线程池只适合放「与连接状态无关的纯计算」。Echo 是纯 IO，本不该进线程池。}

11.线程池优化：
