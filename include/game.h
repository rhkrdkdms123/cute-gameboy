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

typedef enum
{
    GAME_TYPE_A,
    GAME_TYPE_B,
    GAME_TYPE_C,
    GAME_COUNT
} GameType_t;

GameType_t menu_update(Buttons btn, GameType_t selected_game, GameState *state);
int game_update(GameType_t selected_game, GameState *state, int score, Buttons btn, int *frame_count);
void score_screen_update(Buttons btn, int score, GameState *state);