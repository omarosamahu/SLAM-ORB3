#include "ThreadPool.h"
#include <chrono>
int main(int argc, char const *argv[])
{
    
    ThreadPool th1(1);
    for(int i =0 ; i < 100 ;i++){
        th1.addTasks([i]{
            // std::this_thread::sleep_for(std::chrono::milliseconds(200));
            std::cout << (i%2 == 0 ? "Thread1" : "Thread2")  << "\n";
        });

    // th1.wait();
    }
    // th1.wait();
    
    return 0;
}
