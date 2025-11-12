#ifndef GAME_H
#define GAME_H

//*******************************************************************************
//                                                                              
//       Game Unit                                                              
//                                                                              
//       Copyright (c) 2012-2013 Gregor A. Cieslak      
//                                                                              
//*******************************************************************************

#include "Vector.h"
#include "Constants.h"
#include "PolyMap.h"
#include "Parts.h"
#include "Sprites.h"
#include "Bullets.h"
#include "Things.h"
#include "Waypoints.h"
#include "Anims.h"
#include "Weapons.h"
#include "Sha1.h"
#include "Util.h"
#include <vector>
#include <string>

// Forward declarations
struct TSHA1Digest;
struct TMapInfo;
struct ParticleSystem;

// Structures
struct TKillSort {
    int Kills;
    int Deaths;
    uint8_t Flags;
    int PlayerNum;
    uint32_t Color;
};

// Declare global variables
extern int Ticks;
extern int TicksPerSecond;
extern int Frames;
extern int FramesPerSecond;
extern int TickTime;
extern int TickTimeLast;
extern int GOALTICKS;

extern int BulletTimeTimer;

// Particle systems
extern ParticleSystem SpriteParts;
extern ParticleSystem BulletParts;
extern ParticleSystem SparkParts;
extern ParticleSystem GostekSkeleton;
extern ParticleSystem BoxSkeleton;
extern ParticleSystem FlagSkeleton;
extern ParticleSystem ParaSkeleton;
extern ParticleSystem StatSkeleton;
extern ParticleSystem RifleSkeleton10;
extern ParticleSystem RifleSkeleton11;
extern ParticleSystem RifleSkeleton18;
extern ParticleSystem RifleSkeleton22;
extern ParticleSystem RifleSkeleton28;
extern ParticleSystem RifleSkeleton36;
extern ParticleSystem RifleSkeleton37;
extern ParticleSystem RifleSkeleton39;
extern ParticleSystem RifleSkeleton43;
extern ParticleSystem RifleSkeleton50;
extern ParticleSystem RifleSkeleton55;

// Animations
extern TAnimation Run;
extern TAnimation Stand;
extern TAnimation RunBack;
extern TAnimation Jump;
extern TAnimation JumpSide;
extern TAnimation Roll;
extern TAnimation RollBack;
extern TAnimation Fall;
extern TAnimation Crouch;
extern TAnimation CrouchRun;
extern TAnimation CrouchRunBack;
extern TAnimation Reload;
extern TAnimation Throw;
extern TAnimation Recoil;
extern TAnimation Shotgun;
extern TAnimation Barret;
extern TAnimation SmallRecoil;
extern TAnimation AimRecoil;
extern TAnimation HandsUpRecoil;
extern TAnimation ClipIn;
extern TAnimation ClipOut;
extern TAnimation SlideBack;
extern TAnimation Change;
extern TAnimation ThrowWeapon;
extern TAnimation WeaponNone;
extern TAnimation Punch;
extern TAnimation ReloadBow;
extern TAnimation Melee;
extern TAnimation Cigar;
extern TAnimation Match;
extern TAnimation Smoke;
extern TAnimation Wipe;
extern TAnimation Groin;
extern TAnimation TakeOff;
extern TAnimation Victory;
extern TAnimation Piss;
extern TAnimation Mercy;
extern TAnimation Mercy2;
extern TAnimation Own;
extern TAnimation Prone;
extern TAnimation GetUp;
extern TAnimation ProneMove;
extern TAnimation Aim;
extern TAnimation HandsUpAim;

#ifndef SERVER_CODE
extern int GameWidth;
extern int GameHeight;
extern float GameWidthHalf;
extern float GameHeightHalf;
#endif

// Ping improvement vars
extern TVector2 OldSpritePos[MAX_SPRITES + 1][MAX_OLDPOS + 1];

// Survival vars
extern uint8_t AliveNum;
extern int TeamAliveNum[6];
extern int TeamPlayersNum[5];
extern bool SurvivalEndRound;
extern bool WeaponsCleaned;

