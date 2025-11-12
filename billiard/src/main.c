#define _POSIX_C_SOURCE 200809L
#include "billiard.h"
#include "raster_renderer.h"
#include "opengl_renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

// Game mode enumeration
typedef enum {
    SOFTWARE_RENDERING = 0,
    OPENGL_RENDERING = 1
} RenderMode;

int main(int argc, char* argv[]) {
    // Initialize game state
    GameState state;
    init_game_state(&state);
    
    RenderMode render_mode = OPENGL_RENDERING;  // Default to OpenGL
    
    // Check command line arguments
    if (argc > 1) {
        if (strcmp(argv[1], "--software") == 0) {
            render_mode = SOFTWARE_RENDERING;
        } else if (strcmp(argv[1], "--opengl") == 0) {
            render_mode = OPENGL_RENDERING;
        }
    }
    
    if (render_mode == SOFTWARE_RENDERING) {
        // Software rendering mode
        Framebuffer *fb = init_framebuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
        if (!fb) {
            printf("Failed to initialize framebuffer\n");
            return -1;
        }
        
        printf("Running billiard simulation in software rendering mode...\n");
        printf("Press Ctrl+C to exit\n");
        
        // Main game loop for software rendering
        int frame_count = 0;
        struct timeval start_time, current_time;
        gettimeofday(&start_time, NULL);
        
        while (frame_count < 3600) {  // Run for ~60 seconds at 60 FPS
            // Update physics
            update_physics(&state);
            
            // Render scene
            render_scene_software(fb, &state);
            
            // For demonstration, print every 60 frames
            if (frame_count % 60 == 0) {
                printf("Frame: %d, Game State: %.1f, Step: %.1f\n", 
                       frame_count, state.run_state, state.n_step);
            }
            
            frame_count++;
            
            // Simple delay to control frame rate
            struct timespec ts = {0, 16666667}; // ~60 FPS (1/60 second)
            nanosleep(&ts, NULL);
        }
        
        free_framebuffer(fb);
        printf("Software rendering demo completed.\n");
    }
    else {
        // OpenGL rendering mode
        OpenGLRenderer *renderer = init_opengl_renderer(800, 600, "Billiard Simulation");
        if (!renderer) {
            printf("Failed to initialize OpenGL renderer\n");
            return -1;
        }
        
        printf("Running billiard simulation in OpenGL mode...\n");
        printf("Close the window to exit\n");
        
        // Main game loop for OpenGL rendering
        int frame_count = 0;
        while (!should_window_close(renderer) && frame_count < 3600) {  // Limit for demo
            // Update physics
            update_physics(&state);
            
            // Render scene
            render_scene_opengl(renderer, &state);
            
            // Handle events
            poll_events();
            
            // Swap buffers
            swap_buffers(renderer);
            
            // Print status every 60 frames
            if (frame_count % 60 == 0) {
                printf("Frame: %d, Game State: %.1f, Step: %.1f\n", 
                       frame_count, state.run_state, state.n_step);
            }
            
            frame_count++;
        }
        
        free_opengl_renderer(renderer);
        printf("OpenGL rendering demo completed.\n");
    }
    
    return 0;
}