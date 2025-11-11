#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "../Keen.h"
#include "ActionManager.h"
#include <memory>
#include <string>

// Game difficulty levels
enum class Difficulty {
    NotPlaying = 0,
    Easy = 1,
    Normal = 2,
    Hard = 3
};

// Level states
enum class LevelState {
    Playing = 0,          // In Level
    Died = 1,             // Keen Died
    LevelComplete = 2,    // Level Completed
    CouncilRescued = 3,   // Rescued Council Member (Keen 4)
    AboutToRecordDemo = 4, // Warped to a new level
    ResetGame = 5,        // Started a new game
    LoadedGame = 6,       // Loaded a saved game
    Foot = 7,             // Keen exited level by touching foot (keen 4)
    AbortGame = 8,        // Game failed to load
    Sandwich = 9,         // Keen exited level by getting items (Keen 6)
    Rope = 10,
    Passcard = 11,
    Molly = 12,           // Keen rescues Molly (Keen 6)
    TeleportToKorath = 13, // Keen teleported to Korath III Base (Keen 5)
    KorathFuse = 14,      // Keen broke the Korath III fuse (Keen 5)
    DestroyedQED = 15     // Destroyed QED (Keen 5)
};

// Game state structure - holds player stats and game progress
struct GameState {
    uint16_t mapPosX;
    uint16_t mapPosY;
    int16_t levelsDone[25]; // Number of levels complete
    int32_t keenScore;      // Keen's score
    int32_t nextKeenAt;     // Score keen will get a new life at
    int16_t numShots;
    int16_t numCentilife;

    // Episode-specific variables
    union {
        struct {
            int16_t wetsuit;
            int16_t membersRescued;
        } ck4;

        struct {
            int16_t securityCard;
            int16_t word_4729C;
            int16_t fusesRemaining;
        } ck5;

        struct {
            int16_t sandwich;
            int16_t rope; // 1 == collected, 2 == deployed on cliff
            int16_t passcard;
            int16_t inRocket; // true if flying
        } ck6;
    } ep;

    int16_t keyGems[4];
    int16_t currentLevel;
    int16_t numLives;       // Number of lives keen has
    Difficulty difficulty;  // Difficulty level of current game
    LevelState levelState;  // Current level state
    bool jumpCheat;         // Is the jump cheat enabled?

    GameState() : mapPosX(0), mapPosY(0), keenScore(0), nextKeenAt(20000),
                  numShots(5), numCentilife(0), currentLevel(0), 
                  numLives(3), difficulty(Difficulty::Normal), 
                  levelState(LevelState::Playing), jumpCheat(false) {
        for (int i = 0; i < 25; i++) levelsDone[i] = 0;
        for (int i = 0; i < 4; i++) keyGems[i] = 0;
    }
};

// Game Manager class - controls game state, save/load, and main game loop
class GameManager {
private:
    static bool initialized;
    static GameState gameState;
    static bool isRunning;
    
public:
    static bool Initialize();
    static void Shutdown();
    
    // Game state management
    static void NewGame();
    static void ResetGame();
    static void GameOver();
    
    // Save and load game functions
    static bool SaveGame(const std::string& filename);
    static bool LoadGame(const std::string& filename, bool fromMenu = false);
    
    // Game loop functions
    static void GameLoop();
    static void LoadLevel(bool doCache = true, bool silent = false);
    static bool TryAgainMenu();
    
    // Access to game state
    static GameState& GetGameState() { return gameState; }
    static const GameState& GetConstGameState() { return gameState; }
    
    // Game control
    static void SetRunning(bool running) { isRunning = running; }
    static bool IsRunning() { return isRunning; }
    
    static bool IsInitialized() { return initialized; }
};

#endif // GAME_MANAGER_H