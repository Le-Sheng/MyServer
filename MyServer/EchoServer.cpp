#include "EchoServer.hpp"

EchoServer::EchoServer(EventLoop *loop, const int port, const int threadnum) : tcpserver_(loop, port, threadnum)
{
    tcpserver_.SetNewConnCallback(std::bind(&EchoServer::HandleNewConnection, this, std::placeholders::_1));
    tcpserver_.SetMessageCallback(std::bind(&EchoServer::HandleMessage, this, std::placeholders::_1, std::placeholders::_2));
    tcpserver_.SetSendCompleteCallback(std::bind(&EchoServer::HandleSendComplete, this, std::placeholders::_1));
    tcpserver_.SetCloseCallback(std::bind(&EchoServer::HandleClose, this, std::placeholders::_1));
    tcpserver_.SetErrorCallback(std::bind(&EchoServer::HandleError, this, std::placeholders::_1));
}

void EchoServer::HandleNewConnection(const pTcpConnection &tcpconn)
{
    std::cout << "new connection:" << std::endl;
}
void EchoServer::HandleMessage(const pTcpConnection &ptcpconn, std::string &str)
{
    std::string msg;
    swap(msg, str); //copy-on write;
    msg.insert(0, "reply:");
    ptcpconn->Send(msg);
}
void EchoServer::HandleSendComplete(const pTcpConnection &ptcpconn)
{
}
void EchoServer::HandleClose(const pTcpConnection &ptcpconn)
{
    std::cout << "close." << std::endl;
}
void EchoServer::HandleError(const pTcpConnection &ptcpconn)
{
    std::cout << "error" << std::endl;
}