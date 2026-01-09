/**
 * screen.c
 * LCD viewport rendering and diff-based updates
 */

#include "screen.h"
#include "hal.h"
#include <string.h>
#include <stdbool.h>

static LCDScreen prev_screen;
static bool prev_inited = false;

/**
 * @brief   Ensure previous screen buffer is initialized
 * @details This function initializes the previous screen buffer to spaces
 *         if it has not been initialized yet.
 */
static void ensure_prev_init(void)
{
    if (!prev_inited) {
        for (int r = 0; r < LCD_HEIGHT; ++r) {
            for (int c = 0; c < LCD_WIDTH; ++c) prev_screen.buf[r][c] = ' ';
            prev_screen.buf[r][LCD_WIDTH] = '\0';
        }
        prev_inited = true;
    }
}

/**
 * @brief   Render the viewport from GameWorld into an LCDScreen
 * @param   gw   Pointer to the GameWorld structure
 * @param   out  Pointer to the output LCDScreen structure
 */
void render_viewport(const GameWorld *gw, LCDScreen *out)
{
    if (!out) return;
    /* Fill with spaces by default */
    for (int r = 0; r < LCD_HEIGHT; ++r) {
        for (int c = 0; c < LCD_WIDTH; ++c) out->buf[r][c] = ' ';
        out->buf[r][LCD_WIDTH] = '\0';
    }

    if (!gw || !gw->world || gw->world_width <= 0) return;

    for (int r = 0; r < LCD_HEIGHT; ++r) {
        for (int c = 0; c < LCD_WIDTH; ++c) {
            int world_col = gw->view_x + c;
            if (world_col < 0 || world_col >= gw->world_width) continue;
            char ch = gw->world[r * gw->world_width + world_col];
            out->buf[r][c] = ch ? ch : ' ';
        }
    }
}

/**
 * @brief   Apply differences from current LCDScreen to physical LCD
 * @param   cur  Pointer to the current LCDScreen structure
 */
void lcd_apply_diff(const LCDScreen *cur)
{
    if (!cur) return;
    ensure_prev_init();

    for (int r = 0; r < LCD_HEIGHT; ++r) {
        int c = 0;
        while (c < LCD_WIDTH) {
            if (cur->buf[r][c] != prev_screen.buf[r][c]) {
                /* find contiguous run */
                int start = c;
                while (c < LCD_WIDTH && cur->buf[r][c] != prev_screen.buf[r][c]) {
                    /* update single char via HAL */
                    lcd_draw(c, r, cur->buf[r][c]);
                    prev_screen.buf[r][c] = cur->buf[r][c];
                    ++c;
                }
                (void)start;
            } else {
                ++c;
            }
        }
    }
}

/**
 * @brief   Draw a single line to the LCD with diff check
 * @param   lcd_line  Pointer to the LCDLine structure
 */
void lcd_draw_line(const LCDLine *lcd_line)
{
    if (!lcd_line) return;
    ensure_prev_init();
    int r = lcd_line->row;
    if (r < 0 || r >= LCD_HEIGHT) return;
    for (int c = 0; c < LCD_WIDTH && lcd_line->text[c] != '\0'; ++c) {
        if (prev_screen.buf[r][c] != lcd_line->text[c]) {
            lcd_draw(c, r, lcd_line->text[c]);
            prev_screen.buf[r][c] = lcd_line->text[c];
        }
    }
}

/**
 * @brief   Draw the entire LCDScreen to the LCD with diff check
 * @param   screen  Pointer to the LCDScreen structure
 */
void lcd_draw_screen(const LCDScreen *screen)
{
    if (!screen) return;
    lcd_apply_diff(screen);
}

/**
 * @brief   Stream a new column into the GameWorld and update LCD
 * @details Pushes `col_data` as the rightmost column (shifting rows left),
 *          renders the visible viewport and applies only the diff to the LCD.
 * @param   gw        Pointer to mutable GameWorld
 * @param   col_data  Pointer to array of length `gw->world_height` containing
 *                    the characters for the new rightmost column
 */
void world_stream_step(GameWorld *gw, const char *col_data)
{
    if (!gw || !col_data) return;
    world_push_column(gw, col_data);
    LCDScreen cur;
    render_viewport(gw, &cur);
    lcd_apply_diff(&cur);
}

