#include "ThreadPool.h"

ThreadPool::ThreadPool(std::size_t numThreads){
    
    start(numThreads);
    // thread = std::make_unique<std::thread>([this,numThreads]{
    //     isRunning = true;
    //     this->start(numThreads);    
    // });
}

ThreadPool::~ThreadPool(){
    stop();    
}

void ThreadPool::start(size_t numThreads){
    for(size_t i = 0 ; i < numThreads; ++i){
        mThreads.emplace_back([=]{
            std::unique_lock<std::mutex> lck(mtx);
            while (isRunning)
            {
                if (isRunning && tasks.empty())
                {
                    break;
                }
                
                while (!tasks.empty())
                {
                    std::cout << "Number of remaning tasks: " << tasks.size() << "\n";
                    const std::function<void()> t = tasks.front();
                    tasks.pop_front();
                    lck.unlock();
                    t();
                    lck.lock();
                }
                // itemQueue.notify_all();        
            }
            // itemQueue.notify_all();        
        });
    }
    itemQueue.notify_all();
}

void ThreadPool::addTasks(const std::function<void()>& t){
    
    std::lock_guard<std::mutex> _lck(mtx);
    tasks.push_back(t);
    itemQueue.notify_one(); // One of the threads get notified and wake up
}
void ThreadPool::wait(){
    std::unique_lock<std::mutex> _lck(mtx);
    while (!tasks.empty()){
            itemQueue.wait(_lck);
    }
    
}
void ThreadPool::stop(){
    {
        std::unique_lock<std::mutex> _lck(mtx);
        isRunning = false;
        // itemQueue.notify_one();
    }
    itemQueue.notify_all();
    for(auto& th : mThreads){
        th.join();
    }
    // Lock after join
}