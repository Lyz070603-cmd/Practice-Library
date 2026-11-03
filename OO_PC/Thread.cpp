/**
 * Project Untitled
 */


#include "Thread.h"
#include <memory.h>

/**
 * Thread implementation
 */


Thread::Thread()
{
   isRunning = false;

}

Thread::~Thread() {

}

/**
 * @param arg
 * @return void *
 */
void * Thread::Function(void * arg) {
     Thread *ptr = (Thread*)arg;
     if (ptr){
        ptr->run();
     }
    return NULL;
}

/**
 * @return void
 */

void Thread::start() {
    isRunning = true;

   _thid = pthread_create(&_thid,NULL,Function,this); 

    return;
}

/**
 * @return void
 */
void Thread::stop() {
    if (isRunning){
        pthread_join(_thid,NULL);
    }
    return;
}
