#include "../include/renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// This is a placeholder for the old school software renderer
// In a real implementation, this would use software rendering techniques

static int window_width = 800;
static int window_height = 600;

static void old_school_init(int width, int height) {
    window_width = width;
    window_height = height;
    printf("Old school software renderer initialized: %dx%d\n", width, height);
    // In a real implementation, we would:
    // 1. Initialize a software rendering surface
    // 2. Set up any necessary buffers
}

static void old_school_render(GameState* state) {
    printf("Rendering with old school software renderer\n");
    // In a real implementation, we would:
    // 1. Render the game state using software algorithms
    // 2. Draw tiles, player, diamonds, stones, etc. pixel by pixel
    
    // For demonstration purposes, simply print the map
    printf("Game Map (first few rows):\n");
    for (int y = 0; y < (state->map_height < 10 ? state->map_height : 10); y++) {
        for (int x = 0; x < (state->map_width < 20 ? state->map_width : 20); x++) {
            int idx = y * state->map_width + x;
            int tile = state->game_map[idx];
            char tile_char;
            
            switch (tile) {
                case 0: tile_char = '.'; break; // EMPTY
                case 1: tile_char = ':'; break; // SAND
                case 2: tile_char = '#'; break; // WALL
                case 3: tile_char = 'O'; break; // STONE
                case 4: tile_char = '*'; break; // DIAMOND
                case 5: tile_char = 'P'; break; // PLAYER
                default: tile_char = '?'; break;
            }
            
            printf("%c", tile_char);
        }
        printf("\n");
    }
}

static void old_school_cleanup(void) {
    printf("Cleaning up old school software renderer\n");
    // In a real implementation, we would:
    // 1. Free any allocated rendering buffers
}

// Define the renderer interface
Renderer old_school_renderer = {
    .type = RENDERER_OLD_SCHOOL,
    .init = old_school_init,
    .render = old_school_render,
    .cleanup = old_school_cleanup,
    .name = "Old School Software Renderer"
};