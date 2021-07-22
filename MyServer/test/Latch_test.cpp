#include"../Latch.hpp"
#include"../Thread.hpp"
#include"../MutexLock.hpp"
#include<vector>

CountDownLatch latch(1);
MutexLock mutex;

void latch_wait()
{
    latch.wait();

    {
        MutexLockGuard lock(mutex);
        std::cout<<"thread:"<<gettid()<<" is running"<<std::endl;
    }
    sleep(1);
}

int main()
{
    std::vector<Thread*> threads;

    for(int i=0;i<3;i++)
    {
        threads.push_back(new Thread(std::bind(latch_wait)));

    }
    for(int i=0;i<3;i++)
    {
        threads[i]->Start();
    }
    sleep(3);

    latch.countDown();


    for(int i=0;i<3;i++)
    {
        threads[i]->Join();
        delete threads[i];
    }

    std::cout<<Thread::getNumCreated()<<std::endl;
    return 0;
}