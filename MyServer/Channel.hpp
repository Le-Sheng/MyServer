#pragma once

#include <functional>
#include <sys/epoll.h>
class Channel
{
public:
    typedef std::function<void()> Callback;

    Channel() : fd_(-1){};
    ~Channel(){};

    void SetFd(int fd)
    {
        fd_ = fd;
    }
    int GetFd()
    {
        return fd_;
    }
    void SetEvents(int events)
    {
        events_ = events;
    }
    int GetEvents() const
    {
        return events_;
    }

    void HandleEvent();

    void SetRead(const Callback &cb)
    {
        readCallback_ = cb;
    }
    void SetWrite(const Callback &cb)
    {
        writeCallback_ = cb;
    }
    void SetError(const Callback &cb)
    {
        errorCallback_ = cb;
    }
    void SetClose(const Callback &cb)
    {
        closeCallback_ = cb;
    }

private:
    int fd_;
    int events_;

    Callback readCallback_;
    Callback writeCallback_;
    Callback errorCallback_;
    Callback closeCallback_;
};