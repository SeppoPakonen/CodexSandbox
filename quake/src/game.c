#include "game.h"
#include "old_school_renderer.h"
#include "modern_renderer.h"
#include <stdlib.h>

// Initialize the game
GameState* init_game(int width, int height) {
    GameState *game = malloc(sizeof(GameState));
    if (!game) return NULL;
    
    game->window_width = width;
    game->window_height = height;
    game->current_time = 0.0f;
    game->frame_count = 0;
    
    // Initialize player
    init_player(&game->player);
    
    // Set initial renderer type (can be switched)
    game->renderer_type = RENDERER_MODERN;  // Start with modern renderer
    
    // Initialize renderer based on type
    if (game->renderer_type == RENDERER_OLD_SCHOOL) {
        game->renderer = init_old_school_renderer(width, height);
    } else {
        game->renderer = init_modern_renderer(width, height);
    }
    
    if (!game->renderer) {
        free(game);
        return NULL;
    }
    
    return game;
}

// Clean up the game
void cleanup_game(GameState *game) {
    if (game) {
        // Clean up renderer based on type
        if (game->renderer_type == RENDERER_OLD_SCHOOL) {
            cleanup_old_school_renderer((OldSchoolRenderer*)game->renderer);
        } else {
            cleanup_modern_renderer((ModernRenderer*)game->renderer);
        }
        
        free(game);
    }
}

// Update game state
void update_game(GameState *game, float dt) {
    // Update player physics
    apply_gravity(&game->player, dt);
    apply_friction(&game->player, dt);
    
    // Update player position
    update_player_position(&game->player, dt);
    
    // Handle collisions
    handle_collision(&game->player, dt);
    
    // Update game time
    game->current_time += dt;
    game->frame_count++;
}

// Render the current frame
void render_game(GameState *game) {
    if (game->renderer_type == RENDERER_OLD_SCHOOL) {
        render_old_school((OldSchoolRenderer*)game->renderer, game);
    } else {
        render_modern((ModernRenderer*)game->renderer, game);
    }
    
    // Present the frame
    if (game->renderer_type == RENDERER_OLD_SCHOOL) {
        present_frame((Renderer*)game->renderer);
    } else {
        present_frame((Renderer*)game->renderer);
    }
}

// Handle input
void handle_input(GameState *game, float forward_move, float side_move, int jump) {
    move_player(&game->player, forward_move, side_move, jump, 0.016f);  // Assuming ~60 FPS
}