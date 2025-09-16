#include "ManiaCurvy.h"
#include <float.h>

NAMESPACE_UPP

void CurvySpline::Clear()
{
    cp.Clear();
    Invalidate();
}

void CurvySpline::AddPoint(const MVec3f& p)
{
    cp.Add(p);
    Invalidate();
    // extend per-point T/C/B with defaults if used
    if (tcbT.GetCount() == cp.GetCount() - 1) { tcbT.Add(0.0f); tcbC.Add(0.0f); tcbB.Add(0.0f); }
}

void CurvySpline::SetPointTCB(int idx, float t, float c, float b)
{
    if (idx < 0) return;
    if (tcbT.GetCount() < cp.GetCount()) {
        tcbT.SetCount(cp.GetCount(), 0.0f);
        tcbC.SetCount(cp.GetCount(), 0.0f);
        tcbB.SetCount(cp.GetCount(), 0.0f);
    }
    if (idx >= tcbT.GetCount()) return;
    tcbT[idx] = t; tcbC[idx] = c; tcbB[idx] = b;
    Invalidate();
}

void CurvySpline::SetTCBBulk(const Vector<float>& T, const Vector<float>& C, const Vector<float>& B)
{
    int n = cp.GetCount();
    if (n <= 0) { tcbT.Clear(); tcbC.Clear(); tcbB.Clear(); return; }
    tcbT.SetCount(n, 0.0f);
    tcbC.SetCount(n, 0.0f);
    tcbB.SetCount(n, 0.0f);
    for (int i = 0; i < n; ++i) {
        if (i < T.GetCount()) tcbT[i] = T[i];
        if (i < C.GetCount()) tcbC[i] = C[i];
        if (i < B.GetCount()) tcbB[i] = B[i];
    }
    Invalidate();
}

bool CurvySpline::GetPointTCB(int idx, float& t, float& c, float& b) const
{
    if (idx < 0 || idx >= cp.GetCount()) return false;
    if (tcbT.GetCount() == cp.GetCount()) {
        t = tcbT[idx]; c = tcbC[idx]; b = tcbB[idx];
    } else {
        t = tension; c = continuity; b = bias;
    }
    return true;
}

static inline MVec3f CR(const MVec3f& p0, const MVec3f& p1, const MVec3f& p2, const MVec3f& p3, float t)
{
    // Catmull-Rom spline (uniform)
    float t2 = t * t;
    float t3 = t2 * t;
    float a0 = -0.5f*t3 + t2 - 0.5f*t;
    float a1 =  1.5f*t3 - 2.5f*t2 + 1.0f;
    float a2 = -1.5f*t3 + 2.0f*t2 + 0.5f*t;
    float a3 =  0.5f*t3 - 0.5f*t2;
    return MVec3f(p0.x*a0 + p1.x*a1 + p2.x*a2 + p3.x*a3,
                  p0.y*a0 + p1.y*a1 + p2.y*a2 + p3.y*a3,
                  p0.z*a0 + p1.z*a1 + p2.z*a2 + p3.z*a3);
}

static inline MVec3f TCBInterp(const MVec3f& p0, const MVec3f& p1, const MVec3f& p2, const MVec3f& p3,
                               float t, float T, float C, float B)
{
    // Kochanek–Bartels hermite form with global T/C/B
    float t2 = t * t;
    float t3 = t2 * t;
    float h00 =  2*t3 - 3*t2 + 1;
    float h10 =      t3 - 2*t2 + t;
    float h01 = -2*t3 + 3*t2;
    float h11 =      t3 -   t2;

    // Outgoing tangent at p1
    float A1 = (1 - T) * (1 + C) * (1 + B) * 0.5f;
    float A2 = (1 - T) * (1 - C) * (1 - B) * 0.5f;
    MVec3f m1 = MVec3f((p1.x - p0.x) * A1 + (p2.x - p1.x) * A2,
                       (p1.y - p0.y) * A1 + (p2.y - p1.y) * A2,
                       (p1.z - p0.z) * A1 + (p2.z - p1.z) * A2);
    // Incoming tangent at p2
    float B1 = (1 - T) * (1 - C) * (1 + B) * 0.5f;
    float B2 = (1 - T) * (1 + C) * (1 - B) * 0.5f;
    MVec3f m2 = MVec3f((p2.x - p1.x) * B1 + (p3.x - p2.x) * B2,
                       (p2.y - p1.y) * B1 + (p3.y - p2.y) * B2,
                       (p2.z - p1.z) * B1 + (p3.z - p2.z) * B2);

    return MVec3f(h00*p1.x + h10*m1.x + h01*p2.x + h11*m2.x,
                  h00*p1.y + h10*m1.y + h01*p2.y + h11*m2.y,
                  h00*p1.z + h10*m1.z + h01*p2.z + h11*m2.z);
}

