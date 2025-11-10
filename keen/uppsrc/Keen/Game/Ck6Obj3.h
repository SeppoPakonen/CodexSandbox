#ifndef CK6_OBJ3_H
#define CK6_OBJ3_H

#include "Game.h"

NAMESPACE_UPP

// Episode 6 object functions (formerly in ck6_obj3.c)

// Fleex
void CK6_SpawnFleex(int tileX, int tileY);
void CK6_FleexWalk(CK_object *obj);
void CK6_FleexSearch(CK_object *obj);
void CK6_FleexCol(CK_object *a, CK_object *b);

// Bobbas
void CK6_SpawnBobba(int tileX, int tileY);
void CK6_BobbaFireball(CK_object *obj);
void CK6_Bobba(CK_object *obj);
void CK6_BobbaCol(CK_object *a, CK_object *b);
void CK6_BobbaJumpDraw(CK_object *obj);
void CK6_BobbaFireballDraw(CK_object *obj);

// Babobbas
void CK6_SpawnBabobba(int tileX, int tileY);
void CK6_BabobbaSit(CK_object *obj);
void CK6_BabobbaCol(CK_object *a, CK_object *b);
void CK6_BabobbaNapCol(CK_object *a, CK_object *b);
void CK6_BabobbaJumpDraw(CK_object *obj);
void CK6_BabobbaShot(CK_object *obj);
void CK6_BabobbaShotVanish(CK_object *obj);

// Blorbs
void CK6_SpawnBlorb(int tileX, int tileY);
void CK6_BlorbDraw(CK_object *obj);

// Ceilicks
void CK6_SpawnCeilick(int tileX, int tileY);
void CK6_Ceilick(CK_object *obj);
void CK6_CeilickDescend(CK_object *obj);
void CK6_CeilickStunned(CK_object *obj);
void CK6_CeilickCol(CK_object *a, CK_object *b);

void CK6_Obj3_SetupFunctions();

END_UPP_NAMESPACE

#endif