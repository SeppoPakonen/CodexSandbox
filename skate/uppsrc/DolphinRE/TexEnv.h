#pragma once

typedef unsigned char   undefined;



#define FLAG_COUNT 8
#define CLRCHAN_COUNT 2

extern GXBool		GXTexCoordFlag[FLAG_COUNT];
extern GXBool		GXTexMapFlag[FLAG_COUNT];
extern GXBool		GXColorChanFlag[CLRCHAN_COUNT];
extern TevStatus	GXTevStatus[CLRCHAN_COUNT];
extern uint			GXNumTevStages;

void GXUpdateTevSetting();
void GXUpdateResourceFlag();
/*
void GXSetNumTevStages(u8 nStages);
void GXSetTevOrder(GXTevStageID stage,GXTexCoordID coord,GXTexMapID map,GXChannelID color);
void GXSetTevOp(GXTevStageID id,GXTevMode mode);
void GXSetAlphaCompare(GXCompare comp0,u8 ref0,GXAlphaOp op,GXCompare comp1,u8 ref1);
*/



