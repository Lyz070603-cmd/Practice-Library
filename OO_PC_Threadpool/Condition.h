#ifndef _CONDITION_H
#define _CONDITION_H

#include "Mutexlock.h"

class Condition {
public: 
    Condition(Mutexlock & mutex);
    ~Condition();
    void wait();
    void notify();
    void notifyAll();
private: 
    pthread_cond_t _cond;
    Mutexlock &_mutex;
};

#endif //_CONDITION_H
