#include "physics.h"
#include <math.h>
#include <stdlib.h>

// Helper functions
float vec3_length(float v[3]) {
    return sqrtf(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

void vec3_normalize(float v[3]) {
    float len = vec3_length(v);
    if (len > 0.00001f) {
        v[0] /= len;
        v[1] /= len;
        v[2] /= len;
    }
}

void vec3_copy(float dest[3], float src[3]) {
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
}

float vec3_dot(float a[3], float b[3]) {
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

void vec3_cross(float result[3], float a[3], float b[3]) {
    result[0] = a[1]*b[2] - a[2]*b[1];
    result[1] = a[2]*b[0] - a[0]*b[2];
    result[2] = a[0]*b[1] - a[1]*b[0];
}

void vec3_add(float result[3], float a[3], float b[3]) {
    result[0] = a[0] + b[0];
    result[1] = a[1] + b[1];
    result[2] = a[2] + b[2];
}

void vec3_sub(float result[3], float a[3], float b[3]) {
    result[0] = a[0] - b[0];
    result[1] = a[1] - b[1];
    result[2] = a[2] - b[2];
}

void vec3_scale(float result[3], float v[3], float s) {
    result[0] = v[0] * s;
    result[1] = v[1] * s;
    result[2] = v[2] * s;
}

float clamp(float value, float min_val, float max_val) {
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}

float smoothstep(float edge0, float edge1, float x) {
    float t = clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

// Hash function for random numbers (from GLSL version)
unsigned int hash(unsigned int seed) {
    seed = (seed << 13) ^ seed;
    seed = seed * (seed * seed * 15731 + 789221) + 1376312589;
    return seed;
}

float rand_float(unsigned int seed) {
    return (float)(hash(seed) & 0x7fffffff) / (float)0x7fffffff;
}

// SDF for the ball
float ball_sdf(float pos[3], GameState* state) {
    float diff[3];
    diff[0] = pos[0] - state->ball_pos[0];
    diff[1] = pos[1] - state->ball_pos[1];
    diff[2] = pos[2] - state->ball_pos[2];
    
    return vec3_length(diff) - BALL_RADIUS;
}

// SDF for the hole
float hole_sdf(float pos[3], GameState* state) {
    float dhole[3];
    dhole[0] = pos[0] - state->hole_pos[0];
    dhole[1] = pos[1] - state->hole_pos[1];
    dhole[2] = pos[2] - state->hole_pos[2];
    
    dhole[1] = (dhole[1] < 0.0f) ? dhole[1] : 0.0f; // sphere bottom with cylinder above
    
    return HOLE_RADIUS - vec3_length(dhole);
}

// Noise function
float get_noise(float pos[3]) {
    float noise = (sinf(pos[0]/3.0f) + sinf(pos[2]/3.0f)) * 3.0f / 2.0f;
    float noise2 = (sinf(pos[2]/6.283f) + sinf((-pos[0]*sqrtf(0.75f) + pos[2]*sqrtf(0.25f))/6.283f))/2.0f;
    float noise3 = (sinf(pos[0]/28.0f) + sinf((pos[2]*sqrtf(0.75f) - pos[0]*sqrtf(0.25f))/28.0f))/2.0f;
    float noise4 = (sinf(pos[0]/100.0f + sinf((pos[0]+pos[2])/61.8f)) + 
                    sinf(pos[2]/100.0f + sinf((pos[0]-pos[2])/77.7f)) + 2.0f)/4.0f;
    
    return ((noise * (0.5f - noise2)) * (6.283f + noise3 * 28.0f) * 0.5f) * (1.0f - powf(noise4, 2.0f)); // flatter spots
}

// Get noise at hole position for flattening around hole
float get_noise_at_hole(GameState* state) {
    float hole_pos[3] = {state->hole_pos[0], state->hole_pos[1], state->hole_pos[2]};
    return get_noise(hole_pos);
}

// Main SDF function
float sdf_func_with_state(float pos[3], bool include_dynamic, GameState* state) {
    const float maxgrad = 1.0f; // steepest gradient the noise can output
    
    // Get base noise
    float noise = get_noise(pos);
    
    // Flatten around hole
    float dist_to_hole = sqrtf(powf(pos[0] - state->hole_pos[0], 2) + 
                               powf(pos[1] - state->hole_pos[1], 2) + 
                               powf(pos[2] - state->hole_pos[2], 2));
    float hole_noise = get_noise_at_hole(state);
    noise = noise + powf(smoothstep(8.0f, 2.0f, dist_to_hole), 2.0f) * (hole_noise - noise);
    
    float f = (pos[1] + noise) / sqrtf(1.0f + maxgrad*maxgrad); // normalize so 3D gradient is <= 1.0 at all points
    
    if (include_dynamic) {
        float ball_dist = ball_sdf(pos, state);
        if (ball_dist < f) f = ball_dist;
    }
    
    // Add hole to the SDF
    float hole_dist = hole_sdf(pos, state);
    if (hole_dist > f) f = hole_dist;
    
    return f;
}

// Get normal vector
float* get_normal_with_state(float pos[3], float eps, bool include_dynamic, GameState* state) {
    static float normal[3];
    float d = eps;
    float pos1[3] = {pos[0]-d, pos[1]+d, pos[2]+d};
    float pos2[3] = {pos[0]+d, pos[1]-d, pos[2]+d};
    float pos3[3] = {pos[0]+d, pos[1]+d, pos[2]-d};
    float pos4[3] = {pos[0]-d, pos[1]-d, pos[2]-d};
    
    float f1 = sdf_func_with_state(pos1, include_dynamic, state);
    float f2 = sdf_func_with_state(pos2, include_dynamic, state);
    float f3 = sdf_func_with_state(pos3, include_dynamic, state);
    float f4 = sdf_func_with_state(pos4, include_dynamic, state);
    
    normal[0] = f1 - f2 - f3 + f4;
    normal[1] = -f1 + f2 - f3 + f4;
    normal[2] = -f1 - f2 + f3 + f4;
    
    vec3_normalize(normal);
    return normal;
}

// Collision detection
float collide_with_state(float pos[3], float destination[3], float radius, GameState* state) {
    radius -= COLLISION_THRESHOLD;
    
    float d_vec[3];
    d_vec[0] = destination[0] - pos[0];
    d_vec[1] = destination[1] - pos[1];
    d_vec[2] = destination[2] - pos[2];
    
    float d = vec3_length(d_vec);
    
    float r[3];
    if (d > 0.0001f) {
        r[0] = d_vec[0] / d;
        r[1] = d_vec[1] / d;
        r[2] = d_vec[2] / d;
    } else {
        r[0] = r[1] = r[2] = 0.0f;
    }
    
    // Calculate normal at position
    float n[3] = {0, 1, 0}; // default normal
    float* normal_result = get_normal_with_state(pos, 0.001f, false, state);
    n[0] = normal_result[0];
    n[1] = normal_result[1];
    n[2] = normal_result[2];
    
    // Displace toward closest surface
    float pos_adj[3];
    pos_adj[0] = pos[0] - n[0] * radius;
    pos_adj[1] = pos[1] - n[1] * radius;
    pos_adj[2] = pos[2] - n[2] * radius;
    
    float h = sdf_func_with_state(pos_adj, false, state);
    
    // Early out
    if (h >= d || (h < COLLISION_THRESHOLD && vec3_dot(r, n) > 0.0f)) {
        return 1.0f;
    }
    
    float t = 0.0f;
    for (int i = 0; i < 20; i++) {
        float temp_pos[3];
        temp_pos[0] = pos_adj[0] + r[0] * t;
        temp_pos[1] = pos_adj[1] + r[1] * t;
        temp_pos[2] = pos_adj[2] + r[2] * t;
        
        t += h;
        h = sdf_func_with_state(temp_pos, false, state);
        if (t > d || h < COLLISION_THRESHOLD) {
            break;
        }
    }
    
    t /= d;
    return (t < 1.0f) ? t : 1.0f;
}

// Place a point onto the ground
float* place_on_ground(float xz[2], GameState* state) {
    static float result[3];
    const float minY = -100.0f, maxY = 100.0f;
    
    float start[3] = {xz[0], maxY, xz[1]};
    float end[3] = {xz[0], minY, xz[1]};
    
    float h = collide_with_state(start, end, BALL_RADIUS, state);
    result[0] = xz[0];
    result[1] = minY + h * (maxY - minY);
    result[2] = xz[1];
    
    return result;
}

// Main physics update function
void update_physics_state(GameState* state) {
    float lastCamPos[3] = {state->cam_pos[0], state->cam_pos[1], state->cam_pos[2]};
    float lastBallPos[3] = {state->ball_pos[0], state->ball_pos[1], state->ball_pos[2]};
    
    if (state->frame == 0) {
        // Initialize ball position
        float xz[2] = {0.0f, 0.0f};
        float* placed = place_on_ground(xz, state);
        state->ball_pos[0] = placed[0];
        state->ball_pos[1] = placed[1] + BALL_RADIUS;
        state->ball_pos[2] = placed[2];
        
        state->ball_dpos[0] = 0.0f;
        state->ball_dpos[1] = 0.0f;
        state->ball_dpos[2] = 0.0f;
        
        state->mouse_drag_start[0] = -1.0f;
        state->mouse_drag_start[1] = 0.0f;
        
        state->mouse_drag[0] = 0.0f;
        state->mouse_drag[1] = 0.0f;
        
        state->cam_pos[0] = state->ball_pos[0];
        state->cam_pos[1] = state->ball_pos[1] + 1.0f;
        state->cam_pos[2] = state->ball_pos[2] - 3.0f;
    }
    
    // Check if ball is in hole
    float ball_to_hole[3];
    ball_to_hole[0] = state->ball_pos[0] - state->hole_pos[0];
    ball_to_hole[1] = state->ball_pos[1] - state->hole_pos[1];
    ball_to_hole[2] = state->ball_pos[2] - state->hole_pos[2];
    
    float dist_to_hole = vec3_length(ball_to_hole);
    
    if ((state->stationary && dist_to_hole < HOLE_RADIUS) || state->frame == 0) {
        // Place new hole
        float course_direction[2] = {0.0f, 1.0f};
        float new_xz[2];
        new_xz[0] = state->ball_pos[0] + course_direction[0]*70.0f + rand_float(0)*0.5f*70.0f;
        new_xz[1] = state->ball_pos[2] + course_direction[1]*70.0f + rand_float(42)*0.5f*70.0f; // Use different seed
        
        float* new_hole_pos = place_on_ground(new_xz, state);
        state->hole_pos[0] = new_hole_pos[0];
        state->hole_pos[1] = new_hole_pos[1] - HOLE_RADIUS; // hole position is centre of sphere, with cylindrical hole cut above it
        state->hole_pos[2] = new_hole_pos[2];
        
        // Move ball up onto ground
        float ball_xz[2] = {state->ball_pos[0], state->ball_pos[2]};
        float* ball_placed = place_on_ground(ball_xz, state);
        state->ball_pos[0] = ball_placed[0];
        state->ball_pos[1] = ball_placed[1] + BALL_RADIUS;
        state->ball_pos[2] = ball_placed[2];
    }
    
    // Mouse drag detection
    float lastDragStart[2] = {state->mouse_drag_start[0], state->mouse_drag_start[1]};
    float dragged[2] = {0.0f, 0.0f};
    
    if (state->mouse[0] > 0.0f && state->mouse[2] > 0.0f) { // mouse is dragging
        if (state->mouse_drag_start[0] == -1.0f) {
            state->mouse_drag_start[0] = state->mouse[0];
            state->mouse_drag_start[1] = state->mouse[1];
        }
        state->mouse_drag[0] = state->mouse[0] - state->mouse_drag_start[0];
        state->mouse_drag[1] = state->mouse[1] - state->mouse_drag_start[1];
    } else if (state->mouse_drag_start[0] != -1.0f) { // mouse isn't dragging, but it was on previous frame
        state->mouse_drag[0] = state->mouse[0] - state->mouse_drag_start[0];
        state->mouse_drag[1] = state->mouse[1] - state->mouse_drag_start[1];
        dragged[0] = state->mouse_drag[0];
        dragged[1] = state->mouse_drag[1];
        // reset drag data
        state->mouse_drag_start[0] = -1.0f;
        state->mouse_drag_start[1] = 0.0f;
    }
    
    // Track camera with ball
    float camTarget[3];
    camTarget[0] = state->ball_pos[0] + 0.0f;
    camTarget[1] = state->ball_pos[1] + 0.3f;
    camTarget[2] = state->ball_pos[2] + 0.0f;
    
    float diff[3];
    diff[0] = (state->cam_pos[0] - state->ball_pos[0]);
    diff[1] = 0.0f; // ignore y for horizontal distance
    diff[2] = (state->cam_pos[2] - state->ball_pos[2]);
    
    float dist = vec3_length(diff);
    if (dist > 0.0001f) {
        diff[0] = diff[0] / dist * 2.0f;
        diff[2] = diff[2] / dist * 2.0f;
    }
    
    camTarget[0] += diff[0];
    camTarget[2] += diff[2];
    
    camTarget[0] -= state->cam_pos[0];
    camTarget[1] -= state->cam_pos[1];
    camTarget[2] -= state->cam_pos[2];
    
    float target_dist = vec3_length(camTarget);
    float move_factor = (target_dist * 0.1f > 0.0f) ? target_dist * 0.1f : 0.0f;
    
    if (target_dist > 0.0001f) {
        state->cam_pos[0] += (camTarget[0] / target_dist) * move_factor;
        state->cam_pos[1] += (camTarget[1] / target_dist) * move_factor;
        state->cam_pos[2] += (camTarget[2] / target_dist) * move_factor;
    }
    
    // Keep camera out of ground
    float cam_ground_check[3] = {state->cam_pos[0], state->cam_pos[1] - 0.3f, state->cam_pos[2]};
    float ground_sdf = sdf_func_with_state(cam_ground_check, false, state);
    state->cam_pos[1] += (ground_sdf < 0.0f) ? -1.5f * ground_sdf : 0.0f;
    
    // Golf swing logic
    float swingPos[2];
    swingPos[0] = state->width * 0.499f;
    swingPos[1] = state->height * 0.299f;
    float swingRad = state->height * 0.03f;
    
    if (sqrtf(powf(lastDragStart[0] - swingPos[0], 2) + powf(lastDragStart[1] - swingPos[1], 2)) < swingRad) {
        if (state->stationary) {
            float strength = clamp((-dragged[1])/(0.25f * state->height), 0.0f, 1.0f);
            
            if (strength > 0.0f) {
                float force = powf(2.0f, strength * 3.0f) * 3.0f / 60.0f;
                
                // Calculate direction
                float dir[3];
                dir[0] = state->ball_pos[0] - state->cam_pos[0];
                dir[1] = 0.0f; // horizontal only
                dir[2] = state->ball_pos[2] - state->cam_pos[2];
                
                float len = sqrtf(dir[0]*dir[0] + dir[2]*dir[2]);
                if (len > 0.0001f) {
                    dir[0] /= len;
                    dir[2] /= len;
                }
                
                // Apply the hit
                state->ball_dpos[0] += force * dir[0] * 0.7071f;
                state->ball_dpos[1] += (force > 6.0f/60.0f) ? force - 6.0f/60.0f : 0.0f; 
                state->ball_dpos[2] += force * dir[2] * 0.7071f;
            }
        }
    } else {
        if (state->mouse_drag_start[0] != -1.0f) {
            // Rotate camera by mouse drag
            float a = state->mouse_drag[0] * 0.0001f; // rotation speed
            
            float cam_diff[3];
            cam_diff[0] = state->cam_pos[0] - state->ball_pos[0];
            cam_diff[1] = state->cam_pos[1] - state->ball_pos[1];
            cam_diff[2] = state->cam_pos[2] - state->ball_pos[2];
            
            // Rotate around Y axis
            state->cam_pos[0] = state->ball_pos[0] + cam_diff[0]*cosf(a) - cam_diff[2]*sinf(a);
            state->cam_pos[2] = state->ball_pos[2] + cam_diff[0]*sinf(a) + cam_diff[2]*cosf(a);
            
            state->cam_pos[1] += state->mouse_drag[1] * 0.001f;
        }
    }
    
    // Apply physics
    float t = 1.0f;
    
    // Gravity
    state->ball_dpos[1] -= 9.81f / 3600.0f;
    
    // Air resistance
    float vel_len = vec3_length(state->ball_dpos);
    float air_resistance[3];
    air_resistance[0] = 0.01f * state->ball_dpos[0] * vel_len;
    air_resistance[1] = 0.01f * state->ball_dpos[1] * vel_len;
    air_resistance[2] = 0.01f * state->ball_dpos[2] * vel_len;
    state->ball_dpos[0] -= air_resistance[0];
    state->ball_dpos[1] -= air_resistance[1];
    state->ball_dpos[2] -= air_resistance[2];
    
    // Calculate destination
    float destination[3];
    destination[0] = state->ball_pos[0] + state->ball_dpos[0];
    destination[1] = state->ball_pos[1] + state->ball_dpos[1];
    destination[2] = state->ball_pos[2] + state->ball_dpos[2];
    
    t = collide_with_state(state->ball_pos, destination, BALL_RADIUS, state);
    
    state->ball_pos[0] += t * state->ball_dpos[0];
    state->ball_pos[1] += t * state->ball_dpos[1];
    state->ball_pos[2] += t * state->ball_dpos[2];
    
    if (t < 1.0f) {
        // Collision response
        float collision_normal[3];
        float* normal_result = get_normal_with_state(state->ball_pos, BALL_RADIUS, false, state);
        collision_normal[0] = normal_result[0];
        collision_normal[1] = normal_result[1];
        collision_normal[2] = normal_result[2];
        
        float perp = vec3_dot(state->ball_dpos, collision_normal);
        
        // Update velocity based on collision
        state->ball_dpos[0] -= perp * collision_normal[0];
        state->ball_dpos[1] -= perp * collision_normal[1];
        state->ball_dpos[2] -= perp * collision_normal[2];
        
        // Friction
        float vel_mag = vec3_length(state->ball_dpos);
        if (vel_mag > 0.0001f) {
            float friction_factor = (vel_mag < 0.0007f) ? vel_mag : 0.0007f;
            state->ball_dpos[0] -= friction_factor * (state->ball_dpos[0] / vel_mag);
            state->ball_dpos[1] -= friction_factor * (state->ball_dpos[1] / vel_mag);
            state->ball_dpos[2] -= friction_factor * (state->ball_dpos[2] / vel_mag);
        }
        
        float rebound = (perp < 0.0f) ? fmaxf(-BOUNCE_FACTOR * perp - 0.01f, 0.0f) : 0.0f;
        
        state->ball_dpos[0] += rebound * collision_normal[0];
        state->ball_dpos[1] += rebound * collision_normal[1];
        state->ball_dpos[2] += rebound * collision_normal[2];
        
        // Complete the move after collision
        float remaining_move[3];
        remaining_move[0] = (1.0f - t) * state->ball_dpos[0];
        remaining_move[1] = (1.0f - t) * state->ball_dpos[1];
        remaining_move[2] = (1.0f - t) * state->ball_dpos[2];
        
        state->ball_pos[0] += remaining_move[0];
        state->ball_pos[1] += remaining_move[1];
        state->ball_pos[2] += remaining_move[2];
        
        // Force ball out of ground if needed
        float ball_check_pos[3];
        ball_check_pos[0] = state->ball_pos[0] - collision_normal[0] * BALL_RADIUS;
        ball_check_pos[1] = state->ball_pos[1] - collision_normal[1] * BALL_RADIUS;
        ball_check_pos[2] = state->ball_pos[2] - collision_normal[2] * BALL_RADIUS;
        
        float sdf_check = sdf_func_with_state(ball_check_pos, false, state);
        if (sdf_check < 0.0f) {
            state->ball_pos[0] += (-sdf_check) * collision_normal[0];
            state->ball_pos[1] += (-sdf_check) * collision_normal[1];
            state->ball_pos[2] += (-sdf_check) * collision_normal[2];
        }
    }
    
    // Stop almost-stationary balls
    if (vec3_length(state->ball_dpos) < 0.0001f) {
        state->ball_dpos[0] = 0.0f;
        state->ball_dpos[1] = 0.0f;
        state->ball_dpos[2] = 0.0f;
    }
    
    // Update stationary flag
    state->stationary = (vec3_length(state->ball_dpos) == 0.0f);
    
    // Update TAA value
    float cam_diff[3];
    cam_diff[0] = state->cam_pos[0] - lastCamPos[0];
    cam_diff[1] = state->cam_pos[1] - lastCamPos[1];
    cam_diff[2] = state->cam_pos[2] - lastCamPos[2];
    
    float cam_move = vec3_length(cam_diff);
    
    float ball_dir1[3] = {state->ball_pos[0] - state->cam_pos[0], 
                          state->ball_pos[1] - state->cam_pos[1], 
                          state->ball_pos[2] - state->cam_pos[2]};
    float ball_dir2[3] = {lastBallPos[0] - lastCamPos[0], 
                          lastBallPos[1] - lastCamPos[1], 
                          lastBallPos[2] - lastCamPos[2]};
    
    vec3_normalize(ball_dir1);
    vec3_normalize(ball_dir2);
    
    float rotation = acosf(fmaxf(-1.0f, fminf(1.0f, vec3_dot(ball_dir1, ball_dir2)))) / 0.01f;
    
    state->taa = powf(smoothstep(1.0f, 0.0f, cam_move/1.0f + rotation), 2.0f);
}