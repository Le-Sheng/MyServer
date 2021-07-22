#pragma once

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>

class Socket
{
public:
    Socket()
    {
        serverfd_ = socket(AF_INET, SOCK_STREAM, 0);
    }
    ~Socket()
    {
        close(serverfd_);
    }
    int fd() const
    {
        return serverfd_;
    }
    void SetReuseAddr();
    void SetNonblocking();
    void BindAddress(int serverport);
    void Listen();
    int Accept(sockaddr_in &clientaddr);

    bool Close()
    {
        close(serverfd_);
        return true;
    }

private:
    int serverfd_;
};