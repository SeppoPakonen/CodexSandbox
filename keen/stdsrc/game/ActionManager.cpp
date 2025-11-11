#include "ActionManager.h"

// Static member definitions
bool ActionManager::initialized = false;
std::unordered_map<std::string, std::shared_ptr<Action>> ActionManager::actionMap;
std::unordered_map<std::string, ActionFunction> ActionManager::functionMap;
std::unordered_map<std::string, CollisionFunction> ActionManager::collisionFunctionMap;

bool ActionManager::Initialize() {
    if (initialized) return false;
    
    initialized = true;
    return true;
}

void ActionManager::Shutdown() {
    if (!initialized) return;
    
    // Clear all maps, which will trigger RAII cleanup
    actionMap.clear();
    functionMap.clear();
    collisionFunctionMap.clear();
    
    initialized = false;
}

bool ActionManager::RegisterAction(const std::string& name, std::shared_ptr<Action> action) {
    if (!initialized || !action) return false;
    
    actionMap[name] = action;
    return true;
}

std::shared_ptr<Action> ActionManager::GetAction(const std::string& name) {
    if (!initialized) return nullptr;
    
    auto it = actionMap.find(name);
    if (it != actionMap.end()) {
        return it->second;
    }
    
    return nullptr; // Action not found
}

std::shared_ptr<Action> ActionManager::CreateAction() {
    if (!initialized) return nullptr;
    
    return std::make_shared<Action>();
}

bool ActionManager::RegisterFunction(const std::string& name, ActionFunction function) {
    if (!initialized) return false;
    
    functionMap[name] = function;
    return true;
}

bool ActionManager::RegisterCollisionFunction(const std::string& name, CollisionFunction function) {
    if (!initialized) return false;
    
    collisionFunctionMap[name] = function;
    return true;
}

ActionFunction ActionManager::GetFunction(const std::string& name) {
    if (!initialized) return nullptr;
    
    auto it = functionMap.find(name);
    if (it != functionMap.end()) {
        return it->second;
    }
    
    // Return null function if not found
    return nullptr;
}

CollisionFunction ActionManager::GetCollisionFunction(const std::string& name) {
    if (!initialized) return nullptr;
    
    auto it = collisionFunctionMap.find(name);
    if (it != collisionFunctionMap.end()) {
        return it->second;
    }
    
    // Return null function if not found
    return nullptr;
}

bool ActionManager::LoadActionsFromFile(const std::string& filename) {
    if (!initialized) return false;
    
    // TODO: Implement actual file loading logic
    // This would parse the ACTION.CKx file format and register actions
    // For now, this is a placeholder
    
    // In the original code, this would parse a text file with action definitions
    // and load them into the action system
    
    return true; // Placeholder return
}