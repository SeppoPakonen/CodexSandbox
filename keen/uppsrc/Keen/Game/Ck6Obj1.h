#ifndef CK6_OBJ1_H
#define CK6_OBJ1_H

#include "Game.h"

NAMESPACE_UPP

// Episode 6 object functions (formerly in ck6_obj1.c)

// Grabbiter
void CK6_SpawnGrabbiter(int tileX, int tileY);
void CK6_GrabbiterCol(CK_object *a, CK_object *b);

// Rocket ship
void CK6_SpawnRocket(int tileX, int tileY, int dir);
void CK6_Rocket(CK_object *obj);
void CK6_RocketCol(CK_object *a, CK_object *b);
void CK6_RocketFlyCol(CK_object *a, CK_object *b);
void CK6_RocketFly(CK_object *obj);

// Cliff and Rope
void CK6_SpawnMapCliff(int tileX, int tileY, int dir);
void CK6_MapKeenThrowRope(CK_object *obj);
void CK6_MapKeenClimb(CK_object *obj);
void CK6_MapCliffCol(CK_object *a, CK_object *b);

// Satellite
void CK6_SpawnSatelliteLoading(int tileX, int tileY, int dir);
void CK6_SpawnSatellite(int tileX, int tileY);
void CK6_Satellite(CK_object *obj);
void CK6_SatelliteCol(CK_object *a, CK_object *b);
void CK6_KeenSatelliteDraw(CK_object *obj);

// Story Items
void CK6_SpawnSandwich(int tileX, int tileY);
void CK6_SpawnRope(int tileX, int tileY);
void CK6_SpawnPasscard(int tileX, int tileY);
void CK6_StoryItemCol(CK_object *a, CK_object *b);

// Molly
void CK6_SpawnMolly(int tileX, int tileY);

// Go Plats
void CK6_GoPlatDraw(CK_object *obj);

// Bloogs
void CK6_SpawnBloog(int tileX, int tileY);
void CK6_Bloog(CK_object *obj);
void CK6_BloogCol(CK_object *a, CK_object *b);

// Blooglets
void CK6_SpawnBlooglet(int tileX, int tileY, int type);
void CK6_BloogletCol(CK_object *a, CK_object *b);

// Bloogguards
void CK6_SpawnBloogguard(int tileX, int tileY);
void CK6_BloogguardWalk(CK_object *obj);
void CK6_BloogguardSmash(CK_object *obj);
void CK6_BloogguardCol(CK_object *a, CK_object *b);
void CK6_MultihitDraw(CK_object *obj);

// Global variables
extern int ck6_smashScreenDistance;
extern int16_t ck6_smashScreenOfs[];

void CK6_Obj1_SetupFunctions();

END_UPP_NAMESPACE

#endif