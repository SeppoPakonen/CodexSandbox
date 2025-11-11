#ifndef UPP_KEEN_DEFS_H
#define UPP_KEEN_DEFS_H

//#include "Game.h"

//NAMESPACE_UPP

struct CK_object;  // Forward declaration

#define SLOPEMASK 7

typedef struct CK_objPhysData
{
	// Unit cliping box
	uint16_t unitX1;
	uint16_t unitY1;
	uint16_t unitX2;
	uint16_t unitY2;
	uint16_t unitXmid;

	// Tile clipping box
	uint16_t tileX1;
	uint16_t tileY1;
	uint16_t tileX2;
	uint16_t tileY2;
	uint16_t tileXmid;
} CK_objPhysData;

typedef struct CK_objPhysDataDelta
{
	int16_t unitX1;
	int16_t unitY1;
	int16_t unitX2;
	int16_t unitY2;
	int16_t unitXmid;
} CK_objPhysDataDelta;

typedef enum CK_Difficulty
{
	D_NotPlaying = 0,
	D_Easy = 1,
	D_Normal = 2,
	D_Hard = 3
} CK_Difficulty;

typedef enum CK_ActionType
{
	AT_UnscaledOnce,  // Unscaled Motion, Thinks once.
	AT_ScaledOnce,    // Scaled Motion, Thinks once.
	AT_Frame,	 // No Motion, Thinks each frame (doesn't advance action)
	AT_UnscaledFrame, // Unscaled Motion, Thinks each frame
	AT_ScaledFrame,   // Scaled Motion, Thinks each frame

	AT_NullActionTypeValue = 0x6F42 // FIXME: Ugly hack used for filling ck_play.c:ck_partialNullAction
} CK_ActionType;

// Need to define chunk_id_t based on CK_VARS_LINKED or not
// Using the same approach as the original C code
#ifdef CK_VARS_LINKED
typedef int16_t chunk_id_t;
#else
typedef const char *chunk_id_t;
#endif

// This struct is 0x58 bytes big in Keen5
// It must be preserved (or at least negotiated) if omnispeak savegames are to be compatible
// with those of vanilla keen
typedef struct CK_GameState
{
	uint16_t mapPosX;
	uint16_t mapPosY;
	uint16_t levelsDone[25]; // Number of levels complete
	int32_t keenScore;       // Keen's score. (This _is_ signed, by the looks of all the 'jl' instructions)
	int32_t nextKeenAt;      // Score keen will get a new life at.
	int16_t numShots;
	int16_t numCentilife;

	// The episode-specific variables come here
	// They were probably conditionally compiled in the DOS version
	// so that the Gamestate struct is variably sized between eps.
	union {
		struct
		{
			int16_t wetsuit;
			int16_t membersRescued;
		} ck4;

		struct
		{
			int16_t securityCard;
			int16_t word_4729C;
			int16_t fusesRemaining;
		} ck5;

		struct
		{
			int16_t sandwich;
			int16_t rope; // 1 == collected, 2 == deployed on cliff
			int16_t passcard;
			int16_t inRocket; // true if flying
		} ck6;
	} ep;

	int16_t keyGems[4];
	int16_t currentLevel;
	int16_t numLives;	 // Number of lives keen has.
	CK_Difficulty difficulty; // Difficulty level of current game
	//struct CK_object *platform;  // This was a 16-bit pointer in DOS Keen5.exe

	int levelState; // Level State (should probably be enum)
	bool jumpCheat; // Is the jump cheat enabled? (Not in Keen5 gamestate struct)
} CK_GameState;

extern CK_GameState ck_gameState;

typedef enum CK_LevelState
{
	LS_Playing = 0,		  // In Level
	LS_Died = 1,		  // Keen Died
	LS_LevelComplete = 2,     // Level Completed
	LS_CouncilRescued = 3,    // Rescued Council Member (Keen 4)
	LS_AboutToRecordDemo = 4, // Warped to a new level
	LS_ResetGame = 5,	  // Started a new game
	LS_LoadedGame = 6,	  // Loaded a saved game
	LS_Foot = 7,		  // Keen exited level by touching foot (keen 4)
	LS_AbortGame = 8,	  // Game failed to load
	LS_Sandwich = 9,	  // Keen exited level by getting items (Keen 6)
	LS_Rope = 10,
	LS_Passcard = 11,
	LS_Molly = 12,		  // Keen rescues Molly (Keen 6)
	LS_TeleportToKorath = 13, // Keen teleported to Korath III Base (Keen 5)
	LS_KorathFuse = 14,	  // Keen broke the Korath III fuse (Keen 5)
	LS_DestroyedQED = 15,     // Destroyed QED (Keen 5)
} CK_LevelState;

extern chunk_id_t ck_mapKeenBaseFrame[];
extern chunk_id_t CK_ItemSpriteChunks[];

