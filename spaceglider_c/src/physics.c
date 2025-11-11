#include "physics.h"
#include "spaceglider/types.h"
#include <math.h>
#include <stdio.h>

void initialize_physics() {
    printf("Physics system initialized\n");
}

// Calculate gravitational force based on position relative to planet
vec3 calculate_gravity(const vec3* position, const PlanetData* planet) {
    // Calculate distance from planet center
    float distance = sqrtf(position->x * position->x + 
                           position->y * position->y + 
                           position->z * position->z);
    
    // Avoid division by zero
    if (distance < 0.001f) {
        return (vec3){0.0f, 0.0f, 0.0f};
    }
    
    // Normalize position vector to get direction
    vec3 direction = {
        position->x / distance,
        position->y / distance,
        position->z / distance
    };
    
    // Calculate gravitational force magnitude: F = GM / r^2
    float force_magnitude = -planet->GM / (distance * distance);
    
    // Apply force in the direction toward the planet center
    return (vec3){
        direction.x * force_magnitude,
        direction.y * force_magnitude,
        direction.z * force_magnitude
    };
}

// Calculate aerodynamic forces (simplified)
vec3 calculate_aerodynamic_forces(const VehicleState* vehicle, const PlanetData* planet) {
    // This is a simplified aerodynamic force calculation
    // In the real Shadertoy code, this would be more complex
    
    // Calculate approximate air density based on altitude
    float altitude = sqrtf(vehicle->localr.x * vehicle->localr.x + 
                           vehicle->localr.y * vehicle->localr.y + 
                           vehicle->localr.z * vehicle->localr.z) - planet->radius;
    
    // Exponential model for air density: rho = rho0 * exp(-h/H) where H is scale height
    float H = 8000.0f;  // Scale height in meters (approximate)
    float rho = 1.225f * expf(-altitude / H);  // Sea level density * exponential decay
    
    // Store the calculated density in the vehicle state
    ((VehicleState*)vehicle)->rho = rho;
    
    // Calculate drag force: F_drag = 0.5 * rho * v^2 * Cd * A
    float speed = sqrtf(vehicle->localv.x * vehicle->localv.x +
                        vehicle->localv.y * vehicle->localv.y +
                        vehicle->localv.z * vehicle->localv.z);
    
    // Simplified drag coefficient (in reality would depend on angle of attack, etc)
    float Cd = 0.5f;  // Simplified drag coefficient
    
    // Calculate drag force magnitude
    float drag_magnitude = 0.5f * rho * speed * speed * Cd;
    
    // Drag acts opposite to velocity direction
    vec3 drag_force = {0.0f, 0.0f, 0.0f};
    if (speed > 0.001f) {
        drag_force.x = -vehicle->localv.x / speed * drag_magnitude;
        drag_force.y = -vehicle->localv.y / speed * drag_magnitude;
        drag_force.z = -vehicle->localv.z / speed * drag_magnitude;
    }
    
    return drag_force;
}

// Update orbital mechanics
void update_orbital_mechanics(VehicleState* vehicle, const PlanetData* planet, float delta_time) {
    // Update orbital position based on velocity
    vehicle->orbitr.x += vehicle->orbitv.x * delta_time;
    vehicle->orbitr.y += vehicle->orbitv.y * delta_time;
    vehicle->orbitr.z += vehicle->orbitv.z * delta_time;
    
    // planet parameter is used in more complex implementations,
    // but for now we just use the velocity directly
    (void)planet; // Suppress unused parameter warning
}

// Update physics for the game
void update_physics(GameState* game_state, VehicleState* vehicle_state, PlanetData* planet_data, float delta_time) {
    // Calculate forces acting on the vehicle
    vec3 gravity_force = calculate_gravity(&vehicle_state->localr, planet_data);
    vec3 aero_force = calculate_aerodynamic_forces(vehicle_state, planet_data);
    
    // Total force is sum of all forces
    vec3 total_force = {
        gravity_force.x + aero_force.x,
        gravity_force.y + aero_force.y,
        gravity_force.z + aero_force.z
    };
    
    // For this simulation, we'll assume a constant mass of 1 unit
    // In reality, you'd use actual vehicle mass
    float mass = 1.0f;
    
    // Calculate acceleration using F = ma, so a = F/m
    vec3 acceleration = {
        total_force.x / mass,
        total_force.y / mass,
        total_force.z / mass
    };
    
    // Update velocity based on acceleration
    vehicle_state->localv.x += acceleration.x * delta_time;
    vehicle_state->localv.y += acceleration.y * delta_time;
    vehicle_state->localv.z += acceleration.z * delta_time;
    
    // Update position based on velocity
    vehicle_state->localr.x += vehicle_state->localv.x * delta_time;
    vehicle_state->localr.y += vehicle_state->localv.y * delta_time;
    vehicle_state->localr.z += vehicle_state->localv.z * delta_time;
    
    // If the vehicle has thrust capability, we could apply thrust here:
    // thrust would depend on throttle setting and thrust vector
    if (vehicle_state->throttle > 0.0f) {
        // Apply thrust in the direction of the vehicle's forward vector (localB[0])
        float thrust_magnitude = vehicle_state->throttle * 100.0f;  // Scale factor for thrust
        vehicle_state->localv.x += vehicle_state->B[0].x * thrust_magnitude * delta_time;
        vehicle_state->localv.y += vehicle_state->B[0].y * thrust_magnitude * delta_time;
        vehicle_state->localv.z += vehicle_state->B[0].z * thrust_magnitude * delta_time;
    }
    
    // Update orbital mechanics if needed
    update_orbital_mechanics(vehicle_state, planet_data, delta_time);
    
    // Calculate derived values
    float speed = sqrtf(vehicle_state->localv.x * vehicle_state->localv.x +
                        vehicle_state->localv.y * vehicle_state->localv.y +
                        vehicle_state->localv.z * vehicle_state->localv.z);
    
    // Calculate Mach number (approximate, using standard sea level speed of sound)
    if (vehicle_state->rho > 0.0f) {
        float speed_of_sound = 343.0f; // Approximate speed of sound at sea level
        vehicle_state->mach = speed / speed_of_sound;
    } else {
        vehicle_state->mach = speed / 343.0f; // Use standard value when no atmosphere
    }
    
    // Calculate dynamic pressure: q = 0.5 * rho * v^2
    vehicle_state->dynpres = 0.5f * vehicle_state->rho * speed * speed;
    
    // Update game time
    game_state->time += delta_time;
}