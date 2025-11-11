#ifndef UPP_KEEN_PHYSICS_H
#define UPP_KEEN_PHYSICS_H

//#include "Game.h"

//NAMESPACE_UPP

void CK_ResetClipRects(CK_object *obj);
void CK_SetOldClipRects(CK_object *obj);
void CK_SetDeltaClipRects(CK_object *obj);
void CK_PhysUpdateX(CK_object *obj, int16_t deltaUnitX);
void CK_PhysUpdateY(CK_object *obj, int16_t deltaUnitY);
void CK_PhysKeenClipDown(CK_object *obj);
void CK_PhysKeenClipUp(CK_object *obj);
bool CK_NotStuckInWall(CK_object *obj);
bool CK_PreviewClipRects(CK_object *obj, CK_action *act);
void CK_PhysClipVert(CK_object *obj);
void CK_PhysClipHorz(CK_object *obj);
void CK_PhysUpdateNormalObj(CK_object *obj);
void CK_PhysFullClipToWalls(CK_object *obj);
void CK_PhysUpdateSimpleObj(CK_object *obj);
void CK_PhysPushX(CK_object *pushee, CK_object *pusher);
void CK_PhysPushY(CK_object *pushee, CK_object *pusher);
void CK_PhysPushXY(CK_object *passenger, CK_object *platform, bool squish);
void CK_SetAction(CK_object *obj, CK_action *act);
void CK_SetAction2(CK_object *obj, CK_action *act);
bool CK_ObjectVisible(CK_object *obj);
void CK_PhysGravityHigh(CK_object *obj);
void CK_PhysGravityMid(CK_object *obj);
void CK_PhysGravityLow(CK_object *obj);
void CK_PhysDampHorz(CK_object *obj);

extern CK_objPhysData ck_oldRects;
extern CK_objPhysDataDelta ck_deltaRects;
extern int16_t ck_nextX;
extern int16_t ck_nextY;
extern bool ck_keenIgnoreVertClip;

//END_UPP_NAMESPACE

#endif