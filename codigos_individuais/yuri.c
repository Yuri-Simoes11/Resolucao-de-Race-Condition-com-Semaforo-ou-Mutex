
#include <zephyr/kernel.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/sys/printk.h>

// Configuracoes
#define STACK_SIZE 1024
#define PRIORIDADE 5

// Recursos compartilhados
K_MUTEX_DEFINE(mutex_impressora);
K_MUTEX_DEFINE(mutex_scanner);

// Sincronizacao para garantir que ambas
// as threads adquiram seu primeiro mutex.
atomic_t threads_prontas = ATOMIC_INIT(0);

// Stacks das threads
K_THREAD_STACK_DEFINE(stack_thread_a, STACK_SIZE);
K_THREAD_STACK_DEFINE(stack_thread_b, STACK_SIZE);

struct k_thread thread_a;
struct k_thread thread_b;

// Thread A: Impressao
void thread_impressao(void *p1, void *p2, void *p3)
{
    printk("\n[THREAD A] Iniciando impressao\n");

    // 1. Adquire a impressora
    k_mutex_lock(&mutex_impressora, K_FOREVER);

    printk("[THREAD A] Impressora adquirida!\n");

    // Informa que ja possui seu primeiro recurso
    atomic_inc(&threads_prontas);

    // Aguarda a Thread B adquirir seu primeiro recurso
    while (atomic_get(&threads_prontas) < 2) {
        k_yield();
    }

    printk("[THREAD A] Solicitando scanner...\n");

    // 2. Tenta adquirir o scanner
    // Ficara bloqueada se a Thread B possuir o recurso
    k_mutex_lock(&mutex_scanner, K_FOREVER);

    printk("[THREAD A] Scanner adquirido!\n");

    // Nunca devera chegar aqui
    k_mutex_unlock(&mutex_scanner);
    k_mutex_unlock(&mutex_impressora);
}

// Thread B: Digitalizacao
void thread_digitalizacao(void *p1, void *p2, void *p3)
{
    printk("\n[THREAD B] Iniciando digitalizacao\n");

    // 1. Adquire o scanner
    k_mutex_lock(&mutex_scanner, K_FOREVER);

    printk("[THREAD B] Scanner adquirido!\n");

    // Informa que ja possui seu primeiro recurso
    atomic_inc(&threads_prontas);

    // Aguarda a Thread A adquirir seu primeiro recurso
    while (atomic_get(&threads_prontas) < 2) {
        k_yield();
    }

    printk("[THREAD B] Solicitando impressora...\n");

    // 2. Tenta adquirir a impressora
    // Ficara bloqueada se a Thread A possuir o recurso
    k_mutex_lock(&mutex_impressora, K_FOREVER);

    printk("[THREAD B] Impressora adquirida!\n");

    // Nunca devera chegar aqui
    k_mutex_unlock(&mutex_impressora);
    k_mutex_unlock(&mutex_scanner);
}

// Main
int main(void)
{
    printk("\n==============================\n");
    printk(" DEMONSTRACAO DE DEADLOCK\n");
    printk("==============================\n");

    // Criacao da Thread A
    k_thread_create(&thread_a, stack_thread_a, STACK_SIZE, thread_impressao,
                    NULL, NULL, NULL, PRIORIDADE, 0, K_NO_WAIT);

    // Criacao da Thread B
    k_thread_create(&thread_b, stack_thread_b, STACK_SIZE, thread_digitalizacao,
                    NULL, NULL, NULL, PRIORIDADE, 0, K_NO_WAIT);

    // Main continua executando
    while (1) {
        printk("[MAIN] Sistema principal funcionando...\n");
        k_sleep(K_SECONDS(2));
    }

    return 0;
}
