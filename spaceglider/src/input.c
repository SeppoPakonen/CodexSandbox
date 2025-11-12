#include "input.h"
#include <stdio.h>

static GLFWwindow* g_window = NULL;

// Key state tracking
static int throttle_state = 0; // -1 for down, 0 for neutral, 1 for up
static float pitch_input = 0.0f;
static float roll_input = 0.0f;
static float yaw_input = 0.0f;

void initialize_input(GLFWwindow* window) {
    g_window = window;
    
    // Set up any callbacks if needed
    printf("Input system initialized\n");
}

void process_input(GameState* game_state, VehicleState* vehicle_state) {
    // Reset input values
    pitch_input = 0.0f;
    roll_input = 0.0f;
    yaw_input = 0.0f;
    
    // Handle throttle controls (W/S or Z/S)
    if (glfwGetKey(g_window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(g_window, GLFW_KEY_UP) == GLFW_PRESS) {
        // Increase throttle
        if (vehicle_state->throttle < 1.0f) {
            vehicle_state->throttle += 0.01f;
            if (vehicle_state->throttle > 1.0f) vehicle_state->throttle = 1.0f;
        }
    }
    if (glfwGetKey(g_window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(g_window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        // Decrease throttle
        if (vehicle_state->throttle > 0.0f) {
            vehicle_state->throttle -= 0.01f;
            if (vehicle_state->throttle < 0.0f) vehicle_state->throttle = 0.0f;
        }
    }

    // Handle pitch (up/down arrows or I/K)
    if (glfwGetKey(g_window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(g_window, GLFW_KEY_I) == GLFW_PRESS) {
        pitch_input = 1.0f;
    }
    if (glfwGetKey(g_window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(g_window, GLFW_KEY_K) == GLFW_PRESS) {
        pitch_input = -1.0f;
    }

    // Handle roll (left/right arrows or J/L)
    if (glfwGetKey(g_window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(g_window, GLFW_KEY_J) == GLFW_PRESS) {
        roll_input = -1.0f; // Roll left
    }
    if (glfwGetKey(g_window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(g_window, GLFW_KEY_L) == GLFW_PRESS) {
        roll_input = 1.0f;  // Roll right
    }

    // Handle yaw (A/D or Q/D)
    if (glfwGetKey(g_window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(g_window, GLFW_KEY_Q) == GLFW_PRESS) {
        yaw_input = -1.0f; // Yaw left
    }
    if (glfwGetKey(g_window, GLFW_KEY_D) == GLFW_PRESS) {
        yaw_input = 1.0f;  // Yaw right
    }

    // Handle special keys
    if (glfwGetKey(g_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        // Halt - cut throttle
        vehicle_state->throttle = 0.0f;
    }
    
    // Toggle HMD modes with M key
    if (glfwGetKey(g_window, GLFW_KEY_M) == GLFW_PRESS) {
        static int hmd_toggle = 0;
        hmd_toggle = (hmd_toggle + 1) % 3; // Cycle through 0, 1, 2
        vehicle_state->modes.x = (float)hmd_toggle;
        if (hmd_toggle == 0) printf("HMD: OFF\n");
        else if (hmd_toggle == 1) printf("HMD: SURFACE\n");
        else printf("HMD: ORBIT\n");
    }
    
    // Toggle pause with P key
    if (glfwGetKey(g_window, GLFW_KEY_P) == GLFW_PRESS) {
        static int paused = 0;
        paused = !paused;
        if (paused) printf("Game paused\n");
        else printf("Game resumed\n");
    }
    
    // Handle quit
    if (glfwGetKey(g_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(g_window, GLFW_TRUE);
    }
    
    // In a real implementation, you would apply these inputs to control the spacecraft
    // For now, we'll just store the values
    // Control adjustments would be applied in the physics update loop
}