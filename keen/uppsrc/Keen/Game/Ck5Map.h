#ifndef CK5_MAP_H
#define CK5_MAP_H

//#include "Game.h"

//NAMESPACE_UPP

// Episode 5 map functions (formerly in ck5_map.c)

void CK5_MapMiscFlagsCheck(CK_object *keen);
void CK5_MapKeenTeleSpawn(int tileX, int tileY);
void CK5_MapKeenElevator(CK_object *keen);
void CK5_AnimateMapElevator(int tileX, int tileY, int dir);
void CK5_Map_SetupFunctions();

//END_UPP_NAMESPACE

#endif