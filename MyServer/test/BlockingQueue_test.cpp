#include"../BlockingQueue.hpp"
#include"../Latch.hpp"
#include"../Thread.hpp"
#include<string>
#include<vector>


pid_t gettid(void){return syscall(__NR_gettid);}

class test
{
    public:

    test(int num):latch_(num)
    {
        for(int i=0;i<num;i++)
        {
            threads_.push_back(new Thread(std::bind(&test::func,this)));
        }
        for(int i=0;i<num;i++)
        {
            threads_[i]->Start();
        }
    }

    void run(int numOfProducts)
    {
        latch_.wait();
        const std::string s="product ";
        std::string put;
        for(int i=0;i<numOfProducts;i++)
        {
            put=s+std::to_string(i);
            q_.put(put);
            //std::cout<<"main put "<<put<<std::endl;
            //usleep(1000);
        }
        for(int i=0;i<threads_.size();i++)
        {
            q_.put("quit");
        }
    }

    void joinAll()
    {
        for(int i=0;i<threads_.size();i++)
        {
            threads_[i]->Join();
            std::cout<<"join"<<std::endl;
        }
        for(int i=0;i<threads_.size();i++)
        {
            delete threads_[i];
        }
    }

    private:
    BlockingQueue<std::string> q_;
    CountDownLatch latch_;
    std::vector<Thread*>threads_;

    void func()
    {
        latch_.countDown();
        while(true)
        {
            std::string s=q_.take();
            if(s=="quit")
            break;
            std::cout<<"thread:"<<gettid()<<" take "<<s<<std::endl;
        }
    }
};


int main()
{
    test t(3);

    t.run(1000);
    t.joinAll();

    return 0;

}
