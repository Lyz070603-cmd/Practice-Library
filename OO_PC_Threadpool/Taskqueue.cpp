#include "Taskqueue.h"

Taskqueue::Taskqueue(size_t size) 
:_mutex()
, _quesize(size)
, _que()
, _noFull(_mutex)
, _noEmpty(_mutex)
, _isExit(false) {
}

Taskqueue::~Taskqueue() {
}

void Taskqueue::push(Task* ptask) {
    _mutex.lock();
    while(Full() && !_isExit){
        _noFull.wait();
    }
    if (_isExit) {
        _mutex.unlock();
        return;
    }
    _que.push(ptask);
    _noEmpty.notify();
    _mutex.unlock();
}

Task* Taskqueue::pop() {
    _mutex.lock();
    while(Empty() && !_isExit){
        _noEmpty.wait();
    }
    if (_isExit && _que.empty()) {
        _mutex.unlock();
        return nullptr;
    }
    Task* ptask = _que.front();
    _que.pop();
    _noFull.notify();
    _mutex.unlock();
    return ptask;
}

bool Taskqueue::Full() {
    return _que.size() == _quesize;
}

bool Taskqueue::Empty() {
    return _que.size() == 0;
}

void Taskqueue::wakeup() {
    _mutex.lock();
    _isExit = true;
    _noEmpty.notifyAll();
    _noFull.notifyAll();
    _mutex.unlock();
}
