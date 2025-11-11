#ifndef KEEN_H
#define KEEN_H

// Main header for Commander Keen C++ conversion
// Contains all necessary includes for the entire project

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <chrono>
#include <thread>
#include <mutex>
#include <stdexcept>
#include <cstdint>
#include <cstring>
#include <cassert>

// RAII wrapper for raw pointers - follows project guidelines
template<typename T>
class RawPtr {
private:
    T* ptr;
    
public:
    explicit RawPtr(T* p = nullptr) : ptr(p) {}
    
    // Destructor handles cleanup
    ~RawPtr() {
        delete ptr;
        ptr = nullptr;
    }
    
    // Move constructor
    RawPtr(RawPtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }
    
    // Move assignment operator
    RawPtr& operator=(RawPtr&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }
    
    // Copy operations are disabled to enforce RAII with raw pointers
    RawPtr(const RawPtr&) = delete;
    RawPtr& operator=(const RawPtr&) = delete;
    
    // Access operators
    T* operator->() const { return ptr; }
    T& operator*() const { return *ptr; }
    T* get() const { return ptr; }
    
    // Boolean conversion
    explicit operator bool() const { return ptr != nullptr; }
    
    // Reset method
    void reset(T* p = nullptr) {
        delete ptr;
        ptr = p;
    }
};

// Type aliases for common types
using byte = uint8_t;
using word = uint16_t;
using dword = uint32_t;

// Global constants
namespace Keen {
    const int MAX_PATH = 260;
    const int MAX_BUFFER_SIZE = 4096;
    const int SCREEN_WIDTH = 320;
    const int SCREEN_HEIGHT = 200;
}



// Include core components
#include "core/Utils.h"
#include "core/MemoryManager.h"
#include "core/ConfigManager.h"
#include "core/FileSystem.h"
#include "core/Timer.h"
// Note: Application.h is not included here to avoid circular dependencies
// #include "core/Application.h"

// Include other module headers as they are developed
// #include "video/VideoSystem.h"
// #include "audio/AudioSystem.h"
// #include "input/InputSystem.h"
#include "graphics/GraphicsSystem.h"
#include "game/ActionManager.h"
#include "game/GameManager.h"
#include "game/InteractionManager.h"
#include "game/KeenSystem.h"
// #include "game/GameSystem.h"

#endif // KEEN_H