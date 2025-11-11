#include "physics.h"
#include <math.h>

void initialize_physics() {
    // Initialize physics constants and parameters
    // For now, we just need to ensure our constants are properly set
}

vec3 calculate_gravity(const vec3* position, const PlanetData* planet) {
    // Calculate distance from planet center
    float distance_sq = position->x * position->x + position->y * position->y + position->z * position->z;
    float distance = sqrtf(distance_sq);
    
    if (distance < 1.0f) {  // Avoid division by zero
        return (vec3){0.0f, 0.0f, 0.0f};
    }
    
    // Calculate gravitational force magnitude
    float gravity_magnitude = planet->GM / distance_sq;
    
    // Direction is towards planet center
    vec3 gravity = {
        -gravity_magnitude * (position->x / distance),
        -gravity_magnitude * (position->y / distance),
        -gravity_magnitude * (position->z / distance)
    };
    
    return gravity;
}

vec3 calculate_aerodynamic_forces(const VehicleState* vehicle, const PlanetData* planet) {
    // Simplified aerodynamic calculations
    // In real implementation, this would include lift/drag coefficients based on angle of attack, etc.
    
    vec3 force = {0.0f, 0.0f, 0.0f};
    
    // Only calculate if in atmosphere (simplified check based on altitude)
    float altitude = sqrtf(vehicle->localr.x * vehicle->localr.x + 
                          vehicle->localr.y * vehicle->localr.y + 
                          vehicle->localr.z * vehicle->localr.z) - planet->radius;
    
    if (altitude < 100000.0f) { // Assume atmosphere up to 100km
        // Calculate velocity magnitude
        float speed = sqrtf(vehicle->localv.x * vehicle->localv.x + 
                           vehicle->localv.y * vehicle->localv.y + 
                           vehicle->localv.z * vehicle->localv.z);
        
        // Simplified drag calculation: F_drag = 0.5 * rho * v^2 * Cd * A
        // In this simplified version, we use a basic drag proportional to velocity squared
        float drag_coefficient = 0.1f; // Simplified drag coefficient
        float reference_area = 50.0f;  // Reference area of spacecraft
        
        float drag_magnitude = 0.5f * vehicle->rho * speed * speed * drag_coefficient * reference_area;
        
        // Drag acts opposite to velocity direction
        if (speed > 0.001f) { // Avoid division by zero
            force.x = -drag_magnitude * (vehicle->localv.x / speed);
            force.y = -drag_magnitude * (vehicle->localv.y / speed);
            force.z = -drag_magnitude * (vehicle->localv.z / speed);
        }
    }
    
    return force;
}

void update_orbital_mechanics(VehicleState* vehicle, const PlanetData* planet, float delta_time) {
    // Calculate total force (gravity + aerodynamic + engine thrust)
    vec3 gravity = calculate_gravity(&vehicle->orbitr, planet);
    vec3 aero_forces = calculate_aerodynamic_forces(vehicle, planet);
    
    // Simplified engine thrust (based on throttle setting)
    vec3 engine_thrust = {
        vehicle->throttle * 100000.0f * vehicle->B[0].x, // Forward thrust based on throttle
        vehicle->throttle * 100000.0f * vehicle->B[0].y,
        vehicle->throttle * 100000.0f * vehicle->B[0].z
    };
    
    // Total force = gravity + aero forces + engine thrust
    vec3 total_force = {
        gravity.x + aero_forces.x + engine_thrust.x,
        gravity.y + aero_forces.y + engine_thrust.y,
        gravity.z + aero_forces.z + engine_thrust.z
    };
    
    // Calculate acceleration (F = ma, so a = F/m)
    // Use a simplified mass of 10000 kg for the spacecraft
    float mass = 10000.0f; 
    vec3 acceleration = {
        total_force.x / mass,
        total_force.y / mass,
        total_force.z / mass
    };
    
    // Update velocity
    vehicle->orbitv.x += acceleration.x * delta_time;
    vehicle->orbitv.y += acceleration.y * delta_time;
    vehicle->orbitv.z += acceleration.z * delta_time;
    
    // Update position
    vehicle->orbitr.x += vehicle->orbitv.x * delta_time;
    vehicle->orbitr.y += vehicle->orbitv.y * delta_time;
    vehicle->orbitr.z += vehicle->orbitv.z * delta_time;
    
    // Also update local reference frame (simplified)
    vehicle->localv = vehicle->orbitv; // In this simplified model, local and orbital velocity are the same
    vehicle->localr = vehicle->orbitr; // And local and orbital position are the same
}

void update_physics(GameState* game_state, VehicleState* vehicle_state, PlanetData* planet_data, float delta_time) {
    // Update orbital mechanics
    update_orbital_mechanics(vehicle_state, planet_data, delta_time);
    
    // Update atmospheric properties based on altitude
    float altitude = sqrtf(vehicle_state->localr.x * vehicle_state->localr.x + 
                          vehicle_state->localr.y * vehicle_state->localr.y + 
                          vehicle_state->localr.z * vehicle_state->localr.z) - planet_data->radius;
    
    // Simplified atmospheric model: exponential decrease of density with altitude
    // Standard scale height is approximately 8400m
    float scale_height = 8400.0f;
    if (altitude > -1000.0f) { // Don't go below ground
        vehicle_state->rho = 1.225f * expf(-altitude / scale_height); // Air density decreases exponentially
    } else {
        vehicle_state->rho = 1.225f; // Ground level density
    }
    
    // Update any other physics-based values
}