extern int CeaseFireTime;
extern int MapChangeTime;
extern int MapChangeCounter;
extern std::string MapChangeName;
extern TMapInfo MapChange;
extern uint64_t MapChangeItemID;
extern TSHA1Digest MapChangeChecksum;
extern int TimeLimitCounter;
extern int StartHealth;
extern int TimeLeftSec;
extern int TimeLeftMin;
extern uint8_t WeaponSel[MAX_SPRITES + 1][MAIN_WEAPONS + 1];

extern int TeamScore[6];
extern int TeamFlag[5];

extern float SinusCounter;

extern TPolyMap Map;

extern TSHA1Digest GameModChecksum;
extern TSHA1Digest CustomModChecksum;
extern TSHA1Digest MapCheckSum;

extern int MapIndex;

extern TWaypoints BotPath;

extern TKillSort SortedPlayers[MAX_SPRITES + 1];
#ifndef SERVER_CODE
extern TKillSort SortedTeamScore[MAX_SPRITES + 1];
extern int HeartbeatTime;
extern int HeartbeatTimeWarnings;
#endif

// Game entities
extern TSprite Sprite[MAX_SPRITES + 1];
extern TBullet Bullet[MAX_BULLETS + 1];
#ifndef SERVER_CODE
extern TSpark Spark[MAX_SPARKS + 1];
#endif
extern TThing Thing[MAX_THINGS + 1];

// Voting
extern bool VoteActive;
extern uint8_t VoteType;
extern std::string VoteTarget;
extern std::string VoteStarter;
extern std::string VoteReason;
extern int VoteTimeRemaining;
extern uint8_t VoteNumVotes;
extern uint8_t VoteMaxVotes;
extern bool VoteHasVoted[MAX_SPRITES + 1];
extern int VoteCooldown[MAX_SPRITES + 1];
extern bool VoteKickReasonType;

// Function declarations
void Number27Timing();
void ToggleBulletTime(bool TurnOn, int Duration = 30);
void UpdateGameStats();
bool PointVisible(float X, float Y, int i);
bool PointVisible2(float X, float Y, int i);
void StartVote(uint8_t StarterVote, uint8_t TypeVote, const std::string& TargetVote, const std::string& ReasonVote);
void StopVote();
void TimerVote();
#ifdef SERVER_CODE
void CountVote(uint8_t Voter);
#endif
void ShowMapChangeScoreboard(); // overload
void ShowMapChangeScoreboard(const std::string& NextMap); // overload
bool IsTeamGame();
#ifndef SERVER_CODE
bool IsPointOnScreen(TVector2 Point);
#endif
void ChangeMap();
void SortPlayers();

namespace GameImpl {
    inline void Number27Timing() {
        // Simplified implementation - in real code this would use proper timing functions
        // TimeInMilLast := TimeInMil;
        // TimeInMil := GetTickCount64;
        // Additional timing logic would go here
    }

    inline void UpdateGameStats() {
        // This function would save game statistics to a file
        // Implementation would require file I/O operations
    }

    inline void ToggleBulletTime(bool TurnOn, int Duration) {
        if (TurnOn) {
            BulletTimeTimer = Duration;
            GOALTICKS = DEFAULT_GOALTICKS / 3;
        } else {
            GOALTICKS = DEFAULT_GOALTICKS;
        }
        Number27Timing();
    }

    inline bool PointVisible(float X, float Y, int i) {
#ifdef SERVER_CODE
        const int GAME_WIDTH = MAX_GAME_WIDTH; // Assuming MAX_GAME_WIDTH is defined
        const int GAME_HEIGHT = 480;
#else
        int GAME_WIDTH = GameWidth;
        int GAME_HEIGHT = GameHeight;
#endif

        if ((i > MAX_PLAYERS) || (i < 1)) {
            return false;
        }

        float SX = SpriteParts.Pos[i].x - ((SpriteParts.Pos[i].x - Sprite[i].Control.MouseAimX) / 2);
        float SY = SpriteParts.Pos[i].y - ((SpriteParts.Pos[i].y - Sprite[i].Control.MouseAimY) / 2);

        return (X > (SX - GAME_WIDTH)) && (X < (SX + GAME_WIDTH)) && 
               (Y > (SY - GAME_HEIGHT)) && (Y < (SY + GAME_HEIGHT));
    }

