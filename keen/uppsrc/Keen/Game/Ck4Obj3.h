#ifndef CK4_OBJ3_H
#define CK4_OBJ3_H

#include "Game.h"

NAMESPACE_UPP

// Episode 4 object functions (formerly in ck4_obj3.c)

// Smirky
void CK4_SpawnSmirky(int tileX, int tileY);
void CK4_SmirkySearch(CK_object *obj);
void CK4_SmirkyTeleport(CK_object *obj);
void CK4_SmirkyCol(CK_object *a, CK_object *b);
void CK4_SmirkyCheckTiles(CK_object *obj);
void CK4_SmirkyDraw(CK_object *obj);

// Mimrocks
void CK4_SpawnMimrock(int tileX, int tileY);
void CK4_MimrockWait(CK_object *obj);
void CK4_MimrockCheckJump(CK_object *obj);
void CK4_MimrockCol(CK_object *a, CK_object *b);
void CK4_MimrockAirCol(CK_object *a, CK_object *b);
void CK4_MimrockJumpDraw(CK_object *obj);
void CK4_MimrockBounceDraw(CK_object *obj);

// Dopefish
void CK4_SpawnDopefish(int tileX, int tileY);
void CK4_KillKeenUnderwater(CK_object *obj);
void CK4_DopefishMove(CK_object *obj);
void CK4_DopefishEat(CK_object *obj);
void CK4_DopefishAfterBurp(CK_object *obj);
void CK4_DopefishBurp(CK_object *obj);
void CK4_Bubbles(CK_object *obj);
void CK4_DopefishCol(CK_object *a, CK_object *b);
void CK4_FishDraw(CK_object *obj);

// Schoolfish
void CK4_SpawnSchoolfish(int tileX, int tileY);
void CK4_SchoolfishMove(CK_object *obj);

// Sprites
void CK4_SpawnSprite(int tileX, int tileY);
void CK4_SpritePatrol(CK_object *obj);
void CK4_SpriteAim(CK_object *obj);
void CK4_SpriteShoot(CK_object *obj);
void CK4_ProjectileDraw(CK_object *obj);

// Mines
void CK4_SpawnMine(int tileX, int tileY, int direction);
void CK4_MineCol(CK_object *a, CK_object *b);

// Lindsey
void CK4_SpawnLindsey(int tileX, int tileY);
void CK4_LindseyFloat(CK_object *obj);

// Dart guns
void CK4_SpawnDartGun(int tileX, int tileY, int direction);
void CK4_DartGun(CK_object *obj);

// Wetsuit
void CK4_SpawnWetsuit(int tileX, int tileY);
void CK4_WetsuitCol(CK_object *a, CK_object *b);

void CK4_Obj3_SetupFunctions();

END_UPP_NAMESPACE

#endif