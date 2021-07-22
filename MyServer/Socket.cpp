#include "Socket.hpp"

void Socket::SetReuseAddr()
{
    int on = 1;
    setsockopt(serverfd_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
}
void Socket::SetNonblocking()
{
    int opts = fcntl(serverfd_, F_GETFL);
    fcntl(serverfd_, F_SETFL, opts | O_NONBLOCK);
}
void Socket::BindAddress(int serverport)
{
    sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(serverport);
    bind(serverfd_, (sockaddr *)&serveraddr, sizeof(serveraddr));
}
void Socket::Listen()
{
    listen(serverfd_, 8192);
}
int Socket::Accept(sockaddr_in &clientaddr)
{
    socklen_t lenofsockaddr = sizeof(clientaddr);
    int clientfd = accept(serverfd_, (sockaddr *)&clientaddr, &lenofsockaddr);
    return clientfd;
}