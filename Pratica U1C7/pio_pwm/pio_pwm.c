/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"   // Biblioteca padrão do Raspberry Pi Pico
#include "hardware/pio.h"  // Biblioteca para controle do periférico PIO
#include "pwm.pio.h"       // Programa PIO para controle de PWM (compilado)

// Define o pino do LED como GPIO 13
#define LED_PIN 13

/**
 * Função: pio_pwm_set_period
 * Descrição: Define o período do sinal PWM (duração total de um ciclo)
 * - O período é o tempo total (em ticks) para que o sinal complete um ciclo (alto + baixo).
 * - Este valor é enviado para o registrador ISR (Input Shift Register) da máquina de estado (SM).
 * 
 * Parâmetros:
 * - pio: ponteiro para o periférico PIO (pio0 ou pio1)
 * - sm: índice da State Machine (máquina de estado) a ser configurada
 * - period: valor do período em ticks (uint32_t)
 */
void pio_pwm_set_period(PIO pio, uint sm, uint32_t period) {
    // Desativa temporariamente a máquina de estado (SM) para configuração
    pio_sm_set_enabled(pio, sm, false);

    // Envia o valor do período para o FIFO (First In First Out) da SM
    pio_sm_put_blocking(pio, sm, period);

    // Executa comandos diretamente na máquina de estado (SM):
    // - PULL: puxa o valor do FIFO para o registrador de entrada (ISR)
    // - OUT: transfere o valor do ISR para o registrador interno da SM
    pio_sm_exec(pio, sm, pio_encode_pull(false, false)); // PULL (sem mover FIFO)
    pio_sm_exec(pio, sm, pio_encode_out(pio_isr, 32));   // OUT (envia ao ISR)

    // Reativa a máquina de estado (SM) após configurar o período
    pio_sm_set_enabled(pio, sm, true);
}

/**
 * Função: pio_pwm_set_level
 * Descrição: Define o nível do sinal PWM (tempo em nível alto)
 * - O nível (duty cycle) é o tempo em que o sinal permanece em nível alto.
 * - Este valor é armazenado no registrador X da máquina de estado (SM).
 * 
 * Parâmetros:
 * - pio: ponteiro para o periférico PIO (pio0 ou pio1)
 * - sm: índice da State Machine (máquina de estado) a ser configurada
 * - level: valor do nível (duty cycle) em ticks (uint32_t)
 */
void pio_pwm_set_level(PIO pio, uint sm, uint32_t level) {
    // Envia o valor de nível (duty cycle) diretamente para o FIFO da SM
    // O valor será transferido para o registrador X da máquina de estado
    pio_sm_put_blocking(pio, sm, level);
}

/**
 * Função: main
 * Descrição: Função principal do programa
 * - Configura o PIO e a máquina de estado (SM) para gerar sinal PWM
 * - Controla o brilho de um LED conectado ao GPIO 13
 */
int main() {
    // Inicializa a comunicação serial padrão (para debug com printf)
    stdio_init_all();
    
    // Verifica se o LED está definido corretamente
#ifndef LED_PIN
#warning pio/pwm example requires a board with a regular LED
    puts("LED pin was not defined");
#else
    // Configuração do PIO e da Máquina de Estado (SM)
    PIO pio = pio0; // Utiliza o periférico PIO 0 (pio0)
    int sm = 0;     // Utiliza a State Machine 0 (primeira máquina de estado)

    // Carrega o programa PIO (PWM) e retorna o offset de memória onde foi carregado
    uint offset = pio_add_program(pio, &pwm_program);
    printf("Programa PWM carregado no offset %d\n", offset);

    // Inicializa a máquina de estado com o programa PWM configurado para GPIO 13
    // Esta função (pwm_program_init) é gerada automaticamente pelo arquivo pwm.pio.h
    pwm_program_init(pio, sm, offset, LED_PIN);

    // Define o período do PWM como 2^16 - 1 (65535 ticks)
    // - Um período maior proporciona maior precisão no controle do brilho
    // - Um ciclo completo (ligado + desligado) tem duração de 65535 ticks
    pio_pwm_set_period(pio, sm, (1u << 16) - 1);

    // Variável para controle do nível de brilho (duty cycle)
    int level = 0;

    // Loop principal: ajusta o brilho do LED de forma suave
    while (true) {
        // Exibe o nível de brilho atual no terminal serial
        printf("Nível de brilho = %d\n", level);
        
        // Define o nível de brilho (duty cycle) do PWM
        // - O brilho é calculado com base em level^2 para criar um efeito suave
        pio_pwm_set_level(pio, sm, level * level);

        // Incrementa o nível de brilho (quadrático para efeito suave)
        level = (level + 1) % 256; // Nível vai de 0 a 255 e reinicia

        // Aguarda 10 milissegundos antes de atualizar o brilho
        sleep_ms(10);
    }
#endif
}
