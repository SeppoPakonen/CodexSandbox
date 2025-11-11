#ifndef CK6_MISC_H
#define CK6_MISC_H

//#include "Game.h"

//NAMESPACE_UPP

// Episode 6 miscellaneous functions (formerly in ck6_misc.c)

extern CK_EpisodeDef ck6_episode;

void CK6_SetupFunctions();
bool CK6_IsPresent();
void CK6_ScanInfoLayer();
void CK6_ToggleBigSwitch(CK_object *obj, bool dir);
void CK6_ShowGetSandwich();
void CK6_ShowGetRope();
void CK6_ShowGetPasscard();

//END_UPP_NAMESPACE

#endif