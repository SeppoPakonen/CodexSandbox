#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

// Constants from the original shader
#define SHIP_SIZE 0.05f
#define SHIP_DAMPING 3.0f
#define SHIP_THRUST 3.0f
#define SHIP_ANGULAR_THRUST 48.0f
#define GRAVITY 0.60f
#define GROUND_FRICTION 0.2f
#define SEED 10.0f

// Ship constants
#define SHIP_NOSE_X 0.0f
#define SHIP_NOSE_Y 1.0f * SHIP_SIZE
#define SHIP_LEFT_WING_X 0.5f * SHIP_SIZE
#define SHIP_LEFT_WING_Y (-0.3f) * SHIP_SIZE
#define SHIP_RIGHT_WING_X (-0.5f) * SHIP_SIZE
#define SHIP_RIGHT_WING_Y (-0.3f) * SHIP_SIZE

// Game state structure
typedef struct {
    float position_x;
    float position_y;
    float rotation;
    float thrust;
    float velocity_x;
    float velocity_y;
    float angular_velocity;
    float zoom;
} GameState;

// Function to generate random value based on coordinates
float rand_val(float x, float y) {
    float co[2] = {x, y};
    co[0] += SEED;
    co[1] += SEED;
    float dot = co[0] * 12.9898f + co[1] * 78.233f;
    return fmodf(sinf(dot) * 43758.5453f, 1.0f);
}

// Function to check if a tile should be filled (cave generation logic)
int get_tile(int x, int y) {
    int up = rand_val(x, y + 1) > 0.5f;
    int down = rand_val(x, y - 1) > 0.5f;
    int left = rand_val(x - 1, y) > 0.5f;
    int right = rand_val(x + 1, y) > 0.5f;
    
    int here = rand_val(x, y) > 0.5f;
    
    return (!((up == down) && (left == right) && (up != right)) && here);
}

// Function to calculate map signed distance field
float get_map_sdf(float pixel_x, float pixel_y) {
    // Calculate the tile X and Y IDs for this pixel
    float inner_x = fmodf(pixel_x, 1.0f);
    float inner_y = fmodf(pixel_y, 1.0f);
    
    if (inner_x < 0) inner_x += 1.0f;
    if (inner_y < 0) inner_y += 1.0f;
    
    int tile_x = (int)(pixel_x - inner_x);
    int tile_y = (int)(pixel_y - inner_y);
    
    float a = get_tile(tile_x, tile_y);
    float b = get_tile(tile_x + 1, tile_y);
    float c = get_tile(tile_x, tile_y + 1);
    float d = get_tile(tile_x + 1, tile_y + 1);
    
    float mixed = (a * (1.0f - inner_x) + b * inner_x) * (1.0f - inner_y) + 
                  (c * (1.0f - inner_x) + d * inner_x) * inner_y;
    
    return mixed;
}

// Initialize game state
void init_game_state(GameState* state) {
    state->position_x = 0.0f;
    state->position_y = 0.2f;
    state->rotation = 0.0f;
    state->thrust = 0.0f;
    state->velocity_x = 0.0f;
    state->velocity_y = 0.0f;
    state->angular_velocity = 0.0f;
    state->zoom = 4.0f;
}

// Render the classic cave environment (no shaders)
void render_classic_scene(GameState* state) {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Set background color (dark brown/cave-like)
    glClearColor(0.4f, 0.2f, 0.1f, 1.0f);
    
    // Draw a simple cave environment using basic shapes
    glColor3f(0.6f, 0.4f, 0.3f); // Cave rock color
    
    // Draw some basic cave formations (just simple shapes)
    glBegin(GL_QUADS);
    // Example: simple cave walls
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(-0.8f, -1.0f);
    glVertex2f(-0.8f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    
    glVertex2f(0.8f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(0.8f, 1.0f);
    glEnd();
    
    // Simple representation of cave floor/ceiling
    glColor3f(0.5f, 0.3f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, 0.8f);
    glVertex2f(1.0f, 0.8f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, -0.8f);
    glVertex2f(-1.0f, -0.8f);
    glEnd();
    
    // Draw the player ship (simple triangle)
    glPushMatrix();
    glTranslatef(state->position_x, state->position_y, 0.0f);
    glRotatef(state->rotation * 180.0f / M_PI, 0.0f, 0.0f, 1.0f);
    
    glColor3f(0.8f, 0.8f, 0.0f); // Yellow ship
    glBegin(GL_TRIANGLES);
    glVertex2f(SHIP_NOSE_X, SHIP_NOSE_Y);       // Nose
    glVertex2f(SHIP_LEFT_WING_X, SHIP_LEFT_WING_Y);  // Left wing
    glVertex2f(SHIP_RIGHT_WING_X, SHIP_RIGHT_WING_Y); // Right wing
    glEnd();
    glPopMatrix();
}

// Update physics in C (converted from GLSL)
void update_physics(GameState* state, int key_up, int key_left, int key_right) {
    // Calculate orientation matrix components
    float c = cosf(state->rotation);
    float s = sinf(state->rotation);
    
    // Calculate acceleration
    float acceleration_x = 0.0f;
    float acceleration_y = -GRAVITY; // Gravity
    float acceleration_angular = 0.0f;
    
    // Thrusters
    float thrust_keys = key_up ? 1.0f : 0.0f;
    state->thrust = thrust_keys;
    
    // Apply angular thrust
    acceleration_angular += (key_left ? -1.0f : 0.0f) + (key_right ? 1.0f : 0.0f);
    acceleration_angular *= SHIP_ANGULAR_THRUST;
    
    // Calculate forward direction based on rotation
    float forwards_x = s; // sin(rotation) -> x component
    float forwards_y = c; // cos(rotation) -> y component (inverted)
    
    acceleration_x += forwards_x * SHIP_THRUST * thrust_keys;
    acceleration_y += forwards_y * SHIP_THRUST * thrust_keys;
    
    // Apply damping
    acceleration_x -= state->velocity_x * SHIP_DAMPING;
    acceleration_y -= state->velocity_y * SHIP_DAMPING;
    
    // Update velocities
    state->velocity_x += acceleration_x * 0.016f; // Assuming ~60 FPS
    state->velocity_y += acceleration_y * 0.016f;
    state->angular_velocity += acceleration_angular * 0.016f;
    
    // Update positions
    state->position_x += state->velocity_x * 0.016f;
    state->position_y += state->velocity_y * 0.016f;
    state->rotation += state->angular_velocity * 0.016f;
    
    // Simple ground collision (simplified version)
    if (state->position_y < -0.8f) { // Ground collision
        state->position_y = -0.8f;
        state->velocity_y *= (1.0f - GROUND_FRICTION);
    }
    
    // Screen boundaries
    if (state->position_x < -1.0f) state->position_x = -1.0f;
    if (state->position_x > 1.0f) state->position_x = 1.0f;
    if (state->position_y > 0.9f) state->position_y = 0.9f;
}