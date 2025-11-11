#ifndef KEEN_UTILS_H
#define KEEN_UTILS_H

#include <new>      // for std::nothrow
#include <cstddef>  // for size_t

// RAII wrapper for automatic memory management
template<typename T>
class MemoryManager_AutoPtr {
private:
    T* ptr;
    size_t count;
    
public:
    MemoryManager_AutoPtr(T* p = nullptr, size_t c = 1) : ptr(p), count(c) {}
    
    ~MemoryManager_AutoPtr() {
        if (ptr) {
            delete[] ptr;
            ptr = nullptr;
        }
    }
    
    // Move constructor
    MemoryManager_AutoPtr(MemoryManager_AutoPtr&& other) noexcept : ptr(other.ptr), count(other.count) {
        other.ptr = nullptr;
        other.count = 0;
    }
    
    // Move assignment operator
    MemoryManager_AutoPtr& operator=(MemoryManager_AutoPtr&& other) noexcept {
        if (this != &other) {
            if (ptr) {
                delete[] ptr;
            }
            ptr = other.ptr;
            count = other.count;
            other.ptr = nullptr;
            other.count = 0;
        }
        return *this;
    }
    
    // Copy operations are disabled to enforce RAII with raw pointers
    MemoryManager_AutoPtr(const MemoryManager_AutoPtr&) = delete;
    MemoryManager_AutoPtr& operator=(const MemoryManager_AutoPtr&) = delete;
    
    // Access operators
    T* operator->() const { return ptr; }
    T& operator*() const { return *ptr; }
    T* get() const { return ptr; }
    
    // Boolean conversion
    explicit operator bool() const { return ptr != nullptr; }
    
    void reset(T* p = nullptr, size_t c = 1) {
        if (ptr) {
            delete[] ptr;
        }
        ptr = p;
        count = c;
    }
};

#endif // KEEN_UTILS_H