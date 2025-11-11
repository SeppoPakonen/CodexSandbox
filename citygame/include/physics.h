#ifndef PHYSICS_H
#define PHYSICS_H

#include "types.h"
#include "game.h"  // For constants

// Hash function declarations
float hash(float x);
float hash_vec2(Vec2 v);

// Physics constants
#define VEHICLE_DENSITY 0.5f
#define ACC_DIST 1.8f
#define DEC_DIST 1.7f
#define VMAX_CAR 5.0f
#define VMAX_SLOWCAR 3.0f
#define VSTEP 32.0f  // 2^5
#define MAX_DIST 89  // min((1<<10)-2, int(CITY_SIZE*3.)) where CITY_SIZE = 30

// Physics functions
void init_vehicle(Vehicle *v, int vtype, float vel, float offset, int dir_in, int dir_out, int lane);
int get_vehicle_type(Vehicle *v);
float get_vehicle_velocity(Vehicle *v);
float get_vehicle_offset(Vehicle *v);
void set_vehicle_brake(Vehicle *v, int brake);
void update_vehicle_physics(Cell *cell, Block *block, int btype);

// Block physics functions
void init_block(Block *block, Vec2 pos, int btype, int ltype, int bh, int th);
int is_road(Block *b, int pos);
int is_shift(Block *b, int pos);
int stop_at_semaphore(Block *b, Cell *c);

// Utility functions for physics
Vec2 next_coord(Cell *c, int direction, int btype);
int is_block_coord(int pos);

#endif