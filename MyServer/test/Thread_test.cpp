#include"../Thread.hpp"

void func()
{
    std::cout<<"func....."<<std::endl;
}

void func2(int count)
{
    while(count--)
    {
        std::cout<<"func2....."<<std::endl;
       sleep(1);
    }
}
int main()
{
    
    Thread *t1=new Thread(func);
    Thread *t2=new Thread(std::bind(func2,3));
    Foo foo(3);
    Thread t3(std::bind(&Foo::mem_fun,&foo));
    Foo foo2(3);
    Thread t4(std::bind(&Foo::mem_fun2,&foo2,1000));

    //t1->SetAutoDelete(true);
    //t2->SetAutoDelete(true);
    
    std::cout<<Thread::getNumCreated()<<std::endl;

    t1->Start();
    t2->Start();
    t3.Start();
    t4.Start();

    t1->Join();
    t2->Join();
    t3.Join();
    t4.Join();

    delete t1;
    delete t2;
    // delete t3;

    std::cout<<"main exit()"<<std::endl;

    return 0;
}