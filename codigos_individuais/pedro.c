#include <zephyr/kernel.h>
#include <stdio.h>

#define STACK_SIZE 1024
#define PRIORITY 5

#define TOTAL_INCREMENTOS 10000

volatile int contador = 0;

K_SEM_DEFINE(fim_threads, 0, 2);

// Função inline que força a divisão da instrução em Leitura -> Pausa -> Escrita
static inline int somar_com_falha(int valor) {
    k_yield(); // Interrompe o processador entre a passagem do parâmetro (Leitura) e a soma/retorno
    return valor + 1;
}

void tarefa_incremento(void *arg1, void *arg2, void *arg3) {
    for (int i = 0; i < TOTAL_INCREMENTOS; i++) {
        // REGIÃO CRÍTICA EXPLICITADA SEM VARIÁVEL AUXILIAR:
        // O valor de 'contador' é lido ao ser passado como argumento. 
        // Durante o k_yield(), a outra thread altera a variável 'contador'.
        // Na volta, a função retorna a soma com base no valor antigo já lido.
        contador = somar_com_falha(contador);
    }

    k_sem_give(&fim_threads);
}

K_THREAD_DEFINE(t1_id, STACK_SIZE, tarefa_incremento, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(t2_id, STACK_SIZE, tarefa_incremento, NULL, NULL, NULL, PRIORITY, 0, 0);

int main(void) {
    printk("Iniciando teste de Race Condition (sem variavel auxiliar)...\n");

    k_sem_take(&fim_threads, K_FOREVER);
    k_sem_take(&fim_threads, K_FOREVER);

    int valor_esperado = TOTAL_INCREMENTOS * 2;

    printk("Valor esperado: %d\n", valor_esperado);
    printk("Valor obtido:   %d\n", contador);
    printk("Incrementos perdidos: %d\n", valor_esperado - contador);

    return 0;
}

