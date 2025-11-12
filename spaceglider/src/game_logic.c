#include "game_logic.h"
#include <string.h>
#include <math.h>

void initialize_game_state(GameState* state) {
    // Initialize with default values
    state->campos.x = 0.0f;
    state->campos.y = 0.0f;
    state->campos.z = 0.0f;
    
    // Initialize camera frame to identity matrix
    state->camframe.m[0][0] = 1.0f; state->camframe.m[0][1] = 0.0f; state->camframe.m[0][2] = 0.0f;
    state->camframe.m[1][0] = 0.0f; state->camframe.m[1][1] = 1.0f; state->camframe.m[1][2] = 0.0f;
    state->camframe.m[2][0] = 0.0f; state->camframe.m[2][1] = 0.0f; state->camframe.m[2][2] = 1.0f;
    
    state->camzoom = 1.0f;
    
    // Initialize waypoint to zero
    state->waypoint.x = 0.0f;
    state->waypoint.y = 0.0f;
    state->waypoint.z = 0.0f;
    
    // Initialize map marker to zero
    state->mapmarker.x = 0.0f;
    state->mapmarker.y = 0.0f;
    state->mapmarker.z = 0.0f;
    
    // Initialize exposure
    state->exposure.x = 1.0f;
    state->exposure.y = 1.0f;
    state->exposure.z = 1.0f;
    state->exposure.w = 0.0f;
    
    state->hudbright = 1.0f;
    state->switches = 0;
    state->stage = 0; // GS_RUNNING equivalent
    state->ipage = 0;
    state->hmdmode = 0; // VS_HMD_OFF equivalent
    state->aeromode = 0; // VS_AERO_OFF equivalent
    state->rcsmode = 0; // VS_RCS_OFF equivalent
    state->engmode = 0; // VS_ENG_OFF equivalent
    state->time = 0.0f;
}

void initialize_vehicle_state(VehicleState* state) {
    // Initialize position and velocity
    state->localr.x = 0.0f;
    state->localr.y = 0.0f;
    state->localr.z = 0.0f;
    
    state->localv.x = 0.0f;
    state->localv.y = 0.0f;
    state->localv.z = 0.0f;
    
    state->orbitr.x = 0.0f;
    state->orbitr.y = 0.0f;
    state->orbitr.z = 0.0f;
    
    state->orbitv.x = 0.0f;
    state->orbitv.y = 0.0f;
    state->orbitv.z = 0.0f;
    
    // Initialize orientation matrix to identity
    state->localB.m[0][0] = 1.0f; state->localB.m[0][1] = 0.0f; state->localB.m[0][2] = 0.0f;
    state->localB.m[1][0] = 0.0f; state->localB.m[1][1] = 1.0f; state->localB.m[1][2] = 0.0f;
    state->localB.m[2][0] = 0.0f; state->localB.m[2][1] = 0.0f; state->localB.m[2][2] = 1.0f;
    
    // Initialize basis vectors
    state->B[0].x = 1.0f; state->B[0].y = 0.0f; state->B[0].z = 0.0f; // Forward
    state->B[1].x = 0.0f; state->B[1].y = 1.0f; state->B[1].z = 0.0f; // Right
    state->B[2].x = 0.0f; state->B[2].y = 0.0f; state->B[2].z = 1.0f; // Up
    
    state->throttle = 0.0f;
    state->tvec = 0.0f; // Thrust vector angle
    
    // Initialize atmospheric properties
    state->rho = 1.225f; // Standard sea level air density
    state->mach = 0.0f;
    state->dynpres = 0.0f; // Dynamic pressure
    
    // Initialize modes
    state->modes.x = 0.0f; // HMD mode (0 = off, 1 = surface, 2 = orbit)
    state->modes.y = 0.0f; // Aero mode
    state->modes.z = 0.0f; // RCS mode
    state->modes.w = 0.0f; // Engine mode
}

void initialize_planet_data(PlanetData* data) {
    // Initialize with Earth-like values
    data->radius = 6371000.0f; // Earth radius in meters
    data->GM = 3.986004418e14f; // Earth's gravitational parameter (m³/s²)
    data->omega = 7.2921159e-5f; // Earth's rotation rate (rad/s) 
}

void update_game_logic(GameState* game_state, VehicleState* vehicle_state, PlanetData* planet_data, float delta_time) {
    // Update game time
    game_state->time += delta_time;
    
    // Update any game logic based on physics state
    // This would include updating HUD elements, map data, etc.
    
    // Example: Calculate dynamic pressure if in atmosphere
    if (vehicle_state->rho > 0.0f) {
        float speed = sqrtf(vehicle_state->localv.x * vehicle_state->localv.x + 
                           vehicle_state->localv.y * vehicle_state->localv.y + 
                           vehicle_state->localv.z * vehicle_state->localv.z);
        vehicle_state->dynpres = 0.5f * vehicle_state->rho * speed * speed;
        vehicle_state->mach = speed / 343.0f; // Simplified: assume speed of sound is 343 m/s
    }
}