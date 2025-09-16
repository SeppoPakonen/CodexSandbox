#include "ManiaCore.h"

NAMESPACE_UPP

static inline bool IsDigit(byte c) {
    return c >= '0' && c <= '9';
}

static inline byte Lower(byte c) {
    if (c >= 'A' && c <= 'Z') return c + ('a' - 'A');
    return c;
}

static int CmpChunk(const String& a, int& i, const String& b, int& j) {
    // Compare a chunk (either numeric block or non-numeric block)
    int na = a.GetCount();
    int nb = b.GetCount();
    if (i >= na || j >= nb)
        return (na - i) - (nb - j);

    bool dig = IsDigit((byte)a[i]) && IsDigit((byte)b[j]);

    if (dig) {
        // Skip leading zeros and count digits
        int ia = i, ib = j;
        int za = 0, zb = 0;
        while (ia < na && a[ia] == '0') { ia++; za++; }
        while (ib < nb && b[ib] == '0') { ib++; zb++; }
        int sa = ia, sb = ib;
        while (ia < na && IsDigit((byte)a[ia])) ia++;
        while (ib < nb && IsDigit((byte)b[ib])) ib++;
        int lena = ia - sa;
        int lenb = ib - sb;
        if (lena != lenb)
            return lena < lenb ? -1 : 1;
        // Same length: lexicographic compare digits
        for (int k = 0; k < lena; k++) {
            byte ca = (byte)a[sa + k];
            byte cb = (byte)b[sb + k];
            if (ca != cb)
                return ca < cb ? -1 : 1;
        }
        // Equal numeric values; tie-breaker on leading zero count (shorter runs first)
        if (za != zb)
            return za > zb ? -1 : 1; // "0001" < "01" < "1" -> prefer fewer leading zeros
        i = ia;
        j = ib;
        return 0;
    }
    else {
        // Non-digit chunk: compare until digit encountered or string end
        int ia = i, ib = j;
        while (ia < na && ib < nb) {
            bool da = IsDigit((byte)a[ia]);
            bool db = IsDigit((byte)b[ib]);
            if (da || db)
                break;
            byte ca = (byte)a[ia];
            byte cb = (byte)b[ib];
            if (ca != cb)
                return ca < cb ? -1 : 1;
            ia++; ib++;
        }
        i = ia; j = ib;
        return 0;
    }
}

int AlphaNumCompare(const String& a, const String& b)
{
    int i = 0, j = 0;
    int na = a.GetCount(), nb = b.GetCount();
    while (i < na && j < nb) {
        int c = CmpChunk(a, i, b, j);
        if (c)
            return c;
    }
    if (i == na && j == nb)
        return 0;
    return (na - i) < (nb - j) ? -1 : ((na - i) > (nb - j) ? 1 : 0);
}

static int CmpChunkI(const String& a, int& i, const String& b, int& j) {
    int na = a.GetCount();
    int nb = b.GetCount();
    if (i >= na || j >= nb)
        return (na - i) - (nb - j);

    bool dig = IsDigit((byte)a[i]) && IsDigit((byte)b[j]);

    if (dig) {
        int ia = i, ib = j;
        int za = 0, zb = 0;
        while (ia < na && a[ia] == '0') { ia++; za++; }
        while (ib < nb && b[ib] == '0') { ib++; zb++; }
        int sa = ia, sb = ib;
        while (ia < na && IsDigit((byte)a[ia])) ia++;
        while (ib < nb && IsDigit((byte)b[ib])) ib++;
        int lena = ia - sa;
        int lenb = ib - sb;
        if (lena != lenb)
            return lena < lenb ? -1 : 1;
        for (int k = 0; k < lena; k++) {
            byte ca = (byte)a[sa + k];
            byte cb = (byte)b[sb + k];
            if (ca != cb)
                return ca < cb ? -1 : 1;
        }
        if (za != zb)
            return za > zb ? -1 : 1;
        i = ia;
        j = ib;
        return 0;
    }
    else {
        int ia = i, ib = j;
        while (ia < na && ib < nb) {
            bool da = IsDigit((byte)a[ia]);
            bool db = IsDigit((byte)b[ib]);
            if (da || db)
                break;
            byte ca = Lower((byte)a[ia]);
            byte cb = Lower((byte)b[ib]);
            if (ca != cb)
                return ca < cb ? -1 : 1;
            ia++; ib++;
        }
        i = ia; j = ib;
        return 0;
    }
}

int AlphaNumCompareI(const String& a, const String& b)
{
    int i = 0, j = 0;
    int na = a.GetCount(), nb = b.GetCount();
    while (i < na && j < nb) {
        int c = CmpChunkI(a, i, b, j);
        if (c)
            return c;
    }
    if (i == na && j == nb)
        return 0;
    return (na - i) < (nb - j) ? -1 : ((na - i) > (nb - j) ? 1 : 0);
}

END_UPP_NAMESPACE
