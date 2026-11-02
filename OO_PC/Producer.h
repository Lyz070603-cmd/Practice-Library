/**
 * Project Untitled
 */


#ifndef _PRODUCER_H
#define _PRODUCER_H

#include "Thread.h"


class Producer: public Thread {
public: 
    
void run();
    
/**
 * @param _taskque
 */
void Producer(Taskqueue & _taskque);
private: 
    Taskqueue & _taskque;
};

#endif //_PRODUCER_H