#include <iostream>
#include <thread>
#include <mutex>
#include <list>
#include <functional>
#include <condition_variable>
#include <vector>

class ThreadPool{
public:
    //ThreadPool() = default;
    explicit ThreadPool(std::size_t size);
    ~ThreadPool();

    void addTasks(const std::function<void()>& t);
    void wait();
private:
    void start(size_t numThreads);
    void stop();
private:
    bool isRunning;
    std::mutex mtx;
    std::vector<std::thread> mThreads;
    std::list<std::function<void()>> tasks;
    // std::unique_ptr<std::thread> thread;
    std::condition_variable itemQueue;
    
};