MVec3f CurvySpline::Interpolate(float t) const
{
    int n = cp.GetCount();
    if (n == 0)
        return MVec3f();
    if (n == 1)
        return cp[0];
    if (!closed && n == 2)
        return MVec3f(cp[0].x + (cp[1].x - cp[0].x) * t,
                      cp[0].y + (cp[1].y - cp[0].y) * t,
                      cp[0].z + (cp[1].z - cp[0].z) * t);
    float segf, lt; int seg;
    if (closed && n >= 3) {
        // wrap t into [0,1)
        t = fmod(t, 1.0f);
        if (t < 0) t += 1.0f;
        if (t >= 1.0f) t = 0.0f;
        int seg_count = n; // closed has n segments
        segf = t * seg_count;
        seg = (int)segf;
        lt = segf - seg;
        auto idx = [n](int k){ int r = k % n; return r < 0 ? r + n : r; };
        int i0 = idx(seg - 1);
        int i1 = idx(seg);
        int i2 = idx(seg + 1);
        int i3 = idx(seg + 2);
        if (mode == CatmullRom)
            return CR(cp[i0], cp[i1], cp[i2], cp[i3], lt);
        else {
            float T1 = (tcbT.GetCount() == n ? tcbT[i1] : tension);
            float C1 = (tcbC.GetCount() == n ? tcbC[i1] : continuity);
            float B1 = (tcbB.GetCount() == n ? tcbB[i1] : bias);
            float T2 = (tcbT.GetCount() == n ? tcbT[i2] : tension);
            float C2 = (tcbC.GetCount() == n ? tcbC[i2] : continuity);
            float B2 = (tcbB.GetCount() == n ? tcbB[i2] : bias);
            // Use average of end params for continuity along segment
            float Tm = 0.5f * (T1 + T2);
            float Cm = 0.5f * (C1 + C2);
            float Bm = 0.5f * (B1 + B2);
            return TCBInterp(cp[i0], cp[i1], cp[i2], cp[i3], lt, Tm, Cm, Bm);
        }
    }
    else {
        if (t < 0) t = 0; else if (t > 1) t = 1;
        float seg_count = float(n - 1);
        segf = t * seg_count;
        seg = (int)segf;
        lt = segf - seg;
        int i0 = seg > 0 ? seg - 1 : 0;
        int i1 = seg;
        int i2 = (seg + 1 < n) ? seg + 1 : (n - 1);
        int i3 = (seg + 2 < n) ? seg + 2 : (n - 1);
        if (mode == CatmullRom)
            return CR(cp[i0], cp[i1], cp[i2], cp[i3], lt);
        else {
            float T1 = (tcbT.GetCount() == n ? tcbT[i1] : tension);
            float C1 = (tcbC.GetCount() == n ? tcbC[i1] : continuity);
            float B1 = (tcbB.GetCount() == n ? tcbB[i1] : bias);
            float T2 = (tcbT.GetCount() == n ? tcbT[i2] : tension);
            float C2 = (tcbC.GetCount() == n ? tcbC[i2] : continuity);
            float B2 = (tcbB.GetCount() == n ? tcbB[i2] : bias);
            float Tm = 0.5f * (T1 + T2);
            float Cm = 0.5f * (C1 + C2);
            float Bm = 0.5f * (B1 + B2);
            return TCBInterp(cp[i0], cp[i1], cp[i2], cp[i3], lt, Tm, Cm, Bm);
        }
    }
}

