#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include "spaceglider/types.h"
#include "game_logic.h"
#include "physics.h"
#include "input.h"
#include "renderer_old.h"
#include "renderer_modern.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define TARGET_FPS 60

static GLFWwindow* window = NULL;
static GameState game_state;
static VehicleState vehicle_state;
static PlanetData planet_data;
static bool use_modern_renderer = false;
static bool running = true;

// Function prototypes
void initialize_game();
void handle_input();
void update_game(float delta_time);
void render_frame();
void cleanup();

int main(int argc, char* argv[]) {
    printf("Space Glider C Implementation\n");
    
    // Check for modern renderer flag
    if (argc > 1 && strcmp(argv[1], "--modern") == 0) {
        use_modern_renderer = true;
        printf("Using modern renderer with shaders\n");
    } else {
        printf("Using old school renderer without shaders\n");
    }

    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Create window
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Space Glider", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    // Make context current
    glfwMakeContextCurrent(window);

    // Initialize OpenGL
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Initialize game
    initialize_game();

    // Timing variables
    double last_time = glfwGetTime();
    double frame_time = 1.0 / TARGET_FPS;

    // Main game loop
    while (!glfwWindowShouldClose(window) && running) {
        double current_time = glfwGetTime();
        double delta_time = current_time - last_time;
        last_time = current_time;

        // Handle input
        handle_input();

        // Update game state
        update_game((float)delta_time);

        // Render frame
        render_frame();

        // Swap buffers
        glfwSwapBuffers(window);

        // Poll events
        glfwPollEvents();

        // Frame rate control to prevent excessive CPU usage
        if (delta_time < frame_time) {
            double sleep_time = frame_time - delta_time;
            struct timespec ts;
            ts.tv_sec = 0;
            ts.tv_nsec = (long)(sleep_time * 1000000000.0);
            nanosleep(&ts, NULL);
        }
    }

    cleanup();
    return 0;
}

void initialize_game() {
    // Initialize game state
    initialize_game_state(&game_state);
    
    // Initialize vehicle state
    initialize_vehicle_state(&vehicle_state);
    
    // Initialize planet data
    initialize_planet_data(&planet_data);
    
    // Initialize physics system
    initialize_physics();
    
    // Initialize renderer
    if (use_modern_renderer) {
        initialize_modern_renderer();
    } else {
        initialize_old_renderer();
    }
    
    // Initialize input system
    initialize_input(window);
    
    printf("Game initialized\n");
}

void handle_input() {
    process_input(&game_state, &vehicle_state);
}

void update_game(float delta_time) {
    // Update physics
    update_physics(&game_state, &vehicle_state, &planet_data, delta_time);
    
    // Update game logic
    update_game_logic(&game_state, &vehicle_state, &planet_data, delta_time);
}

void render_frame() {
    // Clear framebuffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (use_modern_renderer) {
        render_with_modern_renderer(&game_state, &vehicle_state, &planet_data);
    } else {
        render_with_old_renderer(&game_state, &vehicle_state, &planet_data);
    }
}

void cleanup() {
    // Cleanup renderer
    if (use_modern_renderer) {
        cleanup_modern_renderer();
    } else {
        cleanup_old_renderer();
    }
    
    // Terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    
    printf("Game cleaned up\n");
}