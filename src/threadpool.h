#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__


#include <condition_variable>
#include <functional>
#include <future>
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
//    void add_task(std::function<void()> task);
    template<class F,class... Args>
    auto add_task(F&&f,Args&&...args)->std::future<typename std::result_of<F(Args...)>::type>{
        /*{   
            std::unique_lock<std::mutex>lock(task_mutex);
            if(stop)throw std::runtime_error("ThreadPool already stop, can't add task any more");
            queue_tasks.emplace(task);
        }
        cv.notify_one();*/
    
        using resultype=typename std::result_of<F(Args...)>::type;
        auto task=std::make_shared<std::packaged_task<resultype()>>(std::bind(std::forward<F>(f),std::forward<Args>(args)...)) ;
        
        std::future<resultype>m_future=task->get_future();
        {   
            std::unique_lock<std::mutex>lock(task_mutex);
            if(stop)throw std::runtime_error("ThreadPool already stop, can't add task any more");
            queue_tasks.emplace([task]{(*task)();});
        }
        cv.notify_one();
    
        return m_future;
    
    }
};

#endif