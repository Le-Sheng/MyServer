#include"../Atomic.hpp"
#include<iostream>
#include<assert.h>

int main()
{
    AtomicInt a0;

    assert(a0.get()==0);
    assert(a0.getAndAdd(1)==0);
    assert(a0.get()==1);
    assert(a0.addAndGet(2)==3);
    assert(a0.get()==3);
    assert(a0.increAndGet()==4);
    assert(a0.get()==4);
    a0.incre();
    assert(a0.get()==5);
    assert(a0.getAndSet(100)==5);
    assert(a0.get()==100);

    std::cout<<"Atomic sucessful!"<<std::endl;
}