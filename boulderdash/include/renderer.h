#ifndef RENDERER_H
#define RENDERER_H

#include <stdbool.h>

// Renderer type enumeration
typedef enum {
    RENDERER_MODERN_SHADER = 0,
    RENDERER_OLD_SCHOOL = 1,
    RENDERER_RETRO = 2
} RendererType;

// Game state structure
typedef struct {
    float player_pos[2];
    float camera_center[2];
    float camera_mod[4];
    float smashed;
    int gems;
    int map_width;
    int map_height;
    int* game_map;  // 2D map stored as 1D array
    int* game_states;  // States for each tile
    float* texmods;  // Texture modifications for each tile
} GameState;

// Function pointer types for renderer functions
typedef void (*InitRendererFunc)(int width, int height);
typedef void (*RenderFunc)(GameState* state);
typedef void (*CleanupRendererFunc)(void);

// Renderer interface structure
typedef struct {
    RendererType type;
    InitRendererFunc init;
    RenderFunc render;
    CleanupRendererFunc cleanup;
    const char* name;
} Renderer;

// Game state management functions
GameState* get_game_state(void);
void set_game_state(GameState* state);

// Main renderer functions
void renderer_init(RendererType type, int width, int height);
void renderer_render(GameState* state);
void renderer_cleanup(void);
void renderer_switch(RendererType new_type);

// Game logic functions
void init_game_state(GameState* state, int width, int height);
void update_game_state(GameState* state, float delta_time);
void cleanup_game_state(GameState* state);

// Input handling
void handle_input(GameState* state);

// Physics functions
void update_physics(GameState* state);

#endif