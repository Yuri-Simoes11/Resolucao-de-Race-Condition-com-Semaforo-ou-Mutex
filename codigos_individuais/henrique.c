#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#define SLEEP_TIME_MS 500

LOG_MODULE_REGISTER(MEGABRAIN, LOG_LEVEL_DBG); 
// Define o LED usando Device Tree
#define BUFFER_SIZE 10
static int buffer[BUFFER_SIZE];

int saldo_vitrine = 0;
int numero_pao = 0;


void thread_padeiro(void *p1, void *p2, void *p3) {
    int saldo_padeiro;
    while(1){
        saldo_padeiro = saldo_vitrine;
        numero_pao++;
        buffer[saldo_vitrine] = numero_pao;
        saldo_padeiro++;
        k_msleep(1000);
        saldo_vitrine = saldo_padeiro;
        LOG_INF("PADEIRO: Pão #%d pronto (Saldo vitrine = %d)", buffer[saldo_vitrine-1], saldo_vitrine);
    }
}

void thread_cliente(void *p1, void *p2, void *p3) {
    int saldo_cliente;   
    while(1) {
        saldo_cliente = saldo_vitrine;
        saldo_cliente--;
        k_msleep(1500);
        saldo_vitrine = saldo_cliente;
        LOG_INF("CLIENTE: Peguei pão #%d (Saldo vitrine = %d)", buffer[saldo_vitrine+1], saldo_vitrine);
        buffer[saldo_vitrine+1] = 0;
    }
}

K_THREAD_DEFINE(padeiro_tid, 512, thread_padeiro, NULL, NULL, NULL, 7, 0, 0);
K_THREAD_DEFINE(cliente_tid, 512, thread_cliente, NULL, NULL, NULL, 7, 0, 0);


void main(void)
{
    k_sleep(K_FOREVER);
}