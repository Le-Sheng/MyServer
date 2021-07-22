#pragma once

#include <unistd.h>
#include <vector>
#include <thread>
#include <memory>
#include "EventLoop.hpp"

class EventLoopThread
{
public:
    EventLoopThread();
    ~EventLoopThread()
    {
        loop_->Quit();
        th_.join();
    }

    EventLoop* GetLoop()
    {
        return loop_;
    }

    void Start()
    {

       th_=std::thread(&EventLoopThread::ThreadFunc,this);
    }

    void ThreadFunc();

private:
    std::thread th_;
    std::thread::id threadid_;

    EventLoop* loop_;
};