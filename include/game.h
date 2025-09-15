/**
 * @file game.h
 * @author aheun kwak (rhkrdkdms1234@gmail.com)
 * @brief Header file for game logic functions
 * @details This file contains declarations for game logic functions including menu handling, game updates, and score screen updates. 
 * @version 0.1
 * @date 2025-09-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once
#include "hal.h"

#define MAX_OBSTACLES 12

int menu_update(Buttons btn, int selected_game, GameState *state);
int game_update(int selected_game, GameState *state, int score, Buttons btn, int *frame_count);
void score_screen_update(Buttons btn, int score, GameState *state);