#include<iostream>
#include<vector>
#include<ctime>
#include<chrono>
#include"../MutexLock.hpp"
#include"../Thread.hpp"

using namespace std::chrono;



MutexLock mutex;
std::vector<int> v;

void func(const int num)
{
    for(int i=0;i<num;i++)
    {
        sleep(1);
        MutexLockGuard lock(mutex);
        v.push_back(i);
    }
}

int main()
{
    
    Thread t1(std::bind(func,10));
    Thread t2(std::bind(func,10));

    system_clock::duration start= system_clock::now().time_since_epoch();
    milliseconds mil_start =duration_cast<milliseconds>(start);

    t1.Start();
    t2.Start();
    for(int i=0;i<10;i++)
    {
        sleep(1);
        MutexLockGuard lock(mutex);
        v.push_back(i);
    }

    t1.Join();
    t2.Join();
    // for(int i=0;i<v.size();i++)
    // {
    //    std::cout<<v[i]<<" ";
    // }

    system_clock::duration end= system_clock::now().time_since_epoch();
    milliseconds mil_end =duration_cast<milliseconds>(end);

    std::cout<<mil_end.count()-mil_start.count()<<" millisconds"<<std::endl;

    return 0;
}