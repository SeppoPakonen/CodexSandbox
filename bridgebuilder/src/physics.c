#include "physics.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

void init_physics_world(PhysicsWorld* world) {
    memset(world, 0, sizeof(PhysicsWorld));
    world->simulation_enabled = false;
    world->edit_mode = true;
    
    // Initialize with default bridge structure (similar to GLSL initialization)
    // Adding nodes similar to the GLSL initialization
    add_node(world, (Vector2){-0.5f, 0.0f}, true);   // Fixed node
    add_node(world, (Vector2){-0.3f, 0.0f}, false);
    add_node(world, (Vector2){-0.1f, 0.0f}, false);
    add_node(world, (Vector2){0.1f, 0.0f}, false);
    add_node(world, (Vector2){0.3f, 0.0f}, false);
    add_node(world, (Vector2){0.5f, 0.0f}, true);    // Fixed node
    
    // Add upper nodes
    add_node(world, (Vector2){-0.4f, 0.2f}, false);
    add_node(world, (Vector2){-0.2f, 0.2f}, false);
    add_node(world, (Vector2){0.0f, 0.2f}, false);
    add_node(world, (Vector2){0.2f, 0.2f}, false);
    add_node(world, (Vector2){0.4f, 0.2f}, false);
    
    // Add bottom support node
    add_node(world, (Vector2){0.0f, -0.3f}, true);
    
    // Add segments (similar to GLSL initialization)
    // Horizontal segments at bottom
    add_segment(world, 1, 2);  // Connecting nodes 1-2
    add_segment(world, 2, 3);  // Connecting nodes 2-3
    add_segment(world, 3, 4);  // Connecting nodes 3-4
    add_segment(world, 4, 5);  // Connecting nodes 4-5
    add_segment(world, 5, 6);  // Connecting nodes 5-6
    
    // Diagonal support segments
    add_segment(world, 7, 1);  // upper left to lower left
    add_segment(world, 8, 2);  // upper to lower
    add_segment(world, 10, 4); // upper to lower
    add_segment(world, 11, 5); // upper right to lower right
    
    // Additional diagonal supports
    add_segment(world, 7, 2);
    add_segment(world, 8, 3);
    add_segment(world, 9, 4);
    add_segment(world, 10, 5);
    add_segment(world, 11, 6);
    
    // Horizontal segments at top
    add_segment(world, 7, 8);
    add_segment(world, 8, 9);
    add_segment(world, 9, 10);
    add_segment(world, 10, 11);
}

void add_node(PhysicsWorld* world, Vector2 position, bool pinned) {
    if (world->node_count < MAX_NODES) {
        Node* node = &world->nodes[world->node_count];
        node->pos = position;
        node->prev_pos = position; // Initialize with same position
        node->pinned = pinned;
        world->node_count++;
    }
}

void remove_node(PhysicsWorld* world, int index) {
    if (index < 0 || index >= world->node_count) return;
    
    // Remove any segments connected to this node first
    for (int i = world->segment_count - 1; i >= 0; i--) {
        if (world->segments[i].node_a_idx == index || world->segments[i].node_b_idx == index) {
            // Shift remaining segments down
            for (int j = i; j < world->segment_count - 1; j++) {
                world->segments[j] = world->segments[j + 1];
            }
            world->segment_count--;
        }
    }
    
    // Shift remaining nodes down
    for (int i = index; i < world->node_count - 1; i++) {
        world->nodes[i] = world->nodes[i + 1];
    }
    world->node_count--;
    
    // Update segment indices that may have been affected
    for (int i = 0; i < world->segment_count; i++) {
        if (world->segments[i].node_a_idx > index) {
            world->segments[i].node_a_idx--;
        }
        if (world->segments[i].node_b_idx > index) {
            world->segments[i].node_b_idx--;
        }
    }
}

