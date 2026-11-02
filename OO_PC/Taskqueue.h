/**
 * Project Untitled
 */


#ifndef _TASKQUEUE_H
#define _TASKQUEUE_H
#include "Condition.h"
#include <queue>
using std::queue;

class Taskqueue {
public: 
    
Taskqueue(size_t size);
    
~Taskqueue();
    
void push(const int value);
    
int pop();
    
bool Full();
    
bool Empty();
private: 
    Mutexlock _mutex;
    size_t _quesize;
    queue<int> _que;
    Condition _noFull;
    Condition _noEmpty;
};

#endif //_TASKQUEUE_H