MVec3f CurvySpline::GetTangent(float t) const
{
    // Finite difference around t
    const float eps = 1e-3f;
    float t0 = t - eps;
    float t1 = t + eps;
    if (t0 < 0) t0 = 0;
    if (t1 > 1) t1 = 1;
    MVec3f a = Interpolate(t0);
    MVec3f b = Interpolate(t1);
    return MVec3f(b.x - a.x, b.y - a.y, b.z - a.z);
}

Vector<MVec3f> CurvySpline::GetApproximation() const
{
    EnsureCache();
    Vector<MVec3f> out = approx; // return a copy
    return out;
}

static inline double Dist3(const MVec3f& a, const MVec3f& b)
{
    double dx = (double)b.x - (double)a.x;
    double dy = (double)b.y - (double)a.y;
    double dz = (double)b.z - (double)a.z;
    return sqrt(dx*dx + dy*dy + dz*dz);
}

double CurvySpline::GetLength() const
{
    EnsureCache();
    return cumlen.GetCount() ? cumlen.Top() : 0.0;
}

double CurvySpline::TFToDistance(float t) const
{
    EnsureCache();
    int N = approx.GetCount();
    if (N < 2) return 0.0;
    if (t <= 0) return 0.0;
    if (t >= 1) return cumlen.Top();
    // find bracketing samples in tsample
    int lo = 0, hi = N - 1;
    while (lo + 1 < hi) {
        int mid = (lo + hi) >> 1;
        if (tsample[mid] <= t) lo = mid; else hi = mid;
    }
    double seg = (double)(tsample[hi] - tsample[lo]);
    double frac = seg > 0 ? (t - tsample[lo]) / seg : 0.0;
    return cumlen[lo] + frac * (cumlen[hi] - cumlen[lo]);
}

float CurvySpline::DistanceToTF(double d) const
{
    EnsureCache();
    int N = approx.GetCount();
    if (N < 2) return 0.0f;
    double L = cumlen.Top();
    if (L <= 0) return 0.0f;
    if (d <= 0) return 0.0f;
    if (d >= L) return 1.0f;
    // find segment
    int lo = 0, hi = N - 1;
    while (lo + 1 < hi) {
        int mid = (lo + hi) >> 1;
        if (cumlen[mid] < d) lo = mid; else hi = mid;
    }
    double seglen = cumlen[hi] - cumlen[lo];
    double frac = seglen > 0 ? (d - cumlen[lo]) / seglen : 0.0;
    float t = (float)(tsample[lo] + frac * (tsample[hi] - tsample[lo]));
    if (t < 0) t = 0; else if (t > 1) t = 1;
    return t;
}

float CurvySpline::GetNearestPointTF(const MVec3f& p) const
{
    EnsureCache();
    const Vector<MVec3f>& a = approx;
    int N = a.GetCount();
    if (N == 0) return 0.0f;
    if (N == 1) return 0.0f;
    // Use cached cumulative distances
    double bestd2 = DBL_MAX;
    double bestdist = 0.0; // distance along curve at projection
    for (int i = 0; i + 1 < N; ++i) {
        const MVec3f& s = a[i];
        const MVec3f& e = a[i+1];
        double vx = (double)e.x - (double)s.x;
        double vy = (double)e.y - (double)s.y;
        double vz = (double)e.z - (double)s.z;
        double wx = (double)p.x - (double)s.x;
        double wy = (double)p.y - (double)s.y;
        double wz = (double)p.z - (double)s.z;
        double seg2 = vx*vx + vy*vy + vz*vz;
        double t = seg2 > 0 ? (vx*wx + vy*wy + vz*wz) / seg2 : 0.0;
        if (t < 0) t = 0; else if (t > 1) t = 1;
        double px = (double)s.x + t * vx;
        double py = (double)s.y + t * vy;
        double pz = (double)s.z + t * vz;
        double dx = (double)p.x - px;
        double dy = (double)p.y - py;
        double dz = (double)p.z - pz;
        double d2 = dx*dx + dy*dy + dz*dz;
        if (d2 < bestd2) {
            bestd2 = d2;
            double seglen = sqrt(seg2);
            bestdist = cumlen[i] + t * seglen;
        }
    }
    return DistanceToTF(bestdist);
}

