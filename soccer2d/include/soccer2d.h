#ifndef SOCCER2D_H
#define SOCCER2D_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

// Constants
#define PC_X 0.95f      // Pitch size X
#define PC_Y 0.52f      // Pitch size Y
#define F_START 180     // Waiting time before kick off
#define TK 0.006f       // Line thickness
#define MAX_PLAYERS 22
#define BUFFER_SIZE 32  // Size of buffer arrays (we'll use 32 to have some extra space)

// Game constants
#define CORNERS 2       // Enable corners & throw-ins

// Player positions (default 4-4-2 formation)
typedef struct {
    float x, y, z, w;   // x, y = position; z, w = velocity/direction
} vec4;

typedef struct {
    float x, y;         // x, y = position
} vec2;

// Game state
typedef struct {
    vec4 ball;                       // Ball position and velocity
    vec4 players[MAX_PLAYERS];       // Player positions and states
    int mode[4];                    // Game mode [0=goal kick, 1=game, 2=throw-in], frame count, last player, etc.
    int scores[2];                  // Scores [red, blue]
    int last_kicker;                // Last player who kicked the ball
    vec2 offside_pos;               // Offside positions
    vec4* buffer_a;                 // Buffer A: game engine
    vec4* buffer_b;                 // Buffer B: player positions
    int frame_count;                // Frame counter
    bool demo_mode;                 // Whether demo mode is active
    float time;                     // Game time
    vec2 mouse_pos;                 // Mouse position
    bool mouse_active;              // Whether mouse is active
    int key_states[256];           // Key states
    vec2 resolution;               // Screen resolution
    float dt;                      // Delta time
} GameState;

// Physics functions
void initialize_game_state(GameState* state);
void update_physics(GameState* state);
void handle_ball_physics(GameState* state);
void handle_player_physics(GameState* state);
void handle_collisions(GameState* state);

// Math utilities
float sd_box(const vec2 p, const vec2 b);
float sd_segment(const vec2 p, const vec2 a, const vec2 b);
float length_vec2(const vec2 v);
vec2 normalize_vec2(const vec2 v);
float dot_vec2(const vec2 a, const vec2 b);
vec2 clamp_vec2(const vec2 v, const vec2 min_val, const vec2 max_val);
float sign(float x);
vec2 mix_vec2(const vec2 a, const vec2 b, float t);

// Game logic
void update_game_logic(GameState* state);
int get_closest_player(const GameState* state, const vec2 pos, bool red_team);
void calculate_buffer_b(GameState* state);

// Rendering
void render_old_school(const GameState* state);
void render_with_shaders(const GameState* state);

// Input handling
void handle_input(GameState* state);

#endif