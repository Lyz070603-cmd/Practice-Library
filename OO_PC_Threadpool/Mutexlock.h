#ifndef _MUTEXLOCK_H
#define _MUTEXLOCK_H

#include <pthread.h>

class Mutexlock {
public: 
    Mutexlock();
    ~Mutexlock();
    void lock();
    void unlock();
    void trylock();
    pthread_mutex_t * getMutexlock();
private: 
    pthread_mutex_t _mutex;
};

#endif //_MUTEXLOCK_H
