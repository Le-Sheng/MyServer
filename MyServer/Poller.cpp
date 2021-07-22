#include "Poller.hpp"

void Poller::poll(ChannelList &activeChannellist)
{
    int nfds_ = epoll_wait(pollfd_, &*eventlist_.begin(), (int)eventlist_.size(), 1000);

    for (int i = 0; i < nfds_; i++)
    {
        int events = eventlist_[i].events;

        Channel *pchannel = (Channel *)eventlist_[i].data.ptr;
        pchannel->SetEvents(eventlist_[i].events);
        activeChannellist.push_back(pchannel);
    }
    if (nfds_ == eventlist_.size())
        eventlist_.resize(nfds_ * 2);
}

void Poller::addChannel(Channel *pchannel)
{
    int fd = pchannel->GetFd();
    epoll_event ev;
    ev.events = pchannel->GetEvents();
    ev.data.ptr = pchannel;

    epoll_ctl(pollfd_, EPOLL_CTL_ADD, fd, &ev);
}

void Poller::RemoveChannel(Channel *pchannel)
{
    int fd = pchannel->GetFd();
    epoll_event ev;
    ev.events = pchannel->GetEvents();
    ev.data.ptr = pchannel;

    epoll_ctl(pollfd_, EPOLL_CTL_DEL, fd, &ev);
}
void Poller::UpdateChannel(Channel *pchannel)
{
    int fd = pchannel->GetFd();
    epoll_event ev;
    ev.events = pchannel->GetEvents();
    ev.data.ptr = pchannel;

    epoll_ctl(pollfd_, EPOLL_CTL_MOD, fd, &ev);
}