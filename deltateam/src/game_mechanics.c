#include "game_mechanics.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

void init_game_state(GameState_t *state) {
    // Initialize player state
    state->player_pos.x = 0.0f;
    state->player_pos.y = 0.0f;
    state->player_pos.z = 0.0f;
    
    state->player_rot.x = 0.0f;  // pitch
    state->player_rot.y = 0.0f;  // yaw
    state->player_rot.z = 0.0f;  // roll
    
    state->player_speed = 0.05f;
    state->mouse_sensitivity = 0.005f;
    
    // Initialize time
    state->current_time = 0.0f;
    state->last_time = 0.0f;
    
    // Initialize input state
    for (int i = 0; i < 1024; i++) {
        state->keys[i] = false;
    }
    
    for (int i = 0; i < 3; i++) {
        state->mouse_buttons[i] = false;
    }
    
    state->mouse_x = 0.0;
    state->mouse_y = 0.0;
    state->last_mouse_x = 0.0;
    state->last_mouse_y = 0.0;
    
    // Initialize game settings
    state->use_modern_renderer = true;
    state->first_mouse = true;
}

void handle_input(GLFWwindow *window, GameState_t *state) {
    // Handle keyboard input
    state->keys[GLFW_KEY_W] = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    state->keys[GLFW_KEY_A] = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    state->keys[GLFW_KEY_S] = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    state->keys[GLFW_KEY_D] = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
    state->keys[GLFW_KEY_SPACE] = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    state->keys[GLFW_KEY_LEFT_SHIFT] = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
    state->keys[GLFW_KEY_ESCAPE] = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    
    // Toggle renderer with R key
    static bool last_r_state = false;
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !last_r_state) {
        toggle_renderer(state);
        last_r_state = true;
    } else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) {
        last_r_state = false;
    }
    
    // Handle mouse input
    state->mouse_buttons[0] = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    state->mouse_buttons[1] = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    state->mouse_buttons[2] = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;
    
    // Get mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    
    if (state->first_mouse) {
        state->last_mouse_x = xpos;
        state->last_mouse_y = ypos;
        state->first_mouse = false;
    }
    
    state->mouse_x = xpos;
    state->mouse_y = ypos;
}

void update_player_position(GameState_t *state, float delta_time) {
    float velocity = state->player_speed / delta_time;
    
    // Calculate movement based on rotation
    float yaw = state->player_rot.y;
    
    if (state->keys[GLFW_KEY_W]) {  // Move forward
        state->player_pos.x += cosf(yaw) * velocity;
        state->player_pos.z += sinf(yaw) * velocity;
    }
    if (state->keys[GLFW_KEY_S]) {  // Move backward
        state->player_pos.x -= cosf(yaw) * velocity;
        state->player_pos.z -= sinf(yaw) * velocity;
    }
    if (state->keys[GLFW_KEY_A]) {  // Strafe left
        state->player_pos.x -= cosf(yaw + PI/2) * velocity;
        state->player_pos.z -= sinf(yaw + PI/2) * velocity;
    }
    if (state->keys[GLFW_KEY_D]) {  // Strafe right
        state->player_pos.x += cosf(yaw + PI/2) * velocity;
        state->player_pos.z += sinf(yaw + PI/2) * velocity;
    }
    
    // Update player Y position based on heightmap (simplified)
    // In a real implementation, you'd check the heightmap at the player's position
    // and adjust Y accordingly to match terrain height
    // state->player_pos.y = heightmap((vec2){state->player_pos.x, state->player_pos.z}) + PLAYER_HEIGHT;
}

void update_camera_rotation(GameState_t *state) {
    float xoffset = state->mouse_x - state->last_mouse_x;
    float yoffset = -(state->mouse_y - state->last_mouse_y); // Reversed since y-coordinates range from bottom to top
    
    state->last_mouse_x = state->mouse_x;
    state->last_mouse_y = state->mouse_y;
    
    xoffset *= state->mouse_sensitivity;
    yoffset *= state->mouse_sensitivity;
    
    state->player_rot.y += xoffset;
    state->player_rot.x += yoffset;
    
    // Limit pitch to avoid screen flipping
    if (state->player_rot.x > 1.57f) state->player_rot.x = 1.57f;
    if (state->player_rot.x < -1.57f) state->player_rot.x = -1.57f;
}

void toggle_renderer(GameState_t *state) {
    state->use_modern_renderer = !state->use_modern_renderer;
    printf("Renderer switched to %s\n", state->use_modern_renderer ? "modern" : "classic");
}

void update_game_state(GameState_t *state, float delta_time) {
    // Update player position based on input
    update_player_position(state, delta_time);
    
    // Update camera rotation based on mouse input
    update_camera_rotation(state);
    
    // Update game time
    state->last_time = state->current_time;
    state->current_time = glfwGetTime();
}