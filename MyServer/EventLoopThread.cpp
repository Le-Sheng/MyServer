#include "EventLoopThread.hpp"

EventLoopThread::EventLoopThread() : th_(),
                                     threadid_(-1),
                                     loop_(nullptr)
{
}

void EventLoopThread::ThreadFunc()
{
    EventLoop loop;
    loop_ = &loop;

    threadid_ = std::this_thread::get_id();
    loop_->loop();
}