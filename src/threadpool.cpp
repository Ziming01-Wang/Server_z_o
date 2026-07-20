#include "threadpool.h"
#include <functional>
#include <linux/falloc.h>
#include <mutex>
#include <thread>
Threadpool::Threadpool(int size){
    stop=false;
    for(int i=0;i<size;i++){
        m_threadpool.emplace_back(std::thread([this](){
            while(1){
                std::function<void()>task;
                {
                    std::unique_lock<std::mutex> lock(task_mutex);
                    cv.wait(lock,[this]{
                        return stop||!queue_tasks.empty();
                    });
                    if(stop&&queue_tasks.empty())return;
                    task=queue_tasks.front();
                    queue_tasks.pop();
                }
                task();
            }
        }));
    }
}

Threadpool::~Threadpool(){
    {
        std::unique_lock<std::mutex> lock(task_mutex);
        stop = true;
    }
    cv.notify_all();

    for (std::thread &t : m_threadpool)
        if (t.joinable()) t.join();
}

