#ifndef PHYSICS_H
#define PHYSICS_H

#include "game.h"

// Physics functions
void update_physics_state(GameState* state);
float ball_sdf(float pos[3], GameState* state);
float hole_sdf(float pos[3], GameState* state);
float get_noise(float pos[3]);
float sdf_func_with_state(float pos[3], bool include_dynamic, GameState* state);
float* get_normal_with_state(float pos[3], float eps, bool include_dynamic, GameState* state);
float collide_with_state(float pos[3], float destination[3], float radius, GameState* state);
float* place_on_ground(float xz[2], GameState* state);

#endif