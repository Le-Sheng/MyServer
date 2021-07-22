#pragma once

#include "TcpConnection.hpp"
#include "EventLoopThreadPool.hpp"
#include "Socket.hpp"
#include <mutex>
#include <unordered_map>
#include <iostream>

#define MAXCONNECTION 20000

void Setnonblocking(int fd);

class TcpServer
{
public:
    typedef std::shared_ptr<TcpConnection> spTcpConnection;

    typedef std::function<void(const spTcpConnection &, std::string &)> MessageCallback;
    typedef std::function<void(const spTcpConnection &)> Callback;

    TcpServer(EventLoop *loop, const int port, const int threadnum = 0);
    ~TcpServer()
    {
    }

    void Start();
    void SetNewConnCallback(const Callback &cb)
    {
        newconncallback_ = cb;
    }

    //注册数据回调函数
    void SetMessageCallback(const MessageCallback &cb)
    {
        messagecallback_ = cb;
    }

    //注册数据发送完成回调函数
    void SetSendCompleteCallback(const Callback &cb)
    {
        sendcompletecallback_ = cb;
    }

    //注册连接关闭回调函数
    void SetCloseCallback(const Callback &cb)
    {
        closecallback_ = cb;
    }

    //注册连接异常回调函数
    void SetErrorCallback(const Callback &cb)
    {
        errorcallback_ = cb;
    }

private:
    Socket serversocket_;
    EventLoop *loop_;
    Channel serverchannel_;
    int count_;

    std::unordered_map<int, std::shared_ptr<TcpConnection>> tcpconnlist_;
    std::mutex mutex_;
    EventLoopThreadPool threadpool_;

    Callback newconncallback_;
    Callback sendcompletecallback_;
    Callback closecallback_;
    Callback errorcallback_;
    MessageCallback messagecallback_;

    void OnNewConnection();
    void RemoveConnection(std::shared_ptr<TcpConnection> tcpconnection);

    void OnConnectionError()
    {
        serversocket_.Close();
    }
};