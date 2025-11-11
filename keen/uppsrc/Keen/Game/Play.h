#ifndef UPP_KEEN_PLAY_H
#define UPP_KEEN_PLAY_H

//#include <Keen/Id/Id.h>

//NAMESPACE_UPP

// Function declarations from original ck_play.h
// Status window constants
#define STATUS_W 192	// Width in pixels
#define STATUS_H 152	// Height in pixels
#define STATUS_X 64	// X-coord in pixels (without scroll adjustment)
#define STATUS_Y 16	// Y-coord in pixels when fully down
#define STATUS_BOTTOM (STATUS_H + STATUS_Y)	// Bottom of status window

struct CK_object;

extern int ck_activeX0Tile;
extern int ck_activeY0Tile;
extern int ck_activeX1Tile;
extern int ck_activeY1Tile;

extern bool ck_scrollDisabled;

extern bool ck_godMode;

extern bool ck_debugActive;

extern int16_t ck_invincibilityTimer;

extern bool ck_scoreBoxEnabled;
extern struct CK_object *ck_scoreBoxObj;

extern bool ck_twoButtonFiring;

extern bool ck_fixJerkyMotion;
extern bool ck_svgaCompatibility;

extern bool ck_gamePadEnabled;

// Object Mgmt
struct CK_object *CK_GetNewObj(bool nonCritical);
void CK_SetupObjArray();
void CK_RemoveObj(struct CK_object *obj);

// Actions/Camera
void CK_RunAction(struct CK_object *obj);
void CK_CentreCamera(struct CK_object *obj);
void CK_MapCamera(struct CK_object *keen);
void CK_NormalCamera(struct CK_object *obj);

// Status Window
extern void *ck_backupSurface;
extern void *ck_statusSurface;
void CK_ShowStatusWindow(void);

// Playing
void CK_PlayDemo(int demoChunk);
void CK_PlayLoop();

//END_UPP_NAMESPACE

#endif