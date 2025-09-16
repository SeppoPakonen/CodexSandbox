#include "ManiaDemo.h"

using namespace Upp;

static void TestAlphaNum()
{
    Index<String> v;
    v << "a1" << "a10" << "a2" << "b1" << "a01" << "a001" << "a0" << "a" << "a0001";
    Sort(v, AlphaNumLess());
    Cout() << "AlphaNum sorted:" << '\n';
    for (const auto& s : v)
        Cout() << s << '\n';
    // Basic ordering expectations
    int ia = v.Find("a");
    int ia0 = v.Find("a0");
    int ia01 = v.Find("a01");
    int ia001 = v.Find("a001");
    int ia0001 = v.Find("a0001");
    int ia1 = v.Find("a1");
    int ia2 = v.Find("a2");
    int ia10 = v.Find("a10");
    ASSERT(ia < ia0 && ia0 < ia01 && ia01 < ia001 && ia001 < ia0001 && ia0001 < ia1);
    ASSERT(ia1 < ia2 && ia2 < ia10);
}

static void TestAlphaNumI()
{
    Index<String> v;
    v << "A1" << "a10" << "a2" << "B1" << "a01" << "A001" << "a0" << "a" << "a0001";
    Sort(v, AlphaNumLessI());
    // Expect natural order ignoring case: a, a0, a01, a001, a0001, a1, a2, a10, b1
    int ia = v.Find("a");
    int ia0 = v.Find("a0");
    int ia01 = v.Find("a01");
    int ia001 = v.Find("A001");
    int ia0001 = v.Find("a0001");
    int ia1 = v.Find("A1");
    int ia2 = v.Find("a2");
    int ia10 = v.Find("a10");
    int ib1 = v.Find("B1");
    ASSERT(ia < ia0 && ia0 < ia01 && ia01 < ia001 && ia001 < ia0001 && ia0001 < ia1);
    ASSERT(ia1 < ia2 && ia2 < ia10 && ia10 < ib1);
}

static void DemoCurvy()
{
    CurvySpline s;
    s.AddPoint(MVec3f(0,0,0));
    s.AddPoint(MVec3f(1,2,0));
    s.AddPoint(MVec3f(3,3,0));
    s.AddPoint(MVec3f(4,0,0));
    s.SetGranularity(10);
    Vector<MVec3f> approx = s.GetApproximation();
    Cout() << "CurvySpline approx points: " << approx.GetCount() << '\n';
    // Catmull-Rom
    s.SetInterpolationCatmullRom();
    MVec3f p05_cr = s.Interpolate(0.5f);
    Cout() << Format("CR  P(0.5)=(%.3f, %.3f, %.3f)\n", p05_cr.x, p05_cr.y, p05_cr.z);
    // TCB with some parameters
    s.SetInterpolationTCB(0.3f, 0.2f, -0.1f);
    MVec3f p05_tcb = s.Interpolate(0.5f);
    Cout() << Format("TCB P(0.5)=(%.3f, %.3f, %.3f) (t=0.3 c=0.2 b=-0.1)\n", p05_tcb.x, p05_tcb.y, p05_tcb.z);
    // restore Catmull-Rom for later tests
    s.SetInterpolationCatmullRom();
    // Closed curve test with per-point TCB
    CurvySpline sc;
    sc.AddPoint(MVec3f(0,0,0));
    sc.AddPoint(MVec3f(2,0,0));
    sc.AddPoint(MVec3f(2,2,0));
    sc.AddPoint(MVec3f(0,2,0));
    sc.SetClosed(true);
    sc.SetGranularity(8);
    sc.SetInterpolationTCB(0.0f, 0.0f, 0.0f);
    // Sharpen one corner
    sc.SetPointTCB(1, 0.6f, 0.0f, 0.0f);
    double Lc = sc.GetLength();
    ASSERT(Lc > 0);
    // Verify wrap behavior: Interpolate(0) equals Interpolate(1)
    MVec3f c0 = sc.Interpolate(0.0f);
    MVec3f c1 = sc.Interpolate(1.0f);
    ASSERT(fabs(c0.x - c1.x) < 1e-5 && fabs(c0.y - c1.y) < 1e-5 && fabs(c0.z - c1.z) < 1e-5);

    // LUT invert sanity on a curved spline
    s.SetGranularity(20);
    s.SetInterpolationCatmullRom();
    {
        double tests[] = {0.1, 0.3, 0.5, 0.7, 0.9};
        for (double tt : tests) {
            double d = s.TFToDistance((float)tt);
            float t2 = s.DistanceToTF(d);
            ASSERT(fabs(t2 - tt) < 2e-2);
        }
    }

    // Length sanity checks on straight line (2 points)
    CurvySpline line;
    line.AddPoint(MVec3f(0,0,0));
    line.AddPoint(MVec3f(10,0,0));
    line.SetGranularity(10);
    double L = line.GetLength();
    ASSERT(fabs(L - 10.0) < 1e-6);
    ASSERT(fabs(line.TFToDistance(0.25f) - 2.5) < 1e-3);
    ASSERT(fabs(line.TFToDistance(0.75f) - 7.5) < 1e-3);
    ASSERT(fabs(line.DistanceToTF(2.5) - 0.25f) < 1e-3);
    ASSERT(fabs(line.DistanceToTF(7.5) - 0.75f) < 1e-3);

    // Nearest TF on line
    float tfnp = line.GetNearestPointTF(MVec3f(2.5f, 1.0f, 0.0f));
    ASSERT(fabs(tfnp - 0.25f) < 1e-3);

    // MoveBy with clamp/loop/pingpong
    float t = 0.25f; int dir = +1;
    line.MoveBy(t, dir, 2.5, Clamp);
    ASSERT(fabs(t - 0.5f) < 1e-3 && dir == +1);
    t = 0.95f; dir = +1; line.MoveBy(t, dir, 10.0, Clamp);
    ASSERT(fabs(t - 1.0f) < 1e-6);
    t = 0.95f; dir = +1; line.MoveBy(t, dir, 10.0, Loop);
    ASSERT(t <= 1.0f && t >= 0.0f);
    t = 0.95f; dir = +1; line.MoveBy(t, dir, 10.0, PingPong);
    ASSERT(dir == -1);

    // Fast MoveBy on closed curve
    double Ls = sc.GetLength();
    float ts = 0.9f; int ds = +1; sc.MoveByFast(ts, ds, Ls * 0.25, Loop);
    ASSERT(ts >= 0.0f && ts <= 1.0f);
    // PingPong on closed: direction flips when exceeding cycle
    ts = 0.9f; ds = +1; sc.MoveByFast(ts, ds, Ls * 1.2, PingPong);
    ASSERT(ds == -1);
    // Constant-step MoveBy along LUT indices
    float tstep = 0.0f; int dstep = +1; sc.MoveByFastSteps(tstep, dstep, 5, Loop);
    ASSERT(tstep >= 0.0f && tstep <= 1.0f);
}

