#include "ManiaGeometry.h"

NAMESPACE_UPP

double PolygonArea(const Vector<MVec2f>& pts)
{
    int n = pts.GetCount();
    if (n < 3) return 0.0;
    double A = 0.0;
    int q = n - 1;
    for (int p = 0; p < n; ++p) {
        A += (double)pts[q].x * (double)pts[p].y - (double)pts[p].x * (double)pts[q].y;
        q = p;
    }
    return 0.5 * A;
}

bool IsCCW(const Vector<MVec2f>& pts)
{
    return PolygonArea(pts) > 0.0;
}

bool IsConvex(const Vector<MVec2f>& pts)
{
    int n = pts.GetCount();
    if (n < 3) return false;
    bool sign = false;
    for (int i = 0; i < n; ++i) {
        const MVec2f& a = pts[i];
        const MVec2f& b = pts[(i + 1) % n];
        const MVec2f& c = pts[(i + 2) % n];
        float z = (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);
        if (i == 0)
            sign = z > 0;
        else {
            if ((z > 0) != sign && z != 0)
                return false;
        }
    }
    return IsCCW(pts);
}

END_UPP_NAMESPACE

