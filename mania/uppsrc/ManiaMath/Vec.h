#pragma once

// Internal header; aggregated by ManiaMath.h

struct MVec2f {
    float x = 0, y = 0;
    MVec2f() {}
    MVec2f(float x, float y) : x(x), y(y) {}
};

struct MVec3f {
    float x = 0, y = 0, z = 0;
    MVec3f() {}
    MVec3f(float x, float y, float z) : x(x), y(y), z(z) {}
};

inline MVec2f operator+(const MVec2f& a, const MVec2f& b) { return MVec2f(a.x + b.x, a.y + b.y); }
inline MVec2f operator-(const MVec2f& a, const MVec2f& b) { return MVec2f(a.x - b.x, a.y - b.y); }
inline MVec2f operator*(const MVec2f& a, float s) { return MVec2f(a.x * s, a.y * s); }

inline MVec3f operator+(const MVec3f& a, const MVec3f& b) { return MVec3f(a.x + b.x, a.y + b.y, a.z + b.z); }
inline MVec3f operator-(const MVec3f& a, const MVec3f& b) { return MVec3f(a.x - b.x, a.y - b.y, a.z - b.z); }
inline MVec3f operator*(const MVec3f& a, float s) { return MVec3f(a.x * s, a.y * s, a.z * s); }

inline float Dot(const MVec2f& a, const MVec2f& b) { return a.x * b.x + a.y * b.y; }
inline float CrossZ(const MVec2f& a, const MVec2f& b) { return a.x * b.y - a.y * b.x; }

