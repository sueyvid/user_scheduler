#include "scheduler.h"
#include <stdio.h>

void tarefa(void *arg) {
    char *nome = (char *)arg;
    for (int i = 0; i < 5; ++i) {
        printf("Tarefa %s - passo %d\n", nome, i + 1);
        task_yield();
    }
    printf("Tarefa %s terminou\n", nome);
}

int main() {
    scheduler_init();
    task_create(tarefa, "A");
    task_create(tarefa, "B");
    scheduler_start();
    printf("Todas as tarefas concluídas.\n");
    return 0;
}