    inline bool PointVisible2(float X, float Y, int i) {
#ifdef SERVER_CODE
        const int GAME_WIDTH = MAX_GAME_WIDTH; // Assuming MAX_GAME_WIDTH is defined
        const int GAME_HEIGHT = 480;
#else
        const int GAME_WIDTH = 600;
        const int GAME_HEIGHT = 440;
#endif

        float SX = SpriteParts.Pos[i].x;
        float SY = SpriteParts.Pos[i].y;

        return (X > (SX - GAME_WIDTH)) && (X < (SX + GAME_WIDTH)) && 
               (Y > (SY - GAME_HEIGHT)) && (Y < (SY + GAME_HEIGHT));
    }

#ifndef SERVER_CODE
    inline bool IsPointOnScreen(TVector2 Point) {
        bool result = true;
        float P1 = GameWidthHalf - (CameraX - Point.x);
        float P2 = GameHeightHalf - (CameraY - Point.y);
        if ((P1 < 0) || (P1 > GameWidth)) {
            result = false;
        }
        if ((P2 < 0) || (P2 > GameHeight)) {
            result = false;
        }
        return result;
    }
#endif

    inline void StartVote(uint8_t StarterVote, uint8_t TypeVote, const std::string& TargetVote, const std::string& ReasonVote) {
        VoteActive = true;
        if ((StarterVote < 1) || (StarterVote > MAX_PLAYERS)) {
            VoteStarter = "Server";
        } else {
            // Set vote starter name and cooldown
            VoteStarter = Sprite[StarterVote].Player.Name;
            VoteCooldown[StarterVote] = DEFAULT_VOTE_TIME;
            // Additional client-specific code would go here
        }
        VoteType = TypeVote;
        VoteTarget = TargetVote;
        VoteReason = ReasonVote;
        VoteTimeRemaining = DEFAULT_VOTING_TIME;
        VoteNumVotes = 0;
        VoteMaxVotes = 0;
        
        for (int i = 1; i <= MAX_PLAYERS; i++) {
            if (Sprite[i].Active) {
                if (Sprite[i].Player.ControlMethod == HUMAN) {
                    VoteMaxVotes++;
                }
            }
        }
    }

    inline void StopVote() {
        VoteActive = false;
        VoteNumVotes = 0;
        VoteMaxVotes = 0;
        VoteType = 0;
        VoteTarget = "";
        VoteStarter = "";
        VoteReason = "";
        VoteTimeRemaining = -1;
        
        for (int i = 1; i <= MAX_PLAYERS; i++) {
            VoteHasVoted[i] = false;
        }
    }

    inline void TimerVote() {
#ifdef SERVER_CODE
        if (VoteActive) {
#endif
            if (VoteTimeRemaining > -1) {
                VoteTimeRemaining--;
            }

            if (VoteTimeRemaining == 0) {
                // MainConsole.console message would go here
                StopVote();
            }
#ifdef SERVER_CODE
        }
#endif
    }

#ifdef SERVER_CODE
    inline void CountVote(uint8_t Voter) {
        if (VoteActive && !VoteHasVoted[Voter]) {
            VoteNumVotes++;
            VoteHasVoted[Voter] = true;
            float edge = static_cast<float>(VoteNumVotes) / VoteMaxVotes;
            if (edge >= (sv_votepercent.Value() / 100.0f)) {
                if (VoteType == VOTE_MAP) {
                    // Handle map vote
                    if (!PrepareMapChange(VoteTarget)) {
                        // MainConsole.Console('Map not found (' + VoteTarget + ')', WARNING_MESSAGE_COLOR);
                        // MainConsole.Console('No map has been voted', VOTE_MESSAGE_COLOR);
                    }
                } else if (VoteType == VOTE_KICK) {
                    // Handle kick vote
                    int target = std::stoi(VoteTarget);
                    // KickPlayer implementation would go here
                }
                StopVote();
                // ServerSendVoteOff implementation would go here
            }
        }
    }
#endif

    inline void ShowMapChangeScoreboard() {
        ShowMapChangeScoreboard("EXIT*!*");
    }

    inline void ShowMapChangeScoreboard(const std::string& NextMap) {
        MapChangeName = NextMap;
        MapChangeCounter = MapChangeTime;
        
#ifndef SERVER_CODE
        // GameMenuShow(LimboMenu, False);
        // FragsMenuShow := True;
        // Additional client-specific code
#endif
    }

