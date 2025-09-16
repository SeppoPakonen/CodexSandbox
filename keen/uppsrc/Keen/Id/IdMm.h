// Minimal memory manager shims for the Keen port
#pragma once

// Keep compatibility typedefs for FS helpers
typedef void* mm_ptr_t;

// Basic allocation helpers
void MM_GetPtr(mm_ptr_t* pptr, int length);
void MM_FreePtr(mm_ptr_t* pptr);

