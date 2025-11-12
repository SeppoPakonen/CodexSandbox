#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Constants from the original shader
#define SHIP_SIZE 0.05f
#define SHIP_DAMPING_XY 3.0f
#define SHIP_DAMPING_Z 3.0f
#define SHIP_DAMPING_W 0.0f
#define SHIP_THRUST 3.0f
#define SHIP_ANGULAR_THRUST 48.0f
#define GRAVITY 0.60f
#define GROUND_FRICTION 0.2f
#define SEED 10.0f

#define SHIP_NOSE_X 0.0f
#define SHIP_NOSE_Y 1.0f * SHIP_SIZE
#define SHIP_LEFT_WING_X 0.5f * SHIP_SIZE
#define SHIP_LEFT_WING_Y (-0.3f) * SHIP_SIZE
#define SHIP_RIGHT_WING_X (-0.5f) * SHIP_SIZE
#define SHIP_RIGHT_WING_Y (-0.3f) * SHIP_SIZE

#define ASPECT_RATIO 1.0f // This would be calculated based on screen resolution

// Game state structure
typedef struct {
    float position_x;
    float position_y;
    float rotation;
    float thrust;  // w component
    float velocity_x;
    float velocity_y;
    float angular_velocity;
    float extra_velocity; // w component
    float zoom;
} ShipState;

// Function to generate random value based on coordinates
float rand_val(float x, float y) {
    float co[2] = {x, y};
    co[0] += SEED;
    co[1] += SEED;
    float dot = co[0] * 12.9898f + co[1] * 78.233f;
    return fmodf(sinf(dot) * 43758.5453f, 1.0f);
}

// Function to check if a tile should be filled (cave generation logic)
int get_tile(int x, int y) {
    int up = rand_val(x, y + 1) > 0.5f;
    int down = rand_val(x, y - 1) > 0.5f;
    int left = rand_val(x - 1, y) > 0.5f;
    int right = rand_val(x + 1, y) > 0.5f;
    
    int here = rand_val(x, y) > 0.5f;
    
    return (!((up == down) && (left == right) && (up != right)) && here);
}

// Function to calculate map signed distance field
float get_map_sdf(float pixel_x, float pixel_y) {
    // Calculate the tile X and Y IDs for this pixel
    float inner_x = fmodf(pixel_x, 1.0f);
    float inner_y = fmodf(pixel_y, 1.0f);
    
    if (inner_x < 0) inner_x += 1.0f;
    if (inner_y < 0) inner_y += 1.0f;
    
    int tile_x = (int)(pixel_x - inner_x);
    int tile_y = (int)(pixel_y - inner_y);
    
    float a = get_tile(tile_x, tile_y);
    float b = get_tile(tile_x + 1, tile_y);
    float c = get_tile(tile_x, tile_y + 1);
    float d = get_tile(tile_x + 1, tile_y + 1);
    
    float mixed = (a * (1.0f - inner_x) + b * inner_x) * (1.0f - inner_y) + 
                  (c * (1.0f - inner_x) + d * inner_x) * inner_y;
    
    return mixed;
}

// Simplified function to sample the map at a specific position
float sample_map_at(float world_x, float world_y) {
    // This is a simplified version - in the original shader, this would sample from a texture
    float sdf = get_map_sdf(world_x, world_y);
    return 0.6f - sdf; // Similar to the original: float raw_sdf = 0.6 - get_map_sdf(world_coords);
}

