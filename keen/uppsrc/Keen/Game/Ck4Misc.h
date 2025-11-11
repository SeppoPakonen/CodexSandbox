#ifndef CK4_MISC_H
#define CK4_MISC_H

//#include "Game.h"

//NAMESPACE_UPP

// Episode 4 specific functions (formerly in ck4_misc.c)

extern CK_EpisodeDef ck4_episode;

void CK4_SetupFunctions();
bool CK4_IsPresent();
CK_object *CK4_SpawnEnemyShot(int posX, int posY, CK_action *action);
void CK4_ScanInfoLayer();

// Dialog functions
void CK4_ShowPrincessMessage(void);
void CK4_ShowJanitorMessage(void);
void CK4_ShowCantSwimMessage(void);
void CK4_ShowWetsuitMessage(void);
void CK4_ShowCouncilMessage(void);

// Scuba Keen functions
void CK4_SpawnScubaKeen(int tileX, int tileY);
void CK4_SpawnKeenBubble(CK_object *obj);
void CK4_KeenSwim(CK_object *obj);
void CK4_KeenSwimFast(CK_object *obj);
void CK4_KeenSwimCol(CK_object *a, CK_object *b);
void CK4_KeenSwimDraw(CK_object *obj);

void CK4_Misc_SetupFunctions();

//END_UPP_NAMESPACE

#endif