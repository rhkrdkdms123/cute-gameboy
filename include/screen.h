#include "hal.h"
#include "game.h"

/* LCD helper types and prototypes
 * Uses LCD dimensions from hal.h: LCD_WIDTH, LCD_HEIGHT
 */
typedef struct {
    char buf[LCD_HEIGHT][LCD_WIDTH + 1]; /* null-terminated rows */
} LCDScreen;

typedef struct {
    int row; /* 0-based */
    char text[LCD_WIDTH + 1];
} LCDLine;

typedef struct {
    int world_width; /* total columns in the world */
    char *world;     /* row-major buffer: row * world_width + col 
                        e.g., the char at row r, col c in current screen is: 
                        char ch = gw->world[r * gw->world_width + (gw->view_x + c)];*/
    int world_height; /* total rows in the world (useful if != LCD_HEIGHT) */
    int view_x;      /* leftmost column shown on LCD */
} GameWorld;

/* Render the viewport from GameWorld into an LCDScreen (null-terminated rows)
 * and apply differences to the physical LCD using lcd_apply_diff.
 */
void render_viewport(const GameWorld *gw, LCDScreen *out);
void lcd_apply_diff(const LCDScreen *cur);
void lcd_draw_line(const LCDLine *lcd_line);
void lcd_draw_screen(const LCDScreen *screen);
void world_stream_step(GameWorld *gw, const char *col_data);

/* GameWorld helper prototypes (dynamic / streaming-friendly)
 * - world_create/free: allocate/free a mutable world buffer
 * - world_set/get_tile: direct tile access
 * - world_push_column: streaming append a column on the right (shifts left)
 * - world_shift_left: discard left columns (useful for streaming)
 * - world_spawn_obstacle: convenience to place an obstacle character
 */
GameWorld *world_create(int width, int height);
void world_free(GameWorld *gw);
int world_set_tile(GameWorld *gw, int row, int col, char ch);
char world_get_tile(const GameWorld *gw, int row, int col);
int world_push_column(GameWorld *gw, const char *col_data); /* col_data length == world_height */
void world_shift_left(GameWorld *gw, int cols);
int world_spawn_obstacle(GameWorld *gw, int row, int col, char ch);