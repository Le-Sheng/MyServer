//简易原子变量模板类
#ifndef _ATOMIC_H
#define _ATOMIC_H

#include <unistd.h>
#include <stdint.h>

template <typename T>
class Atomic
{
public:
    Atomic() : value_(0) {}
    T get()
    {
        return __sync_val_compare_and_swap(&value_, 0, 0);
    }

    T getAndAdd(T x)
    {
        return __sync_fetch_and_add(&value_, x);
    }

    T addAndGet(T x)
    {
        return getAndAdd(x) + x;
    }

    T increAndGet()
    {
        return addAndGet(1);
    }

    T decreAndGet()
    {
        return addAndGet(-1);
    }

    void add(T x)
    {
        getAndGet(x);
    }

    void incre()
    {
        increAndGet();
    }
    void decre()
    {
        decreAndGet();
    }

    T getAndSet(T newvalue)
    {
        return __sync_lock_test_and_set(&value_, newvalue);
    }

private:
    volatile T value_;
};

typedef Atomic<int> AtomicInt;

#endif