void add_segment(PhysicsWorld* world, int node_a, int node_b) {
    if (node_a < 0 || node_a >= world->node_count || 
        node_b < 0 || node_b >= world->node_count || 
        node_a == node_b || 
        world->segment_count >= MAX_SEGMENTS) {
        return;
    }
    
    // Check if segment already exists
    for (int i = 0; i < world->segment_count; i++) {
        if ((world->segments[i].node_a_idx == node_a && world->segments[i].node_b_idx == node_b) ||
            (world->segments[i].node_a_idx == node_b && world->segments[i].node_b_idx == node_a)) {
            return; // Segment already exists
        }
    }
    
    Segment* seg = &world->segments[world->segment_count];
    seg->node_a_idx = node_a;
    seg->node_b_idx = node_b;
    
    // Calculate rest length based on initial positions
    Vector2 pos_a = world->nodes[node_a].pos;
    Vector2 pos_b = world->nodes[node_b].pos;
    seg->rest_length = sqrtf((pos_a.x - pos_b.x) * (pos_a.x - pos_b.x) + 
                             (pos_a.y - pos_b.y) * (pos_a.y - pos_b.y));
    
    world->segment_count++;
}

void remove_segment(PhysicsWorld* world, int index) {
    if (index < 0 || index >= world->segment_count) return;
    
    // Shift remaining segments down
    for (int i = index; i < world->segment_count - 1; i++) {
        world->segments[i] = world->segments[i + 1];
    }
    world->segment_count--;
}

void update_node(PhysicsWorld* world, int index, Vector2 new_pos) {
    if (index >= 0 && index < world->node_count) {
        world->nodes[index].pos = new_pos;
    }
}

float calculate_distance(Vector2 a, float ax, float ay, float bx, float by) {
    float pa_x = a.x - ax;
    float pa_y = a.y - ay;
    float ba_x = bx - ax;
    float ba_y = by - ay;
    
    float h = 0.0f;
    float ba_dot = ba_x * ba_x + ba_y * ba_y;
    if (ba_dot != 0.0f) {
        float h_val = (pa_x * ba_x + pa_y * ba_y) / ba_dot;
        h = (h_val < 0.0f) ? 0.0f : ((h_val > 1.0f) ? 1.0f : h_val);
    }
    
    float dx = pa_x - ba_x * h;
    float dy = pa_y - ba_y * h;
    return sqrtf(dx * dx + dy * dy);
}

void simulate_physics(PhysicsWorld* world) {
    if (!world->simulation_enabled) return;
    
    // Perform substeps for stability
    for (int s = 0; s < SIMULATION_STEPS; s++) {
        // Verlet integration + gravity
        for (int i = 0; i < world->node_count; i++) {
            Node* node = &world->nodes[i];
            if (!node->pinned) {
                Vector2 pos = node->pos;
                Vector2 prev_pos = node->prev_pos;
                
                // Calculate new position with gravity
                Vector2 new_pos;
                new_pos.x = 2.0f * pos.x - prev_pos.x;  // Verlet: x_new = 2*x_current - x_prev
                new_pos.y = 2.0f * pos.y - prev_pos.y - 0.5f * DT * DT * GRAVITY;  // Include gravity
                
                // Store current position as previous
                node->prev_pos = pos;
                node->pos = new_pos;
            }
        }
        
        // Apply constraints (segments)
        for (int k = 0; k < world->segment_count; k++) {
            Segment* seg = &world->segments[k];
            int i = seg->node_a_idx;
            int j = seg->node_b_idx;
            
            Vector2 pos_i = world->nodes[i].pos;
            Vector2 pos_j = world->nodes[j].pos;
            
            float current_distance = sqrtf((pos_i.x - pos_j.x) * (pos_i.x - pos_j.x) + 
                                          (pos_i.y - pos_j.y) * (pos_i.y - pos_j.y));
            
            if (current_distance > 0.0f) {
                float diff = (seg->rest_length - current_distance) / current_distance;
                Vector2 offset;
                offset.x = (pos_i.x - pos_j.x) * diff * 0.5f / STIFFNESS;
                offset.y = (pos_i.y - pos_j.y) * diff * 0.5f / STIFFNESS;
                
                // Apply corrections, but only if nodes aren't pinned
                if (!world->nodes[i].pinned) {
                    world->nodes[i].pos.x -= offset.x;
                    world->nodes[i].pos.y -= offset.y;
                }
                
                if (!world->nodes[j].pinned) {
                    world->nodes[j].pos.x += offset.x;
                    world->nodes[j].pos.y += offset.y;
                }
            }
        }
    }
}