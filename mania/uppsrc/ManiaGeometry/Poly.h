#pragma once

// Internal header; aggregated by ManiaGeometry.h

double PolygonArea(const Vector<MVec2f>& pts); // signed area (CCW positive)
bool   IsCCW(const Vector<MVec2f>& pts);
bool   IsConvex(const Vector<MVec2f>& pts); // returns true if polygon is strictly convex and CCW

