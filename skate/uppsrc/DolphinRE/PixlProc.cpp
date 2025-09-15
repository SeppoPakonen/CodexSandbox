#include "DolphinRE.h"




void GXSetFog(GXFogType type, f32 startz, f32 endz, f32 nearz, f32 farz, GXColor color) {
	Gxf param;
	Gxf fog_density;
	Gxf fog_clr[4];
	uint alpha, blue, green;
	
	if (type == GX_FOG_NONE) {
		Disable(FOG);
	}
	else {
		Enable(FOG);
		
		if (type == GX_FOG_LIN) {
			Fog(FOGPARAM_MODE,		FOGMODE_LINEAR);
			Fog(FOGPARAM_START,	startz);
			Fog(FOGPARAM_END,		endz);
		}
		else if (type == GX_FOG_EXP) {
			param = 5.5451775f / (float)endz;
			Fog(FOGPARAM_MODE,		FOGMODE_EXP);
			Fog(FOGPARAM_DENSITY,	param);
		}
		else if (type == GX_FOG_EXP2) {
			param = 2.35482f / (float)endz;
			Fog(FOGPARAM_MODE,		FOGMODE_EXP2);
			Fog(FOGPARAM_DENSITY,	param);
		}
		else {
			ASSERTMSG(false, "Invalid fog mode");
		}
		
		fog_clr[0] = (float)color.r / 255.0;
		fog_clr[1] = (float)color.g / 255.0;
		fog_clr[2] = (float)color.b / 255.0;
		fog_clr[3] = (float)color.a / 255.0;
		
		Fog(FOGPARAM_COLOR,		fog_clr);
	}
}


