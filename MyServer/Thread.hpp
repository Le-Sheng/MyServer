#ifndef _THREAD_H_
#define _THREAD_H_

#include<iostream>
#include<unistd.h>
#include<pthread.h>
#include<atomic>
#include<functional>
#include<assert.h>


class Thread
{
public:
    typedef std::function<void()> ThreadFunc;
    Thread ();
    explicit Thread(const ThreadFunc &func);
    ~Thread();

    void Start();
    void Join();

    static int getNumCreated(){
        return numCreated_;
    }
private: 
    //bool auto_delete;

    bool started_;
    bool joined_;
    pid_t pid_;
    pthread_t thread_id;
    ThreadFunc func_;

    static void * thread_routine(void* arg);
    void run();
    
    static std::atomic_int numCreated_;
};

std::atomic_int Thread::numCreated_;



Thread::Thread():started_(false),joined_(false)
{
    //std::cout<<"Thread()"<<std::endl;
    numCreated_++;
};
Thread::Thread(const ThreadFunc&func):func_(func)
{
    //std::cout<<"Thread(const ThreadFunc&func)"<<std::endl;
    numCreated_++;
}
Thread::~Thread()
{
    //std::cout<<"~Thread()"<<std::endl;
};

void Thread::Start()
{
    assert(!started_);
    started_=true;
    
    pthread_create(&thread_id,nullptr,thread_routine,this);
}
void Thread::Join()
{
    assert(started_);
    assert(!joined_);
    joined_=true;
    pthread_join(thread_id,nullptr);
}

void* Thread::thread_routine(void *arg)
{
    Thread*thread=static_cast<Thread*>(arg);

    thread->run();

    return nullptr;
}

void Thread:: run()
{
   func_();
}


// class Thread_test:public Thread
// {
// public:
//     Thread_test()
//     {
//         std::cout<<"Thread_test()"<<std::endl;
//     }

//     Thread_test(int count):count_(count)
//     {
//         std::cout<<"Thread_test(int count):count_(count)"<<std::endl;
//     }

//     ~Thread_test()
//     {
//         std::cout<<"~Thread_test()"<<std::endl;
//     }

// private:

//     int count_;
//     void run()
//     {
//         while(count_--)
//         {
//             std::cout<<"thest_Thread::run()"<<std::endl;
//             sleep(1);
//         }
//     }
// };

class Foo
{
public:
    Foo(int count):count_(count){}

    void mem_fun()
    {
        while(count_--)
        {
            std::cout<<"men_fun"<<std::endl;
            sleep(1);
        }   
    }

     void mem_fun2(int x)
    {
        while(count_--)
        {
            std::cout<<"x="<<x<<" men_fun2"<<std::endl;
            sleep(1);
        }   
    }
private:
    int count_;
};

#endif