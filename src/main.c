/**
 * @file main.c
 * @author aheun kwak (rhkrdkdms1234@gmail.com)
 * @brief main entry point for the retro game project
 * @details This file contains the main function which initializes the system and runs the game loop.
 * @version 0.1
 * @date 2025-09-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "hal.h"
#include "game.h"
#ifdef PICO
// #include "pico/stdlib.h"
#else
#include <windows.h>
#define sleep_ms(ms) Sleep(ms)
#endif

#define FPS 10  // Base frames per second

int main() {
    #ifdef PICO
    stdio_init_all();
    #endif
    io_init();

    GameState state = STATE_MENU;
    GameType_t selected_game = 0;
    int score = 0;
    int frame_count = 0;

    while(1) {
        Buttons btn = read_buttons();

        switch(state) {
            case STATE_MENU:
                selected_game = menu_update(btn, selected_game, &state);
                break;

            case STATE_PLAY:
                score = game_update(selected_game, &state, score, btn, &frame_count);
                break;

            case STATE_SCORE:
                score_screen_update(btn, score, &state);
                break;
        }

        io_update(state, selected_game, score);

        sleep_ms(1000 / FPS);
    }
}
