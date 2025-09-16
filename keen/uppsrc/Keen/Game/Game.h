// Umbrella header for Keen/Game (gameplay)
#pragma once

#include <Keen/Id/Id.h>

NAMESPACE_UPP

namespace Game {
void Bootstrap();
}

#include "Map.h"
#include "Assets.h"
#include "Resources.h"
#include "Player.h"
#include "State.h"
#include "Loop.h"
#include "Hud.h"
#include "TitleMenu.h"

END_UPP_NAMESPACE
