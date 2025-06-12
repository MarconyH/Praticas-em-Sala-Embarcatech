#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c1
#define I2C_SDA 18
#define I2C_SCL 19

/* Código de exemplo para comunicação com o acelerômetro e giroscópio MPU6050

   Este exemplo utiliza uma abordagem simples de leitura direta dos registradores.
   É perfeitamente possível conectar uma linha de interrupção e configurar o uso do FIFO interno,
   o que o tornaria mais útil.
*/

// Por padrão, estes dispositivos utilizam o endereço 0x68 no barramento I2C
static int addr = 0x68;

#ifdef I2C_PORT
// Função para resetar o MPU6050
static void mpu6050_reset() {
    // Envia dois bytes: o registrador e o valor de reset (0x80)
    uint8_t buf[] = {0x6B, 0x80};
    i2c_write_blocking(I2C_PORT, addr, buf, 2, false);
}

// Função para ler dados brutos do acelerômetro, giroscópio e temperatura
static void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t *temp) {
    uint8_t buffer[6];

    // Lê os dados do acelerômetro a partir do registrador 0x3B (6 bytes)
    uint8_t val = 0x3B;
    i2c_write_blocking(I2C_PORT, addr, &val, 1, true); // true para manter o controle do barramento
    i2c_read_blocking(I2C_PORT, addr, buffer, 6, false);

    for (int i = 0; i < 3; i++) {
        accel[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }

    // Lê os dados do giroscópio a partir do registrador 0x43 (6 bytes)
    val = 0x43;
    i2c_write_blocking(I2C_PORT, addr, &val, 1, true);
    i2c_read_blocking(I2C_PORT, addr, buffer, 6, false);

    for (int i = 0; i < 3; i++) {
        gyro[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }

    // Lê os dados da temperatura a partir do registrador 0x41 (2 bytes)
    val = 0x41;
    i2c_write_blocking(I2C_PORT, addr, &val, 1, true);
    i2c_read_blocking(I2C_PORT, addr, buffer, 2, false);

    *temp = buffer[0] << 8 | buffer[1];
}
#endif

int main() {
    stdio_init_all();
    sleep_ms(2000); // Aguarda a inicialização do sistema

#if !defined(I2C_PORT) || !defined(I2C_SDA) || !defined(I2C_SCL)
    // Verifica se os pinos I2C padrão estão definidos
    #warning Exemplo i2c/mpu6050_i2c requer uma placa com pinos I2C
    puts("Os pinos I2C padrão não foram definidos");
    return 0;
#else
    printf("Olá, MPU6050! Lendo dados brutos dos registradores...\n");

    // Inicializa o I2C0 nos pinos padrão (SDA=4, SCL=5 no Pico)
    i2c_init(I2C_PORT, 400 * 1000); // 400 kHz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Torna os pinos I2C visíveis para o picotool
    bi_decl(bi_2pins_with_func(I2C_SDA, I2C_SCL, GPIO_FUNC_I2C));

    // Reseta o MPU6050
    mpu6050_reset();

    int16_t acceleration[3], gyro[3], temp;

    while (1) {
        mpu6050_read_raw(acceleration, gyro, &temp);

        // Estes são os valores brutos do chip. É necessário calibrar para uso prático.
        // Consulte o datasheet para mais informações.
        printf("Acel. X = %d, Y = %d, Z = %d\n", acceleration[0], acceleration[1], acceleration[2]);
        printf("Giro. X = %d, Y = %d, Z = %d\n", gyro[0], gyro[1], gyro[2]);

        // Temperatura interna do chip. Cálculo conforme datasheet.
        printf("Temp. = %f\n", (temp / 340.0) + 36.53);

        sleep_ms(500); // Espera 100 ms antes de repetir a leitura
    }
#endif
}
