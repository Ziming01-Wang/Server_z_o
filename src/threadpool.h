#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__


#include <condition_variable>
#include <functional>
#include <map>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
class Threadpool{
private:
    std::vector<std::thread> m_threadpool;
    std::queue<std::function<void()>> queue_tasks;
    std::condition_variable cv;
    std::mutex task_mutex;
    bool stop;

public:
    Threadpool(int num=5);
    ~Threadpool();
    void add_task(std::function<void()> task);

};

#endif