#include "Application.h"

// Static member definitions
bool Application::initialized = false;
bool Application::running = false;
std::string Application::applicationName = "";
int Application::argc = 0;
char** Application::argv = nullptr;

// RAII smart pointers for core systems
MemoryManager_AutoPtr<MemoryManager> Application::memoryManager(nullptr);
ConfigManager Application::configManager;
FileSystem Application::fileSystem;
Timer Application::appTimer;

bool Application::Initialize(int appArgc, char* appArgv[], const std::string& name) {
    if (initialized) return false;
    
    argc = appArgc;
    argv = appArgv;
    applicationName = name;
    
    // Initialize core systems in proper order
    if (!MemoryManager::Initialize(32 * 1024 * 1024)) { // 32MB initial allocation
        std::cerr << "Failed to initialize Memory Manager" << std::endl;
        return false;
    }
    
    if (!ConfigManager::Initialize()) {
        std::cerr << "Failed to initialize Config Manager" << std::endl;
        MemoryManager::Shutdown();
        return false;
    }
    
    if (!FileSystem::Initialize()) {
        std::cerr << "Failed to initialize File System" << std::endl;
        ConfigManager::Shutdown();
        MemoryManager::Shutdown();
        return false;
    }
    
    initialized = true;
    running = true;
    
    std::cout << "Application '" << applicationName << "' initialized successfully." << std::endl;
    return true;
}

void Application::Shutdown() {
    if (!initialized) return;
    
    std::cout << "Shutting down application..." << std::endl;
    
    // Stop running state
    running = false;
    
    // Shutdown systems in reverse order
    FileSystem::Shutdown();
    ConfigManager::Shutdown();
    MemoryManager::Shutdown();
    
    initialized = false;
    std::cout << "Application '" << applicationName << "' shutdown completed." << std::endl;
}

void Application::Run() {
    if (!initialized) {
        std::cerr << "Application not initialized!" << std::endl;
        return;
    }
    
    std::cout << "Starting application loop..." << std::endl;
    appTimer.Start();
    
    while (running) {
        // Perform one iteration of the main loop
        // In the future, this could handle events, update game state, render, etc.
        
        // For now, just a simple loop that can be exited
        // with some condition or input
        
        // Small sleep to prevent 100% CPU usage
        TimeUtils::SleepMs(1);
    }
    
    appTimer.Stop();
    long long elapsedMs = appTimer.GetElapsedMilliseconds();
    std::cout << "Application loop ended after " << elapsedMs << "ms" << std::endl;
}