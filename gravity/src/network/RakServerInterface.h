/*
 * Stub for RakServerInterface.h
 * This is a minimal stub to allow compilation
 */

#ifndef RakServerInterface_h
#define RakServerInterface_h

#include "NetworkCore.h"
#include "BitStream.h"

class RakServerInterface {
public:
    RakServerInterface() {}
    ~RakServerInterface() {}
    
    // Methods needed by gsnetsrv.cpp
    // Signature matching the actual call in gsnetsrv.cpp: Start(MAX_NET_PLAYERS, 0, 1, SERVER_PORT)
    bool Start(int AllowedPlayers, int connectionValidationInteger, int threadSleepTimer, int port) { return false; }
    void Disconnect(unsigned int blockDuration = 0, unsigned char orderingChannel = 0) {}
    bool Send(const char* data, int length, unsigned char priority, char reliability, char orderingChannel, PlayerID playerId, bool broadcast) { return true; }
    bool Send(class BitStream *bitStream, unsigned char priority, char reliability, char orderingChannel, PlayerID playerId, bool broadcast) { return true; }
    void PushBackPacket(Packet* packet, bool pushAtHead) {}
    void DeallocatePacket(Packet* packet) {}
    int GetAveragePing(PlayerID playerId) { return 0; }
    int GetLastPing(PlayerID playerId) { return 0; }
    int GetLowestPing(PlayerID playerId) { return 0; }
    void Kick(PlayerID playerId) {}
    
    // Additional methods needed by gsnetsrv.cpp
    void SetAllowedPlayers(int maxPlayers) {}
    void StartOccasionalPing(void) {}
    void StartSynchronizedRandomInteger(void) {}
    int GetConnectedPlayers(void) { return 0; }
    void* GetStatistics(PlayerID playerId) { return 0; }
    
    // Return the number of connected players
    unsigned int NumberOfConnectedClients(void) { return 0; }
    unsigned int GetMaximumNumberOfPeers(void) { return 0; }
    
    // Add synchronized random integer method
    unsigned int GetSynchronizedRandomInteger(void) { return 0; }
    
    // Add missing Receive method
    Packet* Receive(void) { return 0; }
};

// Define UNASSIGNED_PLAYER_ID and other constants
static PlayerID UNASSIGNED_PLAYER_ID = {0, 0};

#endif