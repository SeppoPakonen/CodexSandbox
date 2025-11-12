#include "physics.h"
#include <math.h>
#include <stdio.h>

// Initialize player
void init_player(Player *player) {
    player->position[0] = 0.0f;
    player->position[1] = 0.0f;
    player->position[2] = 0.0f;
    
    player->velocity[0] = 0.0f;
    player->velocity[1] = 0.0f;
    player->velocity[2] = 0.0f;
    
    player->angles[0] = 0.0f; // pitch
    player->angles[1] = 0.0f; // yaw
    player->angles[2] = 0.0f; // roll
    
    player->on_ground = false;
    player->step_up = 18.0f; // Standard Quake step height
}

// Apply movement to player based on input
void move_player(Player *player, float forward_move, float side_move, bool jump, float dt) {
    // Convert angles to radians for calculations
    float yaw_rad = player->angles[1] * M_PI / 180.0f;
    
    // Calculate movement direction vectors
    float forward_x = cos(yaw_rad);
    float forward_y = sin(yaw_rad);
    float side_x = -forward_y;
    float side_y = forward_x;
    
    // Calculate movement velocity
    float move_x = forward_x * forward_move * MAX_SPEED + side_x * side_move * MAX_SPEED;
    float move_y = forward_y * forward_move * MAX_SPEED + side_y * side_move * MAX_SPEED;
    
    // Apply movement (only update horizontal velocity, vertical is handled separately)
    player->velocity[0] = move_x;
    player->velocity[1] = move_y;
    
    // Handle jumping if player is on ground
    if (jump && player->on_ground) {
        player->velocity[2] = JUMP_VELOCITY;
        player->on_ground = false;
    }
    (void)dt; // Suppress unused parameter warning for now
}

// Apply gravity to player
void apply_gravity(Player *player, float dt) {
    if (!player->on_ground) {
        player->velocity[2] -= GRAVITY * dt;
    }
}

// Apply friction when player is on ground
void apply_friction(Player *player, float dt) {
    if (player->on_ground) {
        // Calculate horizontal speed
        float speed = sqrt(player->velocity[0] * player->velocity[0] + 
                           player->velocity[1] * player->velocity[1]);
        
        if (speed > 0) {
            // Apply friction to slow down player
            float drop = speed * FRICTION * dt;
            float new_speed = speed - drop;
            
            if (new_speed < 0) {
                new_speed = 0;
            }
            
            // Update velocity proportionally
            if (speed > 0) {
                float scale = new_speed / speed;
                player->velocity[0] *= scale;
                player->velocity[1] *= scale;
            }
        }
    }
}

// Apply acceleration based on input (simplified Quake-style acceleration)
void apply_acceleration(Player *player, float forward_move, float side_move, float dt) {
    // Calculate desired velocity based on input
    float yaw_rad = player->angles[1] * M_PI / 180.0f;
    
    float desired_x = cos(yaw_rad) * forward_move * MAX_SPEED;
    float desired_y = sin(yaw_rad) * forward_move * MAX_SPEED;
    
    // Apply acceleration logic similar to Quake
    if (player->on_ground) {
        // Calculate the change in velocity
        float change_x = desired_x - player->velocity[0];
        float change_y = desired_y - player->velocity[1];
        
        // Apply acceleration
        float acceleration = ACCELERATION;
        float accelspeed = acceleration * dt * sqrt(change_x * change_x + change_y * change_y);
        
        if (accelspeed > sqrt(change_x * change_x + change_y * change_y)) {
            accelspeed = sqrt(change_x * change_x + change_y * change_y);
        }
        
        player->velocity[0] += change_x * accelspeed;
        player->velocity[1] += change_y * accelspeed;
    }
    (void)side_move; // Suppress unused parameter warning
}

// Update player position based on velocity and time
void update_player_position(Player *player, float dt) {
    // Update position based on velocity
    player->position[0] += player->velocity[0] * dt;
    player->position[1] += player->velocity[1] * dt;
    player->position[2] += player->velocity[2] * dt;
}

// Check if player is colliding with world geometry 
// (This is a simplified version - in a real implementation, you'd check against the game's BSP tree or collision mesh)
bool check_collision(Player *player) {
    // Simple boundary check - in reality this would use the actual map collision data
    // Check if player is trying to go through typical Quake level boundaries
    if (player->position[2] < -1000.0f || player->position[2] > 1000.0f) {
        return true; // Collision with vertical boundaries
    }
    
    // For now, just return false - actual collision would check BSP nodes
    return false;
}

// Handle collision detection and response
void handle_collision(Player *player, float dt) {
    // In a complete implementation, this would:
    // 1. Check collision against the map geometry (BSP tree)
    // 2. Slide player along surfaces when colliding
    // 3. Handle step-ups and other Quake-specific collision behaviors
    
    // For now, implement a basic collision check
    if (check_collision(player)) {
        // Simple collision response: stop movement in the direction of collision
        // In a real implementation, we'd slide along walls
        player->velocity[0] = 0.0f;
        player->velocity[1] = 0.0f;
    }
    (void)dt; // Suppress unused parameter warning
}