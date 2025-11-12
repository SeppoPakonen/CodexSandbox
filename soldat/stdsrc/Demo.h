#ifndef DEMO_H
#define DEMO_H

//*******************************************************************************
//                                                                              
//       Demo Unit for SOLDAT                                                   
//                                                                              
//       Copyright (c) 2002 Michal Marcinkowski          
//                                                                              
//*******************************************************************************

#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <cstdint>
#include "Vector.h"
#include "Sprites.h"
#include "Net.h"
#include "Constants.h"
#include "Version.h"

// Constants
const std::string DEMO_MAGIC = "SOLDEM";

// Structures
struct TDemoHeader {
    char Header[6];
    uint16_t Version;
    char MapName[161];  // 0-indexed array of 161 elements
    int32_t StartDate;
    int32_t TicksNum;
    
    TDemoHeader() {
        std::fill(Header, Header + 6, 0);
        Version = 0;
        std::fill(MapName, MapName + 161, 0);
        StartDate = 0;
        TicksNum = 0;
    }
};

// Forward declarations
class TDemo;
class TDemoRecorder;
#ifdef CLIENT_CODE
class TDemoPlayer;
#endif

class TDemo {
protected:
    std::shared_ptr<std::vector<uint8_t>> FDemoFile;
    TDemoHeader FDemoHeader;
    bool FActive;
    std::string FName;
    uint8_t FOldCam;

public:
    TDemo() : FActive(false), FName(""), FOldCam(0) {
        FDemoFile = std::make_shared<std::vector<uint8_t>>();
    }
    
    bool GetActive() const { return FActive; }
    void SetActive(bool value) { FActive = value; }
    const std::string& GetName() const { return FName; }
    const TDemoHeader& GetHeader() const { return FDemoHeader; }
};

class TDemoRecorder : public TDemo {
private:
    int32_t FTicksNum;
    
    int CreateDemoPlayer();
#ifndef SERVER_CODE
    void SaveCamera();
#endif

public:
    TDemoRecorder() : FTicksNum(0) {}
    
    bool StartRecord(const std::string& Filename);
    void StopRecord();
    void SaveRecord(const void* R, int Size);
    void SaveNextFrame();
#ifndef SERVER_CODE
    void SavePosition();
#endif
    
    int GetTicksNum() const { return FTicksNum; }
};

#ifdef CLIENT_CODE
class TDemoPlayer : public TDemo {
private:
    int32_t FSkipTo;

public:
    TDemoPlayer() : FSkipTo(0) {}
    
    bool OpenDemo(const std::string& Filename);
    void StopDemo();
    void ProcessDemo();
    void Position(int Ticks);
    
    int GetSkipTo() const { return FSkipTo; }
};
#endif

// Global variables
extern std::unique_ptr<TDemoRecorder> DemoRecorder;
#ifdef CLIENT_CODE
extern std::unique_ptr<TDemoPlayer> DemoPlayer;
#endif
extern uint16_t RSize;
extern uint8_t FreeCam;
extern uint8_t NoTexts;

namespace DemoImpl {
    inline bool TDemoRecorder::StartRecord(const std::string& Filename) {
        // Result := False;
        bool result = false;
        
#ifndef SERVER_CODE
        // if DemoPlayer.Active then Exit;
        if (DemoPlayer && DemoPlayer->GetActive()) {
            return false;
        }
#endif

        // Create new demo file (memory stream)
        FDemoFile = std::make_shared<std::vector<uint8_t>>();
        
        // TODO: Console message about recording demo
        // MainConsole.Console(...);
        
        size_t lastSlash = Filename.find_last_of("/\\");
        FName = (lastSlash != std::string::npos) ? 
                Filename.substr(lastSlash + 1) : Filename;
        FActive = true;
        FOldCam = 255;
        FTicksNum = 0;

        // Set up header
        std::fill(FDemoHeader.Header, FDemoHeader.Header + 6, 0);
        std::copy(DEMO_MAGIC.begin(), DEMO_MAGIC.end(), FDemoHeader.Header);
        FDemoHeader.TicksNum = 0;
        FDemoHeader.StartDate = /* TODO: DateTimeToUnix(Now) */ 0;
        FDemoHeader.Version = DEMO_VERSION;

        std::fill(FDemoHeader.MapName, FDemoHeader.MapName + 161, 0);
        // TODO: Copy map name to header
        // StringToArray(FDemoHeader.MapName, Map.Name);

        // Write header to file
        // TODO: Write header to memory stream
        // FDemoFile.WriteBuffer(FDemoHeader, SizeOf(FDemoHeader));
        
        int spriteID = CreateDemoPlayer();
        
        if (spriteID == MAX_SPRITES) {
            result = true;  // Success
        }
        
        return result;
    }

