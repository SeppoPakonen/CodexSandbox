#ifndef APPLICATION_H
#define APPLICATION_H

#include "../Keen.h"
#include "Utils.h"  // Include this first to get MemoryManager_AutoPtr
#include "MemoryManager.h"
#include "ConfigManager.h"
#include "FileSystem.h"
#include "Timer.h"

class Application {
private:
    static bool initialized;
    static bool running;
    static std::string applicationName;
    static int argc;
    static char** argv;
    
    // Core systems that need cleanup in RAII manner
    static MemoryManager_AutoPtr<MemoryManager> memoryManager;
    static ConfigManager configManager;
    static FileSystem fileSystem;
    static Timer appTimer;
    
public:
    static bool Initialize(int argc, char* argv[], const std::string& name = "KeenApplication");
    static void Shutdown();
    static bool IsInitialized() { return initialized; }
    static bool IsRunning() { return running; }
    static void SetRunning(bool state) { running = state; }
    static void Run();
    
    // File validation methods
    static bool ValidateGameDataFiles(int episode = 0); // 0 = auto-detect
    static void LogMissingGameFiles(int episode = 0);
    
    // Access to core systems
    static MemoryManager* GetMemoryManager() { return memoryManager.get(); }
    static ConfigManager* GetConfigManager() { return &configManager; }
    static FileSystem* GetFileSystem() { return &fileSystem; }
    static Timer* GetTimer() { return &appTimer; }
    
    static std::string GetApplicationName() { return applicationName; }
    static int GetArgCount() { return argc; }
    static char** GetArgs() { return argv; }
};

#endif // APPLICATION_H