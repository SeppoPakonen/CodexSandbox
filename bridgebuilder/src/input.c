#include "input.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

InputState* init_input_state() {
    InputState* input = malloc(sizeof(InputState));
    if (!input) return NULL;
    
    memset(input, 0, sizeof(InputState));
    input->edit_mode = true;  // Start in edit mode
    return input;
}

void update_input_state(InputState* input, GLFWwindow* window) {
    // Update frame count
    input->frame_count++;
    
    // Update mouse position in world coordinates
    double mouse_x, mouse_y;
    int window_width, window_height;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);
    glfwGetWindowSize(window, &window_width, &window_height);
    
    // Convert screen coordinates to world coordinates
    float aspect_ratio = (float)window_width / (float)window_height;
    input->mouse_prev_pos = input->mouse_pos;
    input->mouse_pos.x = (float)mouse_x / (float)window_width * 2.0f * aspect_ratio - aspect_ratio;
    input->mouse_pos.y = 1.0f - (float)mouse_y / (float)window_height * 2.0f;
    
    // Update mouse state
    input->mouse_clicked = false;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (!input->mouse_down) {
            input->mouse_clicked = true;
        }
        input->mouse_down = true;
    } else {
        input->mouse_down = false;
    }
    
    // Update key states
    for (int i = 0; i < 512; i++) {
        int state = glfwGetKey(window, i);
        if (state == GLFW_PRESS && !input->keys_down[i]) {
            input->keys_just_pressed[i] = true;
        } else {
            input->keys_just_pressed[i] = false;
        }
        input->keys_down[i] = (state == GLFW_PRESS);
    }
    
    // Update mode states
    input->vertex_mode = input->keys_down[VERTEX_MODE_KEY];
    input->edge_mode = input->keys_down[EDGE_MODE_KEY];
    input->pin_mode = input->keys_down[PIN_MODE_KEY];
    
    // Toggle edit/sim mode with spacebar
    if (input->keys_just_pressed[SPACE_KEY]) {
        input->edit_mode = !input->edit_mode;
        input->last_action_frame = input->frame_count;
    }
}

Vector2 get_mouse_world_pos(GLFWwindow* window, int window_width, int window_height) {
    double mouse_x, mouse_y;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);
    
    // Convert screen coordinates to world coordinates (-1 to 1)
    float aspect_ratio = (float)window_width / (float)window_height;
    Vector2 pos;
    pos.x = (float)mouse_x / (float)window_width * 2.0f * aspect_ratio - aspect_ratio;
    pos.y = 1.0f - (float)mouse_y / (float)window_height * 2.0f;
    
    return pos;
}

bool is_key_down(InputState* input, int key) {
    return input->keys_down[key];
}

bool is_key_just_pressed(InputState* input, int key) {
    return input->keys_just_pressed[key];
}

float point_to_line_distance(Vector2 point, Vector2 a, Vector2 b) {
    float pa_x = point.x - a.x;
    float pa_y = point.y - a.y;
    float ba_x = b.x - a.x;
    float ba_y = b.y - a.y;
    
    float h = 0.0f;
    float ba_dot = ba_x * ba_x + ba_y * ba_y;
    if (ba_dot != 0.0f) {
        float h_val = (pa_x * ba_x + pa_y * ba_y) / ba_dot;
        h = (h_val < 0.0f) ? 0.0f : ((h_val > 1.0f) ? 1.0f : h_val);
    }
    
    float dx = pa_x - ba_x * h;
    float dy = pa_y - ba_y * h;
    return sqrtf(dx * dx + dy * dy);
}