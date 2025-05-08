#include "MyBarrier.h"

void MyBarrier::Wait()
{
    std::unique_lock<std::mutex> lock(aMutex);
    int gen = generation_;  // 记录当前屏障的“代数”
    if (--CurrentCount== 0) {
        generation_ = (generation_ + 1) % 2048;          
        CurrentCount = ThreShold;
        C_V.notify_all();
    }
    else {
        // 非最后一个线程，等待条件变量
        C_V.wait(lock, [this, gen] {
            return gen != generation_;
            });
    }
}
