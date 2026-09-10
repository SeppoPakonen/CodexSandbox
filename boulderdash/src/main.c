#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include "../include/renderer.h"

int kbhit() {
    fd_set read_fds;
    struct timeval timeout;
    
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    
    int result = select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &timeout);
    return result > 0;
}

int main(int argc, char* argv[]) {
    // Default renderer type
    RendererType renderer_type = RENDERER_MODERN_SHADER;
    int width = 800;
    int height = 600;
    
    // Process command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--renderer") == 0 && i + 1 < argc) {
            i++;
            if (strcmp(argv[i], "modern") == 0) {
                renderer_type = RENDERER_MODERN_SHADER;
            } else if (strcmp(argv[i], "oldschool") == 0) {
                renderer_type = RENDERER_OLD_SCHOOL;
            } else if (strcmp(argv[i], "retro") == 0) {
                renderer_type = RENDERER_RETRO;
            }
        } else if (strcmp(argv[i], "--size") == 0 && i + 2 < argc) {
            i++;
            width = atoi(argv[i]);
            i++;
            height = atoi(argv[i]);
        } else if (strcmp(argv[i], "--help") == 0) {
            printf("Boulder Dash Game\n");
            printf("Usage: %s [OPTIONS]\n", argv[0]);
            printf("Options:\n");
            printf("  --renderer [modern|oldschool|retro]  Set renderer type\n");
            printf("  --size WIDTH HEIGHT                Set window size\n");
            printf("  --help                             Show this help\n");
            return 0;
        }
    }
    
    // Initialize the selected renderer
    renderer_init(renderer_type, width, height);
    
    // Get access to the game state
    GameState* game_state = get_game_state();
    
    // Main game loop
    int running = 1;
    clock_t last_time = clock();
    
    printf("Boulder Dash Game\n");
    printf("Controls: Arrow keys to move, 'R' to restart, 'C' to change renderer\n");
    printf("Starting with %s...\n", 
           renderer_type == RENDERER_MODERN_SHADER ? "Modern Shader Renderer" :
           renderer_type == RENDERER_OLD_SCHOOL ? "Old School Renderer" :
           "Retro Renderer");
    
    while (running) {
        // Calculate delta time
        clock_t current_time = clock();
        float delta_time = ((float)(current_time - last_time)) / CLOCKS_PER_SEC;
        if (delta_time < 0.001f) delta_time = 0.001f; // Prevent division by zero
        last_time = current_time;
        
        // Handle input without blocking
        if (kbhit()) {
            char input[10];
            if (fgets(input, sizeof(input), stdin)) {
                if (input[0] == 'q' || input[0] == 'Q') {
                    running = 0;
                } else if (input[0] == 'c' || input[0] == 'C') {
                    // Cycle through renderers
                    renderer_type = (renderer_type + 1) % 3;
                    renderer_switch(renderer_type);
                    printf("\nSwitched to %s\n", 
                           renderer_type == RENDERER_MODERN_SHADER ? "Modern Shader Renderer" :
                           renderer_type == RENDERER_OLD_SCHOOL ? "Old School Renderer" :
                           "Retro Renderer");
                } else if (input[0] == 'r' || input[0] == 'R') {
                    // Restart game - reinitialize game state
                    printf("\nGame restarted!\n");
                    cleanup_game_state(game_state);
                    init_game_state(game_state, width/32, height/32);
                }
            }
        }
        
        // Update game state
        update_game_state(game_state, delta_time);
        
        // Render the game
        renderer_render(game_state);
        
        // Brief pause to control frame rate
        usleep(16667); // ~60 FPS (1/60 second = 16667 microseconds)
    }
    
    // Cleanup
    renderer_cleanup();
    
    printf("\nGame exited.\n");
    return 0;
}