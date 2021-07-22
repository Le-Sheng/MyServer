#pragma once

#include <pthread.h>
#include <stdlib.h>

template <typename T>
class Singelton
{
public:
    Singleton() = delete;
    ~Singleton() = delete;
    static T &instance()
    {
        pthread_once(&ponce, &Singelton::init);
        return *value_;
    }

private:
    static void init()
    {
        value_ = new T();
        ::atexit(destory);
    }

    static void destory()
    {
        delete value_;
        value_ = nullptr;
    }

private:
    static T *value_;
    static pthread_once_t ponce_;
};

template<typename T>
T* Singelton<T>::value_=nullptr;

template<typename T>
pthread_once_t Singelton<T>::ponce_=PTHREAD_ONCE_INIT;