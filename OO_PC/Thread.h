/**
 * Project Untitled
 */


#ifndef _THREAD_H
#define _THREAD_H

#include <pthread.h>

class Thread {
public: 
    
Thread();
    
~Thread();
    
/**
 * @param arg
 */
static void * Function(void * arg);
    
    
void start();
    
void stop();

private:

virtual void run() = 0;
private: 
    pthread_t _thid;
    bool isRunning;
};

#endif //_THREAD_H
