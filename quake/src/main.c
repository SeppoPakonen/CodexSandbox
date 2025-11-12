#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"

int main() {
    printf("Quake-inspired game starting...\n");
    
    // Initialize game with 800x600 resolution
    GameState *game = init_game(800, 600);
    if (!game) {
        printf("Failed to initialize game!\n");
        return -1;
    }
    
    printf("Game initialized successfully!\n");
    printf("Renderer type: %s\n", 
           game->renderer_type == RENDERER_OLD_SCHOOL ? "Old School (Software)" : "Modern (Shader-based)");
    
    // Simple game loop demonstration
    float dt = 1.0f / 60.0f;  // 60 FPS time step
    int frame_count = 0;
    const int max_frames = 100;  // Run for a few frames to demonstrate
    
    while (frame_count < max_frames) {
        // Handle some basic input (in a real game, this would be from user input)
        float forward_move = 0.0f;
        float side_move = 0.0f;
        int jump = 0;
        
        // Simulate some movement for demonstration
        if (frame_count < 60) {  // Move forward for first second
            forward_move = 1.0f;
        }
        
        // Process input
        handle_input(game, forward_move, side_move, jump);
        
        // Update game state
        update_game(game, dt);
        
        // Render frame
        render_game(game);
        
        frame_count++;
        
        // Print progress every 10 frames
        if (frame_count % 10 == 0) {
            printf("Frame: %d, Player position: (%.2f, %.2f, %.2f)\n", 
                   frame_count, 
                   game->player.position[0], 
                   game->player.position[1], 
                   game->player.position[2]);
        }
    }
    
    printf("Game loop completed. Cleaning up...\n");
    
    // Cleanup
    cleanup_game(game);
    
    printf("Game ended successfully!\n");
    return 0;
}