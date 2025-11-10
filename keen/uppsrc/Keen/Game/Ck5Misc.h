#ifndef CK5_MISC_H
#define CK5_MISC_H

#include "Game.h"

NAMESPACE_UPP

// Episode 5 miscellaneous functions (formerly in ck5_misc.c)

extern CK_EpisodeDef ck5_episode;

// Check if all the game files are present.
bool CK5_IsPresent();

void CK5_PurpleAxisPlatform(CK_object *obj);

// MISC Keen 5 functions
void CK5_SpawnLightning();
void CK5_SpawnFuseExplosion(int tileX, int tileY);
void CK5_SpawnLevelEnd(void);
void CK5_LevelEnd(CK_object *obj);
void CK5_SetupFunctions();

CK_object *CK5_SpawnEnemyShot(int posX, int posY, CK_action *action);

void CK5_OpenMapTeleporter(int tileX, int tileY);
void CK5_CloseMapTeleporter(int tileX, int tileY);
void CK5_ScanInfoLayer();

void CK5_ExplodeGalaxy();
void CK5_FuseMessage();
void CK5_MapKeenTeleSpawn(int tileX, int tileY);

END_UPP_NAMESPACE

#endif