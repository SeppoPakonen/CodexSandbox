/*
 * Core network types for Gravity Strike
 */

#ifndef NetworkCore_h
#define NetworkCore_h

struct PlayerID {
    unsigned int binaryAddress;
    unsigned short port;
};

struct Packet {
    PlayerID playerId;
    unsigned int length;
    int bitSize;
    char* data;
};

// Add comparison operator for PlayerID
inline bool operator==(const PlayerID& a, const PlayerID& b) {
    return (a.binaryAddress == b.binaryAddress) && (a.port == b.port);
}

// Add inequality operator
inline bool operator!=(const PlayerID& a, const PlayerID& b) {
    return !(a == b);
}

#endif