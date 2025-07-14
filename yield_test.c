#include <stdio.h>
#include <setjmp.h>

jmp_buf context_main;
jmp_buf context_func;

void func() {
    if (setjmp(context_func) == 0) {
        printf("[func] primeira parte\n");
        longjmp(context_main, 1); // Volta para main
    }

    // Segunda parte (continuação)
    printf("[func] segunda parte\n");
    longjmp(context_main, 2);  // Termina e volta para main
}

int main() {
    int ret = setjmp(context_main);
    if (ret == 0) {
        func();  // Executa func pela primeira vez
    } else if (ret == 1) {
        // Retorno após primeira parte
        printf("[main] de volta da primeira parte\n");
        longjmp(context_func, 1); // Retorna para func
    } else if (ret == 2) {
        printf("[main] fim\n");
    }

    return 0;
}
