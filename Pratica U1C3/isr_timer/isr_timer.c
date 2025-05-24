#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define LED 12

int64_t meu_callback(alarm_id_t id, void *user_data) {
    printf("🚨 Alarme disparado!\n");
    gpio_put(LED, 1); // Acende LED (ânodo comum)
    return 0; // Não reprograma (evento único)
}

int main() {
    stdio_init_all();
    sleep_ms(10000);
    printf("Aguardando alarme...\n");

    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);
    gpio_put(LED, 0); // Apagado

    // Agenda o alarme para daqui 3000 ms (3 segundos)
    add_alarm_in_ms(3000, meu_callback, NULL, true);

    while (true) {
        tight_loop_contents(); // Não faz nada, só espera interrupção
    }

    return 0;
}
