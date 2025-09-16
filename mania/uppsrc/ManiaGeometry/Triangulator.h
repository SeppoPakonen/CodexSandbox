#pragma once

// Internal header; aggregated by ManiaGeometry.h

class Triangulator {
public:
    Triangulator() {}
    explicit Triangulator(const Vector<MVec2f>& points);
    Triangulator(const Vector<MVec3f>& points, int ignore_axis);

    Vector<int> Triangulate() const; // returns triplets of indices

private:
    Vector<MVec2f> pts;

    float Area() const;
    static bool InsideTriangle(const MVec2f& A, const MVec2f& B, const MVec2f& C, const MVec2f& P);
    bool Snip(int u, int v, int w, int n, const Vector<int>& V) const;
};

