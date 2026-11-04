#ifndef _MYTASK_H
#define _MYTASK_H

#include "Task.h"
#include <cstdio>
#include <unistd.h>

class MyTask : public Task {
public:
    MyTask(int id) : _id(id) {}
    void process() override {
        printf("Thread [%lu] is processing Task [%d]\n", (unsigned long)pthread_self(), _id);
        sleep(1); // 模拟耗时
    }
private:
    int _id;
};

#endif //_MYTASK_H
