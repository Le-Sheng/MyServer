# MyServer
基于C++11的多线程服务器

## 已实现功能：
- 使用线程池模型，分为主线程，IO线程和业务线程。
- 采用智能指针管理对象资源。
- 对需要实现线程安全的内置数据类型采用C++11提供的原子变量以减少锁竞争。
- 采用epoll的IO复用机制。使用ET模式以减少事件通知。
- 封装Socket的基本操作以使使用更加便捷。
- 
## 预计：
- 已实现一个基本的回射服务器作为测试，预计实现一个实用的http服务器。
- 预计在实际测试后增肌LT模式供用户选择。
- 预计增加线程池的动态管理线程数量功能，使性能进一步提升。
- 预计增加定时功能，从而可以定期检查不活跃的链接，将其从epoll的中移除。
- 预计增加日志库。

## Build：
```
$make
$make clean
```

