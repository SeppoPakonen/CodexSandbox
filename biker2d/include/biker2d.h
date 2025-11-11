#ifndef BIKER2D_H
#define BIKER2D_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define LEVELS 4
#define SCALE 0.5f
#define DIFFICULTY_H 0.2f
#define DIFFICULTY_E 1e-2f
#define NPARTICLES 3
#define PI 3.14159265359f

#define WHEEL_RADIUS 0.05f
#define TIRE_THICK 0.02f

// Particle IDs
#define PID_BODY 0
#define PID_WHEEL_BACK 1
#define PID_WHEEL_FRONT 2

// Key codes
#define KEY_SPACE 32
#define KEY_W 87
#define KEY_A 65
#define KEY_S 83
#define KEY_D 68
#define KEY_LEFT 37
#define KEY_RIGHT 39
#define KEY_UP 38
#define KEY_DOWN 40
#define KEY_ESC 27

// Physics constants
#define TENSION 384.0f  // exp2(8.5f) 
#define SDAMP 0.04f
#define FRICTION 6.0f
#define GRAVITY 1.5f
#define MOTOR_POWER 3.0f
#define LEAN_POWER 1.6f

// Game state structure
typedef struct {
    // Particle data: each particle has position (xy) and velocity (zw)
    float particles[NPARTICLES][4];  // [particle_id][x, y, vx, vy]
    
    // Bike links (constraints)
    int links[3][2];  // [link_id][particle_a, particle_b]
    float link_lengths[3];  // [link_id] = length
    
    // Bike state
    bool inverted;
    bool crashed;
    
    // Player input
    bool keys[256];  // Key states
    
    // Game state
    float time;
    float time_delta;
    int frame;
    float score;
    
    // Resolution
    int width, height;
    
    // Mouse state
    double mouse_x, mouse_y;
    bool mouse_pressed;
    
    // Camera
    float camera_x, camera_y;
    
    // Renderer mode
    int renderer_mode;  // 0 for old school, 1 for modern with shaders
} GameState;

// Function prototypes
GameState* init_game_state(int width, int height);
void update_game_state(GameState* state);
void render_frame(GameState* state);
void cleanup_game_state(GameState* state);

// Input handling
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

// External variables
extern GameState* g_state;
extern GLFWwindow* g_window;

#endif