#ifndef _TASKQUEUE_H
#define _TASKQUEUE_H

#include "Condition.h"
#include "Task.h"
#include <queue>

using std::queue;

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
    Mutexlock _mutex;
    size_t _quesize;
    queue<Task*> _que;
    Condition _noFull;
    Condition _noEmpty;
    bool _isExit;
};

#endif //_TASKQUEUE_H
