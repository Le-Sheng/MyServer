#include "EventLoop.hpp"

EventLoop::EventLoop() : functorlist_(),
                         activechannellist_(),
                         poller_(),
                         quit_(true),
                         tid_(std::this_thread::get_id()),
                         mutex_(),
                         wakeupfd_(eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC)),
                         wakeupchannel_()
{
    wakeupchannel_.SetFd(wakeupfd_);
    wakeupchannel_.SetEvents(EPOLLIN | EPOLLET);
    wakeupchannel_.SetRead(std::bind(&EventLoop::HandleRead, this));
    //wakeupchannel_.SetError(std::bind(&EventLoop::HandleError,this));
    AddChannel(&wakeupchannel_);
}
void EventLoop::loop()
{
    quit_ = false;
    while (!quit_)
    {
        poller_.poll(activechannellist_);
        for (Channel *pchannel : activechannellist_)
        {
            pchannel->HandleEvent();
        }

        activechannellist_.clear();
        ExecuteTask();
    }
}

void EventLoop::Wakeup()
{
    uint64_t one = 1;
    ssize_t n=write(wakeupfd_, (char *)(&one), sizeof one);
}

void EventLoop::HandleRead()
{
    uint64_t one = 1;
    ssize_t n=read(wakeupfd_, &one, sizeof one);
}

void EventLoop::AddTask(Functor fun)
{
    {
        std::lock_guard<std::mutex> lock(mutex_);

        functorlist_.push_back(fun);
    }

    Wakeup();
}

void EventLoop::ExecuteTask()
{
    std::vector<Functor> functorlist;

    {
        std::lock_guard<std::mutex> lock(mutex_);
        std::swap(functorlist, functorlist_);
    }

    for (auto &functor : functorlist)
    {
        functor();
    }

    functorlist.clear();
}