    inline bool IsTeamGame() {
        switch (sv_gamemode.Value()) {
            case GAMESTYLE_TEAMMATCH:
            case GAMESTYLE_CTF:
            case GAMESTYLE_INF:
            case GAMESTYLE_HTF:
                return true;
            default:
                return false;
        }
    }

    inline void ChangeMap() {
        // Implementation would go here
        // This is a complex function that handles map changes
    }

    inline void SortPlayers() {
        // Initialize counters
        PlayersNum = 0;
        BotsNum = 0;
        SpectatorsNum = 0;
        for (int i = 1; i <= 4; i++) {
            PlayersTeamNum[i] = 0;
        }

        // Initialize sorted players array
        for (int i = 1; i <= MAX_SPRITES; i++) {
            SortedPlayers[i].Kills = 0;
            SortedPlayers[i].Deaths = 0;
            SortedPlayers[i].Flags = 0;
            SortedPlayers[i].PlayerNum = 0;
        }

        // Count active players
        for (int i = 1; i <= MAX_SPRITES; i++) {
            if (Sprite[i].Active && (!Sprite[i].Player.DemoPlayer)) {
                PlayersNum++;
                if (Sprite[i].Player.ControlMethod == BOT) {
                    BotsNum++;
                }

                if (Sprite[i].IsSpectator()) {
                    SpectatorsNum++;
                }

                if (Sprite[i].IsNotSolo() && Sprite[i].IsNotSpectator()) {
                    PlayersTeamNum[Sprite[i].Player.Team]++;
                }

                if (Sprite[i].IsNotSpectator()) {
                    SortedPlayers[PlayersNum].Kills = Sprite[i].Player.Kills;
                    SortedPlayers[PlayersNum].Deaths = Sprite[i].Player.Deaths;
                    SortedPlayers[PlayersNum].Flags = Sprite[i].Player.Flags;
                    SortedPlayers[PlayersNum].PlayerNum = i;
                } else {
                    SortedPlayers[PlayersNum].Kills = 0;
                    SortedPlayers[PlayersNum].Deaths = INT32_MAX; // High value for sorting
                    SortedPlayers[PlayersNum].Flags = 0;
                    SortedPlayers[PlayersNum].PlayerNum = i;
                }
            }
        }

        // Sort by flags, kills, and deaths
        for (int i = 1; i <= PlayersNum; i++) {
            for (int j = i + 1; j <= PlayersNum; j++) {
                if (SortedPlayers[j].Flags > SortedPlayers[i].Flags) {
                    TKillSort temp = SortedPlayers[i];
                    SortedPlayers[i] = SortedPlayers[j];
                    SortedPlayers[j] = temp;
                }
            }
        }

        for (int i = 1; i <= PlayersNum; i++) {
            for (int j = i + 1; j <= PlayersNum; j++) {
                if (SortedPlayers[j].Flags == SortedPlayers[i].Flags) {
                    if (SortedPlayers[j].Kills > SortedPlayers[i].Kills) {
                        TKillSort temp = SortedPlayers[i];
                        SortedPlayers[i] = SortedPlayers[j];
                        SortedPlayers[j] = temp;
                    }
                }
            }
        }

        for (int i = 1; i <= PlayersNum; i++) {
            for (int j = i + 1; j <= PlayersNum; j++) {
                if (SortedPlayers[j].Flags == SortedPlayers[i].Flags) {
                    if (SortedPlayers[j].Kills == SortedPlayers[i].Kills) {
                        if (SortedPlayers[j].Deaths < SortedPlayers[i].Deaths) {
                            TKillSort temp = SortedPlayers[i];
                            SortedPlayers[i] = SortedPlayers[j];
                            SortedPlayers[j] = temp;
                        }
                    }
                }
            }
        }
    }
}

