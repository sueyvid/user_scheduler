/*
 * COMPILAR (Linux x86‑64 + GCC):
 *   gcc -Wall -g -O0 -o thread_step2 thread_step2.c
 *
 * OBS.: este código usa índices de __jmpbuf que valem para
 *       GLIBC + GCC em x86‑64.  Para outra arquitetura será
 *       preciso ajustar.
 */

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

#define STACK_SIZE   (64 * 1024)   /* 64 KB */

/* Contextos globais */
static jmp_buf main_ctx;     /* onde voltaremos quando a thread terminar */
static jmp_buf thr_ctx;      /* contexto da “thread” */

/* ---------- Função que será executada pela thread ---------- */
void thread_func(void)
{
    puts("[thread] Olá! Estou rodando na minha própria pilha.");

    /* … aqui poderia haver trabalho real … */

    puts("[thread] Vou terminar e devolver o controle a main.");

    /* Voltamos para main: 2 = código de retorno arbitrário */
    longjmp(main_ctx, 2);
}

/* ---------- Cria contexto da thread (pilha + jmp_buf) ------ */
void create_thread_context(void (*func)(void))
{
    /* 1) Aloca pilha */
    void *stack = malloc(STACK_SIZE);
    if (!stack) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    void *stack_top = (char *)stack + STACK_SIZE;   /* topo cresce p/ baixo */

    /* 2) Salva estado base no jmp_buf */
    if (setjmp(thr_ctx) != 0) {
        /* Nunca deveremos cair aqui na criação */
        fprintf(stderr, "setjmp inesperado\n");
        exit(EXIT_FAILURE);
    }

    thr_ctx->__jmpbuf[6] = (long)stack_top;    // RSP
    thr_ctx->__jmpbuf[7] = (long)func;         // RIP

}

/* ----------------------------- main ------------------------- */
int main(void)
{
    /* 🔹 Cria a thread */
    create_thread_context(thread_func);

    /* 🔹 Salva contexto da main e pula para a thread */
    int ret = setjmp(main_ctx);
    if (ret == 0) {
        /* Primeira vez: saltamos para a thread */
        longjmp(thr_ctx, 1);
    } else if (ret == 2) {
        /* Voltamos da thread */
        puts("[main] A thread terminou, estamos de volta em main.");
    }

    puts("[main] Fim normal do programa.");
    return 0;
}
