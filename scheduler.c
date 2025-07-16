#include "scheduler.h"
#include <stdlib.h>
#include <stdio.h>

static Task tasks[MAX_TASKS];
static int num_tasks = 0;
static int current_task = -1;
static jmp_buf main_context;

static void scheduler();

static void task_wrapper(Task *t) {
    t->entry_point(t->arg);
    t->state = TASK_TERMINATED;
    task_yield();
}

void scheduler_init() {
    num_tasks = 0;
    current_task = -1;
}

int task_create(void (*entry_point)(void *), void *arg) {
    if (num_tasks >= MAX_TASKS) return -1;

    Task *t = &tasks[num_tasks];
    t->id = num_tasks;
    t->state = TASK_READY;
    t->entry_point = entry_point;
    t->arg = arg;
    t->started = 0;
    t->stack = malloc(STACK_SIZE);
    if (!t->stack) return -1;

    return num_tasks++;
}

static void scheduler() {
    int next = (current_task + 1) % num_tasks;
    int count = 0;

    while (tasks[next].state == TASK_TERMINATED) {
        next = (next + 1) % num_tasks;
        if (++count >= num_tasks) {
            longjmp(main_context, 1);
        }
    }

    current_task = next;
    Task *t = &tasks[current_task];

    if (!t->started) {
        t->started = 1;

        void *stack_top = (char *)t->stack + STACK_SIZE;

        asm volatile (
            "mov %0, %%rsp\n"      // define o topo da pilha
            "mov %1, %%rdi\n"      // passa Task* no primeiro argumento (rdi)
            "call *%2\n"           // chama task_wrapper
            :
            : "r"(stack_top), "r"(t), "r"(task_wrapper)
            : "rsp", "rdi"
        );
    } else {
        longjmp(t->context, 1);
    }
}

void task_yield() {
    if (setjmp(tasks[current_task].context) == 0) {
        scheduler();
    }
}

void scheduler_start() {
    if (setjmp(main_context) == 0) {
        scheduler();
    }

    for (int i = 0; i < num_tasks; ++i) {
        free(tasks[i].stack);
    }
}
