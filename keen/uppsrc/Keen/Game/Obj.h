#ifndef UPP_KEEN_OBJ_H
#define UPP_KEEN_OBJ_H

#include "Game.h"

NAMESPACE_UPP

// extern chunk_id_t CK_ItemSpriteChunks[];  // Moved to Defs.h

void CK_SpawnItem(int tileX, int tileY, int itemNumber);
void CK_SpawnCentilifeNotify(int tileX, int tileY);
void CK_SpawnAxisPlatform(int tileX, int tileY, int direction, bool purple);
void CK_SpawnFallPlat(int tileX, int tileY);
void CK_SpawnStandPlatform(int tileX, int tileY);
void CK_SpawnGoPlat(int tileX, int tileY, int direction, bool purple);
void CK_SneakPlatSpawn(int tileX, int tileY);
void CK_TurretSpawn(int tileX, int tileY, int direction);
void CK_DoorOpen(CK_object *obj);
void CK_SecurityDoorOpen(CK_object *obj);
void CK_PointItem(CK_object *obj);
void CK_FallingItem(CK_object *obj);
void CK_AxisPlatform(CK_object *obj);
void CK_FallPlatSit(CK_object *obj);
void CK_FallPlatFall(CK_object *obj);
void CK_FallPlatRise(CK_object *obj);
void CK_GoPlatThink(CK_object *obj);
void CK_SneakPlatThink(CK_object *obj);
void CK_TurretShoot(CK_object *obj);
void CK_TurretShotCol(CK_object *me, CK_object *other);
void CK_TurretShotDraw(CK_object *obj);
void CK_OBJ_SetupFunctions();

END_UPP_NAMESPACE

#endif