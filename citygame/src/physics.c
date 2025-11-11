#include <math.h>
#include <stdlib.h>
#include "physics.h"
#include "game.h"  // For constants like BLOCK_BUFFER_X

// Physics implementation based on the Shadertoy GLSL code

void init_vehicle(Vehicle *v, int vtype, float vel, float offset, int dir_in, int dir_out, int lane) {
    v->vtype = vtype;
    v->vel = vel;
    v->offset = offset;
    v->dir_in = dir_in;
    v->dir_out = dir_out;
    v->brake = 0;
    v->lane = lane;
    #ifdef DOUBLE_LANES
    v->lane_out = lane;
    #endif
}

int get_vehicle_type(Vehicle *v) {
    return v->vtype;
}

float get_vehicle_velocity(Vehicle *v) {
    return v->vel;
}

float get_vehicle_offset(Vehicle *v) {
    return v->offset;
}

void set_vehicle_brake(Vehicle *v, int brake) {
    v->brake = brake;
}



// Helper function to get bits from a value (similar to GLSL gb function)
float get_bits(float c, float start, float bits) {
    return fmodf(floorf(c / powf(2.0f, start)), powf(2.0f, bits));
}

// Update vehicle physics based on neighboring cells
void update_vehicle_physics(Cell *cell, Block *block, int btype) {
    // For each vehicle in the cell
    for (int n = 0; n < LANES; n++) {
        if (cell->v[n].vtype > 0) {
            cell->v[n].brake = 0;
            
            // Next direction initialization
            if (cell->v[n].offset <= -0.5f) {
                // This would involve calling the routing functions
                // Simplified for now - just maintaining current direction
            }
            
            // Check for braking conditions (simplified)
            float dist = ACC_DIST;  // default distance to next vehicle
            
            // Simplified vehicle interaction
            if (dist < DEC_DIST || stop_at_semaphore(block, cell)) {
                // Slow down
                cell->v[n].vel = fmaxf(0.0f, cell->v[n].vel - 1.0f/VSTEP);
                cell->v[n].brake = 1;
            } else if (dist >= ACC_DIST) {
                // Speed up
                float vmax = (cell->v[n].vtype == 1) ? VMAX_CAR : VMAX_SLOWCAR;
                cell->v[n].vel = fminf(vmax/VSTEP, cell->v[n].vel + 1.0f/VSTEP);
            }
            
            // Update offset
            cell->v[n].offset += cell->v[n].vel;
            
            // Reset if vehicle leaves cell
            if (cell->v[n].offset >= 0.5f) {
                cell->v[n].offset = -0.5f;
                cell->v[n].vel = 1.0f/VSTEP;
            }
        }
    }
}

// Initialize a block
void init_block(Block *block, Vec2 pos, int btype, int ltype, int bh, int th) {
    block->block_pos = pos;
    block->btype = btype;
    block->ltype = ltype;
    block->bh = bh;
    block->th = th;
    block->conns = 15;  // All connections allowed initially
    block->counter = 0.0f;
    block->dist = MAX_DIST;
    block->dir = 0;
    block->aconns = 15;  // All connections allowed initially
    block->semaphore = 0;
}

// Check if position is a road
int is_road(Block *b, int pos) {
    // This is a simplified version of the GLSL isRoad function
    // Convert pos to 2D coordinates within the block
    int x = pos / BLOCK_BUFFER_X;
    int y = pos % BLOCK_BUFFER_X;
    float fx = (float)x + 0.5f;
    float fy = (float)y + 0.5f;
    
    // Check against different block types
    if (b->btype == 0) return 0;  // No road if no block type
    
    // Roundabout check
    if (b->btype == 1 && (fabsf(fx-3.0f) + fabsf(fy-3.0f) > 3.0f || fabsf(fx-3.0f) + fabsf(fy-3.0f) < 2.0f)) {
        return 0;
    }
    
    // Connection checks based on the block's connections
    if ((b->conns & 1) == 0 && fabsf(fy - 1.0f) > 3.0f) return 0;  // No North connection
    if ((b->conns & 2) == 0 && fabsf(fx - 1.0f) > 3.0f) return 0;  // No East connection
    if ((b->conns & 4) == 0 && fabsf(fy - 5.0f) > 3.0f) return 0;  // No South connection
    if ((b->conns & 8) == 0 && fabsf(fx - 5.0f) > 3.0f) return 0;  // No West connection
    
    // Junction checks
    if ((b->btype == 4 || b->btype == 2 || b->btype == 5) && 
        (fminf(fabsf(fx - 3.0f), fabsf(fy - 3.0f)) > 1.0f)) {
        return 0;
    }
    
    return 1;
}

// Check if position is a shift (overpass connections)
int is_shift(Block *b, int pos) {
    // Simplified check for overpass shifts
    if (b->btype == 5 && (pos == 7 || pos == 10 || pos == 25 || pos == 28)) {
        return 1;
    }
    return 0;
}

// Check if vehicle should stop at semaphore
int stop_at_semaphore(Block *b, Cell *c) {
    // Specific positions for each semaphore type
    if (b->btype == 2 && b->semaphore == 1 && 
        (c->pos == 16 || c->pos == 117 || c->pos == 118 || c->pos == 19)) {
        return 1;
    }
    if (b->btype == 2 && b->semaphore == 0 && 
        (c->pos == 102 || c->pos == 8 || c->pos == 27 || c->pos == 133)) {
        return 1;
    }
    return 0;
}

// Check if pos is a block coordinate (z == 0)
int is_block_coord(int pos) {
    // In our system, if pos is in the range of the first layer (z=0), it's a block coord
    // For now, we'll consider positions 0-35 as block coordinates (corresponding to 6x6 block)
    return (pos >= 0 && pos < 36);
}

// Calculate next coordinate (simplified)
Vec2 next_coord(Cell *c, int direction, int btype) {
    Vec2 result = {0.0f, 0.0f};
    
    // This would need more complex logic as in the GLSL, but for now:
    // Directions: 0=North, 1=East, 2=South, 3=West
    switch (direction) {
        case 0: // North
            result.x = c->block_pos.x;
            result.y = c->block_pos.y + 1.0f;
            break;
        case 1: // East
            result.x = c->block_pos.x + 1.0f;
            result.y = c->block_pos.y;
            break;
        case 2: // South
            result.x = c->block_pos.x;
            result.y = c->block_pos.y - 1.0f;
            break;
        case 3: // West
            result.x = c->block_pos.x - 1.0f;
            result.y = c->block_pos.y;
            break;
    }
    
    return result;
}