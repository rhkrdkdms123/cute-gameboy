/**
 * @file hal.h
 * @author aheun kwak (rhkrdkdms1234@gmail.com)
 * @brief declarations for input/output functions and types
 * @details This file contains declarations for input/output functions and types used in the retro game project.
 * @version 0.1
 * @date 2025-09-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once
#include <stdbool.h>

// #define PICO   
#define LCD_WIDTH 16
#define LCD_HEIGHT 8

#define BUTTON_UP 'w'
#define BUTTON_DOWN 's'
#define BUTTON_LEFT 'a'
#define BUTTON_RIGHT 'd'
#define BUTTON_A 'j'
#define BUTTON_B 'k'

typedef struct {
    bool up, down, left, right;
    bool a, b;
} Buttons;

typedef enum {STATE_MENU, STATE_PLAY, STATE_SCORE} GameState;

// Platform-independent interface
void io_init();  // Initialize hardware or simulation
Buttons read_buttons();
void lcd_clear();
void lcd_draw(int x, int y, char c);
void io_update(GameState state, int selected_game, int score);
void play_sound(int freq, int duration_ms);
