#ifndef CK5_OBJ2_H
#define CK5_OBJ2_H

//#include "Game.h"

//NAMESPACE_UPP

// Episode 5 object functions (formerly in ck5_obj2.c)

// Sparky
void CK5_SpawnSparky(int tileX, int tileY);
void CK5_SparkyWait(CK_object *obj);
void CK5_SparkyPrepareCharge(CK_object *obj);
void CK5_SparkySearchLeft(CK_object *obj);
void CK5_SparkySearchRight(CK_object *obj);
void CK5_SparkyCharge0(CK_object *obj);
void CK5_SparkyCharge1(CK_object *obj);
void CK5_SparkyCol(CK_object *obj1, CK_object *obj2);
void CK5_SparkyTileCol(CK_object *obj);

// Ampton
void CK5_SpawnAmpton(int tileX, int tileY);
void CK5_AmptonWalk(CK_object *obj);
void CK5_AmptonPoleClimb(CK_object *obj);
void CK5_AmptonSwitch(CK_object *obj);
void CK5_AmptonCol(CK_object *obj1, CK_object *obj2);
void CK5_AmptonTileCol(CK_object *obj);

// Slice
void CK5_SpawnSlice(int tileX, int tileY, int dir);
void CK5_SpawnSliceDiag(int tileX, int tileY);
void CK5_SliceCol(CK_object *obj1, CK_object *obj2);
void CK5_SliceDiagTileCol(CK_object *obj);

// Shelly
void CK5_SpawnShelly(int tileX, int tileY);
void CK5_ShellyWait(CK_object *obj);
void CK5_ShellyStartJump(CK_object *obj);
void CK5_SpawnShellyBits(CK_object *obj);
void CK5_ShellyCol(CK_object *obj1, CK_object *obj2);
void CK5_ShellyGroundTileCol(CK_object *obj);
void CK5_ShellyAirTileCol(CK_object *obj);

void CK5_Obj2_SetupFunctions();

//END_UPP_NAMESPACE

#endif