#pragma once

#include <unistd.h>
#include <vector>
#include <mutex>
#include <map>
#include <sys/epoll.h>
#include "Channel.hpp"

class Poller
{
public:
    typedef std::vector<Channel *> ChannelList;

    Poller() : pollfd_(-1),
               eventlist_(4096),
               channelmap_(),
               mutex_()
    {
        pollfd_ = epoll_create(256);
    }
    ~Poller()
    {
        close(pollfd_);
    }

    void poll(ChannelList &activeChannellist);
    void addChannel(Channel *pchannel);
    void RemoveChannel(Channel *pchannel);
    void UpdateChannel(Channel *pchannel);

private:
    int pollfd_;
    std::vector<epoll_event> eventlist_;
    std::map<int, Channel *> channelmap_;
    std::mutex mutex_;
};