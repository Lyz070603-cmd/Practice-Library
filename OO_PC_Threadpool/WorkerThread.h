#ifndef _WORKERTHREAD_H
#define _WORKERTHREAD_H

#include "Thread.h"
#include "Threadpool.h"

class Threadpool; // 前向声明

class WorkerThread : public Thread {
public:
    WorkerThread(Threadpool& pool) : _pool(pool) {}
    
protected:
    void run() override {
        while (true) {
            Task* ptask = _pool.getTask();
            if (ptask == nullptr) {
                break; 
            }
            ptask->process(); 
            delete ptask;     
        }
    }
private:
    Threadpool& _pool;
};

#endif //_WORKERTHREAD_H
