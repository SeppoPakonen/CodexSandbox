#include "Timer.h"

Timer::Timer() : isRunning(false), isPaused(false), accumulatedTime(0) {
    startTime = std::chrono::high_resolution_clock::time_point::min();
    pauseTime = std::chrono::high_resolution_clock::time_point::min();
}

Timer::~Timer() {
    // Nothing special needed
}

void Timer::Start() {
    if (!isRunning) {
        startTime = std::chrono::high_resolution_clock::now();
        isRunning = true;
        isPaused = false;
        accumulatedTime = 0;
    }
}

void Timer::Stop() {
    isRunning = false;
    isPaused = false;
    accumulatedTime = 0;
}

void Timer::Pause() {
    if (isRunning && !isPaused) {
        pauseTime = std::chrono::high_resolution_clock::now();
        isPaused = true;
    }
}

void Timer::Resume() {
    if (isRunning && isPaused) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto pausedDuration = std::chrono::duration_cast<std::chrono::milliseconds>(
            currentTime - pauseTime
        ).count();
        accumulatedTime += pausedDuration;
        isPaused = false;
    }
}

void Timer::Reset() {
    if (isRunning) {
        startTime = std::chrono::high_resolution_clock::now();
    }
    accumulatedTime = 0;
    isPaused = false;
}

long long Timer::GetElapsedMilliseconds() {
    if (!isRunning) return 0;
    
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        currentTime - startTime
    ).count();
    
    if (isPaused) {
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            pauseTime - startTime
        ).count();
    }
    
    return duration - accumulatedTime;
}

long long Timer::GetElapsedSeconds() {
    return GetElapsedMilliseconds() / 1000;
}

long long Timer::GetElapsedMicroseconds() {
    if (!isRunning) return 0;
    
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        currentTime - startTime
    ).count();
    
    if (isPaused) {
        duration = std::chrono::duration_cast<std::chrono::microseconds>(
            pauseTime - startTime
        ).count();
    }
    
    // Convert accumulated time to microseconds
    long long accumulatedMicroseconds = accumulatedTime * 1000;
    return duration - accumulatedMicroseconds;
}

// Global timing utilities implementation
long long TimeUtils::GetTimeMs() {
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    ).count();
}

void TimeUtils::SleepMs(long long milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

std::chrono::high_resolution_clock::time_point TimeUtils::GetTimePoint() {
    return std::chrono::high_resolution_clock::now();
}