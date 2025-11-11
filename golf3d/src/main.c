#include "game.h"
#include "renderer.h"
#include "physics.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Global variables
GameState* g_state = NULL;
GLFWwindow* g_window = NULL;

// Initialize game state
GameState* init_game_state(int width, int height) {
    GameState* state = (GameState*)malloc(sizeof(GameState));
    if (!state) {
        fprintf(stderr, "Failed to allocate memory for game state\n");
        return NULL;
    }
    
    // Initialize ball position
    state->ball_pos[0] = 0.0f;
    state->ball_pos[1] = 0.0f;
    state->ball_pos[2] = 0.0f;
    
    state->ball_dpos[0] = 0.0f;
    state->ball_dpos[1] = 0.0f;
    state->ball_dpos[2] = 0.0f;
    
    // Initialize mouse drag
    state->mouse_drag_start[0] = -1.0f;
    state->mouse_drag_start[1] = 0.0f;
    state->mouse_drag[0] = 0.0f;
    state->mouse_drag[1] = 0.0f;
    
    // Initialize camera
    state->cam_pos[0] = 0.0f;
    state->cam_pos[1] = 1.0f;
    state->cam_pos[2] = -3.0f;
    
    state->taa = 0.0f;
    
    // Initialize hole
    state->hole_pos[0] = 0.0f;
    state->hole_pos[1] = 0.0f;
    state->hole_pos[2] = 10.0f;
    
    state->stationary = true;
    state->width = width;
    state->height = height;
    
    state->time = 0.0f;
    state->frame = 0;
    
    // Initialize mouse state
    state->mouse[0] = 0.0f;  // x
    state->mouse[1] = 0.0f;  // y
    state->mouse[2] = 0.0f;  // button1 state
    state->mouse[3] = 0.0f;  // button2 state
    
    state->renderer_mode = 0;  // 0 for old school, 1 for modern
    
    return state;
}

// Update game state
void update_game_state(GameState* state) {
    // Update physics
    update_physics_state(state);
    
    // Update time and frame
    state->time += 1.0f/60.0f;  // Assuming 60 FPS
    state->frame++;
}

// Render the frame
void render_frame(GameState* state) {
    if (state->renderer_mode == 0) {
        render_old_school(state);
    } else {
        render_modern(state);
    }
}

// Clean up game state
void cleanup_game_state(GameState* state) {
    if (state) {
        free(state);
    }
}

// Physics functions (stubs that call the ones in physics.c)
void update_physics(GameState* state) {
    update_physics_state(state);
}

float sdf_func(float pos[3], bool include_dynamic) {
    return sdf_func_with_state(pos, include_dynamic, g_state);
}

float* get_normal(float pos[3], float eps, bool include_dynamic) {
    return get_normal_with_state(pos, eps, include_dynamic, g_state);
}

// Callback functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    set_viewport(width, height);
    if (g_state) {
        g_state->width = width;
        g_state->height = height;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            g_state->mouse[2] = 1.0f;  // Left button pressed
        } else {
            g_state->mouse[2] = 0.0f;  // Left button released
        }
    }
    
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            g_state->mouse[3] = 1.0f;  // Right button pressed
        } else {
            g_state->mouse[3] = 0.0f;  // Right button released
        }
    }
    
    // Update mouse position
    g_state->mouse[0] = (float)xpos;
    g_state->mouse[1] = (float)g_state->height - (float)ypos;  // Flip Y coordinate
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    g_state->mouse[0] = (float)xpos;
    g_state->mouse[1] = (float)g_state->height - (float)ypos;  // Flip Y coordinate
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_R) {
            // Toggle renderer mode
            g_state->renderer_mode = 1 - g_state->renderer_mode;
            printf("Renderer mode switched to %s\n", 
                   g_state->renderer_mode ? "modern (with shaders)" : "old school (no shaders)");
        } else if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }
}

// Main function
int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }
    
    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create window
    int width = 1024;
    int height = 768;
    g_window = glfwCreateWindow(width, height, "Golf3D - Endless Golf", NULL, NULL);
    if (!g_window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    
    // Make the window's context current
    glfwMakeContextCurrent(g_window);
    
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    
    // Initialize game state
    g_state = init_game_state(width, height);
    if (!g_state) {
        fprintf(stderr, "Failed to initialize game state\n");
        glfwTerminate();
        return -1;
    }
    
    // Initialize renderer
    init_renderer(width, height, g_state->renderer_mode);
    
    printf("Controls:\n");
    printf("- Left mouse: Click and drag down from ball to set power\n");
    printf("- Right mouse: Rotate camera\n");
    printf("- R key: Toggle between old school and modern renderer\n");
    printf("- ESC: Quit\n");
    
    // Set callback functions
    glfwSetFramebufferSizeCallback(g_window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(g_window, mouse_button_callback);
    glfwSetCursorPosCallback(g_window, cursor_position_callback);
    glfwSetKeyCallback(g_window, key_callback);
    
    // Main loop
    double last_time = glfwGetTime();
    while (!glfwWindowShouldClose(g_window)) {
        // Calculate delta time
        double current_time = glfwGetTime();
        double delta_time = current_time - last_time;
        last_time = current_time;
        
        // Process events
        glfwPollEvents();
        
        // Update game state
        update_game_state(g_state);
        
        // Render
        render_frame(g_state);
        
        // Swap buffers
        glfwSwapBuffers(g_window);
    }
    
    // Cleanup
    cleanup_renderer();
    cleanup_game_state(g_state);
    
    glfwDestroyWindow(g_window);
    glfwTerminate();
    
    return 0;
}