static void TestTriangulator2D()
{
    Vector<MVec2f> poly;
    // Square (CCW)
    poly << MVec2f(0,0) << MVec2f(2,0) << MVec2f(2,2) << MVec2f(0,2);
    Triangulator tr(poly);
    Vector<int> idx = tr.Triangulate();
    Cout() << "Tri indices (square):" << '\n';
    for (int i = 0; i < idx.GetCount(); i += 3)
        Cout() << idx[i] << ", " << idx[i+1] << ", " << idx[i+2] << '\n';
    ASSERT(idx.GetCount() == 6); // 2 triangles
    double area = PolygonArea(poly);
    ASSERT(fabs(area - 4.0) < 1e-6);
}

static void TestTriangulator3D()
{
    Vector<MVec3f> poly;
    // Square on XY plane, z=0
    poly << MVec3f(0,0,0) << MVec3f(2,0,0) << MVec3f(2,2,0) << MVec3f(0,2,0);
    Triangulator tr(poly, /*ignore_axis*/2); // project to XY
    Vector<int> idx = tr.Triangulate();
    ASSERT(idx.GetCount() == 6);
}

static void TestTriangulatorConcave()
{
    Vector<MVec2f> poly;
    // Simple concave polygon (arrow-like), CCW
    poly << MVec2f(0,0) << MVec2f(3,0) << MVec2f(3,1) << MVec2f(1.5f,1) << MVec2f(3,2) << MVec2f(3,3) << MVec2f(0,3);
    Triangulator tr(poly);
    Vector<int> idx = tr.Triangulate();
    ASSERT(idx.GetCount() == (poly.GetCount() - 2) * 3);
}

CONSOLE_APP_MAIN
{
    StdLogSetup(LOG_COUT|LOG_FILE);
    Cout() << "Mania demo starting..." << '\n';
    bool run_alpha = true, run_alphai = true, run_geom = true, run_curvy = true, run_prefs = true;
    const Vector<String>& args = CommandLine();
    if (args.GetCount() > 0) {
        run_alpha = run_alphai = run_geom = run_curvy = run_prefs = false;
        for (const String& a : args) {
            if (a == "all") { run_alpha = run_alphai = run_geom = run_curvy = run_prefs = true; break; }
            if (a == "alpha") run_alpha = true;
            else if (a == "alphai") run_alphai = true;
            else if (a == "geom" || a == "tri") run_geom = true;
            else if (a == "curvy") run_curvy = true;
            else if (a == "prefs") run_prefs = true;
            else {
                Cout() << "Unknown arg: " << a << " (use: all|alpha|alphai|geom|curvy|prefs)" << '\n';
            }
        }
    }
    if (run_alpha) TestAlphaNum();
    if (run_alphai) TestAlphaNumI();
    if (run_geom) { TestTriangulator2D(); TestTriangulator3D(); TestTriangulatorConcave(); }
    if (run_curvy) DemoCurvy();
    if (run_prefs) {
        Prefs.Clear();
        Prefs.Set("alpha", "beta");
        String ini = ConfigFile("ManiaDemo.ini");
        Prefs.Save(ini);
        Prefs.Clear();
        Prefs.Load(ini);
        ASSERT(Prefs.Get("alpha") == "beta");
    }
    Cout() << "All checks passed." << '\n';
}
