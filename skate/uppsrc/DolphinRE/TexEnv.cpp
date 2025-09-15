#include "DolphinRE.h"

uint		GXNumTevStages;
GXBool		GXTexCoordFlag[FLAG_COUNT];
GXBool		GXTexMapFlag[FLAG_COUNT];
GXBool		GXColorChanFlag[CLRCHAN_COUNT];
TevStatus	GXTevStatus[CLRCHAN_COUNT];



void GXSetNumTevStages(u8 nStages) {
	ASSERTMSG(nStages > 0, "Exceeded maximum number of stages");
	ASSERTMSG(nStages <= 2, "Unsupported number of stages");
	
	if (nStages != GXNumTevStages){
		GXNumTevStages = nStages;
		
		GXUpdateTevSetting();
		GXUpdateResourceFlag();
	}
}


void GXUpdateResourceFlag() {
	for(int i = 0; i < FLAG_COUNT; i++)
		GXTexCoordFlag[i] = false;
	
	for(int i = 0; i < FLAG_COUNT; i++)
		GXTexMapFlag[i] = false;
	
	for(int i = 0; i < CLRCHAN_COUNT; i++)
		GXColorChanFlag[i] = false;
	
	for(int i = 0; i < GXNumTevStages; i++) {
		
		if (GXTevStatus[i].coord != GX_TEXCOORD_NULL) {
			int j = TranslateGXTexCoord(GXTevStatus[i].coord);
			ASSERTMSG(j >= 0 && j < FLAG_COUNT, "Invalid tex coordinate");
			GXTexCoordFlag[j] = true;
		}
		
		if (GXTevStatus[i].map != GX_TEXMAP_NULL) {
			int j = TranslateGXTexMapID(GXTevStatus[i].map);
			ASSERTMSG(j >= 0 && j < FLAG_COUNT, "Invalid tex coordinate");
			GXTexMapFlag[j] = true;
		}
		
		GXChannelID color = GXTevStatus[i].color;
		
		if (color == GX_COLOR0A0) {
			GXColorChanFlag[0] = true;
		}
		else if (color == GX_COLOR1A1) {
			GXColorChanFlag[1] = true;
		}
		else if (color != GX_COLOR_NULL) {
			ASSERTMSG(false, "Invalid color ID");
		}
	}
}


void GXUpdateTevSetting() {
	Float clr[4];
	TexArb act_tex_enums[CLRCHAN_COUNT] = {TEXTURE0_ARB, TEXTURE1_ARB};
	
	for(int i = 0; i < CLRCHAN_COUNT; i++) {
		ActiveTexture(act_tex_enums[i]);
		
		if (i < GXNumTevStages) {
			GXTevMode mode = GXTevStatus[i].mode;
			
			switch (mode) {
				
			case GX_MODULATE:
				TexEnv(TEXTGT_TEXTURE_ENV, TEV_MODULATE);
				Enable(Gfx::TEXTURE_2D);
				break;
				
			case GX_DECAL:
				TexEnv(TEXTGT_TEXTURE_ENV, TEV_DECAL);
				Enable(Gfx::TEXTURE_2D);
				break;
				
			case GX_BLEND:
				clr[3] = 1.0;
				clr[2] = 1.0;
				clr[1] = 1.0;
				clr[0] = 1.0;
				TexEnv(TEXTGT_TEXTURE_ENV, TEV_BLEND);
				TexEnv(TEXTGT_TEXTURE_ENV, TEXPARAM_TEXTURE_ENV_COLOR, clr);
				Enable(TEXTURE_2D);
				break;
				
			case GX_REPLACE:
				TexEnv(TEXTGT_TEXTURE_ENV, TEV_REPLACE);
				Enable(TEXTURE_2D);
				break;
				
			case GX_PASSCLR:
				Disable(TEXTURE_2D);
				break;
				
			default:
				ASSERTMSG(false, "Invalid tev mode");
				
			}
		}
		else
			Disable(TEXTURE_2D);
		
		Flush();
	}
}



void GXSetTevOrder(GXTevStageID stage, GXTexCoordID coord, GXTexMapID map, GXChannelID color) {
	int trans_stage0;
	int trans_texcoord;
	int trans_texmap;
	int trans_chan;
	int local_10;
	int local_c;
	int trans_stage;
	
	trans_stage = TranslateGXTevStageID(stage);
	trans_texcoord = TranslateGXTexCoord(coord);
	trans_texmap = TranslateGXTexMapID(map);
	trans_chan = TranslateGXChannelID(color);
	
	ASSERTMSG(trans_stage >= -1 && trans_stage < 0x10, "Invalid tev stage");
	ASSERTMSG(trans_texcoord >= -1 && trans_texcoord < 8, "Invalid tex coord");
	ASSERTMSG(trans_texmap >= -1 && trans_texmap < 8, "Invalid tex map index");
	ASSERTMSG(trans_chan >= -1 && trans_chan <= 5, "Invalid color index");
	
	// TODO check again
	GXTevStatus[trans_stage].coord	= coord;
	GXTevStatus[trans_stage].map	= map;
	GXTevStatus[trans_stage].color	= color;
	
	GXUpdateResourceFlag();
}


void GXSetTevOp(GXTevStageID id, GXTevMode mode) {
	int trans_stage = TranslateGXTevStageID(id);
	
	ASSERTMSG(trans_stage >= 0, "Unsupported stage ID");
	
	GXTevStatus[trans_stage].mode = mode;
	GXUpdateTevSetting();
}


void GXSetAlphaCompare(GXCompare comp0, u8 ref0, GXAlphaOp op, GXCompare comp1, u8 ref1) {
	Clamp reff = (float)ref0 / 255.0;
	
	switch (comp0) {
		
	case GX_NEVER:
		AlphaFunc(LOGFU_NEVER, reff);
		break;
		
	case GX_LESS:
		AlphaFunc(LOGFU_LESS, reff);
		break;
		
	case GX_EQUAL:
		AlphaFunc(LOGFU_EQUAL, reff);
		break;
		
	case GX_LEQUAL:
		AlphaFunc(LOGFU_LEQUAL, reff);
		break;
		
	case GX_GREATER:
		AlphaFunc(LOGFU_GREATER, reff);
		break;
		
	case GX_NEQUAL:
		AlphaFunc(LOGFU_NOTEQUAL, reff);
		break;
		
	case GX_GEQUAL:
		AlphaFunc(LOGFU_GEQUAL, reff);
		break;
		
	case GX_ALWAYS:
		AlphaFunc(LOGFU_ALWAYS, reff);
		break;
		
	}
	
	Enable(ALPHA_TEST);
}


