#pragma once
#pragma comment(lib, "pthreadVC2.lib")

#include <pthread.h>

typedef unsigned long long ull;
typedef unsigned uint;

// C++ Qt style mutex 
struct Mutex {
    pthread_mutex_t _mu;
    Mutex() { pthread_mutex_init(&_mu, nullptr); }
    Mutex(const Mutex&) = delete;
    Mutex& operator=(const Mutex&) = delete;
    void lock() { pthread_mutex_lock(&_mu); }
    void unlock() { pthread_mutex_unlock(&_mu); }
    ~Mutex() { pthread_mutex_destroy(&_mu); }
};

template <typename M>
struct Locker {
    M* _mu;
    Locker(M* _mu) : _mu(_mu) { _mu->lock(); }
    Locker(const Locker&) = delete;
    Locker& operator=(const Locker&) = delete;
    ~Locker() { _mu->unlock(); }
};

struct CondVar {
    pthread_cond_t _cond;
    CondVar() { pthread_cond_init(&_cond, nullptr); }
    void wait(Mutex* mu) { pthread_cond_wait(&_cond, &mu->_mu); }
    void wake_one() { pthread_cond_signal(&_cond); }
    void wake_all() { pthread_cond_broadcast(&_cond); }
    ~CondVar() { pthread_cond_destroy(&_cond); }
};