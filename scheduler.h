#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <setjmp.h>

#define MAX_TASKS 10
#define STACK_SIZE (64 * 1024)

typedef enum {
    TASK_READY,
    TASK_RUNNING,
    TASK_TERMINATED
} TaskState;

typedef struct {
    int id;
    TaskState state;
    jmp_buf context;
    void *stack;
    void (*entry_point)(void *);
    void *arg;
    int started;
} Task;

void scheduler_init();
int task_create(void (*entry_point)(void *), void *arg);
void scheduler_start();
void task_yield();

#endif
