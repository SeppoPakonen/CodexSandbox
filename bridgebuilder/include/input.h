#ifndef INPUT_H
#define INPUT_H

#include "physics.h"
#include <GLFW/glfw3.h>

#define VERTEX_MODE_KEY 65  // 'A'
#define EDGE_MODE_KEY 83    // 'S'
#define PIN_MODE_KEY 68     // 'D'
#define RESET_KEY 82        // 'R'
#define SPACE_KEY 32        // Spacebar
#define MOUSE_LEFT_BUTTON GLFW_MOUSE_BUTTON_LEFT

typedef struct {
    Vector2 mouse_pos;      // Current mouse position in world coordinates
    Vector2 mouse_prev_pos; // Previous mouse position
    bool mouse_down;        // Whether mouse is currently down
    bool mouse_clicked;     // Whether mouse was just clicked
    bool keys_down[512];    // State of each key
    bool keys_just_pressed[512]; // Keys that were just pressed
    bool vertex_mode;       // Whether vertex mode is active
    bool edge_mode;         // Whether edge mode is active
    bool pin_mode;          // Whether pin mode is active
    bool edit_mode;         // Whether edit mode is active
    int last_action_frame;  // Frame number of last action
    int frame_count;        // Current frame count
} InputState;

// Initialize input state
InputState* init_input_state();

// Update input state
void update_input_state(InputState* input, GLFWwindow* window);

// Get mouse position in world coordinates
Vector2 get_mouse_world_pos(GLFWwindow* window, int window_width, int window_height);

// Check if a key is currently down
bool is_key_down(InputState* input, int key);

// Check if a key was just pressed
bool is_key_just_pressed(InputState* input, int key);

// Get distance from point to line segment
float point_to_line_distance(Vector2 point, Vector2 a, Vector2 b);

#endif