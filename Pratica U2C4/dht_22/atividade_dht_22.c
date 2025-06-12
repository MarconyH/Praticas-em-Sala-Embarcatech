#include <stdio.h>
#include "pico/stdlib.h"
#include "dht22.h"

#define DHT22_PIN 16  // Pino onde o sensor DHT22 está conectado

int main() {
    int result;
    float temperature, humidity;

    stdio_init_all();  // Inicializa comunicação serial
    sleep_ms(2000);    // Aguarda terminal estabilizar

    // Inicializa o sensor DHT22
    result = dht22_init(DHT22_PIN);
    if (result != DHT22_OK) {
        printf("Erro ao inicializar o sensor DHT22.\n");
        return 1;
    }

    printf("Leitura do sensor DHT22\n");

    while (1) {
        result = dht22_read(&temperature, &humidity);

        if (result == DHT22_OK) {
            printf("Temperatura: %.1f °C | Umidade: %.1f %%\n", temperature, humidity);
        } else {
            printf("Erro na leitura do DHT22: código %d\n", result);
        }

        sleep_ms(3000);  // Aguarda 3 segundos antes da próxima leitura
    }

    return 0;
}
