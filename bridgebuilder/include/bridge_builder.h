#ifndef BRIDGE_BUILDER_H
#define BRIDGE_BUILDER_H

#include "physics.h"
#include "input.h"

typedef struct {
    PhysicsWorld* world;
    InputState* input;
    
    // Bridge building state
    bool building_vertex;   // Currently building vertex
    bool building_segment;  // Currently building segment
    int selected_node;      // Index of selected node (-1 if none)
    int temp_node_index;    // Index of temporary node during building
    
    // Editor state
    int last_action_frame;  // Frame when last action occurred
} BridgeBuilder;

// Initialize bridge builder
BridgeBuilder* init_bridge_builder(PhysicsWorld* world, InputState* input);

// Update bridge builder state
void update_bridge_builder(BridgeBuilder* builder);

// Handle bridge building in edit mode
void handle_edit_mode(BridgeBuilder* builder);

// Handle simulation
void handle_simulation(BridgeBuilder* builder);

// Find closest node to a position
int find_closest_node(PhysicsWorld* world, Vector2 pos, float threshold);

// Find closest segment to a position
int find_closest_segment(PhysicsWorld* world, Vector2 pos, float threshold);

// Check if a potential segment is valid (length within threshold)
bool is_valid_segment(PhysicsWorld* world, int node_a, int node_b, float max_length);

#endif