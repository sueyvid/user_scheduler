#include <stdio.h>
#include "scheduler.h"

thread_t *threads[MAX_THREADS];
int num_threads = 0;
int current = 0;

void yield() {
    int prev = current;

    do {
        current = (current + 1) % num_threads;
    } while (threads[current]->finished);

    if (setjmp(threads[prev]->context) == 0) {
        longjmp(threads[current]->context, 1);
    }
}

void scheduler_start() {
    if (num_threads == 0) {
        fprintf(stderr, "Nenhuma thread criada.\n");
        return;
    }

    current = 0;
    longjmp(threads[current]->context, 1);
}
