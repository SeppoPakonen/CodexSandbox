#ifndef RENDERER_OLD_H
#define RENDERER_OLD_H

#include "spaceglider/types.h"

// Initialize old school renderer (without shaders)
void initialize_old_renderer();

// Render using old school methods (fixed pipeline)
void render_with_old_renderer(const GameState* game_state, const VehicleState* vehicle_state, const PlanetData* planet_data);

// Clean up old school renderer
void cleanup_old_renderer();

#endif // RENDERER_OLD_H