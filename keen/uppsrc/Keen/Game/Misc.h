#ifndef UPP_KEEN_MISC_H
#define UPP_KEEN_MISC_H

#include "Game.h"

NAMESPACE_UPP

void CK_SetDraw(CK_object *obj);
void CK_Fall(CK_object *obj);
void CK_Fall2(CK_object *obj);
void CK_Glide(CK_object *obj);
void CK_BasicDrawFunc1(CK_object *obj);
void CK_BasicDrawFunc2(CK_object *obj);
void CK_BasicDrawFunc3(CK_object *obj);
void CK_ObjBadstate(CK_object *obj);
void CK_BasicDrawFunc4(CK_object *obj);
void CK_StunCreature(CK_object *creature, CK_object *stunner, CK_action *new_creature_act);
void CK_DeadlyCol(CK_object *o1, CK_object *o2);
void CK_LethalCol(CK_object *o1, CK_object *o2);
void CK_PushKeenCol(CK_object *a, CK_object *b);
void CK_CarryKeenCol(CK_object *a, CK_object *b);
void CK_DieOnContactDraw(CK_object *obj);
void CK_ShrapnelTileCol(CK_object *obj);
void CK_Misc_SetupFunctions(void);

END_UPP_NAMESPACE

#endif