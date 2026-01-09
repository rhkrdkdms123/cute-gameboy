/**
 * world.c
 * Helper implementations for dynamic / streaming GameWorld
 */

#include "screen.h"
#include <stdlib.h>
#include <string.h>

GameWorld *world_create(int width, int height)
{
    if (width <= 0 || height <= 0) return NULL;
    GameWorld *gw = (GameWorld *)malloc(sizeof(GameWorld));
    if (!gw) return NULL;
    gw->world_width = width;
    gw->world_height = height;
    gw->view_x = 0;
    gw->world = (char *)malloc((size_t)width * (size_t)height);
    if (!gw->world) {
        free(gw);
        return NULL;
    }
    /* initialize with spaces */
    for (int i = 0; i < width * height; ++i) gw->world[i] = ' ';
    return gw;
}

void world_free(GameWorld *gw)
{
    if (!gw) return;
    free(gw->world);
    free(gw);
}

int world_set_tile(GameWorld *gw, int row, int col, char ch)
{
    if (!gw || !gw->world) return -1;
    if (row < 0 || row >= gw->world_height) return -1;
    if (col < 0 || col >= gw->world_width) return -1;
    gw->world[row * gw->world_width + col] = ch;
    return 0;
}

char world_get_tile(const GameWorld *gw, int row, int col)
{
    if (!gw || !gw->world) return '\0';
    if (row < 0 || row >= gw->world_height) return '\0';
    if (col < 0 || col >= gw->world_width) return '\0';
    return gw->world[row * gw->world_width + col];
}

int world_push_column(GameWorld *gw, const char *col_data)
{
    if (!gw || !gw->world || !col_data) return -1;
    /* shift each row left by 1, then set last column from col_data */
    for (int r = 0; r < gw->world_height; ++r) {
        char *row_ptr = &gw->world[r * gw->world_width];
        if (gw->world_width > 1) memmove(row_ptr, row_ptr + 1, (size_t)(gw->world_width - 1));
        row_ptr[gw->world_width - 1] = col_data[r];
    }
    return 0;
}

void world_shift_left(GameWorld *gw, int cols)
{
    if (!gw || !gw->world || cols <= 0) return;
    if (cols >= gw->world_width) {
        /* clear to spaces */
        for (int r = 0; r < gw->world_height; ++r) {
            char *row_ptr = &gw->world[r * gw->world_width];
            for (int c = 0; c < gw->world_width; ++c) row_ptr[c] = ' ';
        }
        return;
    }
    for (int r = 0; r < gw->world_height; ++r) {
        char *row_ptr = &gw->world[r * gw->world_width];
        memmove(row_ptr, row_ptr + cols, (size_t)(gw->world_width - cols));
        /* fill rightmost cols with spaces */
        for (int c = gw->world_width - cols; c < gw->world_width; ++c) row_ptr[c] = ' ';
    }
}

int world_spawn_obstacle(GameWorld *gw, int row, int col, char ch)
{
    return world_set_tile(gw, row, col, ch);
}