typedef enum CK_ClassType
{
	CT_Nothing = 0,
	CT_Friendly = 1,
	CT_Player = 2,
	CT_Stunner = 3,

	CT4_Item = 4,
	CT4_Slug = 5,
	CT4_CouncilMember = 6,
	CT4_7 = 7,
	CT4_Egg = 8,
	CT4_Mushroom = 9,
	CT4_Arachnut = 10,
	CT4_Skypest = 11,
	CT4_Wormmouth = 12,
	CT4_Cloud = 13,
	CT4_Berkeloid = 14,
	CT4_Bounder = 15,
	CT4_Inchworm = 16,
	CT4_Foot = 17, // what was this?
	CT4_Lick = 18,
	CT4_Mimrock = 19,
	CT4_Platform = 20,
	CT4_Dopefish = 21,
	CT4_Schoolfish = 22,
	CT4_Sprite = 23,
	CT4_Lindsey = 24,
	CT4_Bolt = 25,
	CT4_Smirky = 26,
	CT4_Bird = 27,
	CT4_0x1C = 28,
	CT4_0x1D = 29,
	CT4_Wetsuit = 30,
	CT4_EnemyShot = 31,
	CT4_Mine = 32,
	CT4_StunnedCreature = 33,
	CT4_MapFlag = 34,
	CT4_Turret = 1, // to make the CT_CLASS macro work

	CT5_EnemyShot = 4,
	CT5_Item = 5,
	CT5_Platform = 6,
	CT5_StunnedCreature = 7,
	CT5_MapFlag = 8,
	CT5_Sparky = 9,
	CT5_Mine = 10,
	CT5_SliceStar = 11,
	CT5_Robo = 12,
	CT5_Spirogrip = 13,
	CT5_Ampton = 14,
	CT5_Turret = 15,
	CT5_Volte = 16,
	CT5_0x11 = 17,
	CT5_Spindred = 18,
	CT5_Master = 19,
	CT5_Shikadi = 20,
	CT5_Shocksund = 21,
	CT5_Sphereful = 22,
	CT5_Korath = 23,
	CT5_QED = 25,

	CT6_EnemyShot = 4,
	CT6_Item = 5,
	CT6_Platform = 6,
	CT6_Bloog = 7,
	CT6_Blooglet = 8,
	CT6_Fleex = 10,
	CT6_Molly = 12,
	CT6_Babobba = 13,
	CT6_Bobba = 14,
	CT6_Nospike = 16,
	CT6_Gik = 17,
	CT6_Turret = 18,
	CT6_Orbatrix = 19,
	CT6_Bip = 20,
	CT6_Flect = 21,
	CT6_Blorb = 22,
	CT6_Ceilick = 23,
	CT6_Bloogguard = 24,
	CT6_Bipship = 26,
	CT6_Sandwich = 27,
	CT6_Rope = 28,
	CT6_Passcard = 29,
	CT6_StunnedCreature = 25,
	CT6_Grabbiter = 30,
	CT6_Rocket = 31,
	CT6_MapCliff = 32,
	CT6_Satellite = 33,
	CT6_SatelliteLoading = 34,
	CT6_MapFlag = 35,
} CK_ClassType;

