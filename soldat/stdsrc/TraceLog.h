#ifndef TRACE_LOG_H
#define TRACE_LOG_H

//*******************************************************************************
//                                                                              
//       TraceLog Unit                                                          
//                                                                              
//       Copyright (c) 2012 Daniel Forssten             
//                                                                              
//*******************************************************************************

#include <string>
#include <iostream>

const int LEVEL_OFF = 0;
const int LEVEL_DEBUG = 1;
const int LEVEL_TRACE = 2;

void Debug(const std::string& Msg);
void Trace(const std::string& Msg);
#ifdef STEAM_CODE
extern "C" void SteamWarning(int Severity, const char* WarnMessage);
#endif

namespace TraceLogImpl {
    inline void Debug(const std::string& Msg) {
        // This would check log_level.Value() >= LEVEL_DEBUG
        // For now, a simplified version
        std::cout << Msg << std::endl;
    }

    inline void Trace(const std::string& Msg) {
        // This would check log_level.Value() >= LEVEL_TRACE
        // For now, a simplified version
        std::cout << Msg << std::endl;
    }

#ifdef STEAM_CODE
    extern "C" inline void SteamWarning(int Severity, const char* WarnMessage) {
        std::cout << "[Steam] " << WarnMessage << " Severity:" << Severity << std::endl;
    }
#endif
}

// Using declarations to bring into global namespace
using TraceLogImpl::Debug;
using TraceLogImpl::Trace;
using TraceLogImpl::SteamWarning;

#endif // TRACE_LOG_H