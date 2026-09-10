#include "renderer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Current renderer state
static Renderer current_renderer = {0};
static GameState* current_state = NULL;

// Accessor functions for game state
GameState* get_game_state(void) {
    return current_state;
}

void set_game_state(GameState* state) {
    current_state = state;
}

// Forward declarations of renderer implementations
extern Renderer modern_shader_renderer;
extern Renderer old_school_renderer;
extern Renderer retro_renderer;

// Array of available renderers
static Renderer* available_renderers[] = {
    &modern_shader_renderer,
    &old_school_renderer,
    &retro_renderer
};

static int num_renderers = sizeof(available_renderers) / sizeof(Renderer*);

// Find renderer by type
static Renderer* get_renderer(RendererType type) {
    for (int i = 0; i < num_renderers; i++) {
        if (available_renderers[i]->type == type) {
            return available_renderers[i];
        }
    }
    return NULL;
}

void renderer_init(RendererType type, int width, int height) {
    Renderer* renderer = get_renderer(type);
    if (renderer == NULL) {
        fprintf(stderr, "Error: Unknown renderer type %d\n", type);
        return;
    }
    
    // Initialize the selected renderer
    renderer->init(width, height);
    current_renderer = *renderer;
    
    // Initialize game state
    current_state = (GameState*)malloc(sizeof(GameState));
    init_game_state(current_state, width / 32, height / 32);  // Estimate map size based on tile size
}

void renderer_render(GameState* state) {
    if (current_renderer.render != NULL) {
        current_renderer.render(state);
    }
}

void renderer_cleanup(void) {
    if (current_renderer.cleanup != NULL) {
        current_renderer.cleanup();
    }
    
    if (current_state != NULL) {
        cleanup_game_state(current_state);
        free(current_state);
        current_state = NULL;
    }
}

void renderer_switch(RendererType new_type) {
    // Clean up current renderer
    if (current_renderer.cleanup != NULL) {
        current_renderer.cleanup();
    }
    
    // Initialize new renderer
    renderer_init(new_type, 800, 600); // Default size, should be passed from main
}

void init_game_state(GameState* state, int width, int height) {
    state->map_width = width;
    state->map_height = height;
    state->player_pos[0] = width / 2;
    state->player_pos[1] = height / 2;
    state->camera_center[0] = state->player_pos[0];
    state->camera_center[1] = state->player_pos[1];
    state->camera_mod[0] = 1.0f;  // Zoom level
    state->camera_mod[1] = 0.0f;
    state->camera_mod[2] = 0.0f;
    state->camera_mod[3] = 0.0f;
    state->smashed = 0.0f;
    state->gems = 0;
    
    // Allocate and initialize game map
    state->game_map = (int*)malloc(width * height * sizeof(int));
    state->game_states = (int*)malloc(width * height * sizeof(int));
    state->texmods = (float*)malloc(width * height * sizeof(float));
    
    if (state->game_map == NULL || state->game_states == NULL || state->texmods == NULL) {
        fprintf(stderr, "Error: Failed to allocate game state memory\n");
        return;
    }
    
    // Initialize map with basic patterns (empty, sand, walls, etc.)
    for (int i = 0; i < width * height; i++) {
        float hash = ((i * 12.9898 + i * 78.233) * 123.456);
        float value = hash - (int)hash;  // Simple hash function
        
        if (i < width || i >= width * (height - 1) || (i % width) == 0 || (i % width) == width - 1) {
            // Borders are walls
            state->game_map[i] = 2; // WALL
        } else if (value < 0.3) {
            state->game_map[i] = 1; // SAND
        } else if (value < 0.4) {
            state->game_map[i] = 3; // STONE
        } else if (value < 0.45) {
            state->game_map[i] = 4; // DIAMOND
        } else {
            state->game_map[i] = 0; // EMPTY
        }
        
        state->game_states[i] = 0; // S_REST
        state->texmods[i] = value * 10.0f;
    }
    
    // Place player in center
    int player_idx = (height / 2) * width + (width / 2);
    state->game_map[player_idx] = 5; // PLAYER
}

void update_game_state(GameState* state, float delta_time) {
    // Update game state based on physics
    update_physics(state);
    
    // Update camera following player
    float dx = state->player_pos[0] - state->camera_center[0];
    float dy = state->player_pos[1] - state->camera_center[1];
    
    state->camera_center[0] += delta_time * dx * 0.1f;
    state->camera_center[1] += delta_time * dy * 0.1f;
    
    // Update camera effects (shaking when smashed)
    state->camera_mod[1] = 0.02f * sin(delta_time * 0.2f);
    state->camera_mod[2] = 0.03f * sin(delta_time / 7.7f);
    state->camera_mod[0] += 0.02f * (delta_time * 0.1f) * (1.0f - state->camera_mod[0]);
    
    // Dampen smashed effect over time
    if (state->smashed > 0.0f) {
        state->smashed -= delta_time * 0.1f;
        if (state->smashed < 0.0f) {
            state->smashed = 0.0f;
        }
    }
}

