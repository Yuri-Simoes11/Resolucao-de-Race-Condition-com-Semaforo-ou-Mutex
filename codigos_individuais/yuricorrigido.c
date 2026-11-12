#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/atomic.h>
#include <stdint.h>
#include <stdbool.h>

// Configuracoes
#define STACK_SIZE 1024
#define PRIORIDADE 5
#define OPERACOES_INICIAIS 5

// Recursos compartilhados
K_MUTEX_DEFINE(mutex_impressora);
K_MUTEX_DEFINE(mutex_scanner);

// Sincronizacao entre as threads
atomic_t threads_prontas = ATOMIC_INIT(0);

// Stacks
K_THREAD_STACK_DEFINE(stack_thread_a, STACK_SIZE);
K_THREAD_STACK_DEFINE(stack_thread_b, STACK_SIZE);

struct k_thread thread_a;
struct k_thread thread_b;

// Gerador pseudoaleatorio Xorshift32
static uint32_t pseudo_random(uint32_t *estado)
{
    uint32_t x = *estado;

    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;

    *estado = x;

    return x;
}

// Executa uma operacao utilizando dois recursos
static void executar_operacao(
    const char *nome,
    struct k_mutex *primeiro,
    struct k_mutex *segundo,
    const char *nome_primeiro,
    const char *nome_segundo,
    uint32_t atraso)
{
    printk("[%s] Solicitando %s...\n", nome, nome_primeiro);

    k_mutex_lock(primeiro, K_FOREVER);

    printk("[%s] %s adquirido!\n", nome, nome_primeiro);

    // Intervalo entre as aquisicoes
    k_msleep(atraso);

    printk("[%s] Solicitando %s...\n", nome, nome_segundo);

    k_mutex_lock(segundo, K_FOREVER);

    printk("[%s] %s adquirido!\n", nome, nome_segundo);

    // Simula processamento
    k_msleep(30);

    printk("[%s] Operacao concluida!\n", nome);

    // Libera na ordem inversa da aquisicao
    k_mutex_unlock(segundo);
    k_mutex_unlock(primeiro);
}

// Thread A: Impressao
void thread_impressao(void *p1, void *p2, void *p3)
{
    uint32_t iteracao = 0;
    uint32_t estado = 0x13579BDF;

    while (1) {
        iteracao++;

        printk("\n[A] Impressao - Iteracao %u\n", iteracao);

        uint32_t atraso = 10 + (pseudo_random(&estado) % 61);

        // CORREÇÃO DE DEADLOCK: Hierarquia de bloqueio estrita e fixa.
        // Sempre solicitamos primeiro a impressora e depois o scanner, 
        // eliminando qualquer possibilidade de espera circular.
        executar_operacao(
            "A",
            &mutex_impressora,
            &mutex_scanner,
            "impressora",
            "scanner",
            atraso
        );

        // Marca como pronto na primeira iteracao (para sincronizacao com a main)
        if (iteracao == OPERACOES_INICIAIS) {
            atomic_inc(&threads_prontas);
        }

        // Intervalo entre operacoes
        k_msleep(
            20 + (pseudo_random(&estado) % 81)
        );
    }
}

// Thread B: Digitalizacao
void thread_digitalizacao(void *p1, void *p2, void *p3)
{
    uint32_t iteracao = 0;
    uint32_t estado = 0x2468ACE1;

    while (1) {
        iteracao++;

        printk("\n[B] Digitalizacao - Iteracao %u\n", iteracao);

        uint32_t atraso = 10 + (pseudo_random(&estado) % 61);

        // CORREÇÃO DE DEADLOCK: Mantém exatamente a mesma hierarquia global (Impressora -> Scanner)
        executar_operacao(
            "B",
            &mutex_impressora,
            &mutex_scanner,
            "impressora",
            "scanner",
            atraso
        );

        // Marca como pronto na primeira iteracao (para sincronizacao com a main)
        if (iteracao == OPERACOES_INICIAIS) {
            atomic_inc(&threads_prontas);
        }

        // Intervalo entre operacoes
        k_msleep(
            20 + (pseudo_random(&estado) % 81)
        );
    }
}

// Main
int main(void)
{
    printk("\n==============================\n");
    printk(" SISTEMA CORRIGIDO (SEM DEADLOCK)\n");
    printk("==============================\n");

    // Criacao da Thread A
    k_thread_create(
        &thread_a,
        stack_thread_a,
        STACK_SIZE,
        thread_impressao,
        NULL, NULL, NULL,
        PRIORIDADE,
        0,
        K_NO_WAIT
    );

    // Criacao da Thread B
    k_thread_create(
        &thread_b,
        stack_thread_b,
        STACK_SIZE,
        thread_digitalizacao,
        NULL, NULL, NULL,
        PRIORIDADE,
        0,
        K_NO_WAIT
    );

    // Aguarda ambas concluirem a fase inicial
    while (atomic_get(&threads_prontas) < 2) {
        k_sleep(K_MSEC(10));
    }

    printk("[MAIN] Ambas as threads concluiram a fase inicial!\n");

    // Main continua executando
    while (1) {
        printk("[MAIN] Sistema funcionando...\n");
        k_sleep(K_SECONDS(2));
    }

    return 0;
}