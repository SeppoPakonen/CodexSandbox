/*
 * Stub for RakNetworkFactory.h
 * This is a minimal stub to allow compilation
 */

#ifndef RakNetworkFactory_h
#define RakNetworkFactory_h

#include "RakClientInterface.h"
#include "RakServerInterface.h"

class RakNetworkFactory {
public:
    static void* GetRakNetwork(void) { return 0; }
    static void DestroyRakNetwork(void* network) {}
    
    // Add client methods needed by gsnetcli.cpp
    static RakClientInterface* GetRakClientInterface(void) { return new RakClientInterface(); }
    static void DestroyRakClientInterface(RakClientInterface* i) { delete i; }
    
    // Add server methods needed by gsnetsrv.cpp
    static RakServerInterface* GetRakServerInterface(void) { return new RakServerInterface(); }
    static void DestroyRakServerInterface(RakServerInterface* i) { delete i; }
};

#endif