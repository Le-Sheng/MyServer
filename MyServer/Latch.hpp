#ifndef _LATCH_H
#define _LATCH_H

#include"MutexLock.hpp"
#include"Condition.hpp"

class CountDownLatch
{
public:
    explicit CountDownLatch(int count):
    count_(count),
    mutex_(),
    condition_(mutex_)
    {
    }

    ~CountDownLatch()
    {
    }

    void wait()
    {
        MutexLockGuard lock(mutex_);
        while(count_>0)
        {
            condition_.wait();
        }
    }

    void countDown()
    {
        MutexLockGuard lock(mutex_);
        --count_;
        if(count_==0)
        condition_.notifyAll();
    }

    int getCount() const
    {
        MutexLockGuard lock(mutex_);//mutex_ is mutable
        return count_;
    }

private:
    mutable MutexLock mutex_;
    Condition  condition_;
    int count_;
};

#endif // !_LATCH_H