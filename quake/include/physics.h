#ifndef PHYSICS_H
#define PHYSICS_H

#include <math.h>
#include <stdbool.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Player dimensions (from the GLSL code)
#define PLAYER_WIDTH 16.0f
#define PLAYER_HEIGHT 48.0f

// Player physics constants
#define MAX_SPEED 320.0f
#define ACCELERATION 10.0f
#define FRICTION 6.0f
#define GRAVITY 800.0f
#define JUMP_VELOCITY 270.0f

// Player structure
typedef struct {
    float position[3];  // x, y, z coordinates
    float velocity[3];  // x, y, z velocity
    float angles[3];    // pitch, yaw, roll angles
    float on_ground;    // boolean: is player on ground
    float step_up;      // height player can step up
} Player;

// Initialize player
void init_player(Player *player);

// Apply movement to player based on input
void move_player(Player *player, float forward_move, float side_move, bool jump, float dt);

// Apply gravity to player
void apply_gravity(Player *player, float dt);

// Handle collision detection and response
void handle_collision(Player *player, float dt);

// Apply friction when player is on ground
void apply_friction(Player *player, float dt);

// Update player position based on velocity and time
void update_player_position(Player *player, float dt);

// Check if player is colliding with world geometry 
bool check_collision(Player *player);

#endif // PHYSICS_H