void GXSetBlendMode(GXBlendMode type, GXBlendFactor src_factor, GXBlendFactor dst_factor, GXLogicOp op) {
	DstFactor dfactor;
	SrcFactor sfactor;
	
	if (type == GX_BM_NONE) {
		Disable(BLEND);
		Disable(COLOR_LOGIC_OP);
	}
	
	else if (type == GX_BM_BLEND) {
		Enable(BLEND);
		Disable(COLOR_LOGIC_OP);
		
		switch (src_factor) {
			
		case GX_BL_ZERO:
			sfactor = SRCFAC_ZERO;
			break;
			
		case GX_BL_ONE:
			sfactor = SRCFAC_ONE;
			break;
			
		case GX_BL_SRCCLR:
			sfactor = SRCFAC_DST_COLOR;
			break;
			
		case GX_BL_INVSRCCLR:
			sfactor = SRCFAC_ONE_MINUS_DST_COLOR;
			break;
			
		case GX_BL_SRCALPHA:
			sfactor = SRCFAC_SRC_ALPHA;
			break;
			
		case GX_BL_INVSRCALPHA:
			sfactor = SRCFAC_ONE_MINUS_SRC_ALPHA;
			break;
			
		case GX_BL_DSTALPHA:
			sfactor = SRCFAC_DST_ALPHA;
			break;
			
		case GX_BL_INVDSTALPHA:
			sfactor = SRCFAC_ONE_MINUS_DST_ALPHA;
			break;
			
		default:
			ASSERTMSG(false, "Invalid src mode");
			
		}
		
		switch (dst_factor) {
			
		case GX_BL_ZERO:
			dfactor = DSTFAC_ZERO;
			break;
			
		case GX_BL_ONE:
			dfactor = DSTFAC_ONE;
			break;
			
		case GX_BL_SRCCLR:
			dfactor = DSTFAC_SRC_COLOR;
			break;
			
		case GX_BL_INVSRCCLR:
			dfactor = DSTFAC_ONE_MINUS_SRC_COLOR;
			break;
			
		case GX_BL_SRCALPHA:
			dfactor = DSTFAC_SRC_ALPHA;
			break;
			
		case GX_BL_INVSRCALPHA:
			dfactor = DSTFAC_ONE_MINUS_SRC_ALPHA;
			break;
			
		case GX_BL_DSTALPHA:
			dfactor = DSTFAC_DST_ALPHA;
			break;
			
		case GX_BL_INVDSTALPHA:
			dfactor = DSTFAC_ONE_MINUS_DST_ALPHA;
			break;
			
		default:
			ASSERTMSG(false, "Invalid destination mode");
			
		}
		
		BlendFunc(sfactor, dfactor);
	}
	else if (type == GX_BM_LOGIC) {
		Enable(COLOR_LOGIC_OP);
		Disable(BLEND);
		
		switch (op) {
			
		case GX_LO_CLEAR:
			LogicOp(LOGOP_CLEAR);
			break;
			
		case GX_LO_AND:
			LogicOp(LOGOP_AND);
			break;
			
		case GX_LO_REVAND:
			LogicOp(LOGOP_AND_REVERSE);
			break;
			
		case GX_LO_COPY:
			LogicOp(LOGOP_COPY);
			break;
			
		case GX_LO_INVAND:
			LogicOp(LOGOP_AND_INVERTED);
			break;
			
		case GX_LO_NOOP:
			LogicOp(LOGOP_NOOP);
			break;
			
		case GX_LO_XOR:
			LogicOp(LOGOP_XOR);
			break;
			
		case GX_LO_OR:
			LogicOp(LOGOP_OR);
			break;
			
		case GX_LO_NOR:
			LogicOp(LOGOP_NOR);
			break;
			
		case GX_LO_EQUIV:
			LogicOp(LOGOP_EQUIV);
			break;
			
		case GX_LO_INV:
			LogicOp(LOGOP_INVERT);
			break;
			
		case GX_LO_REVOR:
			LogicOp(LOGOP_OR_REVERSE);
			break;
			
		case GX_LO_INVCOPY:
			LogicOp(LOGOP_COPY_INVERTED);
			break;
			
		case GX_LO_INVOR:
			LogicOp(LOGOP_OR_INVERTED);
			break;
			
		case GX_LO_NAND:
			LogicOp(LOGOP_NAND);
			break;
			
		case GX_LO_SET:
			LogicOp(LOGOP_SET);
			break;
			
		default:
			ASSERTMSG(false, "Invalid logic mode");
			
		}
	}
	else {
		ASSERTMSG(false, "Invalid operation");
	}
}


void GXSetZMode(GXBool compare_enable, GXCompare func, GXBool update_enable) {
	if (compare_enable == false) {
		Disable(DEPTH_TEST);
		DepthMask(false);
	}
	else {
		Enable(DEPTH_TEST);
		
		switch (func) {
			
		case GX_NEVER:
			DepthFunc(LOGFU_NEVER);
			break;
			
		case GX_LESS:
			DepthFunc(LOGFU_LESS);
			break;
			
		case GX_EQUAL:
			DepthFunc(LOGFU_EQUAL);
			break;
			
		case GX_LEQUAL:
			DepthFunc(LOGFU_LEQUAL);
			break;
			
		case GX_GREATER:
			DepthFunc(LOGFU_GREATER);
			break;
			
		case GX_NEQUAL:
			DepthFunc(LOGFU_NOTEQUAL);
			break;
			
		case GX_GEQUAL:
			DepthFunc(LOGFU_GEQUAL);
			break;
			
		case GX_ALWAYS:
			DepthFunc(LOGFU_ALWAYS);
			break;
			
		default:
			ASSERTMSG(false, "Invalid z-compare function");
			
		}
		
		DepthMask(update_enable);
	}
}


void GXSetDither(GXBool dither) {
	if (dither == false)
		Disable(DITHER);
	else
		Enable(DITHER);
}


void GXSetPixelFmt(GXPixelFmt pix_fmt, GXZFmt16 z_fmt) {
	CurrentPixFmt = pix_fmt;
	// TODO
}


