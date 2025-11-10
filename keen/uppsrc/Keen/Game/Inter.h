#ifndef UPP_KEEN_INTER_H
#define UPP_KEEN_INTER_H

#include "Game.h"

NAMESPACE_UPP

// From the original C file ck_inter.c
extern int ck_startingSavedGame;
extern bool ck_inHighScores;
extern CK_Difficulty ck_startingDifficulty;

void CK_HandleDemoKeys();
void CK_DrawTerminator(void);
void CK_DrawStarWars();
void CK_ShowTitleScreen();
void CK_PlayDemoFile(const char *demoName);
void CK_PlayDemo(int demoNumber);
void CK_OverlayHighScores();
void CK_SubmitHighScore(int score, uint16_t arg_4);
void CK_DoHighScores();
void CK_SetDefaultHighScores();

END_UPP_NAMESPACE

#endif