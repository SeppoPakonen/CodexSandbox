#ifndef CK5_OBJ1_H
#define CK5_OBJ1_H

//#include "Game.h"

//NAMESPACE_UPP

// Episode 5 object functions (formerly in ck5_obj1.c)

void CK5_PurpleGoPlatThink(CK_object *obj);
void CK5_SpawnVolte(int tileX, int tileY);
void CK5_VolteMove(CK_object *obj);
void CK5_VolteCol(CK_object *volte, CK_object *other);
void CK5_Obj1_SetupFunctions();

//END_UPP_NAMESPACE

#endif