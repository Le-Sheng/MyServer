//启动一个基本的回射服务器
#include"EchoServer.hpp"
#include"EventLoop.hpp"

int main()
{
    int port=5555;
    int iothreadnum=4;

    EventLoop loop;

    EchoServer server(&loop,port,iothreadnum);
    server.Start();
    loop.loop();

    return 0;
}
