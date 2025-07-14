#include <stdlib.h>
#include <stdio.h>
#include "scheduler.h"

extern thread_t *threads[MAX_THREADS];
extern int num_threads;
extern int current;

void thread_trampoline() {
    printf("Entrando em thread_trampoline (thread %d)\n", current);
    fflush(stdout);  // Garante que print apareça
    threads[current]->function();
    threads[current]->finished = 1;
    yield();
}

void thread_create(void (*function)(void)) {
    if (num_threads >= MAX_THREADS) {
        fprintf(stderr, "Máximo de threads atingido.\n");
        return;
    }

    thread_t *t = malloc(sizeof(thread_t));
    t->id = num_threads;
    t->function = function;
    t->finished = 0;
    t->stack = malloc(STACK_SIZE);

    if (!t->stack) {
        perror("Erro ao alocar pilha");
        exit(1);
    }

    void *stack_top = (char *)t->stack + STACK_SIZE;

    if (setjmp(t->context) == 0) {
#if defined(__x86_64__)
        t->context[0].__jmpbuf[6] = (long)stack_top;          // RSP
        t->context[0].__jmpbuf[7] = (long)thread_trampoline;  // RIP
#else
#error "Este código precisa ser ajustado para outras arquiteturas"
#endif
    }

    threads[num_threads++] = t;
}
