//
// Created by guyan on 2020/12/11.
//

#include "ThreadPool.h"
#include <iostream>
using namespace rsvp;
/**
 * @brief The constructor of thread pool
 */
ThreadPool::ThreadPool() :
        _running(false) {

}

/**
 * @brief The descriptor of thread pool
 */
ThreadPool::~ThreadPool() {
    if(_running)
        stop();
}

/**
 * @brief Start the thread pool. Creat all threads.
 * @param threadNum : the number of threads to create
 */
void ThreadPool::start(int threadNum) {
    _running = true;
    _threads.reserve(threadNum);
    for(int i = 0; i < threadNum; i++) {
        _threads.emplace_back(&ThreadPool::threadHandler, this);
    }
}

/**
 * @brief Stop the thread pool.
 */
void ThreadPool::stop() {
    _running = false;
    _cond.notify_all();
    int threadNum = _threads.size();
    for(int i = 0; i < threadNum; i++)
        _threads[i].join();
}

/**
 * @brief Push a task to the pool
 * @param task : the task function
 */
void ThreadPool::pushTask(const std::function<void()>& task) {
    std::unique_lock<std::mutex> lock(_mutex);
    _tasks.emplace_back(task);
    _cond.notify_one();
}

/**
 * @brief The function run in every thread
 */
void ThreadPool::threadHandler() {
    while(_running) {
        std::function<void()> task(take());
        if(task)
            task();
    }
}

/**
 * @brief Take a task function from the task vector
 * @return the task
 */
std::function<void()> ThreadPool::take() {
    std::unique_lock<std::mutex> lock(_mutex);
    while(_tasks.empty() && _running) {
        _cond.wait(lock);
    }

    std::function<void()> task;
    if(!_tasks.empty()) {
        task = _tasks.back();
        _tasks.pop_back();
    }
    return task;
}




