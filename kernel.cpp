#include "kernel.h"
#include <stdio.h>

#define MAX_PROCESSES 10

process processPool[MAX_PROCESSES];
volatile unsigned int processCount = 0;
volatile unsigned int systemTick = 0;

char kernelAddProc(ptrFunc func, unsigned int deadline, unsigned int period) {
    if (processCount < MAX_PROCESSES) {
        processPool[processCount].func = func;
        processPool[processCount].deadline = systemTick + deadline;
        processPool[processCount].period = period;
        processCount++;
        return SUCCESS;
    }
    
    return FAIL;
}

char kernelInit(void) {
    processCount = 0;
    systemTick = 0;
    return SUCCESS;
}

void kernelTick(void) {
    systemTick++;
}

void kernelLoop(void) {
    for (unsigned int i = 0; i < processCount; i++) {
        if (processPool[i].deadline <= systemTick) {
            char result = processPool[i].func();

            if (result == SUCCESS || result == FAIL) {
                for (unsigned int j = i; j < processCount - 1; j++) {
                    processPool[j] = processPool[j + 1];
                }
                processCount--;
                i--;
            } else if (result == REPEAT) {
                processPool[i].deadline = systemTick + processPool[i].period;
            }
        }
    }
}

unsigned int getProcessCount(void) {
    return processCount;
}
