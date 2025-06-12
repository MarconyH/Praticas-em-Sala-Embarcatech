#include "pico/stdlib.h"
#include <stdio.h>

// Definições de pinos
const uint8_t pir_pin = 16;   // Sensor PIR conectado ao GPIO 15


int main() {
    // Inicializa a comunicação serial (para debug via USB)
    stdio_init_all();

    // Inicializa os pinos
    gpio_init(pir_pin);
    gpio_set_dir(pir_pin, GPIO_IN);



    printf("Sistema iniciado. Monitorando sensor PIR...\n");

    while (true) {
        bool motion_detected = gpio_get(pir_pin);  // Lê estado do PIR

        if (motion_detected) {
            printf("Movimento detectado!\n"); // Liga o LED
            
        } else {
             printf("Movimento não detectado!\n"); // Desliga o LED
        }

        sleep_ms(200);  // Aguarda um curto tempo antes da próxima leitura
    }

    return 0;
}