static inline double ModPos(double x, double m)
{
    if (m <= 0) return 0.0;
    double y = fmod(x, m);
    if (y < 0) y += m;
    return y;
}

MVec3f CurvySpline::MoveBy(float& tf, int& direction, double distance, CurvyClamping clamp) const
{
    double L = GetLength();
    if (L <= 0) { tf = 0; return Interpolate(0); }
    double d = TFToDistance(tf);
    double D = d + (direction >= 0 ? +1.0 : -1.0) * distance;
    switch (clamp) {
    case Clamp:
        if (D <= 0) { tf = 0; return Interpolate(0); }
        if (D >= L) { tf = 1; return Interpolate(1); }
        tf = DistanceToTF(D);
        return Interpolate(tf);
    case Loop: {
        double Dm = ModPos(D, L);
        tf = DistanceToTF(Dm);
        return Interpolate(tf);
    }
    case PingPong: {
        double cycle = 2.0 * L;
        double x = ModPos(D, cycle);
        if (x <= L) {
            tf = DistanceToTF(x);
            return Interpolate(tf);
        } else {
            tf = DistanceToTF(cycle - x);
            direction = -direction;
            return Interpolate(tf);
        }
    }
    }
    // Fallback
    tf = DistanceToTF(D);
    return Interpolate(tf);
}

MVec3f CurvySpline::ApproximateAtTF(float t) const
{
    EnsureCache();
    int N = approx.GetCount();
    if (N == 0) return MVec3f();
    if (N == 1) return approx[0];
    if (t <= 0) return approx[0];
    if (t >= 1) return approx.Top();
    int lo = 0, hi = N - 1;
    while (lo + 1 < hi) {
        int mid = (lo + hi) >> 1;
        if (tsample[mid] <= t) lo = mid; else hi = mid;
    }
    float segt = tsample[hi] - tsample[lo];
    float frac = segt > 0 ? (t - tsample[lo]) / segt : 0.0f;
    const MVec3f& a = approx[lo];
    const MVec3f& b = approx[hi];
    return MVec3f(a.x + (b.x - a.x) * frac,
                  a.y + (b.y - a.y) * frac,
                  a.z + (b.z - a.z) * frac);
}

MVec3f CurvySpline::ApproximateAtDistance(double d) const
{
    EnsureCache();
    int N = approx.GetCount();
    if (N == 0) return MVec3f();
    if (N == 1) return approx[0];
    if (d <= 0) return approx[0];
    double L = cumlen.Top();
    if (d >= L) return approx.Top();
    int lo = 0, hi = N - 1;
    while (lo + 1 < hi) {
        int mid = (lo + hi) >> 1;
        if (cumlen[mid] <= d) lo = mid; else hi = mid;
    }
    double seg = cumlen[hi] - cumlen[lo];
    double frac = seg > 0 ? (d - cumlen[lo]) / seg : 0.0;
    const MVec3f& a = approx[lo];
    const MVec3f& b = approx[hi];
    return MVec3f(a.x + (float)((b.x - a.x) * frac),
                  a.y + (float)((b.y - a.y) * frac),
                  a.z + (float)((b.z - a.z) * frac));
}

MVec3f CurvySpline::MoveByFast(float& tf, int& direction, double distance, CurvyClamping clamp) const
{
    EnsureCache();
    if (approx.GetCount() < 2) { tf = 0; return approx.GetCount() ? approx[0] : MVec3f(); }
    // Map tf to distance via LUT
    double d = TFToDistance(tf);
    double L = cumlen.Top();
    if (L <= 0) { tf = 0; return approx[0]; }
    double D = d + (direction >= 0 ? +1.0 : -1.0) * distance;
    switch (clamp) {
    case Clamp:
        if (D <= 0) { tf = 0; return approx[0]; }
        if (D >= L) { tf = 1; return approx.Top(); }
        tf = DistanceToTF(D);
        return ApproximateAtTF(tf);
    case Loop: {
        double Dm = ModPos(D, L);
        tf = DistanceToTF(Dm);
        return ApproximateAtTF(tf);
    }
    case PingPong: {
        double cycle = 2.0 * L;
        double x = ModPos(D, cycle);
        if (x <= L) {
            tf = DistanceToTF(x);
            return ApproximateAtTF(tf);
        } else {
            tf = DistanceToTF(cycle - x);
            direction = -direction;
            return ApproximateAtTF(tf);
        }
    }
    }
    tf = DistanceToTF(D);
    return ApproximateAtTF(tf);
}

