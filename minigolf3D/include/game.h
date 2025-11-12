#ifndef GAME_H
#define GAME_H

#include "renderer.h"
#include <stdbool.h>

// Game constants
#define BALL_RADIUS 0.025f
#define HOLE_RADIUS 0.08f
#define GRAVITY 9.81f
#define BOUNCE_DAMPING 0.5f

// Game state
typedef enum {
    GAME_STATE_IDLE,
    GAME_STATE_AIMING,
    GAME_STATE_SHOOTING,
    GAME_STATE_BALL_MOVING,
    GAME_STATE_BALL_IN_HOLE
} GameState;

// Game object structures
typedef struct {
    Vec3 position;
    Vec3 velocity;
    float radius;
} Ball;

typedef struct {
    Vec3 position;
    float radius;
} Hole;

typedef struct {
    Vec3 position;
    float strength;
    bool active;
} Shot;

// Main game structure
typedef struct {
    Renderer* renderer;
    GameState state;
    Ball ball;
    Hole hole;
    Shot current_shot;
    Vec3 camera_pos;
    Vec3 camera_target;
    float time;
    bool mouse_pressed;
    float mouse_start_x, mouse_start_y;
    float mouse_current_x, mouse_current_y;
} Game;

// Function declarations
Game* init_game(RendererType renderer_type);
void cleanup_game(Game* game);
void update_game(Game* game, float delta_time);
void render_game(Game* game);
bool is_game_running(Game* game);
void handle_input(Game* game);

#endif