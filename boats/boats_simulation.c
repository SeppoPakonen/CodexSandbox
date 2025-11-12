/*
 * boats_simulation.c
 * 
 * This is a C simulation of the "Boats with Drone" Shadertoy.
 * Since graphics libraries may not be available, this focuses on the physics simulation
 * and can output results to text.
 * 
 * The simulation includes wave physics, boat physics, and drone movement based on the original Shadertoy GLSL code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Game constants
#define PI 3.14159265359f
#define MAX_BOATS 3
#define MAX_DRONES 1
#define SIMULATION_STEPS 1000

// Physics constants
#define GRAVITY 9.81f
#define WATER_HEIGHT 0.0f

// Game objects
typedef struct {
    float x, y, z;
    float angle;
    float speed;
    float size;
} Boat;

typedef struct {
    float x, y, z;
    float pitch, yaw, roll;
    float speed;
    float size;
} Drone;

// Wave and physics functions (converted from GLSL)
static const float cHashA4[4] = {0.0f, 1.0f, 57.0f, 58.0f};
static const float cHashA3[3] = {1.0f, 57.0f, 113.0f};
static const float cHashM = 43758.54f;

// Hash functions
static float* hash_v2f(float p) {
    static float result[2];
    result[0] = fmodf(sinf(p + cHashA4[0]) * cHashM, 1.0f);
    result[1] = fmodf(sinf(p + cHashA4[1]) * cHashM, 1.0f);
    return result;
}

static float* hash_v4f(float p) {
    static float result[4];
    result[0] = fmodf(sinf(p + cHashA4[0]) * cHashM, 1.0f);
    result[1] = fmodf(sinf(p + cHashA4[1]) * cHashM, 1.0f);
    result[2] = fmodf(sinf(p + cHashA4[2]) * cHashM, 1.0f);
    result[3] = fmodf(sinf(p + cHashA4[3]) * cHashM, 1.0f);
    return result;
}

static float* hash_v4v3(float px, float py, float pz) {
    static float result[4];
    const float cHashVA3[3] = {37.1f, 61.7f, 12.4f};
    float dot1 = (px + 0.0f) * cHashVA3[0] + (py + 0.0f) * cHashVA3[1] + (pz + 0.0f) * cHashVA3[2];
    float dot2 = (px + 1.0f) * cHashVA3[0] + (py + 0.0f) * cHashVA3[1] + (pz + 0.0f) * cHashVA3[2];
    float dot3 = (px + 0.0f) * cHashVA3[0] + (py + 1.0f) * cHashVA3[1] + (pz + 0.0f) * cHashVA3[2];
    float dot4 = (px + 1.0f) * cHashVA3[0] + (py + 1.0f) * cHashVA3[1] + (pz + 0.0f) * cHashVA3[2];
    
    result[0] = fmodf(sinf(dot1) * cHashM, 1.0f);
    result[1] = fmodf(sinf(dot2) * cHashM, 1.0f);
    result[2] = fmodf(sinf(dot3) * cHashM, 1.0f);
    result[3] = fmodf(sinf(dot4) * cHashM, 1.0f);
    
    return result;
}

// Noise functions based on Shadertoy implementation
float noise_ff(float p) {
    float ip = floorf(p);
    float fp = fmodf(p, 1.0f);
    fp = fp * fp * (3.0f - 2.0f * fp);  // smoothstep-like
    
    float* t = hash_v2f(ip);
    return t[0] + fp * (t[1] - t[0]);
}

float noise_fv2(float x, float y) {
    float ipx = floorf(x);
    float ipy = floorf(y);
    float fpx = fmodf(x, 1.0f);
    float fpy = fmodf(y, 1.0f);
    
    fpx = fpx * fpx * (3.0f - 2.0f * fpx);
    fpy = fpy * fpy * (3.0f - 2.0f * fpy);
    
    float dot_ip = ipx * cHashA3[0] + ipy * cHashA3[1];
    float* t = hash_v4f(dot_ip);
    
    float a = t[0] + fpx * (t[1] - t[0]);
    float b = t[2] + fpx * (t[3] - t[2]);
    
    return a + fpy * (b - a);
}

float fbm2(float x, float y) {
    float f = 0.0f;
    float a = 1.0f;
    for (int i = 0; i < 5; i++) {
        f += a * noise_fv2(x, y);
        a *= 0.5f;
        x *= 2.0f;
        y *= 2.0f;
    }
    return f;
}

// Wave height calculation based on Shadertoy
float wave_height(float x, float z, float time) {
    float qRot[4] = {1.6f, -1.2f, 1.2f, 1.6f}; // 2x2 rotation matrix
    
    float wFreq = 0.2f;
    float wAmp = 0.3f;
    float t4o_x = time;
    float t4o_z = -time;
    float q2_x = x;
    float q2_z = z;
    
    float ht = 0.0f;
    
    for (int j = 0; j < 3; j++) {
        float t4_x = (t4o_x + q2_x) * wFreq;
        float t4_z = (t4o_z + q2_z) * wFreq;
        
        float* t2 = hash_v4f(t4_x + t4_z); // Simplified for this implementation
        float t4_x_new = t4_x + 2.0f * t2[0] - 1.0f;
        float t4_z_new = t4_z + 2.0f * t2[1] - 1.0f;
        
        float ta_x = fabsf(sinf(t4_x_new));
        float ta_z = fabsf(sinf(t4_z_new));
        
        float v4_x = (1.0f - ta_x) * (ta_x + sqrtf(1.0f - ta_x * ta_x));
        float v4_z = (1.0f - ta_z) * (ta_z + sqrtf(1.0f - ta_z * ta_z));
        
        float v2_x = powf(1.0f - powf(v4_x * v4_x, 0.65f), 8.0f);
        float v2_z = powf(1.0f - powf(v4_z * v4_z, 0.65f), 8.0f);
        
        ht += (v2_x + v2_z) * wAmp;
        
        // Apply rotation
        float new_x = q2_x * qRot[0] + q2_z * qRot[1];
        float new_z = q2_x * qRot[2] + q2_z * qRot[3];
        q2_x = new_x;
        q2_z = new_z;
        
        wFreq *= 2.0f;
        wAmp *= 0.2f;
    }
    
    return ht;
}

// 2D rotation function
float* rotate_2d(float x, float y, float angle) {
    static float result[2];
    float cos_a = cosf(angle);
    float sin_a = sinf(angle);
    
    result[0] = x * cos_a - y * sin_a;
    result[1] = x * sin_a + y * cos_a;
    
    return result;
}

// Boat physics matrix calculation
void boat_physics_matrix(Boat* boat, float time, int boat_index) {
    float bAz = 0.5f * PI - boat->angle;
    float bd_x = 0.0f, bd_y = 0.0f, bd_z = 1.0f;
    
    // Rotate bd vector
    float* rotated_bd = rotate_2d(bd_x, bd_z, bAz);
    bd_x = rotated_bd[0];
    bd_z = rotated_bd[1];
    
    // Calculate wave heights at various points around the boat
    float h[5];
    h[0] = wave_height(boat->x, boat->z, time);
    h[1] = wave_height(boat->x + 0.5f * bd_x, boat->z + 0.5f * bd_z, time);
    h[2] = wave_height(boat->x - 0.5f * bd_x, boat->z - 0.5f * bd_z, time);
    
    // Rotate bd vector 90 degrees
    float* rotated_bd2 = rotate_2d(bd_x, bd_z, -0.5f * PI);
    float bd_x2 = rotated_bd2[0];
    float bd_z2 = rotated_bd2[1];
    
    h[3] = wave_height(boat->x + 1.3f * bd_x2, boat->z + 1.3f * bd_z2, time);
    h[4] = wave_height(boat->x - 1.3f * bd_x2, boat->z - 1.3f * bd_z2, time);
    
    // Update boat position
    boat->y = 0.13f + (2.0f * h[0] + h[1] + h[2] + h[3] + h[4]) / 6.0f;
    
    // Add some rocking motion based on waves
    boat->angle += sinf(time * 2.0f + boat_index) * 0.01f;
}

// Global game state
static Boat boats[MAX_BOATS];
static Drone drones[MAX_DRONES];
static float current_time = 0.0f;

// Initialize game state
void init_simulation() {
    // Initialize boats
    for (int i = 0; i < MAX_BOATS; i++) {
        boats[i].x = (float)(rand() % 100 - 50);
        boats[i].y = 0.0f;
        boats[i].z = (float)(rand() % 100 - 50);
        boats[i].angle = (float)(rand() % 360) * PI / 180.0f;
        boats[i].speed = 1.0f + (float)(rand() % 10) / 5.0f;
        boats[i].size = 1.0f;
    }
    
    // Initialize drones
    for (int i = 0; i < MAX_DRONES; i++) {
        drones[i].x = 0.0f;
        drones[i].y = 5.0f;
        drones[i].z = 0.0f;
        drones[i].pitch = 0.0f;
        drones[i].yaw = 0.0f;
        drones[i].roll = 0.0f;
        drones[i].speed = 0.5f;
        drones[i].size = 0.5f;
    }
    
    printf("Boats with Drone - Physics Simulation\n");
    printf("Simulating boats floating on wave physics and drone movement\n\n");
}

// Update simulation state
void update_simulation(float delta_time) {
    current_time += delta_time;
    
    // Update boats based on wave physics
    for (int i = 0; i < MAX_BOATS; i++) {
        // Apply physics
        boat_physics_matrix(&boats[i], current_time, i);
        
        // Simple movement
        boats[i].x += cosf(boats[i].angle) * boats[i].speed * delta_time;
        boats[i].z += sinf(boats[i].angle) * boats[i].speed * delta_time;
    }
    
    // Update drones
    for (int i = 0; i < MAX_DRONES; i++) {
        drones[i].x += sinf(current_time * 0.5f + i) * drones[i].speed * delta_time;
        drones[i].z += cosf(current_time * 0.5f + i) * drones[i].speed * delta_time;
        
        // Adjust height based on waves and position
        float wave_h = wave_height(drones[i].x, drones[i].z, current_time);
        drones[i].y = 8.0f + wave_h * 0.5f + sinf(current_time * 2.0f + i) * 1.0f;
        
        // Rotate drone based on movement
        drones[i].pitch = sinf(current_time * 3.0f + i) * 0.1f;
        drones[i].roll = cosf(current_time * 2.5f + i) * 0.05f;
        drones[i].yaw = current_time * 0.2f + i;
    }
}

// Print current state
void print_state() {
    printf("Time: %.2f\n", current_time);
    
    for (int i = 0; i < MAX_BOATS; i++) {
        printf("  Boat %d: pos=(%.2f, %.2f, %.2f), angle=%.2f\n", 
               i, boats[i].x, boats[i].y, boats[i].z, boats[i].angle);
    }
    
    for (int i = 0; i < MAX_DRONES; i++) {
        printf("  Drone %d: pos=(%.2f, %.2f, %.2f), rot=(%.2f, %.2f, %.2f)\n", 
               i, drones[i].x, drones[i].y, drones[i].z, drones[i].pitch, drones[i].yaw, drones[i].roll);
    }
    printf("\n");
}

int main() {
    init_simulation();
    
    // Run simulation for a number of steps
    float delta_time = 0.1f;  // 100ms per step
    
    for (int step = 0; step < SIMULATION_STEPS; step++) {
        update_simulation(delta_time);
        
        // Print state every 10 steps
        if (step % 10 == 0) {
            print_state();
        }
    }
    
    printf("Simulation completed.\n");
    return 0;
}