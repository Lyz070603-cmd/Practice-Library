/**
 * Project Untitled
 */


#include "Mutexlock.h"

/**
 * Mutexlock implementation
 */


Mutexlock::Mutexlock() {
    pthread_mutex_init(&_mutex,NULL);
}

/**
 * @return void
 */
void Mutexlock::lock() {
    pthread_mutex_lock(&_mutex);
    return;
}

/**
 * @return void
 */
void Mutexlock::unlock() {
    pthread_mutex_unlock(&_mutex);
    return;
}

/**
 * @return void
 */
void Mutexlock::trylock() {
    pthread_mutex_trylock(&_mutex);
    return;
}

/**
 * @return pthread_mutex_t *
 */
pthread_mutex_t * Mutexlock::getMutexlock() {
    return &_mutex;
}

Mutexlock::~Mutexlock() {
    pthread_mutex_destroy(&_mutex);
}
