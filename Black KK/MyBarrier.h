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
    void ReleaseAll();//�ͷ������߳�
private:
    std::mutex aMutex;
    std::condition_variable C_V;
    const int ThreShold;        // ��Ҫ���߳�����
    std::atomic<int> CurrentCount; // ��ǰʣ��δ������߳���
    int generation_;             // ���ϵĴ�������ֹ��ٻ��ѣ�
};

