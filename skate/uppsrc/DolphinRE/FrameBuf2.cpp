#include "DolphinRE.h"


void GXAdjustForOverscan(const GXRenderModeObj *rm_in, GXRenderModeObj *rm_out, u16 horz, u16 vert) {
	int horz_pow2 = ((uint)horz & 0x7fff) << 1;
	int vert_pow2 = ((uint)vert & 0x7fff) << 1;
	
	if (rm_in != rm_out)
		memcpy(rm_out, rm_in, sizeof(GXRenderModeObj));
	
	rm_out->fbWidth -= horz_pow2;
	
	if (rm_out->aa == 0)
		rm_out->efbHeight -= vert_pow2;
	else
		rm_out->efbHeight -= vert;
	
	if (rm_out->xFBmode == VI_XFBMODE_SF)
		rm_out->xfbHeight -= vert;
	else
		rm_out->xfbHeight -= vert_pow2;
	
	rm_out->viWidth		-= horz_pow2;
	rm_out->viHeight	-= vert_pow2;
	rm_out->viXOrigin	+= horz;
	rm_out->viYOrigin	+= vert;
}

