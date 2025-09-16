#include "ManiaCore.h"

NAMESPACE_UPP

String MD5Hex(const String& s)
{
    return MD5String(s);
}

String MD5Hex(const void* data, dword size)
{
    return MD5String(data, size);
}

END_UPP_NAMESPACE

