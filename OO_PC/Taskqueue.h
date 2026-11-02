/**
 * Project Untitled
 */


#ifndef _TASKQUEUE_H
#define _TASKQUEUE_H

class Taskqueue {
public: 
    
void Taskqueue();
    
void ~Taskqueue();
    
void push();
    
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