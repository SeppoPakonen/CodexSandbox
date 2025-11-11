#ifndef INTERACTION_MANAGER_H
#define INTERACTION_MANAGER_H

#include "../Keen.h"
#include "GameManager.h"

// Interaction Manager - handles UI screens, menus, and various game interactions
class InteractionManager {
private:
    static bool initialized;
    
public:
    static bool Initialize();
    static void Shutdown();
    
    // Handle demo keys (F1, etc.)
    static void HandleDemoKeys();
    
    // Draw and handle terminator screen
    static void DrawTerminator();
    
    // Draw and handle star wars text
    static void DrawStarWars();
    
    // Show title screen
    static void ShowTitleScreen();
    
    // Play demo file
    static void PlayDemoFile(const std::string& demoName);
    
    // Overlay high scores on screen
    static void OverlayHighScores();
    
    // Submit a high score
    static void SubmitHighScore(int score, uint16_t arg4);
    
    // Show high scores menu
    static void DoHighScores();
    
    // Check if in high scores mode
    static bool IsInHighScores() { return inHighScores; }
    static void SetInHighScores(bool inHS) { inHighScores = inHS; }
    
    // Starting saved game flag
    static int GetStartingSavedGame() { return startingSavedGame; }
    static void SetStartingSavedGame(int value) { startingSavedGame = value; }
    
    static bool IsInitialized() { return initialized; }
    
private:
    static bool inHighScores;
    static int startingSavedGame;
};

#endif // INTERACTION_MANAGER_H