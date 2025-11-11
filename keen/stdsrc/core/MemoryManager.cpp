#include "MemoryManager.h"

// Static member definitions
bool MemoryManager::initialized = false;
size_t MemoryManager::totalMemory = 0;
size_t MemoryManager::usedMemory = 0;
size_t MemoryManager::largestBlock = 0;

bool MemoryManager::Initialize(size_t memorySize) {
    if (initialized) return false;
    
    totalMemory = memorySize;
    usedMemory = 0;
    largestBlock = 0;
    initialized = true;
    
    return true;
}

void MemoryManager::Shutdown() {
    initialized = false;
    totalMemory = 0;
    usedMemory = 0;
    largestBlock = 0;
}