#ifndef GAME_H
#define GAME_H

#include "types.h"

#define CITY_SIZE 30
#define BLOCK_BUFFER_X 6
#define BLOCK_BUFFER_Y 6
#define BMAX_X (CITY_SIZE)
#define BMAX_Y (CITY_SIZE)
#define HEIGHT 80
#define THEIGHT 120
#define QUALITY 2

// Game functions
int init_game();
void update_game();
void render_game();
void cleanup_game();

// Input handling
void handle_input();
void handle_mouse_click(int x, int y, int button);
void handle_key_press(int key);

// Game state functions
void reset_game();
int check_win_condition();
void update_counters();

// Block manipulation
Block* get_block(int x, int y);
Cell* get_cell(int x, int y, int z);
void set_block_type(int x, int y, int btype);
void set_block_ltype(int x, int y, int ltype);

// Terrain and building functions
void generate_terrain();
void update_connections();

#endif