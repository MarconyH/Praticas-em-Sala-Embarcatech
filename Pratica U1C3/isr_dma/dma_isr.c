#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/irq.h"

#define LED_PIN 11  // 🛠️ Corrigido para GPIO11 conforme sua placa

#define TAMANHO 16
uint8_t origem[TAMANHO] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
uint8_t destino[TAMANHO];  // Será preenchido pelo DMA

int canal_dma;  // Canal de DMA que vamos usar

// 🧠 Função de interrupção do DMA
void dma_isr() {
    // Limpa o evento de interrupção para o canal
    dma_hw->ints0 = 1u << canal_dma;

    gpio_put(LED_PIN, 1);  // Acende o LED
    printf("✅ Transferência DMA finalizada!\n");
}

int main() {
    stdio_init_all();
    sleep_ms(2000);  // Tempo para estabilizar comunicação USB
    printf("Iniciando exemplo de DMA com interrupção...\n");

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);  // Apaga LED inicialmente

    // 🚀 Solicita um canal de DMA livre
    canal_dma = dma_claim_unused_channel(true);
    dma_channel_config config = dma_channel_get_default_config(canal_dma);

    // Configura para transferência de 8 bits, com incremento nos endereços
    channel_config_set_transfer_data_size(&config, DMA_SIZE_8);
    channel_config_set_read_increment(&config, true);
    channel_config_set_write_increment(&config, true);

    // ⚙️ Configura interrupção
    dma_channel_set_irq0_enabled(canal_dma, true);
    irq_set_exclusive_handler(DMA_IRQ_0, dma_isr);
    irq_set_enabled(DMA_IRQ_0, true);

    // 🔁 Inicia a transferência de origem -> destino
    dma_channel_configure(
        canal_dma,
        &config,
        destino,    // endereço destino
        origem,     // endereço origem
        TAMANHO,    // quantidade de bytes
        true        // iniciar imediatamente
    );

    // Loop vazio aguardando interrupção
    while (true) {
        tight_loop_contents();
    }

    return 0;
}
