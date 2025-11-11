#ifndef PHYSICS_H
#define PHYSICS_H

#include <math.h>
#include <stdbool.h>

#define MAX_NODES 100
#define MAX_SEGMENTS 200
#define STIFFNESS 100.0f  // 1=rigid 1000=soft
#define GRAVITY 0.003f
#define SIMULATION_STEPS 20
#define DT (1.0f / SIMULATION_STEPS)

typedef struct {
    float x, y;
} Vector2;

typedef struct {
    Vector2 pos;      // Current position
    Vector2 prev_pos; // Previous position for verlet integration
    bool pinned;      // Whether the node is pinned (fixed in place)
} Node;

typedef struct {
    int node_a_idx;   // Index of first node
    int node_b_idx;   // Index of second node
    float rest_length; // Rest length of the segment
} Segment;

typedef struct {
    Node nodes[MAX_NODES];
    int node_count;
    
    Segment segments[MAX_SEGMENTS];
    int segment_count;
    
    bool simulation_enabled;
    bool edit_mode;
} PhysicsWorld;

// Function declarations
void init_physics_world(PhysicsWorld* world);
void add_node(PhysicsWorld* world, Vector2 position, bool pinned);
void remove_node(PhysicsWorld* world, int index);
void add_segment(PhysicsWorld* world, int node_a, int node_b);
void remove_segment(PhysicsWorld* world, int index);
void simulate_physics(PhysicsWorld* world);
float calculate_distance(Vector2 a, float ax, float ay, float bx, float by);
void update_node(PhysicsWorld* world, int index, Vector2 new_pos);

#endif