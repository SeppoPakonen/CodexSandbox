#ifndef CK6_OBJ2_H
#define CK6_OBJ2_H

//#include "Game.h"

//NAMESPACE_UPP

// Episode 6 object functions (formerly in ck6_obj2.c)

// Nospikes
void CK6_SpawnNospike(int tileX, int tileY);
void CK6_NospikeWalk(CK_object *obj);
void CK6_NospikeCharge(CK_object *obj);
void CK6_NospikeCol(CK_object *a, CK_object *b);
void CK6_NospikeFall(CK_object *obj);
void CK6_NospikeFallDraw(CK_object *obj);
void CK6_NospikeFallDraw2(CK_object *obj);
void CK6_NospikeChargeDraw(CK_object *obj);

// Giks
void CK6_SpawnGik(int tileX, int tileY);
void CK6_GikWalk(CK_object *obj);
void CK6_GikSlide(CK_object *obj);
void CK6_GikJumpDraw(CK_object *obj);
void CK6_GikSlideDraw(CK_object *obj);

// Orbatrices
void CK6_SpawnOrbatrix(int tileX, int tileY);
void CK6_OrbatrixFloat(CK_object *obj);
void CK6_OrbatrixCol(CK_object *a, CK_object *b);
void CK6_OrbatrixDraw(CK_object *obj);
void CK6_OrbatrixBounceDraw(CK_object *obj);
void CK6_OrbatrixCurl(CK_object *obj);
void CK6_OrbatrixUncurlThink(CK_object *obj);
void CK6_OrbatrixCol2(CK_object *a, CK_object *b);

// Bips
void CK6_BipWalk(CK_object *obj);
void CK6_BipCol(CK_object *a, CK_object *b);
void CK6_BipShotDraw(CK_object *obj);

// Bipships
void CK6_SpawnBipship(int tileX, int tileY);
void CK6_BipshipTurn(CK_object *obj);
void CK6_BipshipFly(CK_object *obj);
void CK6_BipshipCrash(CK_object *obj);
void CK6_BipshipCol(CK_object *a, CK_object *b);

// Flects
void CK6_SpawnFlect(int tileX, int tileY);
void CK6_FlectTurn(CK_object *obj);
void CK6_FlectWalk(CK_object *obj);
void CK6_FlectCol(CK_object *a, CK_object *b);
void CK6_FlectDraw(CK_object *obj);

void CK6_Obj2_SetupFunctions();

//END_UPP_NAMESPACE

#endif