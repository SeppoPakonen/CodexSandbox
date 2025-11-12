/*
 * boats_physics.h
 * 
 * Physics functions for the boats game, converted from GLSL Shadertoy code
 */

#ifndef BOATS_PHYSICS_H
#define BOATS_PHYSICS_H

#include <math.h>

// Constants
#define PI 3.14159265359f
#define MAX_BOATS 3
#define MAX_DRONES 1

// Structure definitions
typedef struct {
    float x, y, z;
    float angle;
    float speed;
    float size;
    float rotation_matrix[9];  // 3x3 rotation matrix
} Boat;

typedef struct {
    float x, y, z;
    float pitch, yaw, roll;
    float speed;
    float size;
} Drone;

// Physics functions

// Noise functions
float noise_ff(float p);
float noise_fv2(float x, float y);
float noise_fv3a(float x, float y, float z);
float fbm1(float p);
float fbm2(float x, float y);
float fbm3(float x, float y, float z);
float fbm_n(float x, float y, float z, float nx, float ny, float nz);

// Wave functions
float wave_height(float x, float z, float time);
float wave_ray(float ro_x, float ro_y, float ro_z, 
               float rd_x, float rd_y, float rd_z);
float wave_normal_x(float x, float y, float z, float distance);
float wave_normal_y(float x, float y, float z, float distance);
float wave_normal_z(float x, float y, float z, float distance);

// Boat physics
void boat_physics_matrix(Boat* boat);
void update_boat_position(Boat* boat, float time);

// Drone physics
void update_drone_position(Drone* drone, float time);

// Utility functions
float smooth_min(float a, float b, float r);
float smooth_bump(float lo, float hi, float w, float x);
float* rotate_2d(float x, float y, float angle);

// Distance functions for boat shapes
float pr_round_box_df(float px, float py, float pz, float bx, float by, float bz, float r);
float pr_sph_df(float px, float py, float pz, float s);
float pr_cyl_df(float px, float py, float pz, float r, float h);
float pr_caps_df(float px, float py, float pz, float r, float h);
float pr_cyl_an_df(float px, float py, float pz, float r, float w, float h);
float pr_r_cyl_df(float px, float py, float pz, float r, float rt, float h);

// Object distance functions
float boat_df(float px, float py, float pz, int boat_index);
float drone_df(float px, float py, float pz);
float rock_df(float px, float py, float pz);
float obj_df(float px, float py, float pz);

#endif // BOATS_PHYSICS_H