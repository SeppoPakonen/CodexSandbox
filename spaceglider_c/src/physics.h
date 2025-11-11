#ifndef PHYSICS_H
#define PHYSICS_H

#include "spaceglider/types.h"

// Initialize physics system
void initialize_physics();

// Update physics
void update_physics(GameState* game_state, VehicleState* vehicle_state, PlanetData* planet_data, float delta_time);

// Helper functions for physics calculations
vec3 calculate_gravity(const vec3* position, const PlanetData* planet);
vec3 calculate_aerodynamic_forces(const VehicleState* vehicle, const PlanetData* planet);
void update_orbital_mechanics(VehicleState* vehicle, const PlanetData* planet, float delta_time);

#endif // PHYSICS_H