void cleanup_game_state(GameState* state) {
    if (state->game_map) {
        free(state->game_map);
        state->game_map = NULL;
    }
    if (state->game_states) {
        free(state->game_states);
        state->game_states = NULL;
    }
    if (state->texmods) {
        free(state->texmods);
        state->texmods = NULL;
    }
}

void handle_input(GameState* state) {
    // Input handling will be implemented based on the graphics library used
    // This is a placeholder for now
}

// Tile definitions (from MstXzN.frag)
#define EMPTY 0
#define SAND 1
#define WALL 2
#define STONE 3
#define DIAMOND 4
#define PLAYER 5

// State definitions (from MstXzN.frag)
#define S_REST 0
#define S_FALL 1
#define S_ROLL_LEFT 2
#define S_ROLL_RIGHT 3
#define S_LANDED 4
#define S_UP 5
#define S_DOWN 6
#define S_LEFT 7
#define S_RIGHT 8
#define S_PUSH_LEFT 9
#define S_PUSH_RIGHT 10
#define S_SMASHED 11
#define S_COLLECTED 12

// Helper functions to check tile and state
static inline int get_tile(GameState* state, int x, int y) {
    if (x < 0 || x >= state->map_width || y < 0 || y >= state->map_height) return WALL;
    int idx = y * state->map_width + x;
    return state->game_map[idx];
}

static inline int get_state(GameState* state, int x, int y) {
    if (x < 0 || x >= state->map_width || y < 0 || y >= state->map_height) return S_REST;
    int idx = y * state->map_width + x;
    return state->game_states[idx];
}

static inline void set_tile(GameState* state, int* new_map, int x, int y, int tile) {
    if (x < 0 || x >= state->map_width || y < 0 || y >= state->map_height) return;
    int idx = y * state->map_width + x;
    new_map[idx] = tile;
}

static inline void set_state(GameState* state, int* new_states, int x, int y, int s) {
    if (x < 0 || x >= state->map_width || y < 0 || y >= state->map_height) return;
    int idx = y * state->map_width + x;
    new_states[idx] = s;
}

static inline void set_texmod(GameState* state, float* new_texmods, int x, int y, float mod) {
    if (x < 0 || x >= state->map_width || y < 0 || y >= state->map_height) return;
    int idx = y * state->map_width + x;
    new_texmods[idx] = mod;
}

static inline int can_player_enter(int tile) {
    return tile == EMPTY || tile == SAND || tile == DIAMOND;
}

