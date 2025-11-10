#pragma once

#include <memory>
#include "renderer.h"
#include "simple_renderer.h"

enum class GameRendererType {
    ShaderSimple,    // MdyGWG
    ShaderAdvanced,  // MdyGDz
    FixedFunction    // Old school OpenGL
};

class Game {
public:
    Game();
    ~Game();
    
    void run();
    void update(float deltaTime);
    void render();
    
    void processInput();
    bool shouldCloseWindow();
    
    void switchRenderer();
    
private:
    std::unique_ptr<Renderer> shaderRenderer;      // For advanced shader-based renderers
    std::unique_ptr<SimpleRenderer> simpleRenderer; // For fixed-function pipeline
    GameRendererType currentRendererType;
    
    bool shouldClose = false;
};