
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <stdint.h>
#include <stdbool.h>

// Configuracoes
#define STACK_SIZE 1024
#define PRIORIDADE 5

// Recursos compartilhados
K_MUTEX_DEFINE(mutex_impressora);
K_MUTEX_DEFINE(mutex_scanner);

// Stacks
K_THREAD_STACK_DEFINE(stack_thread_a, STACK_SIZE);
K_THREAD_STACK_DEFINE(stack_thread_b, STACK_SIZE);

struct k_thread thread_a;
struct k_thread thread_b;

// Gerador pseudoaleatorio
static uint32_t pseudo_random(uint32_t seed, uint32_t iteracao) {
    uint32_t x = k_cycle_get_32() ^ seed ^ (iteracao * 2654435761u);

    x ^= x >> 16;
    x *= 0x7feb352du;
    x ^= x >> 15;
    x *= 0x846ca68bu;
    x ^= x >> 16;

    return x;
}

// Thread A: Impressao
void thread_impressao(void *p1, void *p2, void *p3)
{
    uint32_t iteracao = 0;

    while (1) {

        iteracao++;

        // Escolhe a ordem de aquisicao
        bool inverter = (pseudo_random(0x13579BDF, iteracao) % 100) < 50;

        // Varia o tempo de espera entre 10 e 70 ms
        uint32_t atraso = 10 +
            (pseudo_random(0xABCDEF12, iteracao) % 61);

        printk("\n[A] Impressao - Iteracao %u\n", iteracao);

        if (!inverter) {

            printk("[A] Solicitando impressora...\n");
            k_mutex_lock(&mutex_impressora, K_FOREVER);

            printk("[A] Impressora adquirida!\n");

            k_msleep(atraso);

            printk("[A] Solicitando scanner...\n");
            k_mutex_lock(&mutex_scanner, K_FOREVER);

            printk("[A] Scanner adquirido!\n");

        } else {

            printk("[A] Ordem invertida!\n");

            printk("[A] Solicitando scanner...\n");
            k_mutex_lock(&mutex_scanner, K_FOREVER);

            printk("[A] Scanner adquirido!\n");

            k_msleep(atraso);

            printk("[A] Solicitando impressora...\n");
            k_mutex_lock(&mutex_impressora, K_FOREVER);

            printk("[A] Impressora adquirida!\n");
        }

        // Simula o processamento da operacao
        k_msleep(30);

        printk("[A] Operacao concluida!\n");

        // Libera os recursos
        k_mutex_unlock(&mutex_impressora);
        k_mutex_unlock(&mutex_scanner);

        // Pequeno intervalo entre operacoes
        k_msleep(20 + (pseudo_random(0x12345678, iteracao) % 81));
    }
}

// Thread B: Digitalizacao
void thread_digitalizacao(void *p1, void *p2, void *p3)
{
    uint32_t iteracao = 0;

    while (1) {

        iteracao++;

        // Escolhe a ordem de aquisicao
        bool inverter = (pseudo_random(0x2468ACE0, iteracao) % 100) < 50;

        // Varia o tempo de espera entre 10 e 70 ms
        uint32_t atraso = 10 +
            (pseudo_random(0x98765432, iteracao) % 61);

        printk("\n[B] Digitalizacao - Iteracao %u\n", iteracao);

        if (!inverter) {

            printk("[B] Solicitando scanner...\n");
            k_mutex_lock(&mutex_scanner, K_FOREVER);

            printk("[B] Scanner adquirido!\n");

            k_msleep(atraso);

            printk("[B] Solicitando impressora...\n");
            k_mutex_lock(&mutex_impressora, K_FOREVER);

            printk("[B] Impressora adquirida!\n");

        } else {

            printk("[B] Ordem invertida!\n");

            printk("[B] Solicitando impressora...\n");
            k_mutex_lock(&mutex_impressora, K_FOREVER);

            printk("[B] Impressora adquirida!\n");

            k_msleep(atraso);

            printk("[B] Solicitando scanner...\n");
            k_mutex_lock(&mutex_scanner, K_FOREVER);

            printk("[B] Scanner adquirido!\n");
        }

        // Simula o processamento da operacao
        k_msleep(30);

        printk("[B] Operacao concluida!\n");

        // Libera os recursos
        k_mutex_unlock(&mutex_impressora);
        k_mutex_unlock(&mutex_scanner);

        // Pequeno intervalo entre operacoes
        k_msleep(20 + (pseudo_random(0x87654321, iteracao) % 81));
    }
}

// Main
int main(void)
{
    printk("\n==============================\n");
    printk(" DEMONSTRACAO DE DEADLOCK\n");
    printk("==============================\n");

    // Thread A
    k_thread_create(&thread_a, stack_thread_a, STACK_SIZE,thread_impressao,
                    NULL, NULL, NULL, PRIORIDADE, 0, K_NO_WAIT);

    // Thread B
    k_thread_create(&thread_b, stack_thread_b, STACK_SIZE, thread_digitalizacao,
                    NULL, NULL, NULL, PRIORIDADE, 0, K_NO_WAIT);

    // Main
    while (1) {

        printk("[MAIN] Sistema funcionando...\n");

        k_sleep(K_SECONDS(2));
    }

    return 0;
}
