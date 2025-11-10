#include "Game.h"
#include <stdio.h>
#include <cstring>

NAMESPACE_UPP

// =========================================================================

void CK6_MapMiscFlagsCheck(CK_object *obj)
{
	if (obj->user3 == 0)
	{
		int tileX = obj->clipRects.tileXmid;
		int tileY = RF_UnitToTile(obj->clipRects.unitY1 +
			(obj->clipRects.unitY2 - obj->clipRects.unitY1) / 2);
		uint16_t tile = CA_TileAtPos(tileX, tileY, 1);
		uint8_t miscValue = TI_ForeMisc(tile);

		if (miscValue == MISCFLAG_TELEPORT)
			CK_AnimateMapTeleporter(tileX, tileY);
	}
}

void CK6_Map_SetupFunctions()
{
	CK_ACT_AddFunction("CK6_MapMiscFlagsCheck", &CK6_MapMiscFlagsCheck);
}

END_UPP_NAMESPACE