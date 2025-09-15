#include "DolphinRE.h"


u32 ScreenLeft;
u32 ScreenTop;
GXCullMode CurrentCull;

void GXSetScissor(uint left, uint top, uint width, uint height) {
	Gfx::Static::Enable(SCISSOR_TEST);
	Gfx::Static::Scissor(left, ScreenHeight - (top + height), width, height);
	
	ScreenLeft = left;
	ScreenTop = top;
	ScreenWidth = width;
	ScreenHeight = height;
}


void GXGetScissor(uint *left, uint *top, uint *width, uint *height) {
	*left = ScreenLeft;
	*top = ScreenTop;
	*width = ScreenWidth;
	*height = ScreenHeight;
}


void GXSetCullMode(GXCullMode mode) {
	switch (mode) {
		
	case GX_CULL_NONE:
		Disable(CULL_FACE);
		break;
		
	case GX_CULL_FRONT:
		Enable(CULL_FACE);
		CullFace(CM_BACK);
		break;
		
	case GX_CULL_BACK:
		Enable(CULL_FACE);
		CullFace(CM_FRONT);
		break;
		
	case GX_CULL_ALL:
		Enable(CULL_FACE);
		CullFace(CM_FRONT_AND_BACK);
		
	}
	
	CurrentCull = mode;
}


void GXGetCullMode(GXCullMode *mode) {
	*mode = CurrentCull;
}


void GXSetCoPlanar(GXBool enable) {
	if (enable == false) {
		Disable(POLYGON_OFFSET_POINT);
		Disable(POLYGON_OFFSET_LINE);
		Disable(POLYGON_OFFSET_FILL);
	} else {
		Enable(POLYGON_OFFSET_POINT);
		Enable(POLYGON_OFFSET_LINE);
		Enable(POLYGON_OFFSET_FILL);
		PolygonOffset(1.0, 1.0);
	}
}


