#include "EventLoopThreadPool.hpp"

EventLoopThreadPool::EventLoopThreadPool(EventLoop *mainloop, int threadnum) : mainloop_(mainloop),
                                                                                   threadnum_(threadnum),
                                                                                   threadlist_(),
                                                                                   index_(0)
{
    threadlist_.resize(threadnum_);
    for (int i = 0; i < threadnum_; i++)
    {
        threadlist_[i] = new EventLoopThread;
    }
}
EventLoopThreadPool::~EventLoopThreadPool()
{
    for (int i = 0; i < threadnum_; i++)
        delete threadlist_[i];
    threadlist_.clear();
}

void EventLoopThreadPool::Start()
{
    for (int i = 0; i < threadnum_; i++)
        threadlist_[i]->Start();
}

EventLoop *EventLoopThreadPool::GetNextLoop()
{
    if (threadnum_ <= 0)
        return mainloop_;

    EventLoop *loop = threadlist_[index_]->GetLoop();
    index_ = (index_ + 1) % threadnum_;
    return loop;
}