#pragma once

#include "EventLoop.hpp"
#include <arpa/inet.h>
#include <memory>
#include <string>

#define BUFSIZE 4096

int recvn(int fd, std::string &bufferin);
int sendn(int fd, std::string &bufferout);

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
    typedef std::shared_ptr<TcpConnection> spTcpConnection;
    typedef std::function<void(const spTcpConnection &, std::string&)> MessageCallback;
    typedef std::function<void(const spTcpConnection &)> Callback;

    TcpConnection(EventLoop* loop, int fd, const sockaddr_in &clientaddr);
    ~TcpConnection()
    {
        loop_->RemoveChannel(spchannel_.get());
        close(fd_);
    }

    int fd() const
    {
        return fd_;
    }

    EventLoop* GetLoop() const
    {
        return loop_;
    }

    void AddChannel()
    {
        loop_->AddTask(std::bind(&EventLoop::AddChannel, loop_, spchannel_.get()));
    }

    void Send(const std::string &s);
    void SendInLoop();
    void Shutdown();
    void ShutdownInLoop();
    void HandleRead();
    void HandleWrite();
    void HandleError();
    void HandleClose();

    void SetMessageCallback(const MessageCallback &cb)
    {
        messagecallback_ = cb;
    }
    void SetSendCompleteCallback(const Callback &cb)
    {
        sendcompletecallback_ = cb;
    }

    void SetCloseCallback(const Callback &cb)
    {
        closecallback_ = cb;
    }

    void SetErrorCallback(const Callback &cb)
    {
        errorcallback_ = cb;
    }

    void SetConnectionCleanUp(const Callback &cb)
    {
        connectioncleanup_ = cb;
    }

    void SetAsyncProcessing(const bool asyncprocessing)
    {
        asyncprocessing_ = asyncprocessing;
    }

private:
    EventLoop* loop_;
    std::unique_ptr<Channel> spchannel_;
    int fd_;

    sockaddr_in clientaddr_;
    bool halfclose_;
    bool disconnected_;
    bool asyncprocessing_;

    std::string bufferin_;
    std::string bufferout_;

    MessageCallback messagecallback_;
    Callback sendcompletecallback_;
    Callback closecallback_;
    Callback errorcallback_;
    Callback connectioncleanup_;
};

