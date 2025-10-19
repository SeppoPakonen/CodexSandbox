/*
 * Stub for UtilityFunctions.h
 * This is a minimal stub to provide missing utility functions
 */

#ifndef UtilityFunctions_h
#define UtilityFunctions_h

// Include the allegro wrapper to get PACKFILE definition
#include "../allegro5_wrapper.h"

// File utility functions
inline int file_size(const char* filename) { return 0; }
inline void delete_file(const char* filename) {}

// String utility functions
inline char* get_extension(const char* filename) { 
    static char ext[10] = "txt";
    return ext; 
}

inline char* strlwr(char* str) {
    return str;
}

inline char* strupr(char* str) {
    return str;
}

// Pack file functions (stubbed)
inline PACKFILE* pack_fopen(const char* filename, const char* mode) { return nullptr; }
inline int pack_fread(void* ptr, size_t size, size_t count, PACKFILE* stream) { return 0; }
inline int pack_fclose(PACKFILE* stream) { return 0; }

#endif