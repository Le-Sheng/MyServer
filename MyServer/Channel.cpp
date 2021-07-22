#include "Channel.hpp"

void Channel::HandleEvent()
{
    if (events_ & EPOLLRDHUP)
        closeCallback_();
    else if (events_ & (EPOLLIN | EPOLLPRI))
        readCallback_();
    else if (events_ & EPOLLOUT)
        writeCallback_();
    else
        errorCallback_();
}