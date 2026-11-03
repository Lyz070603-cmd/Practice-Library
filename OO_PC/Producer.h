/**
 * Project Untitled
 */


#ifndef _PRODUCER_H
#define _PRODUCER_H

#include "Thread.h"
#include "Taskqueue.h"


class Producer: public Thread {
public: 
    
void run();
    
/**
 * @param _taskque
 */
Producer(Taskqueue & _taskque);
private: 
    Taskqueue & _taskque;
};

#endif //_PRODUCER_H
