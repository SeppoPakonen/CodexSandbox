#ifndef ACTION_MANAGER_H
#define ACTION_MANAGER_H

#include "../Keen.h"
#include <unordered_map>
#include <string>
#include <functional>
#include <vector>
#include <memory>

// Forward declarations
class Object;

// Function pointer types
using ActionFunction = std::function<void(Object* obj)>;
using CollisionFunction = std::function<void(Object* obj1, Object* obj2)>;

// Action types
enum class ActionType {
    UnscaledOnce,     // Unscaled Motion, Thinks once
    ScaledOnce,       // Scaled Motion, Thinks once
    Frame,            // No Motion, Thinks each frame (doesn't advance action)
    UnscaledFrame,    // Unscaled Motion, Thinks each frame
    ScaledFrame       // Scaled Motion, Thinks each frame
};

// Action structure - represents a behavior or animation frame
struct Action {
    int16_t chunkLeft;
    int16_t chunkRight;
    ActionType type;
    int16_t protectAnimation;
    int16_t stickToGround;
    int16_t timer;
    int16_t velX, velY;
    
    // Function pointers
    ActionFunction think;
    CollisionFunction collide;
    ActionFunction draw;
    std::shared_ptr<Action> next;  // Use shared_ptr for automatic memory management
    
    // Constructor
    Action() : chunkLeft(0), chunkRight(0), type(ActionType::UnscaledOnce),
               protectAnimation(0), stickToGround(0), timer(0), velX(0), velY(0),
               think(nullptr), collide(nullptr), draw(nullptr), next(nullptr) {}
};

// Action Manager class - handles action registration and retrieval
class ActionManager {
private:
    static bool initialized;
    static std::unordered_map<std::string, std::shared_ptr<Action>> actionMap;
    static std::unordered_map<std::string, ActionFunction> functionMap;
    static std::unordered_map<std::string, CollisionFunction> collisionFunctionMap;
    
public:
    static bool Initialize();
    static void Shutdown();
    
    // Register an action
    static bool RegisterAction(const std::string& name, std::shared_ptr<Action> action);
    
    // Get an action by name
    static std::shared_ptr<Action> GetAction(const std::string& name);
    
    // Create a new action
    static std::shared_ptr<Action> CreateAction();
    
    // Register a think function
    static bool RegisterFunction(const std::string& name, ActionFunction function);
    
    // Register a collision function
    static bool RegisterCollisionFunction(const std::string& name, CollisionFunction function);
    
    // Get a function by name
    static ActionFunction GetFunction(const std::string& name);
    
    // Get a collision function by name
    static CollisionFunction GetCollisionFunction(const std::string& name);
    
    // Load actions from a file (for now, just a placeholder)
    static bool LoadActionsFromFile(const std::string& filename);
    
    static bool IsInitialized() { return initialized; }
};

// Utility functions for getting actions by name (similar to the original CK_ACTION macro)
inline std::shared_ptr<Action> GetActionByName(const std::string& name) {
    return ActionManager::GetAction(name);
}

#endif // ACTION_MANAGER_H