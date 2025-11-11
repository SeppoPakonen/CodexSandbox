#ifndef TIMER_H
#define TIMER_H

#include "../Keen.h"

class Timer {
private:
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point pauseTime;
    bool isRunning;
    bool isPaused;
    long long accumulatedTime; // Time accumulated while paused
    
public:
    Timer();
    ~Timer();
    
    void Start();
    void Stop();
    void Pause();
    void Resume();
    void Reset();
    
    // Get elapsed time in various units
    long long GetElapsedMilliseconds();
    long long GetElapsedSeconds();
    long long GetElapsedMicroseconds();
    
    bool IsRunning() const { return isRunning; }
    bool IsPaused() const { return isPaused; }
};

// Global timing utilities
namespace TimeUtils {
    // Get current time in milliseconds since epoch
    long long GetTimeMs();
    
    // Sleep for specified milliseconds
    void SleepMs(long long milliseconds);
    
    // Get current time point
    std::chrono::high_resolution_clock::time_point GetTimePoint();
}

#endif // TIMER_H