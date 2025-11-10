#ifndef UPP_KEEN_DEFS_H
#define UPP_KEEN_DEFS_H

#include "Game.h"

NAMESPACE_UPP

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

typedef enum CK_ActionType
{
	AT_UnscaledOnce,  // Unscaled Motion, Thinks once.
	AT_ScaledOnce,    // Scaled Motion, Thinks once.
	AT_Frame,	 // No Motion, Thinks each frame (doesn't advance action)
	AT_UnscaledFrame, // Unscaled Motion, Thinks each frame
	AT_ScaledFrame,   // Scaled Motion, Thinks each frame

	AT_NullActionTypeValue = 0x6F42 // FIXME: Ugly hack used for filling ck_play.c:ck_partialNullAction
} CK_ActionType;

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

END_UPP_NAMESPACE

#endif