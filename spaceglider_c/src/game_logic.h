#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "spaceglider/types.h"

// Initialize game state
void initialize_game_state(GameState* state);

// Initialize vehicle state
void initialize_vehicle_state(VehicleState* state);

// Initialize planet data
void initialize_planet_data(PlanetData* data);

// Update game logic
void update_game_logic(GameState* game_state, VehicleState* vehicle_state, PlanetData* planet_data, float delta_time);

#endif // GAME_LOGIC_H