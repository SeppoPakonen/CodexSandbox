/*
 * Stub for NetworkTypes.h
 * This is a minimal stub to allow compilation
 */

#ifndef NetworkTypes_h
#define NetworkTypes_h

// Basic network type definitions
typedef unsigned int NetworkID;
typedef unsigned char NetworkByte;
typedef unsigned short NetworkPort;

enum NetworkMessageID {
    ID_CONNECTED,
    ID_DISCONNECTED,
    ID_FAILED,
    ID_MESSAGE
};

#endif