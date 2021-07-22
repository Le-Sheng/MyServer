#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include "Thread.hpp"
#include "BlockingQueue.hpp"
#include <vector>
#include <queue>
#include <functional>

class ThreadPool
{
    typedef std::function<void()> Task;

public:
    ThreadPool() : mutex_(),
                   cond_(mutex_),
                   running_(false)
    {
    }

    ~ThreadPool()
    {
        if (running_)
            stop();
    }

    void start(int numThreads)
    {
        {
            MutexLockGuard lock(mutex_);
            running_ = true;
        }
        for (int i = 0; i < numThreads; i++)
        {
            threads_.push_back(new Thread(std::bind(&ThreadPool::runInThread, this)));
            threads_[i]->Start();
        }
    }

    void stop()
    {
        {
            MutexLockGuard lock(mutex_);
            running_ = false;
            cond_.notifyAll();
        }

        for (int i = 0; i < threads_.size(); i++)
        {
            threads_[i]->Join();
            delete threads_[i];
        }
    }

    void run(const Task &task)
    {
        if (threads_.empty())
        {
            task();
        }
        else
        {
            {
                MutexLockGuard lock(mutex_);
                queue_.push(task);
            }
            cond_.notify();
        }
    }

private:
    MutexLock mutex_;
    Condition cond_;
    std::vector<Thread *> threads_;
    bool running_;
    std::queue<Task> queue_;
    Task take()
    {
        MutexLockGuard lock(mutex_); //dont forget
        while (queue_.empty() && running_)
        {
            cond_.wait();
        }

        Task tmp;
        if (!queue_.empty())
        {
            tmp = queue_.front();
            queue_.pop();
        }
        return tmp;
    }
    void runInThread()
    {
        while (running_)
        {
            Task task = take();
            if (task)
                task();
        }
    }
};

#endif // !_THREADPOOL_H