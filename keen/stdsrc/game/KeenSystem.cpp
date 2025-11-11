#include "KeenSystem.h"
#include <iostream>

// Static member definitions
bool KeenSystem::initialized = false;
KeenState KeenSystem::keenState;

bool KeenSystem::Initialize() {
    if (initialized) return false;
    
    initialized = true;
    // Initialize keen state
    keenState = KeenState();
    
    return true;
}

void KeenSystem::Shutdown() {
    if (!initialized) return;
    
    initialized = false;
}

void KeenSystem::SpawnKeen(int tileX, int tileY, int direction) {
    if (!initialized) return;
    
    std::cout << "Spawning Keen at tile (" << tileX << ", " << tileY 
              << ") facing direction: " << direction << std::endl;
    
    // In a real implementation, this would:
    // 1. Create the Keen object in the object system
    // 2. Set up his position, direction, and initial action
    // 3. Initialize his state variables
}

void KeenSystem::HandleInput() {
    if (!initialized) return;
    
    // In a real implementation, this would:
    // 1. Get input from the input system
    // 2. Update the keenState based on input
    // For now, we'll simulate some input handling
    
    std::cout << "Handling Keen input..." << std::endl;
}

void KeenSystem::UpdateKeen() {
    if (!initialized) return;
    
    // In a real implementation, this would:
    // 1. Process Keen's current action
    // 2. Handle movement, jumping, shooting, etc.
    // 3. Update his physics and animation
    
    std::cout << "Updating Keen..." << std::endl;
}

void KeenSystem::HandleCollision(void* obj1, void* obj2) {
    if (!initialized) return;
    
    // In a real implementation, this would handle collisions between objects
    // including items, enemies, platforms, etc.
    
    std::cout << "Handling collision between objects" << std::endl;
}

void KeenSystem::CheckSpecialTileInfo() {
    if (!initialized) return;
    
    // In a real implementation, this would check for special tiles 
    // like doors, switches, gems, etc. that Keen interacts with
    
    std::cout << "Checking special tile info..." << std::endl;
}

void KeenSystem::IncreaseScore(int score) {
    if (!initialized) return;
    
    // Update the game state with the new score
    GameManager::GetGameState().keenScore += score;
    
    // Check for extra life thresholds
    if (GameManager::GetGameState().keenScore >= GameManager::GetGameState().nextKeenAt) {
        GameManager::GetGameState().numLives++;
        GameManager::GetGameState().nextKeenAt *= 2;
        std::cout << "Extra life earned!" << std::endl;
    }
    
    std::cout << "Score increased by: " << score 
              << ", total score: " << GameManager::GetGameState().keenScore << std::endl;
}

void KeenSystem::KillKeen() {
    if (!initialized) return;
    
    // In a real implementation, this would handle Keen's death
    // including state changes, sound effects, etc.
    
    GameManager::GetGameState().levelState = LevelState::Died;
    std::cout << "Keen has been killed!" << std::endl;
}

void KeenSystem::RidePlatform(void* platformObj) {
    if (!initialized) return;
    
    // In a real implementation, this would handle Keen riding
    // a moving platform
    
    std::cout << "Keen is riding a platform" << std::endl;
    keenState.platform = platformObj;
}