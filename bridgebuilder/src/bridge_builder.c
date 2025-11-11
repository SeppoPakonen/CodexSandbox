#include "bridge_builder.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

BridgeBuilder* init_bridge_builder(PhysicsWorld* world, InputState* input) {
    BridgeBuilder* builder = malloc(sizeof(BridgeBuilder));
    if (!builder) return NULL;
    
    builder->world = world;
    builder->input = input;
    builder->building_vertex = false;
    builder->building_segment = false;
    builder->selected_node = -1;
    builder->temp_node_index = -1;
    builder->last_action_frame = 0;
    
    return builder;
}

int find_closest_node(PhysicsWorld* world, Vector2 pos, float threshold) {
    int closest_idx = -1;
    float closest_dist = threshold;
    
    for (int i = 0; i < world->node_count; i++) {
        float dx = world->nodes[i].pos.x - pos.x;
        float dy = world->nodes[i].pos.y - pos.y;
        float dist = sqrtf(dx * dx + dy * dy);
        
        if (dist < closest_dist) {
            closest_dist = dist;
            closest_idx = i;
        }
    }
    
    return closest_idx;
}

int find_closest_segment(PhysicsWorld* world, Vector2 pos, float threshold) {
    int closest_idx = 0;
    
    for (int j = 0; j < world->segment_count; j++) {
        Vector2 a = world->nodes[world->segments[j].node_a_idx].pos;
        Vector2 b = world->nodes[world->segments[j].node_b_idx].pos;
        
        float dist = point_to_line_distance(pos, a, b);
        if (dist < threshold) {
            closest_idx = j + 1;  // 1-indexed as in GLSL
            break;
        }
    }
    
    return closest_idx;
}

bool is_valid_segment(PhysicsWorld* world, int node_a, int node_b, float max_length) {
    if (node_a < 0 || node_a >= world->node_count || 
        node_b < 0 || node_b >= world->node_count || 
        node_a == node_b) {
        return false;
    }
    
    Vector2 pos_a = world->nodes[node_a].pos;
    Vector2 pos_b = world->nodes[node_b].pos;
    float dist = sqrtf((pos_a.x - pos_b.x) * (pos_a.x - pos_b.x) + 
                       (pos_a.y - pos_b.y) * (pos_a.y - pos_b.y));
    
    return dist < max_length;
}

void handle_edit_mode(BridgeBuilder* builder) {
    PhysicsWorld* world = builder->world;
    InputState* input = builder->input;
    
    if (input->keys_just_pressed[RESET_KEY]) {
        // Reset the world to initial state
        init_physics_world(world);
        builder->last_action_frame = input->frame_count;
        return;
    }
    
    Vector2 mouse_pos = input->mouse_pos;
    float node_threshold = 0.03f;
    float segment_threshold = 0.02f;
    
    // Find closest node and segment to mouse
    int closest_node_idx = find_closest_node(world, mouse_pos, node_threshold);
    int closest_segment_idx = find_closest_segment(world, mouse_pos, segment_threshold);
    
    // Find potential valid segment
    int potential_node_a = -1, potential_node_b = -1;
    float potential_dist = 0.3f;  // Maximum distance for potential segments
    
    if (closest_node_idx == -1) {
        // Look for potential segments among all node pairs
        for (int i = 0; i < world->node_count; i++) {
            for (int j = i + 1; j < world->node_count; j++) {
                float dist = point_to_line_distance(mouse_pos, world->nodes[i].pos, world->nodes[j].pos);
                float length = sqrtf(powf(world->nodes[i].pos.x - world->nodes[j].pos.x, 2) +
                                     powf(world->nodes[i].pos.y - world->nodes[j].pos.y, 2));
                
                if (dist < segment_threshold && length < potential_dist) {
                    potential_node_a = i;
                    potential_node_b = j;
                    potential_dist = length;
                }
            }
        }
    }
    
    // Handle node dragging/creation/removal
    if (input->vertex_mode && input->mouse_clicked && 
        (input->frame_count - builder->last_action_frame) > 30) {  // WAIT equivalent
        
        if (closest_node_idx == -1 && potential_node_a == -1) {
            // Add new node
            add_node(world, mouse_pos, false);
            builder->last_action_frame = input->frame_count;
        }
        else if (closest_node_idx != -1 && potential_node_a == -1) {
            // Check if node is connected to any segments
            bool connected = false;
            for (int j = 0; j < world->segment_count; j++) {
                if (world->segments[j].node_a_idx == closest_node_idx || 
                    world->segments[j].node_b_idx == closest_node_idx) {
                    connected = true;
                    break;
                }
            }
            
            // Remove unconnected node
            if (!connected) {
                remove_node(world, closest_node_idx);
                builder->last_action_frame = input->frame_count;
            }
        }
    }
    else if (input->pin_mode && input->mouse_clicked && 
             (input->frame_count - builder->last_action_frame) > 30) {  // WAIT equivalent
        if (closest_node_idx != -1) {
            // Toggle pin state
            world->nodes[closest_node_idx].pinned = !world->nodes[closest_node_idx].pinned;
            builder->last_action_frame = input->frame_count;
        }
    }
    else if (input->edge_mode && input->mouse_clicked && 
             (input->frame_count - builder->last_action_frame) > 30) {  // WAIT equivalent
        if (closest_segment_idx > 0) {
            // Remove segment
            remove_segment(world, closest_segment_idx - 1);  // Convert from 1-indexed to 0-indexed
            builder->last_action_frame = input->frame_count;
        }
        else if (potential_node_a != -1 && potential_node_b != -1) {
            // Add segment between potential nodes
            add_segment(world, potential_node_a, potential_node_b);
            builder->last_action_frame = input->frame_count;
        }
    }
    
    // If no special mode active and mouse is clicked near a node, drag it
    if (!input->vertex_mode && !input->edge_mode && !input->pin_mode) {
        if (input->mouse_down && closest_node_idx != -1) {
            update_node(world, closest_node_idx, mouse_pos);
        }
    }
}

void handle_simulation(BridgeBuilder* builder) {
    // In simulation mode, physics is handled by the physics simulation
    // No direct manipulation in this mode
    // This function can be expanded later if needed for special simulation behaviors
    (void)builder; // Suppress unused parameter warning
}

void update_bridge_builder(BridgeBuilder* builder) {
    if (builder->input->edit_mode) {
        handle_edit_mode(builder);
    } else {
        // Simulation mode is handled by physics simulation
    }
}