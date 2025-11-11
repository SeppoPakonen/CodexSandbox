#ifndef CK5_OBJ3_H
#define CK5_OBJ3_H

//#include "Game.h"

//NAMESPACE_UPP

// Episode 5 object functions (formerly in ck5_obj3.c)

// Shikadi Mine
void CK5_SpawnMine(int tileX, int tileY);
int CK5_Walk(CK_object *obj, CK_Controldir dir);
int CK5_MinePathClear(int tileX, int tileY);
void CK5_SeekKeen(CK_object *obj);
void CK5_MineMove(CK_object *obj);
void CK5_MineCol(CK_object *o1, CK_object *o2);
void CK5_MineShrapCol(CK_object *o1, CK_object *o2);
void CK5_MineMoveDotsToCenter(CK_object *obj);
void CK5_MineMoveDotsToSides(CK_object *obj);
void CK5_MineExplode(CK_object *obj);
void CK5_MineTileCol(CK_object *obj);

// Robo Red
void CK5_SpawnRobo(int tileX, int tileY);
void CK5_RoboMove(CK_object *obj);
void CK5_RoboCol(CK_object *o1, CK_object *o2);
void CK5_RoboShoot(CK_object *obj);
void CK5_RoboShotCol(CK_object *o1, CK_object *o2);
void CK5_RoboShotTileCol(CK_object *obj);

// Spirogrip
void CK5_SpawnSpirogrip(int tileX, int tileY);
void CK5_SpirogripSpin(CK_object *obj);
void CK5_SpirogripFlyDraw(CK_object *obj);

// Spindred
void CK5_SpawnSpindred(int tileX, int tileY);
void CK5_SpindredBounce(CK_object *obj);
void CK5_SpindredTileCol(CK_object *obj);

// Shikadi Master
void CK5_SpawnMaster(int tileX, int tileY);
void CK5_MasterStand(CK_object *obj);
void CK5_MasterShoot(CK_object *obj);
void CK5_MasterCol(CK_object *o1, CK_object *o2);
void CK5_MasterTele(CK_object *obj);
void CK5_MasterTeleTileCol(CK_object *obj);
void CK5_MasterBallCol(CK_object *o1, CK_object *o2);
void CK5_MasterBallTileCol(CK_object *obj);
void CK5_MasterSparksTileCol(CK_object *obj);

// Shikadi
void CK5_SpawnShikadi(int tileX, int tileY);
void CK5_ShikadiWalk(CK_object *obj);
void CK5_ShikadiCol(CK_object *o1, CK_object *o2);
void CK5_ShikadiPole(CK_object *obj);
void CK5_PoleZap(CK_object *obj);
void CK5_ShikadiTileCol(CK_object *obj);

// Shocksund
void CK5_SpawnShocksund(int tileX, int tileY);
void CK5_ShocksundSearch(CK_object *obj);
void CK5_ShocksundStand(CK_object *obj);
void CK5_ShocksundShoot(CK_object *obj);
void CK5_ShocksundCol(CK_object *o1, CK_object *o2);
void CK5_ShocksundGroundTileCol(CK_object *obj);
void CK5_ShocksundJumpTileCol(CK_object *obj);

// Bark Shot
void CK5_BarkShotCol(CK_object *o1, CK_object *o2);
void CK5_BarkShotTileCol(CK_object *obj);

// Sphereful
void CK5_SpawnSphereful(int tileX, int tileY);
void CK5_SpherefulBounce(CK_object *obj);
void CK5_SpherefulTileCol(CK_object *obj);

// Korath
void CK5_KorathWalk(CK_object *obj);
void CK5_KorathColFunc(CK_object *obj, CK_object *other);
void CK5_SpawnKorath(int tileX, int tileY);

// QED
void CK5_QEDSpawn(int tileX, int tileY);

void CK5_Obj3_SetupFunctions();

//END_UPP_NAMESPACE

#endif