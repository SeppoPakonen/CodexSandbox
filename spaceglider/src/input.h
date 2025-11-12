#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>
#include "spaceglider/types.h"

// Initialize input system
void initialize_input(GLFWwindow* window);

// Process input
void process_input(GameState* game_state, VehicleState* vehicle_state);

#endif // INPUT_H