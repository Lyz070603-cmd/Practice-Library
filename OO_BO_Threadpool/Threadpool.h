#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include "Taskqueue.h"
#include <vector>
#include <memory>
#include <thread>
using std::thread;
using std::vector;

using std::vector;
using std::unique_ptr;

class Threadpool {
public: 
    Threadpool(size_t threadNum, size_t queSize);
    ~Threadpool();
    
    void start();
    void stop();
    void addTask(Task* ptask);
    Task* getTask();
private:
    void threadFuction();

private: 
    size_t _threadNum;
    size_t _queSize;
    vector<unique_ptr<thread>> _threads;
    Taskqueue _taskQue;
    bool _isExit;
};

#endif //_THREADPOOL_H
