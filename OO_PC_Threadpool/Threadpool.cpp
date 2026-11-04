#include "Threadpool.h"
#include "WorkerThread.h"

Threadpool::Threadpool(size_t threadNum, size_t queSize) 
:_threadNum(threadNum)
, _queSize(queSize)
, _taskQue(queSize)
, _isExit(false) {
}

Threadpool::~Threadpool() {
    if (!_isExit) {
        stop();
    }
}

void Threadpool::start() {
    for (size_t i = 0; i < _threadNum; ++i) {
        _threads.push_back(unique_ptr<Thread>(new WorkerThread(*this)));
        _threads.back()->start();
    }
}

void Threadpool::stop() {
    if (!_isExit) {
        _isExit = true;
        _taskQue.wakeup(); 
        for (auto& thread : _threads) {
            thread->stop();
        }
        _threads.clear();
    }
}

void Threadpool::addTask(Task* ptask) {
    _taskQue.push(ptask);
}

Task* Threadpool::getTask() {
    return _taskQue.pop();
}
