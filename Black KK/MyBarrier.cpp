#include "MyBarrier.h"

void MyBarrier::Wait()
{
    std::unique_lock<std::mutex> lock(aMutex);
    int gen = generation_;  
    if (--CurrentCount== 0) {
        generation_ = (generation_ + 1) % 4096;          
        CurrentCount = ThreShold;
        C_V.notify_all();
    }
    else {
        C_V.wait(lock, [this, gen] {
            return gen != generation_;
            });
    }
}

void MyBarrier::ReleaseAll()
{
    std::unique_lock<std::mutex> lock(aMutex);
    generation_ = (generation_ + 1) % 4096; 
    CurrentCount = ThreShold;               
    C_V.notify_all();                        
}
