#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "inc/ssd1306.h"
#include <string.h>

// Definição dos pinos para UART
#define UART_ID uart0
#define BAUD_RATE 9600
#define TX_PIN 17
#define RX_PIN 16

// Definição dos pinos para o OLED SSD1306
#define I2C_SDA 14
#define I2C_SCL 15

// Definição dos pinos para PWM dos LEDs
#define LED_GREEN 12
#define LED_RED 13
#define PERIOD 2000
#define DIVIDER_PWM 16.0

// Buffer do display OLED
uint8_t ssd[ssd1306_buffer_length];

// Área de renderização do display
struct render_area frame_area = {
    .start_column = 0,
    .end_column = ssd1306_width - 1,
    .start_page = 0,
    .end_page = ssd1306_n_pages - 1
};

// Configuração do PWM para LEDs
void setup_pwm(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(pin);
    pwm_set_clkdiv(slice, DIVIDER_PWM);
    pwm_set_wrap(slice, PERIOD);
    pwm_set_gpio_level(pin, 0);
    pwm_set_enabled(slice, true);
}

// Acionar LED via PWM
void set_led_state(uint led, bool active) {
    pwm_set_gpio_level(led, active ? PERIOD / 2 : 0);
}

// Inicializar UART
void init_uart() {
    stdio_init_all();
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(RX_PIN, GPIO_FUNC_UART);
}

// Enviar dados via UART
void send_data(const char *message) {
    uart_puts(UART_ID, message);
}

// Inicializar Display SSD1306
void setup_display() {
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init();
    calculate_render_area_buffer_length(&frame_area);
}

// Limpar tela OLED
void limpar_tela() {
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);
}

int main() {
    // Inicializar todos os componentes
    init_uart();
    setup_display();
    setup_pwm(LED_GREEN);
    setup_pwm(LED_RED);

    // Limpar tela e mostrar mensagem inicial
    limpar_tela();
    ssd1306_draw_string(ssd, 5, 5, "Monitorando...");
    render_on_display(ssd, &frame_area);

    while (1) {
        // Simulação de leitura UART
        send_data("Teste UART\n");

        // Controle de LED PWM
        set_led_state(LED_GREEN, true);
        set_led_state(LED_RED, false);

        // Atualizar Display OLED
        limpar_tela();
        ssd1306_draw_string(ssd, 5, 5, "Analise OK!");
        render_on_display(ssd, &frame_area);

        sleep_ms(1000);

        // Alternar LEDs para simulação de estado
        set_led_state(LED_GREEN, false);
        set_led_state(LED_RED, true);

        sleep_ms(1000);
    }
}