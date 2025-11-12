#ifndef LOBBY_CLIENT_H
#define LOBBY_CLIENT_H

//*******************************************************************************
//                                                                              
//       LobbyClient Unit for SOLDAT                                              
//                                                                              
//       Copyright (c) 2012 Daniel Forssten            
//                                                                              
//*******************************************************************************

#include <string>
#include <thread>
#include <memory>
#include <iostream>
#include <sstream>
#include <json/json.h>  // Using JSON library like nlohmann/json or fpjson equivalent
#include "Constants.h"
#include "Server.h"
#include "Game.h"
#include "Sprites.h"
#include "Version.h"
#include "Weapons.h"
#include "Net.h"

struct TLobbyThread {
private:
    std::string FData;
    std::string FURL;
    // HTTP client would be defined here depending on the library used

public:
    TLobbyThread();
    ~TLobbyThread();
    
    void Execute();
    void Start();
    void Terminate();
};

namespace LobbyClientImpl {
    inline int GetOS() {
#ifdef _WIN32
        return 0;  // Windows
#elif __linux__
        return 1;  // Linux
#elif __APPLE__
        return 2;  // Darwin/MacOS
#else
        return -1; // Unknown OS
#endif
    }
    
    inline TLobbyThread::TLobbyThread() {
        // Initialize and prepare lobby registration
        Debug("[Lobby] Registering the server in lobby");
        FURL = sv_lobbyurl.Value() + "/v0/servers/register";

        Json::Value json;
        Json::StreamWriterBuilder builder;
        std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

        // Add fields to JSON object
        json["AC"] = 
#ifdef ENABLE_FAE_CODE
            ac_enable.Value();  // Assuming this Cvar exists
#else
            false;
#endif
        json["AuthMode"] = 0;
        json["Advanced"] = sv_advancemode.Value();
        json["BonusFreq"] = sv_bonus_frequency.Value();
        json["ConnectionType"] = 0;
        json["CurrentMap"] = Map.Name;
        json["GameStyle"] = sv_gamemode.Value();
        json["Info"] = sv_info.Value();
        json["MaxPlayers"] = sv_maxplayers.Value();
        json["Modded"] = !fs_mod.Value().empty();
        json["Name"] = sv_hostname.Value();
        json["NumBots"] = BotsNum;
        json["NumPlayers"] = PlayersNum;
        json["OS"] = GetOS();
        json["Port"] = net_port.Value();
        json["Private"] = !sv_password.Value().empty();
        json["Realistic"] = sv_realisticmode.Value();
        json["Respawn"] = sv_respawntime.Value();
        json["Survival"] = sv_survivalmode.Value();
        json["Version"] = SOLDAT_VERSION;
        json["WM"] = (LoadedWMChecksum != DefaultWMChecksum);
        
        Json::Value jsonPlayers(Json::arrayValue);
        for (int i = 1; i <= MAX_PLAYERS; i++) {
            if (Sprite[i].Active) {
                jsonPlayers.append(Sprite[i].Player.Name);
            }
        }
        json["Players"] = jsonPlayers;
        
        std::ostringstream oss;
        writer->write(json, &oss);
        FData = oss.str();
    }
    
    inline void TLobbyThread::Execute() {
        // Implementation would use an HTTP client to POST data to the lobby server
        // This is a simplified version since we don't have a specific HTTP client implementation
        // In a real implementation, this would make an HTTP request to the lobby server
        
        try {
            // HTTP client implementation would go here
            // For example, using curl or a similar library:
            // curl_easy_setopt(curl, CURLOPT_URL, FURL.c_str());
            // curl_easy_setopt(curl, CURLOPT_POSTFIELDS, FData.c_str());
            // curl_easy_setopt(curl, CURLOPT_USERAGENT, ("soldatserver/" + SOLDAT_VERSION).c_str());
            // curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            // curl_easy_perform(curl);
            
            // This is just a placeholder implementation
            std::cout << "[Lobby] Registering server: " << sv_hostname.Value() << std::endl;
            std::cout << "[Lobby] Map: " << Map.Name << ", Players: " << PlayersNum << std::endl;
            
            // In a real implementation, we'd parse the HTTP response and check status codes
            if (true) { // Placeholder for successful HTTP status code (204)
                std::cout << "[Lobby] Registration successful" << std::endl;
            } else {
                std::cout << "[Lobby] Registration failed" << std::endl;
            }
        } catch (const std::exception& e) {
            Debug("[Lobby] Lobby register has failed: " + std::string(e.what()));
        }
        // Terminate this thread
    }
    
    inline void TLobbyThread::Start() {
        // Start the execution thread
        std::thread t(&TLobbyThread::Execute, this);
        t.detach(); // Detach thread to run independently
    }
    
    inline void TLobbyThread::Terminate() {
        // Thread would terminate naturally after Execute completes
    }
    
    inline TLobbyThread::~TLobbyThread() {
        // Cleanup resources when object is destroyed
    }
}

// Using declarations to bring functions into global namespace
using LobbyClientImpl::TLobbyThread;
using LobbyClientImpl::GetOS;

#endif // LOBBY_CLIENT_H