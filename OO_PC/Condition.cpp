/**
 * Project Untitled
 */


#include "Condition.h"

/**
 * Condition implementation
 */


/**
 * @param _mutex
 */
Condition::Condition(Mutexlock & mutex)
:_mutex(mutex){
    pthread_cond_init(&_cond,nullptr);
}

Condition::~Condition() {
    pthread_cond_destroy(&_cond);
}

/**
 * @return void
 */
void Condition::wait() {
    pthread_cond_wait(&_cond,_mutex.getMutexlock());
    return;
}

/**
 * @return viod
 */
void Condition::notify() {
    pthread_cond_signal(&_cond);
    return ;
}

/**
 * @return void
 */
void Condition::notifyAll() {
    pthread_cond_broadcast(&_cond);
    return;
}
