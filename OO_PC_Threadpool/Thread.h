#ifndef _THREAD_H
#define _THREAD_H

#include <pthread.h>

class Thread {
public: 
    Thread();
    virtual ~Thread();
    
    static void * Function(void * arg);
    void start();
    void stop();

protected:
    virtual void run() = 0; // 改为 protected 供派生类重写

private: 
    pthread_t _thid;
    bool isRunning;
};

#endif //_THREAD_H
