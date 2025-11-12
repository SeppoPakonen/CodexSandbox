#ifndef BILLIARD_H
#define BILLIARD_H

#include <math.h>
#include <stdio.h>
#include <stdbool.h>

// Constants
#define PI 3.14159265359f
#define NBALL 16
#define TX_ROW 32.0f
#define DST_FAR 100.0f
#define HB_LEN 8.0f

// Vector structures
typedef struct {
    float x, y;
} vec2;

typedef struct {
    float x, y, z;
} vec3;

typedef struct {
    float x, y, z, w;
} vec4;

// Color structure for rendering
typedef struct {
    unsigned char r, g, b;
} Color;

// Matrix structure (3x3)
typedef struct {
    vec3 col0, col1, col2;
} mat3;

// Ball structure containing position, velocity, and rotation quaternion
typedef struct {
    vec2 position;
    vec2 velocity;
    vec4 rotation_quat;  // Quaternion for rotation
} Ball;

// Game state structure
typedef struct {
    Ball balls[NBALL];
    float hb_len;
    float dt;
    float t_cur;
    float a_cue;
    float n_play;
    float n_step;
    float run_state;
    float d_cue;
    bool show_cue;
    
    // Camera parameters
    float az, el, zm_fac;
    
    // Light direction
    vec3 lt_dir;
    
    // Buffer data (for simulating texture buffer in shaders)
    vec4 buffer_data[NBALL * 2 + 1];  // Position/velocity pairs + state data
} GameState;

// Function prototypes
void init_game_state(GameState *state);
void init_balls(GameState *state);
void update_physics(GameState *state);
mat3 quat_to_rotation_matrix(vec4 q);
vec4 rotation_matrix_to_quat(mat3 m);
mat3 axis_angle_to_rotation_matrix(vec3 axis, float angle);
vec4 euler_to_quat(vec3 e);
float hash(float p);
float noise(vec2 p);
float length_vec2(vec2 v);
float distance_vec2(vec2 a, vec2 b);
vec2 normalize_vec2(vec2 v);
vec3 normalize_vec3(vec3 v);
float dot_vec2(vec2 a, vec2 b);
float dot_vec3(vec3 a, vec3 b);
vec3 cross_vec3(vec3 a, vec3 b);
vec2 vec2_add(vec2 a, vec2 b);
vec2 vec2_sub(vec2 a, vec2 b);
vec2 vec2_mul(vec2 a, float s);
vec3 vec3_add(vec3 a, vec3 b);
vec3 vec3_sub(vec3 a, vec3 b);
vec3 vec3_mul(vec3 a, float s);
vec4 vec4_mul(vec4 a, float s);
float clamp(float value, float min_val, float max_val);
float smoothstep(float edge0, float edge1, float x);
float mix(float x, float y, float a);
Color hsv_to_rgb(float h, float s, float v);

#endif // BILLIARD_H