/*
 * Projeto: Controle de Brilho de LED com PWM usando Timer no Raspberry Pi Pico W
 * Autor: Diego Sampaio
 * 
 * Objetivo:
 * Este programa demonstra como controlar o brilho de um LED conectado à GPIO 12
 * do Raspberry Pi Pico W utilizando modulação por largura de pulso (PWM).
 * 
 * O brilho do LED varia suavemente (efeito de "respiração"), alternando entre
 * valores crescentes e decrescentes com base em uma tabela predefinida.
 * Um timer de software chama uma função periodicamente para atualizar o nível
 * de brilho do LED sem a necessidade de interação constante da CPU principal.
 *
 * O projeto foi implementado sem uso de interrupções DMA, mas com lógica baseada
 * em software eficiente, e é ideal para fins didáticos e projetos simples de automação.
 */

 #include <stdio.h>
 #include "pico/stdlib.h"
 #include "hardware/pwm.h"
 #include "hardware/dma.h"
 #include "hardware/timer.h"
 
 // Define o pino onde o LED está conectado
 #define LED_PIN 12
 
 // Define o valor máximo do contador PWM (0 a 255 = resolução de 8 bits)
 #define PWM_WRAP 255
 
 // Define quantas etapas o efeito de fade terá na subida e descida
 #define FADE_STEPS 256
 
 // Tabela de brilho: 256 valores de subida + 256 de descida = 512 valores
 uint16_t fade_table[FADE_STEPS * 2];
 
 // Índice atual na tabela de brilho
 int current_step = 0;
 
 // Número do slice PWM correspondente ao pino
 uint slice;
 
 // Função que preenche a tabela de brilho com uma rampa de subida e descida
 void init_fade_table() {
     for (int i = 0; i < FADE_STEPS; i++) {
         fade_table[i] = i;                        // Fase de aumento de brilho (0 → 255)
         fade_table[FADE_STEPS + i] = PWM_WRAP - i; // Fase de diminuição de brilho (255 → 0)
     }
 }
 
 // Função de callback chamada automaticamente a cada intervalo de tempo (via timer)
 // Atualiza o brilho do LED com base na tabela fade_table
 bool dma_step_callback(repeating_timer_t *t) {
     uint16_t level = fade_table[current_step];     // Pega o valor atual de brilho
     pwm_set_gpio_level(LED_PIN, level);            // Atualiza o PWM com esse nível
 
     current_step = (current_step + 1) % (FADE_STEPS * 2); // Avança para o próximo valor, com retorno ao início
     return true; // Retorna true para manter o timer ativo
 }
 
 int main() {
     stdio_init_all();  // Inicializa a comunicação padrão (USB serial, etc.)
     sleep_ms(2000);    // Aguarda 2 segundos para o terminal estabilizar (opcional)
 
     init_fade_table(); // Preenche a tabela com os valores de brilho
 
     // Configura o pino 12 como função PWM
     gpio_set_function(LED_PIN, GPIO_FUNC_PWM);
 
     // Obtém o número do "slice" PWM que controla o pino 12
     slice = pwm_gpio_to_slice_num(LED_PIN);
 
     // Define a contagem máxima do PWM (de 0 a 255)
     pwm_set_wrap(slice, PWM_WRAP);
 
     // Define o divisor de clock do PWM — reduz a frequência para o olho perceber a mudança de brilho
     pwm_set_clkdiv(slice, 64.0f);
 
     // Ativa o PWM no slice
     pwm_set_enabled(slice, true);
 
     // Cria um timer repetitivo que chama dma_step_callback a cada 5 milissegundos (~200 Hz)
     static repeating_timer_t timer;
     add_repeating_timer_ms(5, dma_step_callback, NULL, &timer);
 
     // Loop principal — vazio, pois o controle está sendo feito no callback do timer
     while (true) {
         tight_loop_contents(); // Mantém o núcleo ativo com uso mínimo de energia
     }
 }
 