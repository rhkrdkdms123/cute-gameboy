/**
 * @file game.c
 * @author aheun kwak (rhkrdkmds1234@gmail.com)
 * @brief Implementation of game logic functions
 * @details This file contains the implementation of game logic functions including menu handling, game updates, and score screen updates.
 * @version 0.1
 * @date 2025-09-15
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "game.h"
#include "hal.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define PLAYER_CHAR 'P'
#define OBSTACLE_CHAR 'X'
#define MAX_GROUP_SIZE 4

typedef struct
{
    int x;
    int y;
    int speed;
    int active;
} Obstacle;

static int player_x;
static Obstacle obstacles[MAX_OBSTACLES];
static int base_speed_max = 3;
static int group_timer = 0;
static int group_interval = 3;
static int score_for_difficulty = 0;

GameType_t menu_update(Buttons btn, GameType_t selected_game, GameState *state)
{
    // Move selection up (decrease index, wrap around)
    if (btn.up)
    {
        selected_game = (selected_game + GAME_COUNT - 1) % GAME_COUNT;
#ifdef DEBUG
        printf("button up, Selected game: %d\n", selected_game);
#endif
    }
        

    // Move selection down (increase index)
    if (btn.down)
    {
        selected_game = (selected_game + 1) % GAME_COUNT;
#ifdef DEBUG
        printf("button down, Selected game: %d\n", selected_game);
#endif
    }
        
    // If 'A' button pressed → start game
    if (btn.a)
    {
        *state = STATE_PLAY;   // Switch to play state
        play_sound(1000, 100); // Play confirm sound

        // Initialize game variables
        init_game(selected_game);

        /*
        // Reset player and obstacles
        player_x = LCD_WIDTH/2;
        for(int i=0;i<MAX_OBSTACLES;i++){
            obstacles[i].active = 0;
            obstacles[i].y = -1;
        }

        // Reset difficulty
        group_timer = 0;
        group_interval = 3 + rand()%4;
        score_for_difficulty = 0;
        */
    }

    return selected_game;
}

// Initialize game variables based on selected game type
void init_game(GameType_t selected_game)
{
    switch (selected_game)
    {
    case GAME_TYPE_A:
        // Reset player and obstacles
        player_x = LCD_WIDTH/2;
        for(int i=0;i<MAX_OBSTACLES;i++){
            obstacles[i].active = 0;
            obstacles[i].y = -1;
        }

        // Reset difficulty
        group_timer = 0;
        group_interval = 3 + rand()%4;
        score_for_difficulty = 0;
        break;
    case GAME_TYPE_B:
        // base_speed_max = 4;
        break;
    case GAME_TYPE_C:
        // base_speed_max = 5;
        break;
    default:
        // base_speed_max = 3;
        break;
    }
}

int PixelRush_update(GameState *state, int score, Buttons btn, int *frame_count)
{
    (*frame_count)++;

    // Dynamic difficulty
    score_for_difficulty = score;
    int current_speed_max = base_speed_max + score_for_difficulty / 5;
    int current_group_max = 2 + (score_for_difficulty / 10);
    if (current_group_max > MAX_GROUP_SIZE)
        current_group_max = MAX_GROUP_SIZE;

    // Player movement
    if (btn.left)
        player_x = (player_x > 0) ? player_x - 1 : 0;
    if (btn.right)
        player_x = (player_x < LCD_WIDTH - 1) ? player_x + 1 : LCD_WIDTH - 1;

    // Update obstacles
    for (int i = 0; i < MAX_OBSTACLES; i++)
    {
        if (!obstacles[i].active)
            continue;

        if ((*frame_count) % (6 - obstacles[i].speed) == 0)
            obstacles[i].y++;

        if (obstacles[i].y == LCD_HEIGHT - 1 && obstacles[i].x == player_x)
        {
            *state = STATE_SCORE;
            play_sound(500, 200);
        }

        if (obstacles[i].y >= LCD_HEIGHT)
        {
            obstacles[i].active = 0;
            score++;
            play_sound(800, 50);
        }
    }

    // Spawn new obstacle group
    group_timer++;
    if (group_timer >= group_interval)
    {
        group_timer = 0;
        group_interval = 2 + rand() % 4;

        int group_size = 2 + rand() % current_group_max;
        int base_x = rand() % (LCD_WIDTH - group_size + 1);
        int spawned = 0;
        for (int i = 0; i < MAX_OBSTACLES && spawned < group_size; i++)
        {
            if (!obstacles[i].active)
            {
                obstacles[i].active = 1;
                obstacles[i].x = base_x + spawned;
                obstacles[i].y = 0;
                obstacles[i].speed = 1 + rand() % current_speed_max;
                spawned++;
            }
        }
    }

    /*
    // LCD update
    lcd_clear();
    lcd_draw(player_x, LCD_HEIGHT - 1, PLAYER_CHAR);
    for (int i = 0; i < MAX_OBSTACLES; i++)
        if (obstacles[i].active)
            lcd_draw(obstacles[i].x, obstacles[i].y, OBSTACLE_CHAR);
    */

    return score;
}

int game_update(GameType_t selected_game, GameState *state, int score, Buttons btn, int *frame_count)
{
    switch (selected_game)
    {
    case GAME_TYPE_A:
        return PixelRush_update(state, score, btn, frame_count);
        break;
    case GAME_TYPE_B:
        return PixelRush_update(state, score, btn, frame_count);
        break;
    case GAME_TYPE_C:
        return PixelRush_update(state, score, btn, frame_count);
        break;
    default:
    #ifdef DEBUG
        assert(0 && "Invalid game type");  // for debugging
    #endif
        return PixelRush_update(state, score, btn, frame_count);
        break;
    }

    return 0;
}

void score_screen_update(Buttons btn, int score, GameState *state)
{
    if (btn.a)
        *state = STATE_MENU;
}
