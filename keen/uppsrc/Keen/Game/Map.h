#ifndef UPP_KEEN_MAP_H
#define UPP_KEEN_MAP_H

#include "Game.h"

NAMESPACE_UPP

// extern chunk_id_t ck_mapKeenBaseFrame[];  // Moved to Defs.h

void CK_DemoSignSpawn();
void CK_UpdateScoreBox(CK_object *scorebox);
void CK_SpawnMapKeen(int tileX, int tileY);
void CK_ScanForLevelEntry(CK_object *obj);
void CK_MapKeenStill(CK_object *obj);
void CK_MapKeenWalk(CK_object *obj);
void CK_AnimateMapTeleporter(int tileX, int tileY);
void CK_MapFlagSpawn(int tileX, int tileY);
void CK_FlippingFlagSpawn(int tileX, int tileY);
void CK_MapFlagThrown(CK_object *obj);
void CK_MapFlagFall(CK_object *obj);
void CK_MapFlagLand(CK_object *obj);
void CK_Map_SetupFunctions();

END_UPP_NAMESPACE

#endif