#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/irq.h"
#include "hardware/gpio.h"
#include "pico/time.h"
#include "inc/buzzer.h"
#include "inc/led.h"

#define RED_LED_PIN 13
#define BUZZER_PIN 10

// Define os pinos do HC-SR04
const uint TRIG_PIN = 16;
const uint ECHO_PIN = 17;

// Variáveis para cálculo da distância
volatile uint32_t start_time_us = 0;
volatile uint32_t end_time_us = 0;
volatile bool echo_received = false;
volatile float distance_cm = 0.0f;
volatile bool buzzer_active = false;
volatile int time_buzzer = 0;

int64_t turn_on_buzzer(alarm_id_t id, void *user_data);
int64_t turn_off_buzzer(alarm_id_t id, void *user_data);

// Manipulador de interrupção GPIO para o pino ECHO

void echo_pin_irq_handler(uint gpio, uint32_t events) { // <- ADICIONAR ESTES ARGUMENTOS
    if (gpio_get(ECHO_PIN)) {
        // Borda de subida (começo do pulso)
        start_time_us = time_us_32();
    } else {
        // Borda de descida (fim do pulso)
        end_time_us = time_us_32();
        echo_received = true;
    }
}

int64_t turn_on_buzzer(alarm_id_t id, void *user_data)
{
    start_buzzer(BUZZER_PIN); // Alterna o estado do buzzer

    time_buzzer = 10 * (int)distance_cm;
    if (!time_buzzer)
    {
        time_buzzer = 1000; // Se a distância for zero, define um tempo padrão de 1 segundo
    }

    add_alarm_in_ms(500, turn_off_buzzer, NULL, false); // Define alarme para desligar o buzzer após 1 segundo
    // Retorna o tempo para o próximo alarme (1 segundo)
    return 0; // 1000 ms = 1 segundo
}

int64_t turn_off_buzzer(alarm_id_t id, void *user_data)
{
    stop_buzzer(BUZZER_PIN); // Desliga o buzzer

    add_alarm_in_ms(time_buzzer, turn_on_buzzer, NULL, false); // Define alarme para ligar o buzzer novamente após 1 segundo
    // Retorna 0 para não repetir o alarme
    return 0; // Não repetir o alarme
}

int64_t parking_callback(alarm_id_t id, void *user_data)
{
    toggle_led(RED_LED_PIN); // Alterna o LED vermelho
    
    int time = 5000 * (int)distance_cm;
    if (!time)
    {
        time = 1000; // Se a distância for zero, define um tempo padrão de 1 segundo
    }
    
    return time;
}

int main() {
    stdio_init_all();
    initialize_leds((uint[]){RED_LED_PIN}, 1); // Inicializa o LED vermelho
    pwm_init_buzzer(BUZZER_PIN, 10000); // Inicializa o Buzzer com frequência de 1kHz
    // Inicializa o pino TRIG
    gpio_init(TRIG_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    gpio_put(TRIG_PIN, 0); // Garante que TRIG esteja baixo inicialmente

    // Inicializa o pino ECHO
    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);

    // Configura a interrupção para o pino ECHO
    gpio_set_irq_enabled_with_callback(ECHO_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &echo_pin_irq_handler);
    
    add_alarm_in_ms(0, parking_callback, NULL, true);
    add_alarm_in_ms(0, turn_on_buzzer, NULL, true);

    printf("Medição de Distância HC-SR04\n");

    while (true) {
        // Dispara o HC-SR04
        gpio_put(TRIG_PIN, 1);
        sleep_us(10); // Pulso de 10us
        gpio_put(TRIG_PIN, 0);

        echo_received = false; // Reinicia o flag para nova medição
        start_time_us = 0;
        end_time_us = 0;

        // Espera o eco ser recebido ou o tempo limite (ex: 50ms)
        uint32_t timeout_start = time_us_32();
        while (!echo_received && (time_us_32() - timeout_start) < 50000) {
            // Espera
            sleep_us(10);
        }

        if (echo_received && (start_time_us > 0) && (end_time_us > start_time_us)) {
            uint32_t pulse_duration_us = end_time_us - start_time_us;

            // Calcula a distância em cm (velocidade do som é aproximadamente 343 metros/segundo ou 0.0343 cm/us)
            // Distância = (Tempo de voo * Velocidade do som) / 2
            // Distância (cm) = (pulse_duration_us * 0.0343) / 2
            distance_cm = (float)pulse_duration_us * 0.0343 / 2.0;

            printf("Distância: %.2f cm\n", distance_cm);
        } else {
            printf("Tempo limite: Nenhum eco recebido ou pulso inválido.\n");
        }
        sleep_ms(250); // Espera 1 segundo antes da próxima medição

    }

    return 0;
}