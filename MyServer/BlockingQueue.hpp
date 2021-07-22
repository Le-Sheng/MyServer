#ifndef _BLOCKINGQUEUE_H
#define _BLOCKINGQUEUE_H

#include "Condition.hpp"
#include "MutexLock.hpp"
#include <queue>
#include <assert.h>

template <typename T>
class BlockingQueue
{
public:
    BlockingQueue() :

                      mutex_(),
                      notEmpty_(mutex_),
                      queue_()
    {
    }

    void put(const T &x)
    {
        {
            MutexLockGuard lock(mutex_);
            queue_.push(x);
        }
        notEmpty_.notify();
    }

    T take()
    {
        MutexLockGuard lock(mutex_);
        while (queue_.empty())
        {
            notEmpty_.wait();
        }
        T tmp = queue_.front();
        queue_.pop();
        return tmp;
    }

    int size()
    {
        MutexLockGuard lock(mutex_);
        return queue_.size();
    }

private:
    mutable MutexLock mutex_;
    Condition notEmpty_;
    std::queue<T> queue_;
    //const bool True=true;
};

#endif // !_BLOCKINGQUEUE_H