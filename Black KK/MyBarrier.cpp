#include "MyBarrier.h"

void MyBarrier::Wait()
{
    std::unique_lock<std::mutex> lock(aMutex);
    int gen = generation_;  // ��¼��ǰ���ϵġ�������
    if (--CurrentCount== 0) {
        generation_ = (generation_ + 1) % 2048;          
        CurrentCount = ThreShold;
        C_V.notify_all();
    }
    else {
        // �����һ���̣߳��ȴ���������
        C_V.wait(lock, [this, gen] {
            return gen != generation_;
            });
    }
}
