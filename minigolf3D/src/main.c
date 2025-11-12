#include "game.h"
#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char* argv[]) {
    // Determine renderer type from command line argument
    RendererType renderer_type = RENDERER_OPENGL;  // Default to OpenGL
    if (argc > 1) {
        if (strcmp(argv[1], "--software") == 0) {
            renderer_type = RENDERER_SOFTWARE;
        } else if (strcmp(argv[1], "--opengl") == 0) {
            renderer_type = RENDERER_OPENGL;
        }
    }

    // Initialize the game
    Game* game = init_game(renderer_type);
    if (!game) {
        fprintf(stderr, "Failed to initialize the game!\n");
        return -1;
    }

    printf("Minigolf 3D Game Started\n");
    printf("Renderer: %s\n", renderer_type == RENDERER_SOFTWARE ? "Software" : "OpenGL");
    printf("Controls: Click and drag to aim and shoot the ball\n");
    printf("          Press ESC to quit\n");

    // Game loop
    float last_time = (float)glfwGetTime();
    while (is_game_running(game)) {
        float current_time = (float)glfwGetTime();
        float delta_time = current_time - last_time;
        last_time = current_time;

        // Update the game state
        update_game(game, delta_time);

        // Render the game
        render_game(game);

        // Simple ball reset after it goes in the hole
        if (game->state == GAME_STATE_BALL_IN_HOLE) {
            printf("Ball is in the hole! Resetting ball...\n");
            
            // Reset ball position
            game->ball.position.x = 0.0f;
            game->ball.position.y = BALL_RADIUS;  // Start above ground
            game->ball.position.z = 0.0f;
            game->ball.velocity.x = 0.0f;
            game->ball.velocity.y = 0.0f;
            game->ball.velocity.z = 0.0f;
            
            // Generate a new random hole location
            srand((unsigned int)time(NULL));
            float rand_x = ((float)(rand() % 200) / 100.0f - 1.0f) * 5.0f; // Between -5 and 5
            float rand_z = 5.0f + (float)(rand() % 100) / 20.0f; // Between 5 and 10
            game->hole.position.x = rand_x;
            game->hole.position.z = rand_z;
            
            game->state = GAME_STATE_IDLE;
        }
    }

    // Cleanup
    cleanup_game(game);
    printf("Game exited successfully.\n");

    return 0;
}