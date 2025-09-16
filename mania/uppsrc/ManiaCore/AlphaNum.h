#pragma once

// Internal header: no includes; aggregated by ManiaCore.h under NAMESPACE_UPP

// Natural/alphanumeric comparator utilities

int AlphaNumCompare(const String& a, const String& b);
int AlphaNumCompareI(const String& a, const String& b);

struct AlphaNumLess {
    bool operator()(const String& a, const String& b) const { return AlphaNumCompare(a, b) < 0; }
};

struct AlphaNumLessI {
    bool operator()(const String& a, const String& b) const { return AlphaNumCompareI(a, b) < 0; }
};
