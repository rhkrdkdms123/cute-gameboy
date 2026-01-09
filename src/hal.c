/**
 * @file hal.c
 * @author aheun kwak (rhkrdkdms1234@gmail.com)
 * @brief definitions for input/output functions and types
 * @details This file contains definitions for input/output functions and types used in the retro game project
 * @version 0.1
 * @date 2025-09-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#if defined(PICO)
#include "hal.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "ssd1306.h"

#define UP 2
#define DOWN 3
#define LEFT 4
#define RIGHT 5
#define A 6
#define B 7
#define SPEAKER_PIN 15
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9
#define OLED_ADDR 0x3C

void io_init() {
    int pins[] = {UP, DOWN, LEFT, RIGHT, A, B};
    for(int i=0;i<6;i++){
        gpio_init(pins[i]);
        gpio_set_dir(pins[i], GPIO_IN);
        gpio_pull_up(pins[i]);
    }

    // I2C OLED init
    i2c_init(I2C_PORT, 400*1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(I2C_PORT, OLED_ADDR, 16, 8); // 16x8 LCD

    // PWM speaker init
    gpio_set_function(SPEAKER_PIN, GPIO_FUNC_PWM);
    pwm_set_enabled(pwm_gpio_to_slice_num(SPEAKER_PIN), false);

    lcd_clear();
}

Buttons read_buttons() {
    Buttons btn;
    btn.up    = !gpio_get(UP);
    btn.down  = !gpio_get(DOWN);
    btn.left  = !gpio_get(LEFT);
    btn.right = !gpio_get(RIGHT);
    btn.a     = !gpio_get(A);
    btn.b     = !gpio_get(B);
    return btn;
}

void lcd_clear() { ssd1306_clear(); }

void lcd_draw(int x, int y, char c) { ssd1306_draw_pixel(x, y, 1); }

void io_update(GameState state, int selected_game, int score) { ssd1306_refresh(); }

void play_sound(int freq, int duration_ms) {
    uint slice = pwm_gpio_to_slice_num(SPEAKER_PIN);
    pwm_set_wrap(slice, 125000000/freq - 1);
    pwm_set_chan_level(slice, PWM_CHAN_A, pwm_get_wrap(slice)/2);
    pwm_set_enabled(slice, true);
    sleep_ms(duration_ms);
    pwm_set_enabled(slice, false);
}
#else
 // Simulation mode: this part is ignored in actual hardware
#include "hal.h"
#include <stdio.h>
#include <conio.h> // _kbhit, _getch
#include <windows.h>

// Virtual LCD buffer
static char lcd_buffer[LCD_HEIGHT][LCD_WIDTH+1]; // +1 for null terminator

void io_init() {
    // Nothing to init for console
}

Buttons read_buttons() {
    Buttons btn = {0};
    if (_kbhit()) {
        int c = _getch();
        switch(c) {
            case BUTTON_UP: btn.up=1; break;
            case BUTTON_DOWN: btn.down=1; break;
            case BUTTON_LEFT: btn.left=1; break;
            case BUTTON_RIGHT: btn.right=1; break;
            case BUTTON_A: btn.a=1; break;
            case BUTTON_B: btn.b=1; break;
        }
    }
    return btn;
}


// Clear LCD buffer and console
void lcd_clear() {
    for (int y = 0; y < LCD_HEIGHT; y++) {
        for (int x = 0; x < LCD_WIDTH; x++) {
            lcd_buffer[y][x] = ' ';  // empty space = off pixel
        }
        lcd_buffer[y][LCD_WIDTH] = '\0'; // end of string
    }
    system("cls"); // clear console
}

// Draw a character at position (x, y)
void lcd_draw(int x, int y, char c) {
    if (x >= 0 && x < LCD_WIDTH && y >= 0 && y < LCD_HEIGHT) {
        lcd_buffer[y][x] = c;  // use given character
    }
}

// Print buffer content to console
void io_update(GameState state, int selected_game, int score) {

    switch(state) {
        case STATE_MENU:
            printf("MENU - Select Game: %d\n", selected_game);
            break;
        case STATE_PLAY:
            printf("PLAY - Score: %d\n", score);
            break;
        case STATE_SCORE:
            printf("SCORE SCREEN - Final Score: %d\n", score);
            break;
    }
    /*
    system("cls");  // clear console
    printf("Score: %d\n", score);
    printf("----------------\n"); // separator line (16 chars)

    for (int y = 0; y < LCD_HEIGHT; y++) {
        printf("%s\n", lcd_buffer[y]);
    }
    */
}

void play_sound(int freq, int duration_ms) {
    Beep(freq, duration_ms );
}
#endif