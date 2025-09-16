#include "ManiaCore.h"

NAMESPACE_UPP

String TimeToStr(double s, bool draw_sign, bool skip)
{
    if (skip)
        return AsString((int)s);
    double a = fabs(s);
    int minutes = int(a) / 60;
    int seconds = int(a) % 60;
    int hundredths = int(fmod(a, 1.0) * 100.0);
    String sign;
    if (s < 0)
        sign = "-";
    else if (draw_sign)
        sign = "+";
    return sign + AsString(minutes) + ":" + Format("%02d", seconds) + "." + Format("%02d", hundredths);
}

String Ordinal(int number)
{
    int n = abs(number);
    int d = n % 10;
    int t = (n / 10) % 10;
    const char* suf = "th";
    if (t != 1) {
        if (d == 1) suf = "st";
        else if (d == 2) suf = "nd";
        else if (d == 3) suf = "rd";
    }
    return AsString(number) + suf;
}

END_UPP_NAMESPACE
