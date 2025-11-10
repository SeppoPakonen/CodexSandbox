#include "renderer.h"
#include <iostream>
#include <chrono>
#include <GLFW/glfw3.h>

Renderer::Renderer(int width, int height, const std::string& title) 
    : width(width), height(height), currentRendererType(RendererType::Simple), currentShader(nullptr) {
    
    iResolution[0] = static_cast<float>(width);
    iResolution[1] = static_cast<float>(height);
    
    // Initialize GLFW
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    
    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    
    // Create window
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    
    glfwMakeContextCurrent(window);
    
    // Set up callback functions
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int w, int h) {
        Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
        renderer->setViewport(w, h);
    });
    
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
        Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
        // Handle mouse input for iMouse uniform
        renderer->setMouse(static_cast<float>(xpos), static_cast<float>(ypos));
    });
    
    // Initialize camera
    camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
}

Renderer::~Renderer() {
    cleanup();
}

void Renderer::initialize() {
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Failed to initialize GLEW");
    }
    
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    
    initShaders();
    initQuad();
    
    // Set initial viewport
    glViewport(0, 0, width, height);
}

void Renderer::cleanup() {
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}

void Renderer::update(float deltaTime) {
    // Update time uniforms
    iTime += deltaTime;
    iTimeDelta = deltaTime;
    iFrame++;
    
    // Process input and events
    glfwPollEvents();
}

void Renderer::render() {
    // Clear the screen
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Use appropriate shader based on current renderer type
    if (currentRendererType == RendererType::Simple && simpleShader) {
        currentShader = simpleShader.get();
    } else if (currentRendererType == RendererType::Advanced && advancedShader) {
        currentShader = advancedShader.get();
    }
    
    if (currentShader) {
        currentShader->use();
        
        // Set Shadertoy uniforms
        currentShader->setVec3("iResolution", iResolution[0], iResolution[1], 1.0f);  // Using vec3 as in original shaders
        currentShader->setFloat("iTime", iTime);
        currentShader->setFloat("iTimeDelta", iTimeDelta);
        currentShader->setVec4("iMouse", iMouse[0], iMouse[1], 0.0f, 0.0f);  // Using vec4 as in original shaders
        currentShader->setFloat("iFrame", static_cast<float>(iFrame));  // Using float as in original shaders
        
        // Additional uniforms that may be needed by the shaders
        // iDate uniform (year, month, day, time in seconds)
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        auto tm = *std::localtime(&time_t);
        currentShader->setVec4("iDate", 
                              static_cast<float>(tm.tm_year + 1900), 
                              static_cast<float>(tm.tm_mon + 1), 
                              static_cast<float>(tm.tm_mday), 
                              static_cast<float>(tm.tm_hour * 3600 + tm.tm_min * 60 + tm.tm_sec + ms.count() / 1000.0f));
        
        // Render the full-screen quad
        renderQuad();
    }
    
    // Swap buffers
    glfwSwapBuffers(window);
}

void Renderer::setRendererType(RendererType type) {
    currentRendererType = type;
}

void Renderer::setViewport(int width, int height) {
    this->width = width;
    this->height = height;
    iResolution[0] = static_cast<float>(width);
    iResolution[1] = static_cast<float>(height);
    glViewport(0, 0, width, height);
}

void Renderer::setResolution(float x, float y) {
    iResolution[0] = x;
    iResolution[1] = y;
}

void Renderer::setTime(float time) {
    iTime = time;
}

void Renderer::setTimeDelta(float deltaTime) {
    iTimeDelta = deltaTime;
}

void Renderer::setMouse(float x, float y) {
    iMouse[0] = x;
    iMouse[1] = y;
}

void Renderer::setFrame(int frame) {
    iFrame = frame;
}

void Renderer::initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::initShaders() {
    // Load the converted Shadertoy shaders
    // When building, CMake will copy shaders to the build directory
    simpleShader = std::make_unique<Shader>(
        "shaders/screen_quad.vert", 
        "shaders/simple_pinball.frag", 
        false  // not source code, but file paths
    );
    
    advancedShader = std::make_unique<Shader>(
        "shaders/screen_quad.vert", 
        "shaders/advanced_pinball.frag", 
        false  // not source code, but file paths
    );
    
    // Initially use the simple shader
    currentShader = simpleShader.get();
}

void Renderer::initQuad() {
    // Define the full-screen quad
    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    
    // Create and configure VAO/VBO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::renderQuad() {
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}