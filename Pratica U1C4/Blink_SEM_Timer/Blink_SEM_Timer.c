#include <stdio.h> // Biblioteca padrão de entrada e saída para uso de funções como printf.
#include "pico/stdlib.h" // Biblioteca padrão do Raspberry Pi Pico para controle de GPIO, temporização e comunicação serial.
#include <time.h> // Biblioteca com funções de tempo.

// Definições de pinos
#define LED_VERDE 11
#define LED_AZUL 12

int main()
{
    stdio_init_all();

    gpio_init(LED_VERDE); // Inicializa o pino GPIO 11, preparando-o para ser usado como saída digital.
    gpio_set_dir(LED_VERDE, GPIO_OUT); // Configura a direção do pino 11 como saída.
    gpio_put(LED_VERDE, 0); // Garante que o LED VERDE inicia apagado.
    
    gpio_init(LED_AZUL); // Inicializa o pino GPIO 12, preparando-o para ser usado como saída digital.
    gpio_set_dir(LED_AZUL, GPIO_OUT); // Configura a direção do pino 12 como saída.
    gpio_put(LED_AZUL, 0); // Garante que o LED AZUL inicia apagado.

    clock_t inicial, atual; // Define as variáveis de tempo "inicial" e "atual"
    double tempo; // Define a variável "tempo"

    inicial = clock(); // Armazena na variável "inicial" o tempo decorrido em ms através da função clock().

    while (true) {
        
        gpio_put(LED_AZUL, 1); // Liga o LED AZUL.
        sleep_ms(100); // Aguarda 100 ms.
        gpio_put(LED_AZUL, 0); // Desliga o LED AZUL.
        sleep_ms(100); // Aguarda 100 ms.

        atual = clock(); // Armazena na variável "atual" o tempo decorrido em ms através da função clock().

        tempo = atual - inicial; // Calcula a variável "tempo".

        if (tempo >= 500) { // Verifica se decorreu 500 ms.
            gpio_put(LED_VERDE, 1); // Liga o LED VERDE.
            sleep_ms(2000); // Aguarda 2 s.
            gpio_put(LED_VERDE, 0); // Desliga o LED VERDE.

            inicial = tempo; // Reinicia a contagem.
        }
    }
}