    inline void TDemoRecorder::StopRecord() {
        if (!FActive) {
            return;
        }

        // TODO: Console message
        // MainConsole.Console(...);

        // Remove demo player
        // Sprite[MAX_SPRITES].Kill;

        // Update header
        FDemoHeader.Version = DEMO_VERSION;
        FDemoHeader.TicksNum = FTicksNum;

        // Update map name in header
        std::fill(FDemoHeader.MapName, FDemoHeader.MapName + 161, 0);
        // TODO: Copy map name to header
        // StringToArray(FDemoHeader.MapName, Map.Name);

        // Save file
        // TODO: Save file to disk
        // FDemoFile.SaveToFile(UserDirectory + 'demos/' + FName);

        FActive = false;
        FName.clear();
        FDemoFile.reset();
    }

    inline int TDemoRecorder::CreateDemoPlayer() {
        // TODO: Implement demo player creation
        // This would create a special sprite for demo recording
        return -1;
    }

    inline void TDemoRecorder::SaveRecord(const void* R, int Size) {
        if (Size == 0) {
            return;
        }

        if (!FActive) {
            return;
        }

        // Write size first
        // FDemoFile.Write(Size, SizeOf(RSize));
        
        // Write actual data
        // FDemoFile.Write(R, Size);
    }

#ifndef SERVER_CODE
    inline void TDemoRecorder::SaveCamera() {
        // TODO: Implement camera saving
    }

    inline void TDemoRecorder::SavePosition() {
        // TODO: Implement position saving
    }
#endif

    inline void TDemoRecorder::SaveNextFrame() {
        if (!FActive) {
            return;
        }

        // Save record type
        RSize = 1;
        // FDemoFile.Write(RSize, SizeOf(RSize));

        // Save camera change
#ifndef SERVER_CODE
        if (FOldCam != CameraFollowSprite) {
            SaveCamera();
            FOldCam = CameraFollowSprite;
        }
#endif

        FTicksNum++;
    }

#ifdef CLIENT_CODE
    inline bool TDemoPlayer::OpenDemo(const std::string& Filename) {
        bool result = false;
        
        FDemoFile = std::make_shared<std::vector<uint8_t>>();
        
        // Load file into memory
        std::ifstream file(Filename, std::ios::binary);
        if (!file.is_open()) {
            // TODO: Log error
            return false;
        }
        
        file.seekg(0, std::ios::end);
        size_t fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        
        FDemoFile->resize(fileSize);
        file.read(reinterpret_cast<char*>(FDemoFile->data()), fileSize);
        file.close();
        
        // Read header
        size_t pos = 0;
        std::copy(FDemoFile->begin(), FDemoFile->begin() + sizeof(TDemoHeader), 
                  reinterpret_cast<uint8_t*>(&FDemoHeader));
        pos += sizeof(TDemoHeader);
        
        // Check header
        if (std::string(FDemoHeader.Header, 6) != DEMO_MAGIC) {
            // TODO: Log error
            return false;
        }
        
        if (FDemoHeader.Version != DEMO_VERSION) {
            // TODO: Log version error
            return false;
        }
        
        size_t lastSlash = Filename.find_last_of("/\\");
        FName = (lastSlash != std::string::npos) ? 
                Filename.substr(lastSlash + 1) : Filename;
        
        // TODO: Console message
        // MainConsole.Console(_('Playing demo') + ' ' + WideString(FName), INFO_MESSAGE_COLOR);
        
        // Spectator := 1;
        FActive = true;
        result = true;
        
        return result;
    }

    inline void TDemoPlayer::StopDemo() {
        if (!FActive) {
            return;
        }

        // TODO: Console message
        // MainConsole.Console('Demo stopped', INFO_MESSAGE_COLOR);

        FDemoFile.reset();

        FActive = false;
    }

    inline void TDemoPlayer::ProcessDemo() {
        // TODO: Implement demo processing
        // This would read demo data and replay the game state
    }

    inline void TDemoPlayer::Position(int Ticks) {
        FSkipTo = Ticks;
        // ShouldRenderFrames := False;

        if (FSkipTo < MainTickCounter) {
            // Reset demo position and game state
            // FDemoFile.Seek(SizeOf(FDemoHeader), soFromBeginning);
            // MainTickCounter := 0;
            // And reset all game objects...
        }

        // GOALTICKS := DEFAULT_GOALTICKS * 20;
    }
#endif
}

// Using declarations to bring into global namespace
using DemoImpl::TDemo;
using DemoImpl::TDemoRecorder;
#ifdef CLIENT_CODE
using DemoImpl::TDemoPlayer;
#endif

// Global variables
extern std::unique_ptr<TDemoRecorder> DemoRecorder;
#ifdef CLIENT_CODE
extern std::unique_ptr<TDemoPlayer> DemoPlayer;
#endif
extern uint16_t RSize = 0;
extern uint8_t FreeCam = 0;
extern uint8_t NoTexts = 0;

// Initialize global variables
namespace DemoImpl {
    inline void DemoInit() {
        DemoRecorder = std::make_unique<TDemoRecorder>();
#ifdef CLIENT_CODE
        DemoPlayer = std::make_unique<TDemoPlayer>();
#endif
    }
}

#endif // DEMO_H