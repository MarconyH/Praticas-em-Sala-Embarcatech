#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/irq.h"

#define JOY_Y_ADC_CHANNEL 1   // ADC1 = GPIO27
#define LED_VERMELHO 12       // LED ânodo comum
#define LIMITE_ACIONAMENTO 3500

volatile bool acionado = false;

// Função de tratamento da interrupção do ADC
void adc_irq_handler() {
    // Ler o valor do FIFO do ADC
    uint16_t valor = adc_fifo_get(); 

    if (valor > LIMITE_ACIONAMENTO) {
        if (!acionado) {
            acionado = true;
            gpio_put(LED_VERMELHO, 0); // Acende LED
            printf("Acionado! Valor do ADC: %d\n", valor);
        }
    } else {
        if (acionado) {
            acionado = false;
            gpio_put(LED_VERMELHO, 1); // Apaga LED
            printf("Desativado. Valor do ADC: %d\n", valor);
        }
    }

    // Limpa a interrupção (automático com FIFO no RP2040)
}

int main() {
    stdio_init_all();
    sleep_ms(2000);
    printf("🎮 Sistema iniciado. Aguarde...\n");

    // Inicializa o LED
    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_put(LED_VERMELHO, 1); // LED apagado inicialmente

    // Inicializa o ADC
    adc_init();
    adc_gpio_init(27); // GPIO27 = ADC1
    adc_select_input(JOY_Y_ADC_CHANNEL);

    // Configura FIFO do ADC
    adc_fifo_setup(
        true,    // Enabling the FIFO
        true,    // Enable DMA data request (não vamos usar aqui, mas o pico SDK recomenda deixar true)
        1,       // DREQ (número de amostras antes de gerar IRQ) = 1
        false,   // Trigger only on threshold (não)
        false    // No error bit
    );

    // Configura a interrupção
    irq_set_exclusive_handler(ADC_IRQ_FIFO, adc_irq_handler);
    irq_set_enabled(ADC_IRQ_FIFO, true);
    adc_irq_set_enabled(true);

    // Começa a primeira conversão
    adc_run(true);
    adc_fifo_drain(); // Limpa qualquer dado antigo

    while (true) {
        adc_run(true); // Garante que ADC esteja rodando
        adc_fifo_drain(); // Limpa qualquer sobra
        adc_hw->cs |= ADC_CS_START_ONCE_BITS; // Começa uma conversão manualmente
        sleep_ms(50); // Pequeno delay para não gerar muitas leituras (pode ser ajustado)
    }
}
