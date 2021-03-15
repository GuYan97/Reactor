//
// Created by guyan on 2020/12/11.
//

#ifndef RSVP_THREADPOOL_H
#define RSVP_THREADPOOL_H
#include <thread>
#include <vector>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
namespace rsvp {
/**
 * @brief a simple thread pool implement
 */
class ThreadPool {
public:
    static ThreadPool& getInstance() {
        static ThreadPool threadPool;
        return threadPool;
    }
    ~ThreadPool();
    void start(int threadNum);
    void stop();
    void pushTask(const std::function<void()>& task);
private:
    explicit ThreadPool();

    std::function<void()> take();
    void threadHandler();
    std::vector<std::thread> _threads;
    std::vector<std::function<void()>> _tasks;
    std::mutex _mutex;
    std::condition_variable _cond;
    std::atomic<bool> _running;
};
}



#endif //RSVP_THREADPOOL_H
