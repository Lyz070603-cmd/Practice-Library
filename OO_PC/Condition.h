/**
 * Project Untitled
 */


#ifndef _CONDITION_H
#define _CONDITION_H

class Condition {
public: 
    
/**
 * @param _mutex
 */
void Condition(Mutexlock & _mutex);
    
void ~Condition();
    
void wait();
    
viod notify();
    
void notifyAll();
private: 
    pthread_cond_t _cond;
    Mutexlock _mutex;
};

#endif //_CONDITION_H