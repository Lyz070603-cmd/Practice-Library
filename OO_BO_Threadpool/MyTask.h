#ifndef _MYTASK_H
#define _MYTASK_H

#include "Task.h"
#include <cstdio>
#include <unistd.h>
#include <thread>
#include <iostream>

class MyTask : public Task {
public:
    MyTask(int id) : _id(id) {}
    void process() override {
        std::cout << "Thread [" << std::this_thread::get_id() 
            << "] is processing Task [" << _id << "]" << std::endl;
        sleep(1); // 模拟耗时
    }
private:
    int _id;
};

#endif //_MYTASK_H
