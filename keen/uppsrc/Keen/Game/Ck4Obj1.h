#ifndef CK4_OBJ1_H
#define CK4_OBJ1_H

//#include "Game.h"

//NAMESPACE_UPP

// Episode 4 object functions (formerly in ck4_obj1.c)

// Miragia
void CK4_SpawnMiragia(int tileX, int tileY);
void CK4_Miragia0(CK_object *obj);
void CK4_Miragia1(CK_object *obj);
void CK4_Miragia2(CK_object *obj);
void CK4_Miragia3(CK_object *obj);
void CK4_Miragia4(CK_object *obj);
void CK4_Miragia5(CK_object *obj);
void CK4_Miragia6(CK_object *obj);
void CK4_Miragia7(CK_object *obj);

// Council Member
void CK4_SpawnCouncilMember(int tileX, int tileY);
void CK4_CouncilWalk(CK_object *obj);

// Slugs
void CK4_SpawnSlug(int tileX, int tileY);
void CK4_SlugMove(CK_object *obj);
void CK4_SlugSlime(CK_object *obj);
void CK4_SlugCol(CK_object *a, CK_object *b);

// Mad Mushrooms
void CK4_SpawnMushroom(int tileX, int tileY);
void CK4_MushroomMove(CK_object *obj);
void CK4_MushroomCol(CK_object *a, CK_object *b);
void CK4_MushroomDraw(CK_object *obj);

// Bluebirds
void CK4_SpawnEgg(int tileX, int tileY);
void CK4_BirdRecover(CK_object *obj);
void CK4_SpawnBird(int tileX, int tileY);
void CK4_EggCol(CK_object *a, CK_object *b);
void CK4_BirdWalk(CK_object *obj);
void CK4_BirdFly(CK_object *obj);
void CK4_BirdCol(CK_object *a, CK_object *b);
void CK4_BirdHatchedCol(CK_object *a, CK_object *b);
void CK4_BirdDraw(CK_object *obj);
void CK4_BirdLandingDraw(CK_object *obj);
void CK4_EggshellDraw(CK_object *obj);
void CK4_BirdFlyingDraw(CK_object *obj);

// Arachnut
void CK4_SpawnArachnut(int tileX, int tileY);
void CK4_ArachnutSearch(CK_object *obj);
void CK4_ArachnutCol(CK_object *a, CK_object *b);
void CK4_ArachnutStunnedCol(CK_object *a, CK_object *b);

// Skypests
void CK4_SpawnSkypest(int tileX, int tileY);
void CK4_SkypestFly(CK_object *obj);
void CK4_SkypestAirCol(CK_object *a, CK_object *b);
void CK4_SkypestGroundCol(CK_object *a, CK_object *b);
void CK4_SkypestTakeoff(CK_object *obj);
void CK4_SkypestDraw(CK_object *obj);

void CK4_Obj1_SetupFunctions();

//END_UPP_NAMESPACE

#endif