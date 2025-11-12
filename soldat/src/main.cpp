#include <iostream>
#include <memory>
#include <SDL2/SDL.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

// Include Soldat headers
#include "Constants.h"
#include "Util.h"
#include "Vector.h"
#include "Calc.h"
#include "Console.h"
#include "Cvar.h"
#include "Command.h"
#include "LogFile.h"
#include "Version.h"
#include "SharedConfig.h"
#include "Demo.h"
#include "AI.h"
#include "Anims.h"
#include "AutoUpdater.h"
#include "Game.h"
#include "MapFile.h"
#include "Parts.h"
#include "PolyMap.h"
#include "TraceLog.h"
#include "Waypoints.h"
#include "Weapons.h"

// Conditional inclusion based on client/server
#ifndef SERVER_CODE
#include "ClientGame.h"  // This would be created from ClientGame.pas
#include "Sprites.h"     // This would be created from Sprites.pas
#else
#include "Server.h"      // This would be created from Server.pas
#endif

// Global variables
bool Running = true;

// Function prototypes
bool InitializeEngine();
void ShutdownEngine();
void ProcessEvents();
void UpdateGame();
void RenderFrame();
void GameLoop();

#ifndef SERVER_CODE
// Client-specific functions
bool InitializeGraphics();
void RenderFrame();
#endif

int main(int argc, char* argv[]) {
    std::cout << "Starting Soldat Game Engine v" << SOLDAT_VERSION << std::endl;

    if (!InitializeEngine()) {
        std::cerr << "Failed to initialize engine!" << std::endl;
        return -1;
    }

    std::cout << "Engine initialized successfully!" << std::endl;

    // Initialize game systems
    CommandInit();
    CvarInit();
    
    // Main game loop
    GameLoop();

    ShutdownEngine();

    std::cout << "Soldat game closed." << std::endl;
    return 0;
}

bool InitializeEngine() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Initialize graphics context if client
#ifndef SERVER_CODE
    if (!InitializeGraphics()) {
        return false;
    }
#endif

    // Initialize logging
    NewLogFiles();

    // Load game constants, settings, etc.
    std::cout << "Engine initialization complete." << std::endl;
    return true;
}

#ifndef SERVER_CODE
bool InitializeGraphics() {
    // Set OpenGL attributes if using OpenGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Create window
    SDL_Window* window = SDL_CreateWindow(
        "Soldat",
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED,
        1024,  // Default width
        768,   // Default height
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create OpenGL context
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Initialize GLEW if using it
    // GLenum err = glewInit();
    // if (err != GLEW_OK) {
    //     std::cerr << "Error initializing GLEW: " << glewGetErrorString(err) << std::endl;
    //     return false;
    // }

    return true;
}
#endif

void ShutdownEngine() {
    // Clean up game systems
    if (GameLog) {
        AddLineToLogFile(GameLog, "Game shutting down", ConsoleLogFileName);
    }

#ifndef SERVER_CODE
    // Clean up graphics resources
    SDL_Quit();
#else
    // Server-specific cleanup
    SDL_Quit();
#endif
}

void GameLoop() {
    Uint32 frameStart, frameTime;
    const int TARGET_FPS = 60;
    const int FRAME_TARGET_TIME = 1000 / TARGET_FPS;

    while (Running) {
        frameStart = SDL_GetTicks();

        ProcessEvents();
        UpdateGame();
        
#ifndef SERVER_CODE
        RenderFrame();
#endif

        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_TARGET_TIME) {
            SDL_Delay(FRAME_TARGET_TIME - frameTime);
        }
    }
}

void ProcessEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                Running = false;
                break;
                
#ifndef SERVER_CODE
            case SDL_KEYDOWN:
                // Handle key presses
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        Running = false;
                        break;
                    default:
                        break;
                }
                break;
#endif
            default:
                break;
        }
    }
}

void UpdateGame() {
    // Update game state
    // This would include updating sprites, physics, AI, etc.
    Ticks++;
    
    // Update timing
    Number27Timing();
}

#ifndef SERVER_CODE
void RenderFrame() {
    // Clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Black background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render game objects here
    // This would include sprites, particles, UI, etc.

    // Swap buffers
    SDL_GL_SwapBuffers();
}
#endif