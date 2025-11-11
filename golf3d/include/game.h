#ifndef GAME_H
#define GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

// Constants from the Shadertoy
#define BALL_RADIUS 0.025f
#define BOUNCE_FACTOR 0.5f
#define COLLISION_THRESHOLD 0.001f
#define HOLE_RADIUS 0.08f

// Game state structure
typedef struct {
    // Ball physics
    float ball_pos[3];
    float ball_dpos[3];  // velocity
    
    // Mouse interaction
    float mouse_drag_start[2];
    float mouse_drag[2];
    
    // Camera
    float cam_pos[3];
    float taa;  // temporal anti aliasing factor
    
    // Hole position
    float hole_pos[3];
    
    // Game state
    bool stationary;
    
    // Resolution
    int width, height;
    
    // Time
    float time;
    int frame;
    
    // Mouse state
    float mouse[4];  // x, y, button1, button2
    
    // Renderer mode
    int renderer_mode;  // 0 for old school, 1 for modern with shaders
} GameState;

// Function prototypes
GameState* init_game_state(int width, int height);
void update_game_state(GameState* state);
void render_frame(GameState* state);
void cleanup_game_state(GameState* state);

// Physics functions
void update_physics(GameState* state);
float sdf_func(float pos[3], bool include_dynamic);
float* get_normal(float pos[3], float eps, bool include_dynamic);

// Callback functions for main loop
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Global variables (extern declarations)
extern GameState* g_state;
extern GLFWwindow* g_window;

#endif