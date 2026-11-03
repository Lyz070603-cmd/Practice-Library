#include <iostream>
#include "Producer.h"
#include "Cosumer.h"
#include "Taskqueue.h"


int main()
{
    Taskqueue task(10);

    Producer p(task);
    Cosumer c(task);

    p.start();
    c.start();

    p.stop();
    c.stop();

    return 0;
}

