#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"  // Controle direto do periférico PIO

#include "blink.pio.h" // Inclusão do programa PIO compilado (arquivo gerado a partir de blink.pio)

/*
 * Função que configura e ativa uma máquina de estado (State Machine) para piscar um pino
 * @param pio - periférico PIO utilizado (pio0 ou pio1)
 * @param sm - número da máquina de estado
 * @param offset - posição do programa PIO na memória
 * @param pin - número do GPIO que será controlado
 * @param freq - frequência de piscar (em Hz)
 */

void blink_pin_forever(PIO pio, uint sm, uint offset, uint pin, uint freq) {
    blink_program_init(pio, sm, offset, pin); // Inicializa a máquina de estado com o programa PIO e configura o GPIO
    pio_sm_set_enabled(pio, sm, true); // Ativa a máquina de estado

    printf("Blinking pin %d at %d Hz\n", pin, freq);

    // Calcula e envia o valor de delay para o programa PIO via FIFO
    // O programa PIO conta n+1 ciclos, então subtrai-se 3 para compensar (instruções extras do PIO)
    pio->txf[sm] = (125000000 / (2 * freq)) - 3; // Calcula o tempo de meio período (meia onda) baseado no clock de 125 MHz
}



int main()
{
    stdio_init_all();
    sleep_ms(2000); // Aguarda 2 segundos (tempo para inicializar terminal serial, útil para debug)

    PIO pio = pio0;  // Seleciona o PIO0 (primeiro periférico PIO disponível)
    uint offset = pio_add_program(pio, &blink_program); // Adiciona o programa PIO na memória do PIO
    printf("Programa carregado no offset %d\n", offset);

    // Vetor com as frequências desejadas de piscar para cada LED
    uint freqs[] = {1, 2, 3};
     // Vetor com os GPIOs onde estão os LEDs
    uint gpios[] = {11, 12, 13};

     // Loop para configurar 3 máquinas de estado diferentes
    for (int i = 0; i < 3; i++) {
        uint sm = i;  // Usa máquinas de estado 0, 1 e 2 (uma para cada LED)
        blink_program_init(pio, sm, offset, gpios[i]); // Inicializa cada SM com o programa e pino correspondente
        pio_sm_set_enabled(pio, sm, true);  // Ativa a SM

         // Calcula o número de ciclos de clock necessários para atingir a frequência desejada
        uint32_t delay_cycles = (125000000 / (2 * freqs[i])) - 3;
        pio->txf[sm] = delay_cycles; // Envia o valor de delay para o FIFO da SM

        // Mensagem no terminal para confirmação
        printf("Piscando GPIO %d a %d Hz (ciclos: %u)\n", gpios[i], freqs[i], delay_cycles);
    }

     // Loop infinito apenas para manter o programa rodando e exibir mensagem a cada segundo
    while (true) {
        printf("Sistema ativo...\n");
        sleep_ms(1000); // Espera 1 segundo entre mensagens
    }
}

