#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // For usleep
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "physics.h"
#include "renderer.h"
#include "input.h"
#include "bridge_builder.h"

// Frame rate and timing
const int TARGET_FPS = 60;
const float FRAME_TIME = 1.0f / TARGET_FPS;

int main() {
    // Initialize physics world
    PhysicsWorld world;
    init_physics_world(&world);

    // Initialize input state
    InputState* input = init_input_state();
    if (!input) {
        printf("Failed to initialize input state\n");
        return -1;
    }

    // Initialize renderer
    int width = 1024;
    int height = 768;
    bool use_shader_renderer = true;  // Set to false for software renderer

    Renderer* renderer = init_renderer(width, height, use_shader_renderer);
    if (!renderer) {
        printf("Failed to initialize renderer\n");
        free(input);
        return -1;
    }

    // Initialize bridge builder
    BridgeBuilder* builder = init_bridge_builder(&world, input);
    if (!builder) {
        printf("Failed to initialize bridge builder\n");
        cleanup_renderer(renderer);
        free(input);
        return -1;
    }

    // Main game loop
    while (!glfwWindowShouldClose(renderer->window)) {
        // Process input
        update_input_state(input, renderer->window);

        // Update bridge builder
        update_bridge_builder(builder);

        // Update physics if simulation is enabled
        if (!input->edit_mode) {
            world.simulation_enabled = true;
            simulate_physics(&world);
        } else {
            world.simulation_enabled = false;
        }

        // Update world edit mode
        world.edit_mode = input->edit_mode;

        // Render the frame
        render_frame(renderer, &world);

        // Handle window resize
        int new_width, new_height;
        glfwGetWindowSize(renderer->window, &new_width, &new_height);
        if (new_width != width || new_height != height) {
            width = new_width;
            height = new_height;
            update_window_size(renderer, width, height);
        }

        // Process events
        glfwPollEvents();

        // Simple frame rate control (in a real game you'd want a more sophisticated approach)
        // This is just a basic implementation
        double current_time = glfwGetTime();
        double frame_end_time = glfwGetTime();
        float frame_time = (float)(frame_end_time - current_time);

        if (frame_time < FRAME_TIME) {
            // Sleep for the remaining time to achieve target FPS
            float sleep_time = FRAME_TIME - frame_time;
            if (sleep_time > 0) {
                usleep((unsigned int)(sleep_time * 1000000));  // Sleep in microseconds
            }
        }
    }

    // Cleanup
    free(builder);
    cleanup_renderer(renderer);
    free(input);

    return 0;
}