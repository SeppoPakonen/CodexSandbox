#ifndef UPP_KEEN_CK6_EP_H
#define UPP_KEEN_CK6_EP_H

#include "Game.h"

NAMESPACE_UPP

/*
 * Contains definitions relevant only to Keen 6
 */
extern CK_EpisodeDef ck6_episode;

/* Action functions setup */
void CK6_Obj1_SetupFunctions(void);
void CK6_Obj2_SetupFunctions(void);
void CK6_Obj3_SetupFunctions(void);
void CK6_Map_SetupFunctions(void);
void CK6_SetupFunctions(void);
bool CK6_IsPresent(void);
void CK6_DefineConstants(void);

void CK6_ScanInfoLayer();

/* Spawning functions */

/* ck6_misc.c */
CK_object *CK6_SpawnEnemyShot(int posX, int posY, CK_action *action);

void CK6_ShowGetSandwich();
void CK6_ShowGetRope();
void CK6_ShowGetPasscard();

/* ck6_obj1.c */
void CK6_SpawnGrabbiter(int tileX, int tileY);
void CK6_SpawnRocket(int tileX, int tileY, int dir);
void CK6_SpawnMapCliff(int tileX, int tileY, int dir);
void CK6_SpawnSatelliteLoading(int tileX, int tileY, int dir);
void CK6_SpawnSatellite(int tileX, int tileY);

void CK6_SpawnSandwich(int tileX, int tileY);
void CK6_SpawnRope(int tileX, int tileY);
void CK6_SpawnPasscard(int tileX, int tileY);
void CK6_SpawnMolly(int tileX, int tileY);
void CK6_SpawnBloog(int tileX, int tileY);
void CK6_SpawnBloogguard(int tileX, int tileY);
void CK6_SpawnBlooglet(int tileX, int tileY, int type);

/* ck6_obj2.c */
void CK6_SpawnNospike(int tileX, int tileY);
void CK6_SpawnGik(int tileX, int tileY);
void CK6_SpawnOrbatrix(int tileX, int tileY);
void CK6_SpawnBipship(int tileX, int tileY);
void CK6_SpawnFlect(int tileX, int tileY);

/* ck6_obj3.c */
void CK6_SpawnFleex(int tileX, int tileY);
void CK6_SpawnBobba(int tileX, int tileY);
void CK6_SpawnBabobba(int tileX, int tileY);
void CK6_SpawnBlorb(int tileX, int tileY);
void CK6_SpawnCeilick(int tileX, int tileY);

/* ck6_map.c */
void CK6_MapMiscFlagsCheck(CK_object *keen);
void CK_SpawnMapKeen(int tileX, int tileY);

/* Map functions */

/* Misc functions */

/* ck6_misc.c */

END_UPP_NAMESPACE

#endif