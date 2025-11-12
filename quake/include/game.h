#ifndef GAME_H
#define GAME_H

#include "renderer.h"
#include "physics.h"

typedef enum {
    RENDERER_OLD_SCHOOL,
    RENDERER_MODERN
} RendererType;

typedef struct {
    Player player;
    RendererType renderer_type;
    void *renderer;  // Either OldSchoolRenderer or ModernRenderer
    int window_width;
    int window_height;
    float current_time;
    int frame_count;
} GameState;

// Initialize the game
GameState* init_game(int width, int height);

// Clean up the game
void cleanup_game(GameState *game);

// Update game state
void update_game(GameState *game, float dt);

// Render the current frame
void render_game(GameState *game);

// Handle input
void handle_input(GameState *game, float forward_move, float side_move, int jump);

#endif // GAME_H