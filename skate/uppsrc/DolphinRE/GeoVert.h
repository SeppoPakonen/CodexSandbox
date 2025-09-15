#pragma once

extern bool		InBegin;
extern bool		InQuad;
extern u32		QuadCount;
extern int		NumTexCoordSrc;
extern u32		RemainingVerts;
extern Vec*		NrmIter;
extern GXColor*	ClrIter;
extern Vec*		TexIter;
extern int		ColorRef_Color0;
extern int		ColorRef_Color1;
extern uint		GXNumTexGenCount;


void GXCmd1u8();
void GXParam1u16();


GXAttrType IntToAttrType(int i);
GXCompType IntToCompType(int i);
GXCompCnt IntToCompCnt(int i);

void IncGfxTracker();
