#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <stdint.h>
#include <string.h>

#define STACK_SIZE 1024 * 64
#define MAX_THREADS 2

typedef struct Thread {
    jmp_buf context;
    void (*func)(void);
    uint8_t* stack;
    int finished;
} Thread;

Thread threads[MAX_THREADS];
int current_thread = -1;
jmp_buf main_context;

void switch_thread();
void thread_exit();

void start_thread(Thread* t) {
    if (setjmp(main_context) == 0) {
        longjmp(t->context, 1);
    }
}

void yield() {
    switch_thread();
}

void switch_thread() {
    int prev = current_thread;
    for (int i = 1; i <= MAX_THREADS; ++i) {
        int next = (current_thread + i) % MAX_THREADS;
        if (!threads[next].finished) {
            current_thread = next;
            if (setjmp(threads[prev].context) == 0) {
                longjmp(threads[next].context, 1);
            }
            return;
        }
    }

    // Nenhum thread restante
    longjmp(main_context, 1);
}

void thread_start() {
    void (*f)(void) = threads[current_thread].func;
    f();
    thread_exit();
}

void thread_exit() {
    threads[current_thread].finished = 1;
    yield();
}

void create_thread(int id, void (*func)(void)) {
    threads[id].func = func;
    threads[id].stack = malloc(STACK_SIZE);
    threads[id].finished = 0;

    if (setjmp(threads[id].context) == 0) {
        // Manipular o jmp_buf diretamente
        uintptr_t* buf = (uintptr_t*)&threads[id].context;

#if defined(__x86_64__)
        // Ajuste baseado em glibc para x86_64
        // __jmp_buf[6] é rsp, [7] é rip
        printf("Vamos definir o jmpbuf");
        buf[6] = (uintptr_t)(threads[id].stack + STACK_SIZE - sizeof(uintptr_t)); // rsp
        buf[7] = (uintptr_t)thread_start; // rip
        printf("jmpbuf deu certo!");
#else
#error "Arquitetura não suportada"
#endif
    }
}

void function1() {
    for (int i = 0; i < 5; ++i) {
        printf("Thread 1 - passo %d\n", i);
        yield();
    }
}

void function2() {
    for (int i = 0; i < 5; ++i) {
        printf("Thread 2 - passo %d\n", i);
        yield();
    }
}

int main() {
    create_thread(0, function1);
    create_thread(1, function2);

    start_thread(&threads[0]);

    while (1) {
        int done = 1;
        for (int i = 0; i < MAX_THREADS; ++i) {
            if (!threads[i].finished) {
                done = 0;
                current_thread = i;
                longjmp(threads[i].context, 1);
            }
        }
        if (done) break;
    }

    for (int i = 0; i < MAX_THREADS; ++i) {
        free(threads[i].stack);
    }

    printf("Todos os threads finalizaram.\n");
    return 0;
}
