#ifndef UPP_KEEN_CK5_EP_H
#define UPP_KEEN_CK5_EP_H

#include "Game.h"

NAMESPACE_UPP

/*
 * Contains definitions relevant only to Keen 5
 */
extern CK_EpisodeDef ck5_episode;

/* Action functions setup */
void CK5_Obj1_SetupFunctions(void);
void CK5_Obj2_SetupFunctions(void);
void CK5_Obj3_SetupFunctions(void);
void CK5_Map_SetupFunctions(void);
void CK5_SetupFunctions(void);
bool CK5_IsPresent(void);
void CK5_DefineConstants(void);

void CK5_ScanInfoLayer();

/* Spawning functions */

/* ck5_misc.c */
void CK5_SpawnFuseExplosion(int tileX, int tileY);
void CK5_SpawnLevelEnd(void);
void CK5_SpawnLightning(void);
CK_object *CK5_SpawnEnemyShot(int posX, int posY, CK_action *action);
void CK5_SpawnRedBlockPlatform(int tileX, int tileY, int direction, bool purple);
void CK5_SpawnItem(int tileX, int tileY, int itemNumber);

/* ck5_obj1.c */
void CK5_SpawnVolte(int tileX, int tileY);

/* ck5_obj2.c */
void CK5_SpawnSparky(int tileX, int tileY);
void CK5_SpawnAmpton(int tileX, int tileY);
void CK5_SpawnSlice(int tileX, int tileY, int dir);
void CK5_SpawnSliceDiag(int tileX, int tileY);
void CK5_SpawnShelly(int tileX, int tileY);

/* ck5_obj3.c */
void CK5_SpawnMine(int tileX, int tileY);
void CK5_SpawnRobo(int tileX, int tileY);
void CK5_SpawnSpirogrip(int tileX, int tileY);
void CK5_SpawnSpindred(int tileX, int tileY);
void CK5_SpawnMaster(int tileX, int tileY);
void CK5_SpawnShikadi(int tileX, int tileY);
void CK5_SpawnShocksund(int tileX, int tileY);
void CK5_SpawnSphereful(int tileX, int tileY);
void CK5_SpawnKorath(int tileX, int tileY);
void CK5_QEDSpawn(int tileX, int tileY);

/* ck5_map.c */
void CK5_MapMiscFlagsCheck(CK_object *keen);
void CK_SpawnMapKeen(int tileX, int tileY);
void CK5_MapKeenTeleSpawn(int tileX, int tileY);

/* Map functions */
void CK5_AnimateMapTeleporter(int tileX, int tileY);
void CK5_AnimateMapElevator(int tileX, int tileY, int dir);

/* Misc functions */

/* ck5_misc.c */
void CK5_ExplodeGalaxy();
void CK5_FuseMessage();

END_UPP_NAMESPACE

#endif