void update_physics(GameState* state) {
    // Full physics update based on MstXzN_physics.frag
    int* new_map = (int*)malloc(state->map_width * state->map_height * sizeof(int));
    int* new_states = (int*)malloc(state->map_width * state->map_height * sizeof(int));
    float* new_texmods = (float*)malloc(state->map_width * state->map_height * sizeof(float));
    
    if (new_map == NULL || new_states == NULL || new_texmods == NULL) {
        fprintf(stderr, "Error: Failed to allocate physics update memory\n");
        return;
    }
    
    // Copy current state to new state
    memcpy(new_map, state->game_map, state->map_width * state->map_height * sizeof(int));
    memcpy(new_states, state->game_states, state->map_width * state->map_height * sizeof(int));
    memcpy(new_texmods, state->texmods, state->map_width * state->map_height * sizeof(float));
    
    // Process each cell in the map
    for (int y = 0; y < state->map_height; y++) {
        for (int x = 0; x < state->map_width; x++) {
            int idx = y * state->map_width + x;
            int tile = state->game_map[idx];
            int tile_state = state->game_states[idx];
            
            // Get neighbor tiles
            int top_tile = get_tile(state, x, y-1);
            int top_state = get_state(state, x, y-1);
            int bot_tile = get_tile(state, x, y+1);
            int bot_state = get_state(state, x, y+1);
            int left_tile = get_tile(state, x-1, y);
            int left_state = get_state(state, x-1, y);
            int right_tile = get_tile(state, x+1, y);
            int right_state = get_state(state, x+1, y);
            int topl_tile = get_tile(state, x-1, y-1);
            int topr_tile = get_tile(state, x+1, y-1);
            int botl_tile = get_tile(state, x-1, y+1);
            int botr_tile = get_tile(state, x+1, y+1);
            
            // Clear previous gem-collection flag
            if (tile_state == S_COLLECTED) {
                set_state(state, new_states, x, y, S_REST);
            }
            
            // Process player move requests
            int is_dia = (tile == DIAMOND) ? 1 : 0;
            if (can_player_enter(tile) || is_dia) {
                int new_state = is_dia ? S_COLLECTED : S_REST;
                
                if (bot_tile == PLAYER && bot_state == S_UP) { 
                    set_tile(state, new_map, x, y, PLAYER);
                    set_state(state, new_states, x, y, new_state);
                }
                else if (top_tile == PLAYER && top_state == S_DOWN) { 
                    set_tile(state, new_map, x, y, PLAYER);
                    set_state(state, new_states, x, y, new_state);
                }
                else if (left_tile == PLAYER && left_state == S_RIGHT) { 
                    set_tile(state, new_map, x, y, PLAYER);
                    set_state(state, new_states, x, y, new_state);
                }
                else if (right_tile == PLAYER && right_state == S_LEFT) { 
                    set_tile(state, new_map, x, y, PLAYER);
                    set_state(state, new_states, x, y, new_state);
                }
            }
            else {
                // Clear self when player move requests succeed
                if ((tile_state == S_UP && can_player_enter(top_tile)) ||
                    (tile_state == S_DOWN && can_player_enter(bot_tile)) ||
                    (tile_state == S_LEFT && can_player_enter(left_tile)) ||
                    (tile_state == S_RIGHT && can_player_enter(right_tile))) {
                    set_tile(state, new_map, x, y, EMPTY);
                }
            }
            
            // Process stone push
            if (tile == EMPTY) {
                if (right_tile == STONE && right_state == S_PUSH_LEFT) { 
                    set_tile(state, new_map, x, y, STONE);
                    set_state(state, new_states, x, y, S_REST);
                    set_texmod(state, new_texmods, x, y, state->texmods[(y) * state->map_width + (x+1)]);
                }
                else if (left_tile == STONE && left_state == S_PUSH_RIGHT) { 
                    set_tile(state, new_map, x, y, STONE);
                    set_state(state, new_states, x, y, S_REST);
                    set_texmod(state, new_texmods, x, y, state->texmods[(y) * state->map_width + (x-1)]);
                }
            }
            
            // Clear stone after push request succeeded
            if (tile == STONE) {
                if ((tile_state == S_PUSH_LEFT && left_tile == EMPTY) ||
                    (tile_state == S_PUSH_RIGHT && right_tile == EMPTY)) {
                    set_tile(state, new_map, x, y, EMPTY);
                }
            }
            
            // Handle falling physics (for stones and diamonds)
            if (tile_state == S_LANDED) {
                set_state(state, new_states, x, y, S_REST);
            }
            
            // Clear self when fall requests succeed
            if ((tile_state == S_FALL && bot_tile == EMPTY) ||
                (tile_state == S_ROLL_LEFT && botl_tile == EMPTY) ||
                (tile_state == S_ROLL_RIGHT && botr_tile == EMPTY)) {
                set_tile(state, new_map, x, y, EMPTY);
            }

            // Process falling objects that want to enter this tile
            if (tile == EMPTY) {
                if (top_state == S_FALL) { 
                    set_tile(state, new_map, x, y, top_tile);
                    set_state(state, new_states, x, y, S_LANDED);
                    set_texmod(state, new_texmods, x, y, state->texmods[(y-1) * state->map_width + x]);
                }
                else if (topr_tile == STONE || topr_tile == DIAMOND) {
                    if (top_state == S_ROLL_RIGHT) { 
                        set_tile(state, new_map, x, y, topr_tile);
                        set_state(state, new_states, x, y, S_LANDED);
                        set_texmod(state, new_texmods, x, y, state->texmods[(y-1) * state->map_width + (x+1)]);
                    }
                }
                else if (topl_tile == STONE || topl_tile == DIAMOND) {
                    if (top_state == S_ROLL_LEFT) { 
                        set_tile(state, new_map, x, y, topl_tile);
                        set_state(state, new_states, x, y, S_LANDED);
                        set_texmod(state, new_texmods, x, y, state->texmods[(y-1) * state->map_width + (x-1)]);
                    }
                }
            }

            // Create fall requests for stones and diamonds
            if (tile == STONE || tile == DIAMOND) {
                // Fall down
                if (bot_tile == EMPTY) {
                    set_state(state, new_states, x, y, S_FALL);
                } 
                // Roll to side
                else if (botl_tile == EMPTY && left_tile == EMPTY) {
                    set_state(state, new_states, x, y, S_ROLL_LEFT);
                }
                else if (botr_tile == EMPTY && right_tile == EMPTY) {
                    set_state(state, new_states, x, y, S_ROLL_RIGHT);
                }
                else {
                    // Clear fall/roll flag but keep for one frame to hit player
                    if (tile_state != S_LANDED) {
                        set_state(state, new_states, x, y, S_REST);
                    }
                }
            }
        }
    }
    
    // Copy new state back to original
    memcpy(state->game_map, new_map, state->map_width * state->map_height * sizeof(int));
    memcpy(state->game_states, new_states, state->map_width * state->map_height * sizeof(int));
    memcpy(state->texmods, new_texmods, state->map_width * state->map_height * sizeof(float));
    
    // Free temporary memory
    free(new_map);
    free(new_states);
    free(new_texmods);
}