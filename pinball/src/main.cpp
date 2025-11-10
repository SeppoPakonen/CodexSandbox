#include <iostream>
#include <memory>

#include "game.h"
#include "renderer.h"

int main() {
    std::cout << "Starting Pinball Game with Three Renderer Systems" << std::endl;
    
    // Check if we can initialize GLFW (requires display)
    if (!glfwInit()) {
        std::cerr << "Error: Cannot initialize GLFW - no display available" << std::endl;
        std::cerr << "This application requires a graphical environment to run." << std::endl;
        std::cerr << "Try running in an environment with X11 or Wayland display server." << std::endl;
        return -1;
    }
    
    // Check if there's a display available
    int monitor_count;
    GLFWmonitor** monitors = glfwGetMonitors(&monitor_count);
    if (monitor_count == 0) {
        std::cerr << "Error: No display monitors detected - running in headless mode?" << std::endl;
        std::cerr << "This application requires a graphical environment to run." << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwTerminate(); // Clean up the test initialization
    
    try {
        // Initialize the game
        Game game;
        
        // Run the game loop
        game.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}