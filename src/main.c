#include <stdio.h>
#include "scheduler.h"

void tarefa1() {
    for (int i = 0; i < 5; i++) {
        printf("Tarefa 1: %d\n", i);
        yield();
    }
}

void tarefa2() {
    for (int i = 0; i < 5; i++) {
        printf("Tarefa 2: %d\n", i);
        yield();
    }
}

int main() {
    thread_create(tarefa1);
    thread_create(tarefa2);

    scheduler_start();

    return 0;
}
