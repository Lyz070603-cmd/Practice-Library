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
        _threads.emplace_back(&Threadpool::threadFuction,this);
    }
}

void Threadpool::stop() {
    if (!_isExit) {
        _isExit = true;
        _taskQue.wakeup(); 
        for (auto& threads_one : _threads) {
            threads_one->join();
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
void Threadpool::threadFuction(){
    while(1){
        Task * ptask = getTask();
        if (ptask == nullptr){
            break;//线程退出
        }

        ptask->process();
        delete ptask;
    }
}
