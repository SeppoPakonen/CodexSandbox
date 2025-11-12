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
    
    // Check for required game data files
    int episode = 0; // Auto-detect
    LogMissingGameFiles(episode);
    
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

bool Application::ValidateGameDataFiles(int episode) {
    std::vector<std::string> requiredFiles;
    
    if (episode == 0) {
        // Auto-detect - check for any episode files
        bool foundEpisode4 = FileSystem::FileExists("EGAGRAPH.CK4") || FileSystem::FileExists("MAPHEAD.CK4");
        bool foundEpisode5 = FileSystem::FileExists("EGAGRAPH.CK5") || FileSystem::FileExists("MAPHEAD.CK5");
        bool foundEpisode6 = FileSystem::FileExists("EGAGRAPH.CK6") || FileSystem::FileExists("MAPHEAD.CK6");
        
        if (foundEpisode4 || FileSystem::FileExists("KEEN4.EXE")) episode = 4;
        else if (foundEpisode5 || FileSystem::FileExists("KEEN5.EXE")) episode = 5;
        else if (foundEpisode6 || FileSystem::FileExists("KEEN6.EXE")) episode = 6;
        else {
            std::cout << "WARNING: No episode data files found. Looking for:" << std::endl;
            std::cout << "  - EGAGRAPH.CK4, MAPHEAD.CK4, etc. for episode 4" << std::endl;
            std::cout << "  - EGAGRAPH.CK5, MAPHEAD.CK5, etc. for episode 5" << std::endl;
            std::cout << "  - EGAGRAPH.CK6, MAPHEAD.CK6, etc. for episode 6" << std::endl;
            std::cout << "  - KEEN4.EXE, KEEN5.EXE, or KEEN6.EXE" << std::endl;
            return false;
        }
    }
    
    // Determine required files based on episode
    switch(episode) {
        case 4:
            requiredFiles = {
                "EGAGRAPH.CK4", "MAPHEAD.CK4", "VGAHEAD.CK4", "VGADICT.CK4", "VGALINF.CK4",
                "VGAMAP.CK4", "AUDIO.CK4", "AUDIOT.CK4", "LEVELS/1.CK4", "LEVELS/2.CK4", 
                "LEVELS/3.CK4", "LEVELS/4.CK4", "LEVELS/5.CK4", "LEVELS/6.CK4", 
                "LEVELS/7.CK4", "LEVELS/8.CK4", "LEVELS/9.CK4", "LEVELS/10.CK4",
                "LEVELS/11.CK4", "LEVELS/12.CK4", "LEVELS/13.CK4", "LEVELS/14.CK4",
                "LEVELS/15.CK4", "LEVELS/16.CK4", "LEVELS/17.CK4"
            };
            break;
        case 5:
            requiredFiles = {
                "EGAGRAPH.CK5", "MAPHEAD.CK5", "VGAHEAD.CK5", "VGADICT.CK5", "VGALINF.CK5",
                "VGAMAP.CK5", "AUDIO.CK5", "AUDIOT.CK5", "LEVELS/1.CK5", "LEVELS/2.CK5", 
                "LEVELS/3.CK5", "LEVELS/4.CK5", "LEVELS/5.CK5", "LEVELS/6.CK5", 
                "LEVELS/7.CK5", "LEVELS/8.CK5", "LEVELS/9.CK5", "LEVELS/10.CK5",
                "LEVELS/11.CK5", "LEVELS/12.CK5", "LEVELS/13.CK5", "LEVELS/14.CK5",
                "LEVELS/15.CK5", "LEVELS/16.CK5", "LEVELS/17.CK5"
            };
            break;
        case 6:
            requiredFiles = {
                "EGAGRAPH.CK6", "MAPHEAD.CK6", "VGAHEAD.CK6", "VGADICT.CK6", "VGALINF.CK6",
                "VGAMAP.CK6", "AUDIO.CK6", "AUDIOT.CK6", "LEVELS/1.CK6", "LEVELS/2.CK6", 
                "LEVELS/3.CK6", "LEVELS/4.CK6", "LEVELS/5.CK6", "LEVELS/6.CK6", 
                "LEVELS/7.CK6", "LEVELS/8.CK6", "LEVELS/9.CK6", "LEVELS/10.CK6",
                "LEVELS/11.CK6", "LEVELS/12.CK6", "LEVELS/13.CK6", "LEVELS/14.CK6",
                "LEVELS/15.CK6", "LEVELS/16.CK6", "LEVELS/17.CK6"
            };
            break;
        default:
            std::cout << "Unknown episode number: " << episode << std::endl;
            return false;
    }
    
    // Check if all required files exist
    bool allFilesPresent = true;
    for (const auto& file : requiredFiles) {
        if (!FileSystem::FileExists(file)) {
            allFilesPresent = false;
            break;
        }
    }
    
    return allFilesPresent;
}