#define CT_CLASS(type) \
	(ck_currentEpisode->ep == EP_CK4 ? CT4_##type : (ck_currentEpisode->ep == EP_CK5 ? CT5_##type : (ck_currentEpisode->ep == EP_CK6 ? CT6_##type : CT_Nothing)))

typedef enum CK_MiscFlag
{
	MF_Nil,
	MF_Pole,
	MF_Door,
	MF_Deadly,
	MF_Centilife,
	MF_SwitchOff,
	MF_SwitchOn,
	MF_KeyholderA,
	MF_KeyholderB,
	MF_KeyholderC,
	MF_KeyholderD,
	MF_WaterN,
	MF_WaterE,
	MF_WaterS,
	MF_WaterW,
	MF_BridgeSwitch = 15,
	MF_Moon = 16,
	MF_PathArrow = 17,
	MF_Bridge = 18,
	MF_ZapperOn = 19,
	MF_Teleporter = 20,
	MF_Points100,
	MF_Points200,
	MF_Points500,
	MF_Points1000,
	MF_Points2000,
	MF_Points5000,
	MF_1UP,
	MF_Stunner,
	MF_ZapperOff = 30,
	MF_AmptonComputer = 31,
	MF_KeycardDoor = 32,
	MF_MapElevatorLeft = 33,
	MF_MapElevatorRight = 34
} CK_MiscFlag;

// Constants that are referenced in game code
#define CK_ACT_NULL 0
#define CK_ACT_DemoSign 1

// Add missing constants used by CK_CHUNKID macro
#define CK_VARS_LINKED 0  // Define as 0 to use the string path version

// Episode definitions if not already defined elsewhere
typedef enum CK_Episode
{
	EP_Nil,
	EP_CK4,
	EP_CK5,
	EP_CK6,
} CK_Episode;

// If this is not defined elsewhere
struct CK_EpisodeDef;
extern struct CK_EpisodeDef *ck_currentEpisode;

// Missing ID Engine function declarations
int16_t SD_GetSpriteSync(void);
void VL_Shutdown(void);
bool US_TerminalOk(void);
int IN_GetLastScan(void);
int16_t CA_TileAtPos(int16_t x, int16_t y, int16_t layer);
void RF_ReplaceTiles(int16_t *tiles, int16_t numTiles, int16_t x, int16_t y, int16_t width, int16_t height);
struct ID_MM_Arena;  // Forward declaration
ID_MM_Arena *MM_ArenaCreate(size_t size);
void MM_SetPurge(void *ptr, int level);
void VL_GetTics(int16_t tics);
void VL_Present(void);
void VL_DelayTics(int16_t tics);
void VHB_DrawBitmap(int16_t x, int16_t y, int16_t chunk);
void VHB_Bar(int16_t x, int16_t y, int16_t w, int16_t h, int16_t color);
void US_SetPrintColour(int16_t color);
int16_t US_GetPrintColour(void);
void US_SetPrintY(int16_t y);
int16_t US_GetPrintY(void);
void US_SetPrintX(int16_t x);
int16_t US_GetPrintX(void);
void US_PrintB8000Text(uint8_t *text, int16_t len);
void VL_FixRefreshBuffer(void);
void US_CenterWindow(int16_t w, int16_t h);
bool IN_DemoGetMode(void);

// Forward declaration for input frame - should be defined in IdInput
extern IN_ControlFrame ck_inputFrame;
void MM_SetPurge(void **ptr, int level);

// Missing constants
typedef struct VH_BitmapTableEntry
{
	int16_t width;
	int16_t height;
} VH_BitmapTableEntry;

VH_BitmapTableEntry *VH_GetBitmapTableEntry(int16_t chunk);

// Add missing macros if needed
#define CK_ACTION(name) (name)
#define CK_FUNCTION(name) (name)

typedef void (*CK_ACT_Function)(CK_object *obj);
typedef void (*CK_ACT_ColFunction)(CK_object *obj1, CK_object *obj2);

typedef struct CK_action
{
	int16_t chunkLeft;
	int16_t chunkRight;
	CK_ActionType type;
	int16_t protectAnimation, stickToGround; // See KeenWiki: Galaxy Action Parameters (lemm/levelass)
	int16_t timer;
	int16_t velX, velY;
#ifdef CK_VAR_FUNCTIONS_AS_STRINGS
	const char *think;
	const char *collide;
	const char *draw;
	const char *next;
#else
	void (*think)(struct CK_object *obj);
	void (*collide)(struct CK_object *obj, struct CK_object *other);
	void (*draw)(struct CK_object *obj);
	struct CK_action *next;
#endif
	// Omnispeak - backwards compatibility:
	// Given an instance of this type, stores what would be
	// the 16-bit offset pointer in the dseg while using the
	// original 16-bit DOS executable (corresponding version).
	uint16_t compatDosPointer;
} CK_action;

typedef enum CK_objActive
{
	OBJ_INACTIVE = 0,
	OBJ_ACTIVE = 1,
	OBJ_ALWAYS_ACTIVE = 2,
	OBJ_EXISTS_ONLY_ONSCREEN = 3
} CK_objActive;

typedef enum CK_clipped
{
	CLIP_not = 0,
	CLIP_normal,
	CLIP_simple,
} CK_ClipType;

typedef struct CK_object
{
	int16_t type;
	CK_objActive active;
	bool visible;
	CK_ClipType clipped;
	uint16_t timeUntillThink;
	uint16_t posX;
	uint16_t posY;
	int16_t xDirection;
	int16_t yDirection;
	int16_t deltaPosX;
	int16_t deltaPosY;
	int16_t velX;
	int16_t velY;
	int16_t actionTimer;
	CK_action *currentAction;
	uint16_t gfxChunk;
	int16_t zLayer;

	CK_objPhysData clipRects;

	//TileInfo for surrounding tiles.
	int16_t topTI, rightTI, bottomTI, leftTI;

	struct RF_SpriteDrawEntry *sde;

	// *** OMNISPEAK - NOTES ABOUT USAGE OF USER VARIABLES: ***
	//
	// The user variables are of type of int16_t for compatibility with
	// saved games, as well, as to be useful with dumper-enabled builds.
	//
	// This is the case even if a user field stores a pointer.
	//
	// If you want to store a pointer in a user field,
	// an appropriate conversion shall be made. A few
	// examples of functions that can be used:
	// - RF_AddSpriteDrawUsing16BitOffset
	// - RF_RemoveSpriteDrawUsing16BitOffset
	// - CK_ConvertObjPointerTo16BitOffset
	// - CK_ConvertObj16BitOffsetToPointer

	int16_t user1, user2, user3, user4;

	struct CK_object *next;
	struct CK_object *prev;
} CK_object;

//END_UPP_NAMESPACE

#endif