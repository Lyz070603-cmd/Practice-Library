/**
 * Project Untitled
 */


#ifndef _THREAD_H
#define _THREAD_H

class Thread {
public: 
    
void Thread();
    
void ~Thread();
    
/**
 * @param arg
 */
static void * Function(void * arg);
    
virtual void run() = 0;
    
void start();
    
void stop();
private: 
    pthread_t _thid;
    bool isRunning;
};

#endif //_THREAD_H