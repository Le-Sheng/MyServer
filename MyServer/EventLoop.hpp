#pragma once

#include <functional>
#include <thread>
#include <mutex>
#include "Poller.hpp"
#include "Channel.hpp"
#include <sys/eventfd.h>

class EventLoop
{
public:
    typedef std::function<void()> Functor;
    typedef std::vector<Channel *> ChannelList;

    EventLoop();
    ~EventLoop()
    {
        close(wakeupfd_);
    }

    void loop();

    void AddChannel(Channel *pchannel)
    {
        poller_.addChannel(pchannel);
    }

    void RemoveChannel(Channel *pchannel)
    {
        poller_.RemoveChannel(pchannel);
    }

    void UpdateChannel(Channel *pchannel)
    {
        poller_.UpdateChannel(pchannel);
    }

    void Quit()
    {
        quit_ = true;
    }

    std::thread::id GetThreadId() const
    {
        return tid_;
    }

    void Wakeup();
    void HandleRead();
    void AddTask(Functor fun);
    void ExecuteTask();

private:
    std::vector<Functor> functorlist_;
    ChannelList activechannellist_;

    Poller poller_;
    bool quit_;

    std::thread::id tid_;
    std::mutex mutex_;

    int wakeupfd_;

    Channel wakeupchannel_;
};