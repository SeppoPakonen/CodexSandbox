#ifndef CK4_OBJ2_H
#define CK4_OBJ2_H

//#include "Game.h"

//NAMESPACE_UPP

// Episode 4 object functions (formerly in ck4_obj2.c)

// Wormmouth
void CK4_SpawnWormmouth(int tileX, int tileY);
void CK4_WormmouthLookRight(CK_object *obj);
void CK4_WormmouthGoToPlayer(CK_object *obj);
void CK4_WormmouthLookLeft(CK_object *obj);
void CK4_WormmouthMove(CK_object *obj);
void CK4_WormmouthCheckShot(CK_object *a, CK_object *b);
void CK4_WormmouthCol(CK_object *a, CK_object *b);

// Clouds
void CK4_SpawnCloud(int tileX, int tileY);
void CK4_CloudMove(CK_object *obj);
void CK4_CloudCheckStrike(CK_object *obj);
void CK4_CloudDraw(CK_object *obj);
void CK4_CloudStrike(CK_object *obj);
void CK4_CloudCol(CK_object *a, CK_object *b);

// Berkeloids
void CK4_SpawnBerkeloid(int tileX, int tileY);
void CK4_BerkeloidMove(CK_object *obj);
void CK4_BerkeloidThrow(CK_object *obj);
void CK4_BerkeloidThrowDone(CK_object *obj);
void CK4_BerkeloidCol(CK_object *a, CK_object *b);
void CK4_FireballDraw(CK_object *obj);
void CK4_BerkeloidHover(CK_object *obj);
void CK4_BerkeloidDraw(CK_object *obj);

// Inchworms and Feet
void CK4_SpawnInchworm(int tileX, int tileY);
void CK4_SpawnFoot(int tileX, int tileY);
void CK4_InchwormMove(CK_object *obj);
void CK4_InchwormCol(CK_object *a, CK_object *b);
void CK4_FootCol(CK_object *a, CK_object *b);

// Bounders
void CK4_SpawnBounder(int tileX, int tileY);
void CK4_BounderCheckShot(CK_object *a, CK_object *b);
void CK4_BounderDraw(CK_object *obj);

// Licks
void CK4_SpawnLick(int tileX, int tileY);
void CK4_LickMove(CK_object *obj);
void CK4_LickCheckShot(CK_object *a, CK_object *b);
void CK4_LickCol(CK_object *a, CK_object *b);
void CK4_LickDraw(CK_object *obj);

// Platforms
void CK4_SpawnAxisPlatform(int tileX, int tileY, int direction);
void CK4_PlatformDraw(CK_object *obj);

void CK4_Obj2_SetupFunctions();

//END_UPP_NAMESPACE

#endif