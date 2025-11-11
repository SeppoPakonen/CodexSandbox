#ifndef KEEN_SYSTEM_H
#define KEEN_SYSTEM_H

#include "../Keen.h"
#include "GameManager.h"
#include "ActionManager.h"

// Structure to represent Keen's state
struct KeenState {
    int jumpTimer;
    int poleGrabTime;
    bool jumpIsPressed;
    bool jumpWasPressed;
    bool pogoIsPressed;
    bool pogoWasPressed;
    bool shootIsPressed;
    bool shootWasPressed;
    bool keenSliding;
    // We'll represent the platform as a general object pointer for now
    void* platform; // This would reference an Object in a complete implementation

    KeenState() : jumpTimer(0), poleGrabTime(0), 
                  jumpIsPressed(false), jumpWasPressed(false),
                  pogoIsPressed(false), pogoWasPressed(false),
                  shootIsPressed(false), shootWasPressed(false),
                  keenSliding(false), platform(nullptr) {}
};

// Keen System - handles the player character logic, input, and state
class KeenSystem {
private:
    static bool initialized;
    static KeenState keenState;
    
public:
    static bool Initialize();
    static void Shutdown();
    
    // Initialize the player character at a tile position
    static void SpawnKeen(int tileX, int tileY, int direction);
    
    // Handle player input and movement
    static void HandleInput();
    
    // Process Keen's state and actions
    static void UpdateKeen();
    
    // Handle collisions with other objects
    static void HandleCollision(void* obj1, void* obj2); // Simplified for now
    
    // Handle special tile interactions
    static void CheckSpecialTileInfo();
    
    // Increase the player's score
    static void IncreaseScore(int score);
    
    // Kill the player character
    static void KillKeen();
    
    // Handle Keen riding a platform
    static void RidePlatform(void* platformObj);
    
    // Get Keen's state
    static KeenState& GetKeenState() { return keenState; }
    static const KeenState& GetConstKeenState() { return keenState; }
    
    static bool IsInitialized() { return initialized; }
};

#endif // KEEN_SYSTEM_H