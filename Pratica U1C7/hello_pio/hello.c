/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
// Nosso programa PIO compilado:
#include "hello.pio.h"

// Este exemplo usa o pino padrão do LED
// Você pode alterá-lo definindo HELLO_PIO_LED_PIN para usar um GPIO diferente
#define HELLO_PIO_LED_PIN 11

// Verifica se o pino padrão do LED foi definido e o configura, se disponível
#if !defined HELLO_PIO_LED_PIN && defined PICO_DEFAULT_LED_PIN
#define HELLO_PIO_LED_PIN PICO_DEFAULT_LED_PIN
#endif

// Verifica se o pino é compatível com a plataforma (GPIO deve ser menor que 32)
#if HELLO_PIO_LED_PIN >= NUM_BANK0_GPIOS
#error Tentativa de usar um pino >=32 em uma plataforma que não o suporta
#endif

int main() {
#ifndef HELLO_PIO_LED_PIN
#warning O exemplo pio/hello_pio requer uma placa com um LED regular
#else
    // Declarações de variáveis:
    PIO pio;      // Variável para armazenar o periférico PIO (pio0 ou pio1)
    uint sm;      // Índice da State Machine (máquina de estado) que será usada
    uint offset;  // Offset do programa PIO carregado na memória PIO

    // Inicializa a comunicação UART padrão (utilizada para debug e mensagens)
    setup_default_uart();

    // Localiza um PIO e uma máquina de estado disponíveis e carrega nosso programa
    // Esta função localiza e reserva automaticamente um PIO e uma state machine (SM)
    // Carrega o programa PIO diretamente, configurando o GPIO especificado
    bool success = pio_claim_free_sm_and_add_program_for_gpio_range(
        &hello_program, // Programa PIO gerado (compilado)
        &pio,           // Ponteiro para armazenar o PIO selecionado (pio0 ou pio1)
        &sm,            // Ponteiro para armazenar o índice da state machine
        &offset,        // Ponteiro para armazenar o offset do programa carregado
        HELLO_PIO_LED_PIN, // GPIO onde o programa atuará (LED)
        1,              // Número de GPIOs configurados (apenas 1, para o LED)
        true            // Permite uso de GPIOs acima de 32, se suportado
    );

    // Verifica se o carregamento do programa PIO foi bem-sucedido
    hard_assert(success);

    // Configura a máquina de estado (SM) para executar nosso programa PIO
    // A função "hello_program_init" é gerada automaticamente pelo programa PIO
    printf("Usando o GPIO %d\n", HELLO_PIO_LED_PIN);
    hello_program_init(pio, sm, offset, HELLO_PIO_LED_PIN);

    // A máquina de estado (SM) agora está em execução
    // Qualquer valor enviado ao FIFO TX da SM será refletido no pino do LED

    // Loop principal: mantém o LED piscando enquanto nenhuma tecla for pressionada
    while (getchar_timeout_us(0) == PICO_ERROR_TIMEOUT) {
        // Envia o valor "1" para o FIFO TX da máquina de estado
        // O LED será ligado (conforme o programa PIO)
        pio_sm_put_blocking(pio, sm, 1);
        sleep_ms(500); // Aguarda 500 milissegundos

        // Envia o valor "0" para o FIFO TX da máquina de estado
        // O LED será desligado
        pio_sm_put_blocking(pio, sm, 0);
        sleep_ms(500); // Aguarda 500 milissegundos
    }

    // O loop termina quando uma tecla é pressionada
    // Este comando libera os recursos, removendo o programa PIO da SM
    pio_remove_program_and_unclaim_sm(&hello_program, pio, sm, offset);
#endif
}
