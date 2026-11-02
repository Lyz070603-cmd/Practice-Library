/**
 * Project Untitled
 */


#ifndef _COSUMER_H
#define _COSUMER_H

#include "Thread.h"


class Cosumer: public Thread {
public: 
    
void run();
    
/**
 * @param _taskque
 */
void Cosumer(Taskqueue & _taskque);
private: 
    Taskqueue & _taskque;
};

#endif //_COSUMER_H