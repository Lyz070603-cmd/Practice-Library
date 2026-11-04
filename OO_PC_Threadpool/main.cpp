#include <iostream>
#include "Threadpool.h"
#include "MyTask.h"

int main()
{
    Threadpool pool(4, 10);
    pool.start();

    for (int i = 0; i < 20; ++i) {
        pool.addTask(new MyTask(i));
    }

    pool.stop();
    return 0;
}
