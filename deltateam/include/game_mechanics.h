#ifndef GAME_MECHANICS_H
#define GAME_MECHANICS_H

#include "physics.h"
#include <GLFW/glfw3.h>
#include <stdbool.h>

// Game state structure
typedef struct {
    // Player state
    vec3 player_pos;
    vec3 player_rot;  // pitch, yaw, roll
    float player_speed;
    float mouse_sensitivity;
    
    // Game time
    float current_time;
    float last_time;
    
    // Input state
    bool keys[1024];
    bool mouse_buttons[3];
    double mouse_x, mouse_y;
    double last_mouse_x, last_mouse_y;
    
    // Game settings
    bool use_modern_renderer;
    bool first_mouse;
    
    // Game objects
    // Add other game objects as needed
    
} GameState_t;

// Function prototypes
void init_game_state(GameState_t *state);
void update_game_state(GameState_t *state, float delta_time);
void handle_input(GLFWwindow *window, GameState_t *state);
void update_player_position(GameState_t *state, float delta_time);
void update_camera_rotation(GameState_t *state);
void toggle_renderer(GameState_t *state);

#endif