/*
 * Stub for RakClientInterface.h
 * This is a minimal stub to allow compilation
 */

#ifndef RakClientInterface_h
#define RakClientInterface_h

#include "NetworkCore.h"
#include "BitStream.h"

class RakClientInterface {
public:
    RakClientInterface() {}
    ~RakClientInterface() {}
    
    bool Connect(const char* serverIP, unsigned short serverPort, const char* clientIP, unsigned short clientPort) { return false; }
    bool Connect(const char* host, unsigned short serverPort, unsigned short clientPort, unsigned int connectionValidationInteger, int timeoutMS) { return false; }
    void Disconnect(unsigned int blockDuration = 0, unsigned char orderingChannel = 0) {}
    void PingServer(void) {}
    void PingServer(const char* host, unsigned short remotePort, unsigned short sendPort) {}
    int GetAveragePing(void) { return 0; }
    int GetLastPing(void) { return 0; }
    int GetLowestPing(void) { return 0; }
    bool Send(const char* data, int length, unsigned char priority, char reliability, char orderingChannel, PlayerID playerId, bool broadcast) { return true; }
    bool Send(class BitStream *bitStream, unsigned char priority, char reliability, char orderingChannel) { return true; }
    void PushBackPacket(Packet* packet, bool pushAtHead) {}
    
    // Add missing methods needed by gsnetcli.cpp
    void StartOccasionalPing(void) {}
    bool IsConnected(void) { return false; }
    PlayerID GetPlayerID(void) { PlayerID id = {0, 0}; return id; }
    PlayerID GetServerID(void) { PlayerID id = {0, 0}; return id; }
    Packet* Receive(void) { return NULL; }
    void DeallocatePacket(Packet *packet) {}
    const char* PlayerIDToDottedIP(PlayerID playerId) { return "0.0.0.0"; }
    
    // Add missing method
    unsigned int GetSynchronizedRandomInteger() { return 0; }
};

#endif