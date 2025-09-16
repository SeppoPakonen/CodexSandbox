#pragma once

// Internal header; aggregated by ManiaCurvy.h

enum CurvyClamping { Clamp, Loop, PingPong };

enum CurvyInterpolationMode { CatmullRom, TCB };

class CurvySpline {
public:
    CurvySpline() {}

    void   Clear();
    void   AddPoint(const MVec3f& p);
    void   SetPointTCB(int idx, float t, float c, float b);
    void   SetTCBBulk(const Vector<float>& T, const Vector<float>& C, const Vector<float>& B);
    bool   GetPointTCB(int idx, float& t, float& c, float& b) const;
    const Vector<float>& GetTVector() const { return tcbT; }
    const Vector<float>& GetCVector() const { return tcbC; }
    const Vector<float>& GetBVector() const { return tcbB; }
    int    GetCount() const { return cp.GetCount(); }

    // t in [0,1]
    MVec3f Interpolate(float t) const; // Catmull-Rom initially

    // Tangent (approx derivative) at t
    MVec3f GetTangent(float t) const;

    // Polyline approximation across all segments (granularity points per segment)
    void   SetGranularity(int g) { granularity = g < 1 ? 1 : g; Invalidate(); }
    int    GetGranularity() const { return granularity; }
    Vector<MVec3f> GetApproximation() const;

    // Length utilities (approximate using polyline)
    double GetLength() const;
    double TFToDistance(float t) const;   // [0,1] -> [0,Length]
    float  DistanceToTF(double d) const;  // [0,Length] -> [0,1]

    // Nearest point on polyline approximation
    float  GetNearestPointTF(const MVec3f& p) const;

    // Move by distance along curve using clamping mode; updates tf and direction
    MVec3f MoveBy(float& tf, int& direction, double distance, CurvyClamping clamp) const;
    // Fast MoveBy using cached LUT and approximated sampling
    MVec3f MoveByFast(float& tf, int& direction, double distance, CurvyClamping clamp) const;
    // Constant-step MoveBy using LUT index stepping (good for many small steps)
    MVec3f MoveByFastSteps(float& tf, int& direction, int steps, CurvyClamping clamp) const;

    // Approximate sampling using LUT (linear between cached points)
    MVec3f ApproximateAtTF(float t) const;
    MVec3f ApproximateAtDistance(double d) const;

    // Interpolation mode/config
    void   SetInterpolationCatmullRom()             { mode = CatmullRom; Invalidate(); }
    void   SetInterpolationTCB(float t, float c, float b) { mode = TCB; tension = t; continuity = c; bias = b; Invalidate(); }
    CurvyInterpolationMode GetInterpolationMode() const { return mode; }

    // Closed curve option
    void   SetClosed(bool b) { closed = b; Invalidate(); }
    bool   IsClosed() const { return closed; }

private:
    Vector<MVec3f> cp; // control points
    int granularity = 20;

    // Cached approximation and cumulative lengths (polyline)
    void   Invalidate();
    void   EnsureCache() const;
    mutable bool cache_dirty = true;
    mutable Vector<MVec3f> approx;
    mutable Vector<double> cumlen;
    mutable Vector<float>  tsample;

    // Interpolation params
    CurvyInterpolationMode mode = CatmullRom;
    float tension = 0.0f;
    float continuity = 0.0f;
    float bias = 0.0f;
    bool  closed = false;

    // Per-point T/C/B (optional). If empty or size mismatch, fall back to global values.
    Vector<float> tcbT, tcbC, tcbB;
};