// Using declarations to bring into global namespace
using GameImpl::TKillSort;
using GameImpl::Ticks;
using GameImpl::TicksPerSecond;
using GameImpl::Frames;
using GameImpl::FramesPerSecond;
using GameImpl::TickTime;
using GameImpl::TickTimeLast;
using GameImpl::GOALTICKS;
using GameImpl::BulletTimeTimer;
using GameImpl::SpriteParts;
using GameImpl::BulletParts;
using GameImpl::SparkParts;
using GameImpl::GostekSkeleton;
using GameImpl::BoxSkeleton;
using GameImpl::FlagSkeleton;
using GameImpl::ParaSkeleton;
using GameImpl::StatSkeleton;
using GameImpl::RifleSkeleton10;
using GameImpl::RifleSkeleton11;
using GameImpl::RifleSkeleton18;
using GameImpl::RifleSkeleton22;
using GameImpl::RifleSkeleton28;
using GameImpl::RifleSkeleton36;
using GameImpl::RifleSkeleton37;
using GameImpl::RifleSkeleton39;
using GameImpl::RifleSkeleton43;
using GameImpl::RifleSkeleton50;
using GameImpl::RifleSkeleton55;
using GameImpl::Run;
using GameImpl::Stand;
using GameImpl::RunBack;
using GameImpl::Jump;
using GameImpl::JumpSide;
using GameImpl::Roll;
using GameImpl::RollBack;
using GameImpl::Fall;
using GameImpl::Crouch;
using GameImpl::CrouchRun;
using GameImpl::CrouchRunBack;
using GameImpl::Reload;
using GameImpl::Throw;
using GameImpl::Recoil;
using GameImpl::Shotgun;
using GameImpl::Barret;
using GameImpl::SmallRecoil;
using GameImpl::AimRecoil;
using GameImpl::HandsUpRecoil;
using GameImpl::ClipIn;
using GameImpl::ClipOut;
using GameImpl::SlideBack;
using GameImpl::Change;
using GameImpl::ThrowWeapon;
using GameImpl::WeaponNone;
using GameImpl::Punch;
using GameImpl::ReloadBow;
using GameImpl::Melee;
using GameImpl::Cigar;
using GameImpl::Match;
using GameImpl::Smoke;
using GameImpl::Wipe;
using GameImpl::Groin;
using GameImpl::TakeOff;
using GameImpl::Victory;
using GameImpl::Piss;
using GameImpl::Mercy;
using GameImpl::Mercy2;
using GameImpl::Own;
using GameImpl::Prone;
using GameImpl::GetUp;
using GameImpl::ProneMove;
using GameImpl::Aim;
using GameImpl::HandsUpAim;
using GameImpl::OldSpritePos;
using GameImpl::AliveNum;
using GameImpl::TeamAliveNum;
using GameImpl::TeamPlayersNum;
using GameImpl::SurvivalEndRound;
using GameImpl::WeaponsCleaned;
using GameImpl::CeaseFireTime;
using GameImpl::MapChangeTime;
using GameImpl::MapChangeCounter;
using GameImpl::MapChangeName;
using GameImpl::MapChange;
using GameImpl::MapChangeItemID;
using GameImpl::MapChangeChecksum;
using GameImpl::TimeLimitCounter;
using GameImpl::StartHealth;
using GameImpl::TimeLeftSec;
using GameImpl::TimeLeftMin;
using GameImpl::WeaponSel;
using GameImpl::TeamScore;
using GameImpl::TeamFlag;
using GameImpl::SinusCounter;
using GameImpl::Map;
using GameImpl::GameModChecksum;
using GameImpl::CustomModChecksum;
using GameImpl::MapCheckSum;
using GameImpl::MapIndex;
using GameImpl::BotPath;
using GameImpl::SortedPlayers;
using GameImpl::VoteActive;
using GameImpl::VoteType;
using GameImpl::VoteTarget;
using GameImpl::VoteStarter;
using GameImpl::VoteReason;
using GameImpl::VoteTimeRemaining;
using GameImpl::VoteNumVotes;
using GameImpl::VoteMaxVotes;
using GameImpl::VoteHasVoted;
using GameImpl::VoteCooldown;
using GameImpl::VoteKickReasonType;
using GameImpl::Sprite;
using GameImpl::Bullet;
using GameImpl::Thing;
using GameImpl::Number27Timing;
using GameImpl::ToggleBulletTime;
using GameImpl::UpdateGameStats;
using GameImpl::PointVisible;
using GameImpl::PointVisible2;
using GameImpl::StartVote;
using GameImpl::StopVote;
using GameImpl::TimerVote;
using GameImpl::CountVote;
using GameImpl::ShowMapChangeScoreboard;
using GameImpl::IsTeamGame;
using GameImpl::IsPointOnScreen;
using GameImpl::ChangeMap;
using GameImpl::SortPlayers;

#endif // GAME_H