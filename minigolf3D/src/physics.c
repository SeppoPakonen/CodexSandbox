#include "physics.h"
#include <math.h>

// Terrain generation function based on the Shadertoy code
float calculate_noise(float x, float z) {
    float noise1 = (sinf(x / 3.0f) + sinf(z / 3.0f)) * 3.0f / 2.0f;
    float noise2 = (sinf(z / 6.283f) + sinf((-x * sqrtf(0.75f) + z * sqrtf(0.25f)) / 6.283f)) / 2.0f;
    float noise3 = (sinf(x / 28.0f) + sinf((z * sqrtf(0.75f) - x * sqrtf(0.25f)) / 28.0f)) / 2.0f;
    float noise4 = (sinf(x / 100.0f + sinf((x + z) / 61.8f)) + sinf(z / 100.0f + sinf((x - z) / 77.7f)) + 2.0f) / 4.0f;
    
    float result = ((noise1 * (0.5f - noise2)) * 0.5f + noise2 * 6.283f + noise3 * 28.0f) * 0.5f;
    result = result * (1.0f - powf(noise4, 2.0f));
    
    return result;
}

float get_terrain_height(float x, float z) {
    float noise = calculate_noise(x, z);
    // Make the terrain flatter around the hole
    Vec3 hole_pos = {0.0f, 0.0f, 5.0f}; // Default hole position
    float dist_to_hole = sqrtf((x - hole_pos.x) * (x - hole_pos.x) + (z - hole_pos.z) * (z - hole_pos.z));
    float hole_influence = 1.0f - fminf(1.0f, fmaxf(0.0f, (dist_to_hole - 2.0f) / 6.0f)); // Smooth transition
    hole_influence *= hole_influence; // Quadratic falloff
    
    float hole_height = calculate_noise(hole_pos.x, hole_pos.z);
    float terrain_height = noise;
    
    // Interpolate between terrain height and hole height based on proximity
    return terrain_height * (1.0f - hole_influence) + hole_height * hole_influence;
}

void update_ball_physics(Ball* ball, float delta_time) {
    if (!ball) return;
    
    // Apply gravity
    ball->velocity.y -= GRAVITY * delta_time;
    
    // Calculate new position
    float new_x = ball->position.x + ball->velocity.x * delta_time;
    float new_y = ball->position.y + ball->velocity.y * delta_time;
    float new_z = ball->position.z + ball->velocity.z * delta_time;
    
    // Get the terrain height at the new position
    float ground_y = get_terrain_height(new_x, new_z) + ball->radius;
    
    // Check for collision with ground
    if (new_y <= ground_y) {
        // Collision with ground
        new_y = ground_y;
        
        // Calculate surface normal (approximate with height differences)
        float dx = get_terrain_height(new_x + 0.1f, new_z) - get_terrain_height(new_x - 0.1f, new_z);
        float dz = get_terrain_height(new_x, new_z + 0.1f) - get_terrain_height(new_x, new_z - 0.1f);
        
        // Create a normal vector pointing up from the surface
        Vec3 normal = {-dx * 0.5f, 1.0f, -dz * 0.5f};
        float normal_len = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
        if (normal_len > 0.0f) {
            normal.x /= normal_len;
            normal.y /= normal_len;
            normal.z /= normal_len;
        }
        
        // Calculate the component of velocity parallel and perpendicular to the surface
        float perp_velocity = ball->velocity.x * normal.x + 
                             ball->velocity.y * normal.y + 
                             ball->velocity.z * normal.z;
        
        // Reflect the velocity vector
        ball->velocity.x -= 2.0f * perp_velocity * normal.x;
        ball->velocity.y -= 2.0f * perp_velocity * normal.y;
        ball->velocity.z -= 2.0f * perp_velocity * normal.z;
        
        // Apply damping
        ball->velocity.x *= (1.0f - 0.05f); // Friction
        ball->velocity.y *= -BOUNCE_DAMPING; // Bounce damping
        ball->velocity.z *= (1.0f - 0.05f);
        
        // Apply additional friction when moving along the ground
        if (new_y <= ground_y + 0.001f) { // If barely above ground
            ball->velocity.x *= 0.95f;
            ball->velocity.z *= 0.95f;
        }
    }
    
    // Update position
    ball->position.x = new_x;
    ball->position.y = new_y;
    ball->position.z = new_z;
    
    // Apply air resistance
    if (ball->position.y > ground_y) { // Only if in the air
        ball->velocity.x *= (1.0f - 0.01f * delta_time);
        ball->velocity.z *= (1.0f - 0.01f * delta_time);
    }
}

int check_ball_collision_with_hole(Ball* ball, Hole* hole) {
    if (!ball || !hole) return 0;
    
    float dx = ball->position.x - hole->position.x;
    float dz = ball->position.z - hole->position.z;
    float horizontal_distance = sqrtf(dx * dx + dz * dz);
    
    // Check if ball is above the hole and within the hole radius
    if (horizontal_distance < hole->radius && 
        ball->position.y <= ball->radius + 0.01f) {
        return 1; // Ball is in the hole
    }
    
    return 0; // Ball is not in the hole
}

void handle_collision_with_surface(Ball* ball, float surface_normal_x, float surface_normal_y, float surface_normal_z) {
    if (!ball) return;
    
    // Normalize the surface normal
    float normal_len = sqrtf(surface_normal_x * surface_normal_x + 
                            surface_normal_y * surface_normal_y + 
                            surface_normal_z * surface_normal_z);
    
    if (normal_len > 0.0f) {
        float inv_len = 1.0f / normal_len;
        surface_normal_x *= inv_len;
        surface_normal_y *= inv_len;
        surface_normal_z *= inv_len;
    }
    
    // Calculate the component of velocity parallel and perpendicular to the surface
    float perp_velocity = ball->velocity.x * surface_normal_x + 
                         ball->velocity.y * surface_normal_y + 
                         ball->velocity.z * surface_normal_z;
    
    // Reflect the velocity vector
    ball->velocity.x -= 2.0f * perp_velocity * surface_normal_x;
    ball->velocity.y -= 2.0f * perp_velocity * surface_normal_y;
    ball->velocity.z -= 2.0f * perp_velocity * surface_normal_z;
    
    // Apply damping
    ball->velocity.x *= (1.0f - 0.05f); // Friction
    ball->velocity.y *= -BOUNCE_DAMPING; // Bounce damping
    ball->velocity.z *= (1.0f - 0.05f);
}