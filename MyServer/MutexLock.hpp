#ifndef _MUTEXLOCK_H
#define _MUTEXLOCK_H

#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <assert.h>

pid_t gettid(void) { return syscall(__NR_gettid); }

class MutexLock
{

public:
    MutexLock();
    ~MutexLock();

    bool isLockedByThisThread();
    void assertLocked();
    void lock();
    void unlock();
    pthread_mutex_t *getPthreadMutex();

private:
    pthread_mutex_t mutex_;
    pid_t holder_;
};

MutexLock::MutexLock() : holder_(0)
{
    int ret = pthread_mutex_init(&mutex_, nullptr);
    assert(ret == 0);
}

MutexLock::~MutexLock()
{
    assert(holder_ == 0);
    int ret = pthread_mutex_destroy(&mutex_);
    assert(ret == 0);
}

bool MutexLock::isLockedByThisThread()
{
    return holder_ == gettid();
}

void MutexLock::assertLocked()
{
    assert(isLockedByThisThread());
}

void MutexLock::lock()
{
    pthread_mutex_lock(&mutex_);
    holder_ = gettid();
}

void MutexLock::unlock()
{
    holder_ = 0;
    pthread_mutex_unlock(&mutex_);
}

pthread_mutex_t *MutexLock::getPthreadMutex()
{
    return &mutex_;
}

class MutexLockGuard
{
public:
    explicit MutexLockGuard(MutexLock &mutex) : mutex_(mutex)
    {
        mutex_.lock();
    }

    ~MutexLockGuard()
    {
        mutex_.unlock();
    }

private:
    MutexLock &mutex_;
};

#endif // ! _MUTEXLOCK_H