#include "ManiaGeometry.h"

NAMESPACE_UPP

Triangulator::Triangulator(const Vector<MVec2f>& points)
{
    pts = points;
}

Triangulator::Triangulator(const Vector<MVec3f>& points, int ignore_axis)
{
    pts.SetCount(points.GetCount());
    switch(ignore_axis) {
    case 0:
        for (int i = 0; i < points.GetCount(); ++i)
            pts[i] = MVec2f(points[i].y, points[i].z);
        break;
    case 1:
        for (int i = 0; i < points.GetCount(); ++i)
            pts[i] = MVec2f(points[i].x, points[i].z);
        break;
    default:
        for (int i = 0; i < points.GetCount(); ++i)
            pts[i] = MVec2f(points[i].x, points[i].y);
        break;
    }
}

Vector<int> Triangulator::Triangulate() const
{
    Vector<int> out;
    int n = pts.GetCount();
    if (n < 3)
        return out;

    Vector<int> V;
    V.SetCount(n);
    if (Area() > 0) {
        for (int i = 0; i < n; i++) V[i] = i;
    } else {
        for (int i = 0; i < n; i++) V[i] = (n - 1) - i;
    }

    int nv = n;
    int count = 2 * nv;
    int v = nv - 1;
    while (nv > 2) {
        if ((count--) <= 0)
            return out; // probably not a simple polygon
        int u = v; if (u >= nv) u = 0;
        v = u + 1; if (v >= nv) v = 0;
        int w = v + 1; if (w >= nv) w = 0;

        if (Snip(u, v, w, nv, V)) {
            int a = V[u];
            int b = V[v];
            int c = V[w];
            out.Add(a); out.Add(b); out.Add(c);
            for (int s = v, t = v + 1; t < nv; s++, t++)
                V[s] = V[t];
            nv--;
            count = 2 * nv;
        }
    }
    return out;
}

float Triangulator::Area() const
{
    int n = pts.GetCount();
    double A = 0.0;
    int q = n - 1;
    for (int p = 0; p < n; p++) {
        A += pts[q].x * pts[p].y - pts[p].x * pts[q].y;
        q = p;
    }
    return float(A * 0.5);
}

bool Triangulator::InsideTriangle(const MVec2f& A, const MVec2f& B, const MVec2f& C, const MVec2f& P)
{
    MVec2f v0 = MVec2f(C.x - B.x, C.y - B.y);
    MVec2f v1 = MVec2f(A.x - C.x, A.y - C.y);
    MVec2f v2 = MVec2f(B.x - A.x, B.y - A.y);
    MVec2f ap = MVec2f(P.x - A.x, P.y - A.y);
    MVec2f bp = MVec2f(P.x - B.x, P.y - B.y);
    MVec2f cp = MVec2f(P.x - C.x, P.y - C.y);

    float c0 = v0.x * bp.y - v0.y * bp.x;
    float c1 = v1.x * cp.y - v1.y * cp.x;
    float c2 = v2.x * ap.y - v2.y * ap.x;
    return (c0 >= 0) && (c1 >= 0) && (c2 >= 0);
}

bool Triangulator::Snip(int u, int v, int w, int n, const Vector<int>& V) const
{
    const MVec2f& A = pts[V[u]];
    const MVec2f& B = pts[V[v]];
    const MVec2f& C = pts[V[w]];
    if (0.0f >= (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x))
        return false;
    for (int p = 0; p < n; p++) {
        if (p == u || p == v || p == w) continue;
        const MVec2f& P = pts[V[p]];
        if (InsideTriangle(A, B, C, P))
            return false;
    }
    return true;
}

END_UPP_NAMESPACE
