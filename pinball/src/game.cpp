#include "game.h"
#include "renderer.h"
#include "simple_renderer.h"
#include <GLFW/glfw3.h>
#include <iostream>

Game::Game() : currentRendererType(GameRendererType::ShaderAdvanced) {
    // Initialize both renderer types
    shaderRenderer = std::make_unique<Renderer>(1024, 768, "Pinball Game - Advanced Renderer");
    shaderRenderer->initialize();
    
    // Initialize the simple renderer as well
    simpleRenderer = std::make_unique<SimpleRenderer>(1024, 768);
    simpleRenderer->initialize();
}

Game::~Game() {
    if (shaderRenderer) {
        shaderRenderer->cleanup();
    }
    if (simpleRenderer) {
        simpleRenderer->cleanup();
    }
}

void Game::run() {
    float lastFrame = 0.0f;
    
    while (!shouldClose && !shouldCloseWindow()) {
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processInput();
        update(deltaTime);
        render();
    }
}

void Game::update(float deltaTime) {
    if (currentRendererType == GameRendererType::FixedFunction) {
        simpleRenderer->updatePhysics(deltaTime);
    } else {
        shaderRenderer->update(deltaTime);
    }
}

void Game::render() {
    switch (currentRendererType) {
        case GameRendererType::ShaderSimple:
            shaderRenderer->setRendererType(RendererType::Simple);
            shaderRenderer->render();
            break;
        case GameRendererType::ShaderAdvanced:
            shaderRenderer->setRendererType(RendererType::Advanced);
            shaderRenderer->render();
            break;
        case GameRendererType::FixedFunction:
            simpleRenderer->render();
            break;
    }
}

void Game::processInput() {
    GLFWwindow* window = nullptr;
    
    // Use the window from the current renderer
    if (currentRendererType == GameRendererType::FixedFunction) {
        if (simpleRenderer) {
            window = simpleRenderer->getWindow();
        }
    } else {
        if (shaderRenderer) {
            window = shaderRenderer->getWindow();
        }
    }
    
    if (window) {
        // Close on ESC
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            shouldClose = true;
        }
        
        // Toggle renderer on 'R' key
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            switchRenderer();
        }
    }
}

bool Game::shouldCloseWindow() {
    // Check the window from the current renderer
    if (currentRendererType == GameRendererType::FixedFunction) {
        if (simpleRenderer) {
            return simpleRenderer->shouldClose();
        }
    } else {
        if (shaderRenderer) {
            return glfwWindowShouldClose(shaderRenderer->getWindow());
        }
    }
    return shouldClose;
}

void Game::switchRenderer() {
    switch (currentRendererType) {
        case GameRendererType::ShaderAdvanced:
            currentRendererType = GameRendererType::FixedFunction;
            std::cout << "Switched to Fixed-Function Pipeline Renderer" << std::endl;
            break;
        case GameRendererType::FixedFunction:
            currentRendererType = GameRendererType::ShaderSimple;
            std::cout << "Switched to Simple Shader Renderer" << std::endl;
            break;
        case GameRendererType::ShaderSimple:
            currentRendererType = GameRendererType::ShaderAdvanced;
            std::cout << "Switched to Advanced Shader Renderer" << std::endl;
            break;
    }
}