#include "Thread.h"
#include <memory.h>

Thread::Thread() {
   isRunning = false;
}

Thread::~Thread() {}

void * Thread::Function(void * arg) {
     Thread *ptr = (Thread*)arg;
     if (ptr){
        ptr->run();
     }
    return NULL;
}

void Thread::start() {
    isRunning = true;
    pthread_create(&_thid, NULL, Function, this); 
}

void Thread::stop() {
    if (isRunning){
        pthread_join(_thid, NULL);
    }
    isRunning = false;
}
