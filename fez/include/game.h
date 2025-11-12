#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

// Constants from GLSL
#define PI 3.14159265359f
#define MAX_STEPS 48
#define EPSILON 0.001f

// Game constants
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define RENDER_SCALE 2.0f

// Vector structures
typedef struct {
    float x, y;
} Vec2;

typedef struct {
    float x, y, z;
} Vec3;

typedef struct {
    float x, y, z, w;
} Vec4;

// Tile structure
typedef struct {
    int ID;
} Tile;

// HitInfo structure
typedef struct {
    Tile tile;
    Vec3 pos;
    Vec3 normal;
} HitInfo;

// Player structure
typedef struct {
    Vec3 position;
    Vec3 velocity;
    float rotation;
    float target_rotation;
    float rotation_time;
    int state;  // w: 0=behind,1=front,2-3=grounded
} Player;

// Game state structure
typedef struct {
    Player player;
    float time;
    float delta_time;
    float frame_count;
    bool keys[256];  // Simple key state array
    Vec4 buffer_a[4];  // Simulate buffer A data
} GameState;

// Function declarations
void init_game(GameState* state);
void update_game(GameState* state);
void render_frame(GameState* state);

// Math functions
void rotate_vec2(float* x, float* y, float angle);
Vec3 vec3_add(Vec3 a, Vec3 b);
Vec3 vec3_sub(Vec3 a, Vec3 b);
Vec3 vec3_mul_scalar(Vec3 v, float s);
float vec3_dot(Vec3 a, Vec3 b);
float vec3_length(Vec3 v);
Vec3 vec3_normalize(Vec3 v);
float mod_func(float x, float y);

// World functions
Tile world(Vec3 p);
HitInfo cast_ray(Vec3 ray_pos, Vec3 ray_dir);
Vec3 get_block_texture(Vec2 uv);
Vec4 get_grass_texture(Vec2 uv);
Vec4 get_player_texture(Vec2 uv);

// Rendering functions
void render_software(GameState* state);
void render_shader(GameState* state);

// Input handling
void handle_input(GameState* state);

#endif // GAME_H