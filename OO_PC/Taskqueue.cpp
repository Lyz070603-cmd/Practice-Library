/**
 * Project Untitled
 */


#include "Taskqueue.h"

/**
 * Taskqueue implementation
 */


Taskqueue::Taskqueue(size_t size) 
:_mutex()
,_quesize(size)
,_que()
,_noFull(_mutex)
,_noEmpty(_mutex){
}

Taskqueue::~Taskqueue() {

}

/**
 * @return void
 */
//生产者
void Taskqueue::push(const int value) {
    //上锁
    //判断
    //加入
    //解锁
    _mutex.lock();

    if(Full()){
        _noFull.wait();
    }
    //没满
    _que.push(value);

    //通知消费者
    _noEmpty.notify();

    _mutex.unlock();

    return;
}

/**
 * @return int
 */
int Taskqueue::pop() {
    _mutex.lock();
    if (Empty()){
        _noEmpty.wait();

    }
    int temp_value = _que.front();
    _que.pop();
    
    _noFull.notify();
    
    _mutex.unlock();
    return temp_value;
}

/**
 * @return bool
 */
bool Taskqueue::Full() {
    if (_que.size() == _quesize){
        return true;
    }
    else{
        return false;

    }
}

/**
 * @return bool
 */
bool Taskqueue::Empty() {
     return _que.size() == 0 ? true : false;
}
