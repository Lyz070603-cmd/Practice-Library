#include "Taskqueue.h"
#include <memory.h>
using std::unique_lock;

Taskqueue::Taskqueue(size_t size) 
:_mutex()
, _quesize(size)
, _que()
, _noFull()
, _noEmpty()
, _isExit(false) {
}

Taskqueue::~Taskqueue() {
}

void Taskqueue::push(Task* ptask) {
    unique_lock<mutex> lock(_mutex);
    _noFull.wait(lock,[this](){return !Full() || _isExit;});
    if (_isExit) {
        _mutex.unlock();
        return;
    }
    _que.push(ptask);
    _noEmpty.notify_one();
    _mutex.unlock();
}

Task* Taskqueue::pop() {
    unique_lock<mutex> lock(_mutex);
    _noEmpty.wait(lock,[this](){return !Empty() || _isExit;});
    if (_isExit && _que.empty()) {
        _mutex.unlock();
        return nullptr;
    }
    Task* ptask = _que.front();
    _que.pop();
    _noFull.notify_one();
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
    unique_lock<mutex> lock(_mutex);
    _isExit = true;
    _noEmpty.notify_all();
    _noFull.notify_all();
}