MVec3f CurvySpline::MoveByFastSteps(float& tf, int& direction, int steps, CurvyClamping clamp) const
{
    EnsureCache();
    int N = approx.GetCount();
    if (N == 0) { tf = 0; return MVec3f(); }
    if (N == 1) { tf = 0; return approx[0]; }
    // Determine effective sample count for stepping
    int M = closed && N > 1 ? (N - 1) : N; // exclude duplicate last sample for closed
    // Find current index corresponding to tf
    int lo = 0, hi = N - 1;
    float t = tf;
    if (t <= 0) lo = 0; else if (t >= 1) lo = N - 1; else {
        while (lo + 1 < hi) {
            int mid = (lo + hi) >> 1;
            if (tsample[mid] <= t) lo = mid; else hi = mid;
        }
    }
    int i = lo;
    if (closed && i >= M) i = 0;
    int delta = (direction >= 0 ? steps : -steps);
    auto clampi = [&](int x){ if (x < 0) return 0; if (x > M - 1) return M - 1; return x; };
    auto modp = [&](int x, int m){ int r = x % m; return r < 0 ? r + m : r; };
    int j = i;
    switch (clamp) {
    case Clamp:
        j = clampi(i + delta);
        break;
    case Loop:
        if (closed) j = modp(i + delta, M);
        else j = clampi(i + delta);
        break;
    case PingPong: {
        int period = (M - 1) * 2;
        if (period <= 0) { j = 0; break; }
        int pos = modp(i + delta, period);
        if (pos <= (M - 1)) { j = pos; direction = +1; }
        else { j = period - pos; direction = -1; }
        break;
    }
    }
    // Map index back to tf and position
    int k = (closed ? j : j);
    tf = tsample[min(k, N - 1)];
    return approx[min(k, N - 1)];
}

void CurvySpline::Invalidate()
{
    cache_dirty = true;
}

void CurvySpline::EnsureCache() const
{
    if (!cache_dirty)
        return;
    approx.Clear();
    cumlen.Clear();
    tsample.Clear();
    int n = cp.GetCount();
    if (n <= 0) { cache_dirty = false; return; }
    if (n == 1) {
        approx.Add(cp[0]);
        cumlen.Add(0.0);
        cache_dirty = false; return;
    }
    int steps = granularity;
    if (closed && n >= 3) {
        int seg_count = n;
        for (int s = 0; s < seg_count; ++s) {
            for (int i = 0; i < steps; ++i) {
                float t = (s + (i / (float)steps)) / (float)seg_count;
                approx.Add(Interpolate(t));
                tsample.Add(t);
            }
        }
        // close loop explicitly
        approx.Add(Interpolate(1.0f));
        tsample.Add(1.0f);
    }
    else {
        for (int s = 0; s < (n - 1); ++s) {
            for (int i = 0; i < steps; ++i) {
                float t = (s + (i / (float)steps)) / (float)(n - 1);
                approx.Add(Interpolate(t));
                tsample.Add(t);
            }
        }
        approx.Add(Interpolate(1.0f));
        tsample.Add(1.0f);
    }
    // cumulative lengths
    cumlen.SetCount(approx.GetCount());
    cumlen[0] = 0.0;
    for (int i = 1; i < approx.GetCount(); ++i)
        cumlen[i] = cumlen[i-1] + Dist3(approx[i-1], approx[i]);
    cache_dirty = false;
}

END_UPP_NAMESPACE
