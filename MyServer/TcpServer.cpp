#include "TcpServer.hpp"

void Setnonblocking(int fd)
{
    int opts = fcntl(fd, F_GETFL);
    if (opts < 0)
    {
        perror("fcntl(fd,GETFL)");
        exit(1);
    }
    if (fcntl(fd, F_SETFL, opts | O_NONBLOCK) < 0)
    {
        perror("fcntl(fd,SETFL,opts)");
        exit(1);
    }
}

TcpServer::TcpServer(EventLoop *loop, const int port, const int threadnum) : serversocket_(),
                                                                                 loop_(loop),
                                                                                 serverchannel_(),
                                                                                 count_(0),
                                                                                 threadpool_(loop, threadnum)
{
    serversocket_.SetReuseAddr();
    serversocket_.BindAddress(port);
    serversocket_.Listen();
    serversocket_.SetNonblocking();

    serverchannel_.SetFd(serversocket_.fd());
    serverchannel_.SetRead(std::bind(&TcpServer::OnNewConnection, this));
    serverchannel_.SetError(std::bind(&TcpServer::OnConnectionError, this));
}

void TcpServer::Start()
{
    threadpool_.Start();

    serverchannel_.SetEvents(EPOLLIN | EPOLLET);

    loop_->AddChannel(&serverchannel_);
}

void TcpServer::OnNewConnection()
{
    sockaddr_in clientaddr;
    int clientfd;

    while ((clientfd = serversocket_.Accept(clientaddr)) > 0)
    {
        std::cout << "New client from IP:" << inet_ntoa(clientaddr.sin_addr)
                  << ":" << ntohs(clientaddr.sin_port) << std::endl;

        if (++count_ >= MAXCONNECTION)
        {
            close(clientfd);
            continue;
        }
        Setnonblocking(clientfd);

        EventLoop *loop = threadpool_.GetNextLoop();

        std::shared_ptr<TcpConnection> tcpconnection = std::make_shared<TcpConnection>(loop, clientfd, clientaddr);
        tcpconnection->SetMessageCallback(messagecallback_);
        tcpconnection->SetSendCompleteCallback(sendcompletecallback_);
        tcpconnection->SetCloseCallback(closecallback_);
        tcpconnection->SetErrorCallback(errorcallback_);
        tcpconnection->SetConnectionCleanUp(std::bind(&TcpServer::RemoveConnection, this, std::placeholders::_1));

        {
            std::lock_guard<std::mutex> lock(mutex_);
            tcpconnlist_[clientfd] = tcpconnection;
        }

        newconncallback_(tcpconnection);
        tcpconnection->AddChannel();
    }
}

void TcpServer::RemoveConnection(std::shared_ptr<TcpConnection> tcpconnection)
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        --count_;
        tcpconnlist_.erase(tcpconnection->fd());
    }
}