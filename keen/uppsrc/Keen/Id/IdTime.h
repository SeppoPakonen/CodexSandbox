// Timing facade
#pragma once


// "namespace Time" clashed with ::Upp::Time class, so "App" was added as prefix.
namespace AppTime {

class Clock {
public:
    static void DelayMs(int ms);
    static uint32_t NowMs();
};

class FrameRate {
public:
    FrameRate(int baseTickMs = 14, int minTics = 2, int maxTics = 5, int demoTics = 3)
        : base_ms(baseTickMs), min_t(minTics), max_t(maxTics), demo_t(demoTics) {}
    int ComputeTicks(uint32_t elapsedMs) const;
    int BaseMs() const { return base_ms; }
    void SetMin(int v) { min_t = v; }
    void SetMax(int v) { max_t = v; }
    void SetDemo(int v) { demo_t = v; }
    int GetMin() const { return min_t; }
    int GetMax() const { return max_t; }
    int GetDemo() const { return demo_t; }
private:
    int base_ms;
    int min_t, max_t, demo_t;
};

}
