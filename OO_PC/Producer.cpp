/**
 * Project Untitled
 */


#include "Producer.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Producer implementation
 */


/**
 * @return void
 */
void Producer::run() {

    int cnt = 20;
        srand(clock());
    while(cnt--){
        int temp = rand() % 100;
        printf (">> Producer %d\n",temp);
        _taskque.push(temp);
    }
    return;
}

/**
 * @param _taskque
 */
Producer::Producer(Taskqueue & taskque) 
:_taskque(taskque){


}
