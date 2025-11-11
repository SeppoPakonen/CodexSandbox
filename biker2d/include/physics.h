#ifndef PHYSICS_H
#define PHYSICS_H

#include "biker2d.h"

// Physics function prototypes

// Initialize bike particles
void init_bike_particles(GameState* state);

// Track functions
float dtrack(float x, float y);
float lnoise1(float x, float y);
float fbm1(float x);
float noise1(float x);
float ntrack_x(float x, float y);  // x component of track normal
float ntrack_y(float x, float y);  // y component of track normal

// Particle and constraint functions
void constraint(float* pa, float* pb, float* force, float* impulse, float rest_length, float kspring);
bool is_inverted(float* p0, float* p1, float* p2);
void update_physics(GameState* state);

// Helper functions
float rand_float(float x);
float clamp(float value, float min_val, float max_val);
float mix_val(float a, float b, float t);
float sign(float x);

#endif