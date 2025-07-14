#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <setjmp.h>

#define STACK_SIZE 1024 * 64  // 64 KB
#define MAX_THREADS 10

typedef struct thread {
    int id;
    void (*function)(void);
    void *stack;
    int finished;
    jmp_buf context;
} thread_t;

void thread_create(void (*function)(void));
void scheduler_start(void);
void yield(void);

#endif // SCHEDULER_H
