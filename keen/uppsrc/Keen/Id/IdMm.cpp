#include "Id.h"


NAMESPACE_UPP

void MM_GetPtr(mm_ptr_t* pptr, int length) {
    if (!pptr || length <= 0) {
        if (pptr) *pptr = nullptr;
        return;
    }
    void* p = std::malloc((size_t)length);
    *pptr = p;
}

void MM_FreePtr(mm_ptr_t* pptr) {
    if (!pptr) return;
    if (*pptr) std::free(*pptr);
    *pptr = nullptr;
}

END_UPP_NAMESPACE

