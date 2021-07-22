#pragma once

#include "EventLoopThread.hpp"
#include "EventLoop.hpp"
#include <vector>

class EventLoopThreadPool
{
public:
    EventLoopThreadPool(EventLoop *mainloop, int threadnum = 0);
    ~EventLoopThreadPool();
    void Start();
    EventLoop *GetNextLoop();

private:
    EventLoop *mainloop_;

    std::vector<EventLoopThread*> threadlist_;

    int threadnum_;

    int index_;
};