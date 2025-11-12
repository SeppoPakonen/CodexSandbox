#ifndef PHYSICS_H
#define PHYSICS_H

#include "game.h"

// Function declarations for physics system
float get_terrain_height(float x, float z);
float calculate_noise(float x, float z);
void update_ball_physics(Ball* ball, float delta_time);
int check_ball_collision_with_hole(Ball* ball, Hole* hole);
void handle_collision_with_surface(Ball* ball, float surface_normal_x, float surface_normal_y, float surface_normal_z);

#endif