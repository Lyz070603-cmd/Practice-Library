#include "Condition.h"

Condition::Condition(Mutexlock & mutex) : _mutex(mutex) {
    pthread_cond_init(&_cond, nullptr);
}

Condition::~Condition() {
    pthread_cond_destroy(&_cond);
}

void Condition::wait() {
    pthread_cond_wait(&_cond, _mutex.getMutexlock());
}

void Condition::notify() {
    pthread_cond_signal(&_cond);
}

void Condition::notifyAll() {
    pthread_cond_broadcast(&_cond);
}
