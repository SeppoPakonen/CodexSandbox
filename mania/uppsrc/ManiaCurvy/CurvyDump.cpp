#include "ManiaCurvy.h"

NAMESPACE_UPP

void DumpSplineCSV(const CurvySpline& s, const String& path, bool with_t)
{
    FileOut out(path);
    if (!out) return;
    Vector<MVec3f> pts = s.GetApproximation();
    if (with_t) out << "t,x,y,z\n";
    int n = pts.GetCount();
    if (n == 0) return;
    for (int i = 0; i < n; ++i) {
        float t = (float)i / (float)(n - 1);
        const MVec3f& p = pts[i];
        if (with_t)
            out << Format("%.6f,%.6f,%.6f,%.6f\n", (double)t, (double)p.x, (double)p.y, (double)p.z);
        else
            out << Format("%.6f,%.6f,%.6f\n", (double)p.x, (double)p.y, (double)p.z);
    }
}

END_UPP_NAMESPACE

