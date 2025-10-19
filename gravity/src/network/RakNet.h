/*
 * Stub for RakNet.h
 * This is a minimal stub to allow compilation
 */

#ifndef RakNet_h
#define RakNet_h

// Simple stub for RakNet namespace and functions
namespace RakNet {
    // Time functions
    inline unsigned int GetTimeMS(void) { return 0; }
    inline unsigned long long GetTimeUS(void) { return 0; }
    
    // String compressor class
    class StringCompressor {
    public:
        static StringCompressor* Instance(void) { 
            static StringCompressor instance;
            return &instance; 
        }
        
        void EncodeString(const char* input, int maxCharsToWrite, class BitStream *output) {}
        void DecodeString(char* output, int maxCharsToRead, class BitStream *input) {}
    };
}

// Typedefs for compatibility
typedef unsigned int RakNetTime;
typedef unsigned long long RakNetTimeUS;

// Global functions in RakNet namespace
inline unsigned int RakNetGetTime(void) { return 0; }

#endif