void Application::LogMissingGameFiles(int episode) {
    std::vector<std::string> requiredFiles;
    
    if (episode == 0) {
        // Auto-detect - check for any episode files
        bool foundEpisode4 = FileSystem::FileExists("EGAGRAPH.CK4") || FileSystem::FileExists("MAPHEAD.CK4");
        bool foundEpisode5 = FileSystem::FileExists("EGAGRAPH.CK5") || FileSystem::FileExists("MAPHEAD.CK5");
        bool foundEpisode6 = FileSystem::FileExists("EGAGRAPH.CK6") || FileSystem::FileExists("MAPHEAD.CK6");
        
        if (foundEpisode4) episode = 4;
        else if (foundEpisode5) episode = 5;
        else if (foundEpisode6) episode = 6;
    }
    
    if (episode == 0) {
        // No episode detected, list all possible files
        std::cout << "INFO: No Commander Keen game data files detected." << std::endl;
        std::cout << std::endl;
        std::cout << "The game requires Commander Keen episode data files to run properly." << std::endl;
        std::cout << "These typically include (for each episode):" << std::endl;
        std::cout << "  - EGAGRAPH.CK[4/5/6]: EGA graphics data" << std::endl;
        std::cout << "  - MAPHEAD.CK[4/5/6]: Map headers" << std::endl;
        std::cout << "  - VGAHEAD.CK[4/5/6]: VGA headers" << std::endl;
        std::cout << "  - VGADICT.CK[4/5/6]: VGA dictionary" << std::endl;
        std::cout << "  - VGALINF.CK[4/5/6]: VGA line information" << std::endl;
        std::cout << "  - VGAMAP.CK[4/5/6]: VGA map data" << std::endl;
        std::cout << "  - AUDIO.CK[4/5/6]: Audio data" << std::endl;
        std::cout << "  - AUDIOT.CK[4/5/6]: Audio table" << std::endl;
        std::cout << "  - LEVELS/*.CK[4/5/6]: Level data files" << std::endl;
        std::cout << std::endl;
        std::cout << "To run the game, please place the original Commander Keen data files" << std::endl;
        std::cout << "in this directory or specify a different location with configuration." << std::endl;
        std::cout << std::endl;
        return;
    }
    
    // Determine required files based on episode
    switch(episode) {
        case 4:
            requiredFiles = {
                "EGAGRAPH.CK4", "MAPHEAD.CK4", "VGAHEAD.CK4", "VGADICT.CK4", "VGALINF.CK4",
                "VGAMAP.CK4", "AUDIO.CK4", "AUDIOT.CK4"
            };
            break;
        case 5:
            requiredFiles = {
                "EGAGRAPH.CK5", "MAPHEAD.CK5", "VGAHEAD.CK5", "VGADICT.CK5", "VGALINF.CK5",
                "VGAMAP.CK5", "AUDIO.CK5", "AUDIOT.CK5"
            };
            break;
        case 6:
            requiredFiles = {
                "EGAGRAPH.CK6", "MAPHEAD.CK6", "VGAHEAD.CK6", "VGADICT.CK6", "VGALINF.CK6",
                "VGAMAP.CK6", "AUDIO.CK6", "AUDIOT.CK6"
            };
            break;
        default:
            std::cout << "ERROR: Unknown episode number: " << episode << std::endl;
            return;
    }
    
    std::cout << "INFO: Commander Keen Episode " << episode << " detected." << std::endl;
    
    // Check which files are missing
    std::vector<std::string> missingFiles;
    for (const auto& file : requiredFiles) {
        if (!FileSystem::FileExists(file)) {
            missingFiles.push_back(file);
        }
    }
    
    if (missingFiles.empty()) {
        std::cout << "SUCCESS: All required core data files found for Episode " << episode << std::endl;
    } else {
        std::cout << "WARNING: Missing " << missingFiles.size() << " of " << requiredFiles.size() 
                  << " required core data files:" << std::endl;
        for (const auto& file : missingFiles) {
            std::cout << "  - " << file << std::endl;
        }
        std::cout << std::endl;
        std::cout << "The game may not run properly without these files." << std::endl;
        std::cout << "Please ensure original Commander Keen Episode " << episode << " data files are present." << std::endl;
        std::cout << std::endl;
    }
}