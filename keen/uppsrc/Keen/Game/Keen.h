#ifndef UPP_KEEN_KEEN_H
#define UPP_KEEN_KEEN_H

//#include "Game.h"

//NAMESPACE_UPP

typedef struct CK_keenState
{
	int jumpTimer;
	int poleGrabTime;
	bool jumpIsPressed;
	bool jumpWasPressed;
	bool pogoIsPressed;
	bool pogoWasPressed;
	bool shootIsPressed;
	bool shootWasPressed;

	bool keenSliding;
	CK_object *platform;
} CK_keenState;

extern CK_keenState ck_keenState;

void CK_KeenColFunc(CK_object *a, CK_object *b);
void CK_IncreaseScore(int score);
void CK_SpawnKeen(int tileX, int tileY, int direction);
void CK_KeenGetTileItem(int tileX, int tileY, int itemNumber);
void CK_KeenGetTileCentilife(int tileX, int tileY);
void CK_KeenCheckSpecialTileInfo(CK_object *obj);
bool CK_KeenPressUp(CK_object *obj);
void CK_KeenSlide(CK_object *obj);
void CK_KeenEnterDoor0(CK_object *obj);
void CK_KeenEnterDoor1(CK_object *obj);
void CK_KeenEnterDoor(CK_object *obj);
void CK_KeenPlaceGem(CK_object *obj);
void CK_KeenRidePlatform(CK_object *obj);
bool CK_KeenTryClimbPole(CK_object *obj);
void CK_KeenRunningThink(CK_object *obj);
void CK_KeenStandingThink(CK_object *obj);
void CK_HandleInputOnGround(CK_object *obj);
void CK_KeenLookUpThink(CK_object *obj);
void CK_KeenLookDownThink(CK_object *obj);
void CK_KeenPressSwitchThink(CK_object *obj);
void CK_KeenDrawFunc(CK_object *obj);
void CK_KeenRunDrawFunc(CK_object *obj);
void CK_KeenReadThink(CK_object *obj);
void CK_KeenJumpThink(CK_object *obj);
void CK_KeenJumpDrawFunc(CK_object *obj);
void CK_KeenPogoThink(CK_object *obj);
void CK_KeenPogoBounceThink(CK_object *obj);
void CK_KeenPogoDrawFunc(CK_object *obj);
void CK_KeenSpecialDrawFunc(CK_object *obj);
void CK_KeenSpecialColFunc(CK_object *obj, CK_object *other);
void CK_KeenHangThink(CK_object *obj);
void CK_KeenPullThink1(CK_object *obj);
void CK_KeenPullThink2(CK_object *obj);
void CK_KeenPullThink3(CK_object *obj);
void CK_KeenPullThink4(CK_object *obj);
void CK_KeenPoleSitThink(CK_object *obj);
void CK_KeenPoleUpThink(CK_object *obj);
void CK_KeenPoleDownThink(CK_object *obj);
void CK_KeenJumpDownThink(CK_object *obj);
void CK_KeenPoleDownDrawFunc(CK_object *obj);
void CK_KeenSetClipped(CK_object *obj);
void CK_KeenDeathThink(CK_object *obj);
void CK_KillKeen();
void CK_SpawnShot(int x, int y, int direction);
void CK_ShotHit(CK_object *obj);
void CK_ShotThink(CK_object *shot);
void CK_ShotDrawFunc(CK_object *obj);
void CK_KeenSpawnShot(CK_object *obj);
void CK_KeenSetupFunctions();

//END_UPP_NAMESPACE

#endif