// Update physics based on the shader logic
void update_physics(ShipState* state, int key_up, int key_left, int key_right, float time_delta) {
    // Calculate orientation matrix components (from the original get_ship_matrix function)
    float c = cosf(state->rotation);
    float s = sinf(state->rotation);
    
    // Calculate acceleration (vector with 4 components like in the shader)
    float acceleration_x = 0.0f;
    float acceleration_y = -GRAVITY; // Gravity
    float acceleration_z = 0.0f;
    float acceleration_w = 0.0f;
    
    // Thrusters
    float thrust_keys = key_up ? 1.0f : 0.0f;
    state->thrust = thrust_keys; // This sets the w component of position in the original

    // Apply angular thrust (z component of acceleration)
    acceleration_z += (key_left ? -1.0f : 0.0f) + (key_right ? 1.0f : 0.0f);
    acceleration_z *= SHIP_ANGULAR_THRUST;

    // Calculate forward direction based on rotation (ori * vec2(0.0, 1.0))
    float forwards_x = -s; // Equivalent to rotation matrix applied to (0, 1)
    float forwards_y = c;
    
    acceleration_x += forwards_x * SHIP_THRUST * thrust_keys;
    acceleration_y += forwards_y * SHIP_THRUST * thrust_keys;

    // Apply damping (equivalent to acceleration -= velocity * SHIP_DAMPING)
    acceleration_x -= state->velocity_x * SHIP_DAMPING_XY;
    acceleration_y -= state->velocity_y * SHIP_DAMPING_XY;
    acceleration_z -= state->angular_velocity * SHIP_DAMPING_Z;
    acceleration_w -= state->extra_velocity * SHIP_DAMPING_W;

    // Update velocities
    state->velocity_x += acceleration_x * time_delta;
    state->velocity_y += acceleration_y * time_delta;
    state->angular_velocity += acceleration_z * time_delta;
    state->extra_velocity += acceleration_w * time_delta;

    // Update positions
    state->position_x += state->velocity_x * time_delta;
    state->position_y += state->velocity_y * time_delta;
    state->rotation += state->angular_velocity * time_delta;

    // Simplified collision detection with the map
    // Sample map at positions corresponding to ship vertices
    float aspect = ASPECT_RATIO; // This would normally be iResolution.y / iResolution.x
    
    // Calculate positions of the ship's vertices in world coordinates
    float pos_x = state->position_x;
    float pos_y = state->position_y;
    
    // Top vertex (nose)
    float top_x = pos_x + (c * SHIP_NOSE_X - s * SHIP_NOSE_Y) * aspect;
    float top_y = pos_y + (s * SHIP_NOSE_X + c * SHIP_NOSE_Y);
    
    // Left vertex 
    float left_x = pos_x + (c * SHIP_LEFT_WING_X - s * SHIP_LEFT_WING_Y) * aspect;
    float left_y = pos_y + (s * SHIP_LEFT_WING_X + c * SHIP_LEFT_WING_Y);
    
    // Right vertex
    float right_x = pos_x + (c * SHIP_RIGHT_WING_X - s * SHIP_RIGHT_WING_Y) * aspect;
    float right_y = pos_y + (s * SHIP_RIGHT_WING_X + c * SHIP_RIGHT_WING_Y);
    
    // Sample the map at these positions
    float top_sdf = sample_map_at(top_x, top_y);
    float left_sdf = sample_map_at(left_x, left_y);
    float right_sdf = sample_map_at(right_x, right_y);
    
    // Handle collisions (simplified from original shader)
    if (top_sdf < 0.0f) {  // If the top of the ship is inside an obstacle
        // Correct position and apply friction
        state->position_x -= 0.0f; // Simplified correction
        state->position_y -= (top_sdf) / 8.0f; 
        state->velocity_x *= (1.0f - GROUND_FRICTION);
        state->velocity_y *= (1.0f - GROUND_FRICTION);
    }
    
    if (left_sdf < 0.0f) {  // If the left of the ship is inside an obstacle
        state->position_x -= (left_sdf) / 8.0f; 
        state->position_y -= 0.0f; 
        state->velocity_x *= (1.0f - GROUND_FRICTION);
        state->velocity_y *= (1.0f - GROUND_FRICTION);
    }
    
    if (right_sdf < 0.0f) {  // If the right of the ship is inside an obstacle
        state->position_x -= (right_sdf) / 8.0f; 
        state->position_y -= 0.0f; 
        state->velocity_x *= (1.0f - GROUND_FRICTION);
        state->velocity_y *= (1.0f - GROUND_FRICTION);
    }
}

// Initialize ship state
void init_ship_state(ShipState* state) {
    state->position_x = 0.0f;
    state->position_y = 0.2f;
    state->rotation = 0.0f;
    state->thrust = 0.0f;
    state->velocity_x = 0.0f;
    state->velocity_y = 0.0f;
    state->angular_velocity = 0.0f;
    state->extra_velocity = 0.0f;
    state->zoom = 4.0f;
}