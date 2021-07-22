#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <functional>
#include "TcpServer.hpp"

class EchoServer
{
public:
    typedef std::shared_ptr<TcpConnection> pTcpConnection;

    EchoServer(EventLoop *loop, const int port, const int threadnum);
    ~EchoServer() {}
    void Start()
    {
        tcpserver_.Start();
    }

private:
    void HandleNewConnection(const pTcpConnection &tcpconn);
    void HandleMessage(const pTcpConnection &ptcpconn, std::string &str);
    void HandleSendComplete(const pTcpConnection &ptcpconn);
    void HandleClose(const pTcpConnection &ptcpconn);
    void HandleError(const pTcpConnection &ptcpconn);

    TcpServer tcpserver_;
};