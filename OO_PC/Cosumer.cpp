/**
 * Project Untitled
 */


#include "Cosumer.h"
#include <stdio.h>

/**
 * Cosumer implementation
 */


/**
 * @return void
 */
void Cosumer::run() {
    int cnt = 20;
    while (cnt--){
        int value = 0;
        value = _taskque.pop();
        printf ("Cosumer pop %d\n",value);

        
    }
    return;
}

/**
 * @param _taskque
 */
Cosumer::Cosumer(Taskqueue & taskque)
:_taskque(taskque){

}
