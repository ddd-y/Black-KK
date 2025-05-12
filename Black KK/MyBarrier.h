#pragma once
#include<mutex>
#include <condition_variable>
#include <atomic>
class MyBarrier
{
public:
    MyBarrier(int count)
        : ThreShold(count),
        CurrentCount(count),
        generation_(0) {
    }
    void Wait();
    void ReleaseAll();//释放所有线程
private:
    std::mutex aMutex;
    std::condition_variable C_V;
    const int ThreShold;        // 需要的线程总数
    std::atomic<int> CurrentCount; // 当前剩余未到达的线程数
    int generation_;             // 屏障的代数（防止虚假唤醒）
};

