#include "TcpConnection.hpp"

int recvn(int fd, std::string &bufferin)
{
    int nbyte = 0;
    int readsum = 0;
    char buffer[BUFSIZE];

    while (true)
    {
        nbyte = read(fd, buffer, BUFSIZE);

        if (nbyte > 0)
        {
            bufferin.append(buffer, nbyte);
            readsum += nbyte;

            if (nbyte < BUFSIZE)
                return readsum;
            else
                continue;
        }
        else if (nbyte < 0)
        {
            if (errno == EAGAIN)
                return readsum;
            else if (errno == EINTR)
                continue;
            else
                return -1;
        }
        else
            return 0;
    }
}

int sendn(int fd, std::string &bufferout)
{
    ssize_t nbyte = 0;
    int sendsum = 0;

    size_t length = 0;

    length = bufferout.size();
    if (length >= BUFSIZE)
        length = BUFSIZE;

    while (true)
    {
        nbyte = write(fd, bufferout.c_str(), length);

        if (nbyte > 0)
        {
            sendsum += nbyte;

            bufferout.erase(0, nbyte);

            length = bufferout.size();
            if (length >= BUFSIZE)
                length == BUFSIZE;
            if (length == 0)
                return sendsum;
        }
        else if (nbyte < 0)
        {
            if (errno == EAGAIN)
                return sendsum;
            else if (errno == EINTR)
                continue;
            else if (errno == EPIPE)
                return -1;
            else
                return -1;
        }
        else
            return 0;
    }
}

TcpConnection::TcpConnection(EventLoop *loop, int fd, const sockaddr_in &clientaddr) : loop_(loop),
                                                                                       spchannel_(std::make_unique<Channel>()),
                                                                                       fd_(fd),
                                                                                       clientaddr_(clientaddr),
                                                                                       halfclose_(false),
                                                                                       disconnected_(false),
                                                                                       asyncprocessing_(false),
                                                                                       bufferin_(),
                                                                                       bufferout_()
{
    spchannel_->SetFd(fd_);
    spchannel_->SetEvents(EPOLLIN | EPOLLET);
    spchannel_->SetRead(std::bind(&TcpConnection::HandleRead, this));
    spchannel_->SetWrite(std::bind(&TcpConnection::HandleWrite, this));
    spchannel_->SetClose(std::bind(&TcpConnection::HandleClose, this));
    spchannel_->SetError(std::bind(&TcpConnection::HandleError, this));
}

void TcpConnection::Send(const std::string &s)
{
    bufferout_ += s;
    if (loop_->GetThreadId() == std::this_thread::get_id())
    {
        SendInLoop();
    }
    else
    {
        asyncprocessing_ = false;
        loop_->AddTask(std::bind(&TcpConnection::SendInLoop, shared_from_this()));
    }
}
void TcpConnection::SendInLoop()
{
    if (disconnected_)
        return;
    int result = sendn(fd_, bufferout_);
    if (result > 0)
    {
        unsigned int events = spchannel_->GetEvents();
        if (bufferout_.length() > 0)
        {
            spchannel_->SetEvents(events | EPOLLOUT);
            loop_->UpdateChannel(spchannel_.get());
        }
        else
        {
            spchannel_->SetEvents(events & (~EPOLLOUT));
            sendcompletecallback_(shared_from_this());
            if (halfclose_)
                HandleClose();
        }
    }
    else
        HandleClose();
}

void TcpConnection::Shutdown()
{
    if (loop_->GetThreadId() == std::this_thread::get_id())
        ShutdownInLoop();
    else
        loop_->AddTask(std::bind(&TcpConnection::ShutdownInLoop, shared_from_this()));
}

void TcpConnection::ShutdownInLoop()
{
    if (disconnected_)
        return;
    closecallback_(shared_from_this());
    loop_->AddTask(std::bind(connectioncleanup_, shared_from_this()));
}

void TcpConnection::HandleRead()
{
    int result = recvn(fd_, bufferin_);

    if (result > 0)
    {
        messagecallback_(shared_from_this(), bufferin_);
    }
    else if (result == 0)
        HandleClose();
    else
        HandleError();
}

void TcpConnection::HandleWrite()
{
    int result = sendn(fd_, bufferout_);
    if (result > 0)
    {
        unsigned int events = spchannel_->GetEvents();
        if (bufferout_.length() > 0)
        {
            spchannel_->SetEvents(events | EPOLLOUT);
            loop_->UpdateChannel(spchannel_.get());
        }
        else
        {
            spchannel_->SetEvents(events & (~EPOLLOUT));
            sendcompletecallback_(shared_from_this());
            if (halfclose_)
                HandleClose();
        }
    }
    else if (result == 0)
        HandleClose();
    else
        HandleError();
}
void TcpConnection::HandleError()
{
    if (disconnected_)
        return;
    errorcallback_(shared_from_this());
    loop_->AddTask(std::bind(connectioncleanup_, shared_from_this()));
    disconnected_ = true;
}
void TcpConnection::HandleClose()
{
    if (disconnected_)
        return;
    if (bufferout_.size() > 0 || bufferin_.size() > 0 || asyncprocessing_)
    {
        halfclose_ = true;
        if (bufferin_.size() > 0)
            messagecallback_(shared_from_this(), bufferin_);
    }
    else
    {
        loop_->AddTask(std::bind(connectioncleanup_, shared_from_this()));
        closecallback_(shared_from_this());
        disconnected_ = true;
    }
}