#ifndef _TASKQUEUE_H
#define _TASKQUEUE_H

#include "Task.h"
#include <queue>
#include <mutex>
#include <condition_variable>


using std::queue;
using std::mutex;
using std::condition_variable;

class Taskqueue {
public: 
    Taskqueue(size_t size);
    ~Taskqueue();
    
    void push(Task* ptask);
    Task* pop();
    
    bool Full();
    bool Empty();
    void wakeup();

private: 
    mutex _mutex;
    size_t _quesize;
    queue<Task*> _que;
    condition_variable _noFull;
    condition_variable _noEmpty;
    bool _isExit;
};

#endif //_TASKQUEUE_H
