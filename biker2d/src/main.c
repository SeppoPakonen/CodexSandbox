#include "biker2d.h"
#include "physics.h"
#include "renderer.h"
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
    
    // Initialize all key states to false
    for (int i = 0; i < 256; i++) {
        state->keys[i] = false;
    }
    
    // Set initial bike particles
    init_bike_particles(state);
    
    // Initialize other state variables
    state->time = 0.0f;
    state->time_delta = 1.0f/60.0f;  // Default to 60 FPS
    state->frame = 0;
    state->score = 0;
    
    state->width = width;
    state->height = height;
    
    state->mouse_x = 0.0;
    state->mouse_y = 0.0;
    state->mouse_pressed = false;
    
    // Initialize camera position
    state->camera_x = state->particles[PID_BODY][0];
    state->camera_y = state->particles[PID_BODY][1];
    
    state->renderer_mode = 0;  // Start with old school renderer
    
    return state;
}

// Update game state
void update_game_state(GameState* state) {
    // Update time
    static double last_time = 0.0;
    double current_time = glfwGetTime();
    if (last_time == 0.0) {
        last_time = current_time;
    }
    
    state->time_delta = current_time - last_time;
    state->time = current_time;
    last_time = current_time;
    
    // Update physics simulation
    update_physics(state);
    
    // Update camera to follow the bike
    state->camera_x = state->particles[PID_BODY][0];
    state->camera_y = state->particles[PID_BODY][1];
    
    // Increment frame counter
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

// Input callbacks
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key >= 0 && key < 256) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            g_state->keys[key] = true;
        } else if (action == GLFW_RELEASE) {
            g_state->keys[key] = false;
        }
    }
    
    // Handle renderer toggle
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        g_state->renderer_mode = 1 - g_state->renderer_mode;
        printf("Renderer mode switched to %s\n", 
               g_state->renderer_mode ? "modern (with shaders)" : "old school (no shaders)");
    }
    
    // Handle escape to quit
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        g_state->mouse_pressed = (action == GLFW_PRESS);
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    g_state->mouse_x = xpos;
    g_state->mouse_y = ypos;
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
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    
    // Create window
    int width = 1024;
    int height = 768;
    g_window = glfwCreateWindow(width, height, "Biker2D - Excitebike-inspired game", NULL, NULL);
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
    
    printf("Biker2D Controls:\n");
    printf("- Arrow keys or A/D: Lean bike\n");
    printf("- Space: Accelerate\n");
    printf("- ESC: Quit\n");
    printf("- R: Toggle between old school and modern renderer\n");
    
    // Set up input callbacks
    glfwSetKeyCallback(g_window, key_callback);
    glfwSetMouseButtonCallback(g_window, mouse_button_callback);
    glfwSetCursorPosCallback(g_window, cursor_position_callback);
    
    // Main game loop
    while (!glfwWindowShouldClose(g_window)) {
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