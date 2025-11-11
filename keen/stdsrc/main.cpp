#include "Keen.h"
#include "core/Application.h"  // Include Application.h explicitly
#include "graphics/GraphicsSystem.h"  // Include GraphicsSystem for testing

int main(int argc, char* argv[]) {
    // Initialize the application
    if (!Application::Initialize(argc, argv, "KeenTestApp")) {
        std::cerr << "Failed to initialize application!" << std::endl;
        return 1;
    }
    
    std::cout << "Commander Keen Standard C++ Application Initialized!" << std::endl;
    
    // Test memory allocation using our RAII manager
    auto memoryPtr = MemoryManager_AutoPtr<int>(MemoryManager::Allocate<int>(10), 10);
    if (memoryPtr) {
        std::cout << "Successfully allocated memory for 10 integers" << std::endl;
        
        // Initialize the array
        for (int i = 0; i < 10; i++) {
            memoryPtr.get()[i] = i * 10;
        }
        
        // Print values
        std::cout << "Array contents: ";
        for (int i = 0; i < 10; i++) {
            std::cout << memoryPtr.get()[i] << " ";
        }
        std::cout << std::endl;
    }
    
    // Test config manager
    ConfigManager::SetValue("test.value", 42);
    ConfigManager::SetValue("test.string", "Hello, Keen!");
    std::cout << "Config test - int value: " << ConfigManager::GetInt("test.value") << std::endl;
    std::cout << "Config test - string value: " << ConfigManager::GetString("test.string", "default") << std::endl;
    
    // Test file system
    FileSystem::AddSearchPath(".");
    FileSystem::AddSearchPath("./data");
    std::cout << "Current directory exists: " << FileSystem::DirectoryExists(".") << std::endl;
    
    // Test timer functionality
    Timer testTimer;
    testTimer.Start();
    TimeUtils::SleepMs(100); // Sleep for 100ms
    std::cout << "Timer test - elapsed time: " << testTimer.GetElapsedMilliseconds() << "ms" << std::endl;
    
    // Test graphics system
    if (GraphicsSystem::Initialize()) {
        std::cout << "Graphics system initialized successfully!" << std::endl;
        
        // Test creating a bitmap
        Bitmap* testBitmap = GraphicsSystem::CreateBitmap(64, 64);
        if (testBitmap) {
            std::cout << "Successfully created a 64x64 bitmap!" << std::endl;
            
            // Test extracting from Keen data (placeholder implementation)
            Bitmap keenBitmap;
            if (GraphicsSystem::ExtractBitmapFromKeenData("TEST_DATA.CK", 1, keenBitmap)) {
                std::cout << "Successfully extracted bitmap from Keen data!" << std::endl;
                std::cout << "Extracted bitmap: " << keenBitmap.width << "x" << keenBitmap.height << std::endl;
            } else {
                std::cout << "Could not extract bitmap from Keen data (expected for placeholder)" << std::endl;
            }
        }
        
        GraphicsSystem::Shutdown();
    } else {
        std::cout << "Failed to initialize graphics system!" << std::endl;
    }
    
    // Test game manager
    if (GameManager::Initialize()) {
        std::cout << "Game manager initialized successfully!" << std::endl;
        
        // Test creating a new game
        GameManager::NewGame();
        std::cout << "New game created. Lives: " << GameManager::GetGameState().numLives << std::endl;
        
        // Test game loop (just call it once)
        GameManager::SetRunning(true);
        GameManager::GameLoop();
        
        GameManager::Shutdown();
    } else {
        std::cout << "Failed to initialize game manager!" << std::endl;
    }
    
    // Run the application loop for a short time, then exit
    TimeUtils::SleepMs(1000); // Run for 1 second
    Application::SetRunning(false); // Signal to stop
    
    // Shutdown the application
    Application::Shutdown();
    
    std::cout << "Application terminated successfully." << std::endl;
    return 0;
}