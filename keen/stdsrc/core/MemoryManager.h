#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "../Keen.h"

class MemoryManager {
private:
    static bool initialized;
    static size_t totalMemory;
    static size_t usedMemory;
    static size_t largestBlock;
    
public:
    static bool Initialize(size_t memorySize);
    static void Shutdown();
    
    template<typename T>
    static T* Allocate(size_t count = 1) {
        if (!initialized) return nullptr;
        
        size_t size = sizeof(T) * count;
        if (usedMemory + size > totalMemory) {
            return nullptr; // Out of memory
        }
        
        T* ptr = new(std::nothrow) T[count];
        if (ptr) {
            usedMemory += size;
            if (size > largestBlock) {
                largestBlock = size;
            }
        }
        return ptr;
    }
    
    template<typename T>
    static void Deallocate(T* ptr, size_t count = 1) {
        if (ptr) {
            delete[] ptr;
            usedMemory -= sizeof(T) * count;
        }
    }
    
    // Utility functions
    static size_t GetUsedMemory() { return usedMemory; }
    static size_t GetTotalMemory() { return totalMemory; }
    static size_t GetLargestBlockSize() { return largestBlock; }
    static size_t GetFreeMemory() { return totalMemory - usedMemory; }
    static bool IsInitialized() { return initialized; }
};

#endif // MEMORY_MANAGER_H