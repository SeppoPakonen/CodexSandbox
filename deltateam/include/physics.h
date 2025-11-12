#ifndef PHYSICS_H
#define PHYSICS_H

#include <math.h>

// Structure to represent a 3D vector
typedef struct {
    float x, y, z;
} vec3;

// Structure to represent a 2D vector
typedef struct {
    float x, y;
} vec2;

// Structure to represent a 2x2 matrix
typedef struct {
    float x, y;
} mat2;

// Structure to represent a 3x3 matrix
typedef struct {
    vec3 x, y, z;
} mat3;

// Vector operations
static inline vec3 vec3_add(vec3 a, vec3 b) {
    return (vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

static inline vec3 vec3_sub(vec3 a, vec3 b) {
    return (vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline vec3 vec3_mul(vec3 a, float s) {
    return (vec3){a.x * s, a.y * s, a.z * s};
}

static inline float vec3_length(vec3 v) {
    return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}

static inline vec3 vec3_normalize(vec3 v) {
    float len = vec3_length(v);
    if (len > 0.0f) {
        return vec3_mul(v, 1.0f / len);
    }
    return (vec3){0.0f, 0.0f, 0.0f};
}

static inline float vec3_dot(vec3 a, vec3 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

static inline vec3 vec3_cross(vec3 a, vec3 b) {
    return (vec3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

static inline vec2 vec2_add(vec2 a, vec2 b) {
    return (vec2){a.x + b.x, a.y + b.y};
}

static inline vec2 vec2_sub(vec2 a, vec2 b) {
    return (vec2){a.x - b.x, a.y - b.y};
}

static inline float vec2_length(vec2 v) {
    return sqrtf(v.x*v.x + v.y*v.y);
}

static inline float vec2_dot(vec2 a, vec2 b) {
    return a.x*b.x + a.y*b.y;
}

// Matrix operations
static inline vec3 mat3_mul_vec3(mat3 m, vec3 v) {
    vec3 result;
    result.x = m.x.x * v.x + m.y.x * v.y + m.z.x * v.z;
    result.y = m.x.y * v.x + m.y.y * v.y + m.z.y * v.z;
    result.z = m.x.z * v.x + m.y.z * v.y + m.z.z * v.z;
    return result;
}

// Math constants
static const float PI = 3.14159265359f;

// Clamp function
static inline float clamp(float x, float min_val, float max_val) {
    if (x < min_val) return min_val;
    if (x > max_val) return max_val;
    return x;
}

// Min function
static inline float fminf_c(float a, float b) {
    return a < b ? a : b;
}

// Max function
static inline float fmaxf_c(float a, float b) {
    return a > b ? a : b;
}

// Power function
static inline float powf_c(float base, float exponent) {
    return powf(base, exponent);
}

// Texture sampling simulation (placeholder - would be implemented with actual texture sampling)
float texture_sample(float* texture_data, int width, int height, float u, float v);

// Heightmap function converted from GLSL
float heightmap(vec2 pos, float* heightmap_texture, int width, int height);

// Scene function converted from GLSL
vec2 scene(vec3 point, float* heightmap_texture, int width, int height);

// Ray marching function converted from GLSL
float ray_march(vec3 ro, vec3 rd, int steps, float* heightmap_texture, int width, int height);

// Set camera function converted from GLSL
mat3 set_camera(vec3 ro, vec3 ta, float cr);

#endif