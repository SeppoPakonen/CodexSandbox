// Umbrella header for Keen/Game (gameplay)
#pragma once

#include <Keen/Id/Id.h>
#include <cstdint>

NAMESPACE_UPP

namespace Game {
void Bootstrap();
}

// Game loop and related functions from original ck_game.c
void CK_GameLoop();
void CK_LoadLevel(bool doCache, bool silent);

void CK_BeginCacheBox(const char *title, int numChunks);
void CK_UpdateCacheBox(void);
void CK_FinishCacheBox(void);

#include "Main.h"
#include "Play.h"
#include "Map.h"
#include "Assets.h"
#include "Resources.h"
#include "Player.h"
#include "State.h"
#include "Loop.h"
#include "Hud.h"
#include "TitleMenu.h"
#include "Cross.h"
#include "Defs.h"
#include "Act.h"
#include "Inter.h"
#include "Keen.h"
#include "Map.h"
#include "Misc.h"
#include "Obj.h"
#include "Quit.h"
#include "Text.h"
#include "Episode.h"
#include "Ck4Ep.h"
#include "Ck5Ep.h"
#include "Ck6Ep.h"
#include "Physics.h"

END_UPP_NAMESPACE
