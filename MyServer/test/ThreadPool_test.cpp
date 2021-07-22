#include"../ThreadPool.hpp"
#include"../Latch.hpp"

void print()
{
    std::cout<<gettid()<<" is running."<<std::endl;
    //usleep(1000);
}
void func(int i)
{
    std::cout<<gettid()<<" is running func"<<i<<std::endl;
    //usleep(1000);
}

int main()
{
    ThreadPool pool;
    pool.start(3);
    pool.run(print);
    pool.run(print);

    for(int i=0;i<100;i++)
        pool.run(print);

    for(int i=0;i<100;i++)
        pool.run(std::bind(func,i));
    

    CountDownLatch latch(1);
    pool.run(std::bind(&CountDownLatch::countDown,&latch));
    latch.wait();
    pool.stop();

    
    return 0;
}