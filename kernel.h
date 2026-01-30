#ifndef KERNEL_H
#define KERNEL_H

#define SUCCESS 0
#define FAIL 1
#define REPEAT 2

typedef char (*ptrFunc) (void);

struct process {
    ptrFunc func;
    unsigned int deadline;
    unsigned int period;

    process() : func(nullptr), deadline(0), period(0) {}

    process& operator=(const process& other) {
        func = other.func;
        deadline = other.deadline;
        period = other.period;
        return *this;
    }
};

char kernelAddProc(ptrFunc func, unsigned int deadline, unsigned int period);
char kernelInit(void);
void kernelLoop(void);
void kernelTick(void);
unsigned int getProcessCount(void);

#endif
