#include <stdio.h>
#include "adc.h"
#include "hardware/pio.h"
#include "hardware/uart.h"
#include "motor.h"
#include "pico/stdlib.h"
#include "pico/stdio_uart.h"
#include "pwm.h"
#include "qenc.h"
#include "servo.h"

Pwm pwm[4] = {Pwm(20, 50000), Pwm(11, 50000), Pwm(12, 50000), Pwm(21, 50000)};
Servo servo(2);
Qenc enc[2] = {Qenc(6), Qenc(3)};

Motor motor[2] = {Motor(pwm[1], pwm[0], enc[0]), Motor(pwm[3], pwm[2], enc[1])};
Adc adc[3] = {Adc(26), Adc(27), Adc(28)};

char buf[255];

void readline(char* buf) {
    int i = 0;
    while (1) {
        char c = getchar();
        if (c == '\n') {
            break;
        }
        buf[i++] = c;
    }
    buf[i] = 0;
}

bool timer_cb(repeating_timer_t* rt) {
    motor[0].timer_cb();
    motor[1].timer_cb();
    return true;
}

bool timer_cb_pos(repeating_timer_t* rt) {
    motor[0].timer_cb_pos();
    motor[1].timer_cb_pos();
    return true;
}

void initTimer() {
    static repeating_timer_t timer;
    static repeating_timer_t timer1;
    add_repeating_timer_ms(-10, timer_cb, NULL, &timer);
    add_repeating_timer_ms(-100, timer_cb_pos, NULL, &timer1);
}

void setup() {
    // stdio_init_all();

        // UART0を初期化（ボーレート115200）
    uart_init(uart0, 115200);
    
    // UART0のピンを設定（GP0 = TX, GP1 = RX）
    gpio_set_function(0, GPIO_FUNC_UART);
    gpio_set_function(1, GPIO_FUNC_UART);
    
    // UARTをstdioとして設定
    stdio_uart_init_full(uart0, 115200, 0, 1);
    
    gpio_init(20);
    gpio_init(11);
    gpio_init(12);
    gpio_init(21);

    motor[0].init();
    motor[1].init();

    motor[0].setVelGain(1, 0.0, 0.09);
    motor[0].setPosGain(2.5, 0.0, 0.09);
    motor[1].setVelGain(1, 0.0, 0.09);
    motor[1].setPosGain(2.5, 0.0, 0.09);
    servo.init();
    initTimer();
}

int main() {
    setup();
   
    while (true) {
        readline(buf);
        int id = 0;
        int mode = 0;
        double val = 0;
        sscanf(buf, "%d %d %lf", &id, &mode, &val);
        printf("id: %d mode: %d val: %f\n", id, mode, val);
        switch (id) {
            case 0:
                if (!mode) {
                    motor[0].disablePosPid();
                    motor[0].setVel(val);
                } else {
                    motor[0].resetPos();
                    motor[0].setPos(val);
                }
                break;
            case 1:
                if (!mode) {
                    motor[1].disablePosPid();
                    motor[1].setVel(val);
                } else {
                    motor[1].resetPos();
                    motor[1].setPos(val);
                }
                break;
            case 2:
                servo.write((int)val);
                break;
        }
        sleep_ms(1);
    }
}