#pragma once

// Internal header; aggregated by ManiaCore.h

String TimeToStr(double s, bool draw_sign = false, bool skip = false);
String Ordinal(int number);

template <class T>
inline T Mod(T a, T n) { return (a % n + n) % n; }

