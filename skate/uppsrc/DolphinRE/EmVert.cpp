#include "DolphinRE.h"

/*Vec CurrentNormals[VTXARR_SIZE];
Vec CurrentTexCoords[VTXARR_SIZE];

Vec CurrentPosition;
bool DisableCurrent;
uint TexMtxIndex[8];
uint PosMtxIndex;
Vec FinalTangent;
Vec FinalPosition;
Vec FinalBinormal;
GXColor FinalColor;
GXColor QuadColor;
Vec QuadPosition;
CompStruct* GfxTracker;
Vec FinalTexCoord[8];
*/

Vec CurrentTexCoords[VTXARR_SIZE];
Vec CurrentNormals[VTXARR_SIZE];
Vec CurrentTangents[8]; // added
Vec CurrentBinormals[8]; // added
Vec CurrentPosition;

bool DisableCurrent;
uint TexMtxIndex[8];
uint PosMtxIndex;

Vec FinalTangent;
Vec FinalPosition;
Vec FinalBinormal;
GXColor FinalColor;
Vec FinalTexCoord[8];

GXColor QuadColor;
Vec QuadTexCoord[8];
Vec QuadPosition;
CompStruct* GfxTracker;




void EmGXCmd1u8(u8 x) {
	OSReport("Dead end: EmGXCmd1u8");
}


void EmGXCmd1u16(u16 x) {
	OSReport("Dead end: EmGXCmd1u16");
}


void EmGXCmd1u32(u32 x) {
	OSReport("Dead end: EmGXCmd1u32");
}


void EmGXParam1u8(u8 x) {
	OSReport("Dead end: EmGXParam1u8");
}


void EmGXParam1u16(u16 x) {
	OSReport("Dead end: EmGXParam1u16");
}


void EmGXParam1u32(u32 x) {
	OSReport("Dead end: EmGXParam1u32");
}


void EmGXParam1s8(s8 x) {
	OSReport("Dead end: EmGXParam1s8");
}


void EmGXParam1s16(s16 x) {
	OSReport("Dead end: EmGXParam1s16");
}


void EmGXParam1s32(s32 x) {
	OSReport("Dead end: EmGXParam1s32");
}


void EmGXParam1f32(f32 x) {
	OSReport("Dead end: EmGXParam1f32");
}


void EmGXParam3f32(f32 x, f32 y, f32 z) {
	OSReport("Dead end: EmGXParam3f32");
}


void EmGXParam4f32(f32 x, f32 y, f32 z, f32 w) {
	OSReport("Dead end: EmGXParam4f32");
}


void EmGXPosition3u8(u8 x, u8 y, u8 z) {
	IncGfxTracker();
	GXPosition3f32_Internal(x, y, z);
	GXCheckApi32(3, 1, 1, 0);
}


void GXCheckApi32(int major, int i, int j, int k) {
	u32 version = major << 0x18 | i << 0x10 | j << 8 | k;
	u32 swap = EndianSwapReturn32(GfxTracker);
	if (swap != version)
		CheckVtx32(version);
}


void GXPosition3f32_Internal(float x, float y, float z) {
	float scale = GfxTracker->scale;
	DisableCurrent = false;
	CurrentPosition.x = x * scale;
	CurrentPosition.y = y * scale;
	CurrentPosition.z = z * scale;
}


void EmGXPosition3s8(s8 x, s8 y, s8 z) {
	IncGfxTracker();
	GXPosition3f32_Internal(x, y, z);
	GXCheckApi32(3, 1, 1, 1);
}


void EmGXPosition3u16(u16 x, u16 y, u16 z) {
	IncGfxTracker();
	GXPosition3f32_Internal(x, y, z);
	GXCheckApi32(3, 1, 1, 2);
}


void EmGXPosition3s16(s16 x, s16 y, s16 z) {
	IncGfxTracker();
	GXPosition3f32_Internal(x, y, z);
	GXCheckApi32(3, 1, 1, 3);
}


void EmGXPosition3f32(float x, float y, float z) {
	IncGfxTracker();
	GXPosition3f32_Internal(x, y, z);
	GXCheckApi32(3, 1, 1, 4);
}


void EmGXPosition2u8(u8 x, u8 y) {
	IncGfxTracker();
	GXPosition3f32_Internal(x, y, 0.0);
	GXCheckApi32(3, 1, 0, 0);
}


void EmGXPosition2s8(s8 x, s8 y) {
	IncGfxTracker();
	GXPosition3f32_Internal(x, y, 0.0);
	GXCheckApi32(3, 1, 0, 1);
}


void EmGXPosition2u16(u16 x, u16 y) {
	IncGfxTracker();
	GXPosition3f32_Internal(x, y, 0.0);
	GXCheckApi32(3, 1, 0, 2);
}


void EmGXPosition2s16(s16 x, s16 y) {
	IncGfxTracker();
	GXPosition3f32_Internal(x, y, 0.0);
	GXCheckApi32(3, 1, 0, 3);
}


void EmGXPosition2f32(float x, float y) {
	IncGfxTracker();
	GXPosition3f32_Internal(x, y, 0.0);
	GXCheckApi32(3, 1, 0, 4);
}


void EmGXPosition1x8(u8 idx) {
	IncGfxTracker();
	DisableCurrent = idx == 0xFF;
	GXPositionIndex_Internal(idx);
	GXCheckApi16(3, 2);
}


void GXCheckApi16(uint major, short i) {
	uint version = (ushort)((major & 0xff) << 8) | i;
	ushort swap = EndianSwapReturn16(GfxTracker);
	if (swap != (version & 0xffff))
		CheckVtx16((ushort)version);
}


void GXPositionIndex_Internal(int idx) {
	short idx16 = idx;
	ASSERT(idx16 >= 0);
	
	CompStruct* header = GfxTracker;
	int trans_arr = TranslateGXAttr_Array(header->GetAttr());
	ASSERT(trans_arr >= 0);
	VtxArray& va = GXVtxArray[trans_arr];
	void *vtx = (void*)(va.arr + (uint)va.stride * idx16);
	
	float x, y, z;
	
	switch (header->GetCompType()) {
	case GX_U8:
		VecGetF32FromU8(&x, &y, &z, (u8*)vtx);
		break;
	case GX_S8:
		VecGetF32FromS8(&x, &y, &z, (s8*)vtx);
		break;
	case GX_U16:
		VecGetF32FromU16(&x, &y, &z, (u16*)vtx);
		break;
	case GX_S16:
		VecGetF32FromS16(&x, &y, &z, (s16*)vtx);
		break;
	case GX_F32:
		VecGetF32FromF32(&x, &y, &z, (f32*)vtx);
		break;
	default:
		StopByMismatch(3);
	}
	if (header->GetCompCnt() == GX_POS_XY)
		z = 0.0;
	else if (header->GetCompCnt() != GX_POS_XYZ)
		StopByMismatch(3);
	
	CurrentPosition.x = x * header->scale;
	CurrentPosition.y = y * header->scale;
	CurrentPosition.z = z * header->scale;
}


void VecGetF32FromF32(float *x, float *y, float *z, f32 *vtx) {
	*x = vtx[0];
	*y = vtx[1];
	if (z)
		*z = vtx[2];
}


void VecGetF32FromS16(float *x, float *y, float *z, s16 *vtx) {
	*x = vtx[0];
	*y = vtx[1];
	if (z)
		*z = vtx[2];
}


void VecGetF32FromU16(float *x, float *y, float *z, u16 *vtx) {
	*x = vtx[0];
	*y = vtx[1];
	if (z)
		*z = vtx[2];
}


void VecGetF32FromS8(float *x, float *y, float *z, s8 *vtx) {
	*x = vtx[0];
	*y = vtx[1];
	if (z)
		*z = vtx[2];
}


void VecGetF32FromU8(float *x, float *y, float *z, u8 *vtx) {
	*x = vtx[0];
	*y = vtx[1];
	if (z)
		*z = vtx[2];
}


void EmGXPosition1x16(u16 idx) {
	IncGfxTracker();
	DisableCurrent = idx == 0xffff;
	GXPositionIndex_Internal(idx);
	GXCheckApi16(3, 3);
}


void EmGXColor4u8(u8 r, u8 g, u8 b, u8 a) {
	IncGfxTracker();
	ClrIter++;
	ClrIter->r = r;
	ClrIter->g = g;
	ClrIter->b = b;
	if (GfxTracker->GetCompType() == GX_RGBA8) {
		ClrIter->a = a;
		GXCheckApi32(5, 1, 1, 5);
	}
	else {
		ClrIter->a = 0xFF;
		GXCheckApi32(5, 1, 0, 2);
	}
}


void EmGXColor3u8(u8 r, u8 g, u8 b) {
	ClrIter++;
	
	CompStruct* header = GfxTracker;
	CompStruct* next = header + 1;
	GXCompType next_type = next->GetCompType();
	
	if (next_type == GX_F32) {
		uint rgb = (uint)r << 0x18 | (uint)g << 0x10 | (uint)b << 8;
		IncGfxTracker();
		u8 red, green, blue, alpha;
		GetRGBA6(&red, &green, &blue, &alpha, &rgb);
		ClrIter->r = red;
		ClrIter->g = green;
		ClrIter->b = blue;
		ClrIter->a = alpha;
		GXCheckApi32(5, 1, 1, 4);
	}
	else {
		IncGfxTracker();
		ClrIter->r = r;
		ClrIter->g = g;
		ClrIter->b = b;
		ClrIter->a = 0xFF;
		GXCheckApi32(5, 1, 0, 1);
	}
}


void EmGXColor1u32(u32 rgba) {
	u8 r, g, b, a;
	GetRGBA8(&r, &g, &b, &a, &rgba);
	GXColor4u8(r, g, b, a);
}



void EmGXColor1u16(u16 rgba) {
	u8 r, g, b, a;
	
	ClrIter++;
	
	CompStruct* header = GfxTracker;
	CompStruct* next = header + 1;
	GXCompType next_type = next->GetCompType();
	
	if (next_type == GX_RGB565) {
		IncGfxTracker();
		GetRGB565(&r, &g, &b, &rgba);
		ClrIter->r = r;
		ClrIter->g = g;
		ClrIter->b = b;
		ClrIter->a = 0xFF;
		GXCheckApi32(5, 1, 0, 0);
	}
	else {
		IncGfxTracker();
		GetRGBA4(&r, &g, &b, &a, (u8 *)&rgba);
		ClrIter->r = r;
		ClrIter->g = g;
		ClrIter->b = b;
		ClrIter->a = a;
		GXCheckApi32(5, 1, 1, 3);
	}
}


void EmGXColor1x8(u8 idx) {
	IncGfxTracker();
	ClrIter++;
	GXColorIndex_Internal(idx);
	GXCheckApi16(5, 2);
}


void GXColorIndex_Internal(int idx) {
	byte r, g, b, a;
	CompStruct* header = GfxTracker;
	int trans_arr = TranslateGXAttr_Array(header->GetAttr());
	VtxArray& va = GXVtxArray[trans_arr];
	void* in = (void*)(va.arr + va.stride * idx);
	
	if (header->GetCompCnt() == GX_CLR_RGB) {
		GXCompType fmt = header->GetCompType();
		if      (fmt == GX_RGB565)	GetRGB565(&r, &g, &b, (ushort *)in);
		else if (fmt == GX_RGB8)	GetRGB8(&r, &g, &b, (u8*)in);
		else if (fmt == GX_RGBX8)	GetRGBA8(&r, &g, &b, &a, (uint *)in);
		else						StopByMismatch(5);
		a = 0xff;
	}
	else if (header->GetCompCnt() == GX_CLR_RGBA) {
		GXCompType fmt = header->GetCompType();
		if      (fmt == GX_RGBA4)	GetRGBA4(&r, &g, &b, &a, (u8 *)in);
		else if (fmt == GX_RGBA6)	GetRGBA6(&r, &g, &b, &a, (uint *)in);
		else if (fmt == GX_RGBA8)	GetRGBA8(&r, &g, &b, &a, (uint *)in);
		else						StopByMismatch(5);
	}
	else {
		StopByMismatch(5);
	}
	
	ClrIter->r = r;
	ClrIter->g = g;
	ClrIter->b = b;
	ClrIter->a = a;
}


void EmGXColor1x16(u16 idx) {
	IncGfxTracker();
	ClrIter++;
	GXColorIndex_Internal(idx);
	GXCheckApi16(5, 3);
}


void GetRGB565(u8 *r, u8 *g, u8 *b, ushort *out) {
	uint swap = EndianSwapReturn16(out);
	
	*b = (swap       & 0x1fU) << 3;
	*g = (swap >>  5 & 0x3fU) << 2;
	*r = (swap >> 11        ) << 3;
	
	*b |= (byte)((uint)*b >> 5);
	*g |= (byte)((uint)*g >> 6);
	*r |= (byte)((uint)*r >> 5);
}


void GetRGBA4(byte *r, byte *g, byte *b, byte *a, u8 *src) {
	ushort rgb16 = EndianSwapReturn16(src);
	
	*a = (byte)((rgb16       & 0xfU) << 4) | (byte)(rgb16      ) & 0xf;
	*b = (byte)((rgb16 >>  4 & 0xfU) << 4) | (byte)(rgb16 >>  4) & 0xf;
	*g = (byte)((rgb16 >>  8 & 0xfU) << 4) | (byte)(rgb16 >>  8) & 0xf;
	*r = (byte)((rgb16 >> 12       ) << 4) | (byte)(rgb16 >> 12);
}


void GetRGBA8(u8 *r, u8 *g, u8 *b, u8 *a, uint *rgb) {
	union {
		uint v;
		byte z[4];
	};
	v = *rgb;
	
	*r = z[0];
	*g = z[1];
	*b = z[2];
	*a = z[3];
}


void GetRGB8(u8 *r, u8 *g, u8 *b, u8 *rgb) {
	*r = rgb[0];
	*g = rgb[1];
	*b = rgb[2];
}


void GetRGBA6(u8 *r, u8 *g, u8 *b, u8 *a, uint *rgb) {
	*r = *(byte*)rgb & 0xfc;
	*r |= *r >> 6;
	
	ushort swap1 = EndianSwapReturn16(rgb);
	*g = (swap1 & 0x03F0) >> 2;
	*g |= *g >> 6;
	
	ushort swap2 = EndianSwapReturn16((byte*)rgb + 1);
	*b = (swap2 & 0x0FC0) >> 4;
	*b |= *b >> 6;
	
	*a = (*((byte*)rgb + 2) & 0x3f) << 2;
	*a |= *a >> 6;
}


void EmGXNormal3s8(s8 x, s8 y, s8 z) {
	IncGfxTracker();
	NrmIter++;
	GXNormal3f32_Internal(x, y, z, 1.0 / 64.0);
	GXCheckApi32(4, 1, 0, 1);
}


void GXNormal3f32_Internal(float x, float y, float z, float n) {
	NrmIter->x = x * n;
	NrmIter->y = y * n;
	NrmIter->z = z * n;
}


void EmGXNormal3s16(s16 x, s16 y, s16 z) {
	IncGfxTracker();
	NrmIter++;
	GXNormal3f32_Internal(x, y, z, 0.00006104);
	GXCheckApi32(4, 1, 0, 3);
}


void EmGXNormal3f32(f32 x, f32 y, f32 z) {
	IncGfxTracker();
	NrmIter++;
	GXNormal3f32_Internal(x, y, z, 1.0);
	GXCheckApi32(4, 1, 0, 4);
}


void EmGXNormal1x8(u8 idx) {
	IncGfxTracker();
	NrmIter++;
	GXNormalIndex_Internal(idx);
	GXCheckApi16(4, 2);
}


void GXNormalIndex_Internal(uint idx) {
	float x0, y0, z0;
	float x1, y1, z1;
	float x2, y2, z2;
	CompStruct* header = GfxTracker;
	
	int trans_arr = TranslateGXAttr_Array(header->GetAttr());
	byte* in = GXVtxArray[trans_arr].arr + GXVtxArray[trans_arr].stride * idx;
	if (header->GetCompCnt() == GX_NRM_XYZ) {
		GXCompType fmt = header->GetCompType();
		double mul;
		
		if (fmt == GX_S8) {
			VecGetF32FromS8(&x0, &y0, &z0, (s8*)in);
			mul = 0.01562500;
		}
		else if (fmt == GX_S16) {
			VecGetF32FromS16(&x0, &y0, &z0, (s16*)in);
			mul = 0.00006104;
		}
		else if (fmt == GX_F32) {
			VecGetF32FromF32(&x0, &y0, &z0, (f32*)in);
			mul = 1.0;
		}
		else {
			StopByMismatch(4);
		}
		
		NrmIter->x = x0 * mul;
		NrmIter->y = y0 * mul;
		NrmIter->z = z0 * mul;
	}
	else if (header->GetCompCnt() == GX_NRM_NBT) {
		GXCompType fmt = header->GetCompType();
		double mul;
		
		if (fmt == GX_S8) {
			VecGetF32FromS8(&x0, &y0, &z0, (s8*)in);
			VecGetF32FromS8(&x1, &y1, &z1, (s8*)(in + 3));
			VecGetF32FromS8(&x2, &y2, &z2, (s8*)(in + 6));
			mul = 0.01562500;
		}
		else if (fmt == GX_S16) {
			VecGetF32FromS16(&x0, &y0, &z0, (s16*)in);
			VecGetF32FromS16(&x1, &y1, &z1, (s16*)(in + 3*2));
			VecGetF32FromS16(&x2, &y2, &z2, (s16*)(in + 6*2));
			mul = 0.00006104;
		}
		else if (fmt == GX_F32) {
			VecGetF32FromF32(&x0, &y0, &z0, (f32*)in);
			VecGetF32FromF32(&x1, &y1, &z1, (f32*)(in + 3*4));
			VecGetF32FromF32(&x2, &y2, &z2, (f32*)(in + 6*4));
			mul = 1.0;
		}
		else {
			StopByMismatch(4);
		}
		
		NrmIter->x = x0 * mul;
		NrmIter->y = y0 * mul;
		NrmIter->z = z0 * mul;
		
		NrmIter++;
		NrmIter->x = x1 * mul;
		NrmIter->y = y1 * mul;
		NrmIter->z = z1 * mul;
		
		NrmIter++;
		NrmIter->x = x2 * mul;
		NrmIter->y = y2 * mul;
		NrmIter->z = z2 * mul;
	}
	else
		StopByMismatch(4);
}


void EmGXNormal1x16(u16 idx) {
	IncGfxTracker();
	NrmIter++;
	GXNormalIndex_Internal(idx);
	GXCheckApi16(4, 3);
}


void EmGXTexCoord2u8(u8 s, u8 t) {
	IncGfxTracker();
	TexIter++;
	GXTexCoord2f32_Internal(s, t);
	GXCheckApi32(6, 1, 1, 0);
}


void GXTexCoord2f32_Internal(float s, float t) {
	CompStruct* header = GfxTracker;
	
	float scale = header->scale;
	TexIter->x = s * scale;
	TexIter->y = t * scale;
	TexIter->z = 1.0; // added
}


void EmGXTexCoord2s8(s8 s, s8 t) {
	IncGfxTracker();
	TexIter++;
	GXTexCoord2f32_Internal(s, t);
	GXCheckApi32(6, 1, 1, 1);
}


void EmGXTexCoord2u16(u16 s, u16 t) {
	IncGfxTracker();
	TexIter++;
	GXTexCoord2f32_Internal(s, t);
	GXCheckApi32(6, 1, 1, 2);
}


void EmGXTexCoord2s16(s16 s, s16 t) {
	IncGfxTracker();
	TexIter++;
	GXTexCoord2f32_Internal(s, t);
	GXCheckApi32(6, 1, 1, 3);
}


void EmGXTexCoord2f32(f32 s, f32 t) {
	IncGfxTracker();
	TexIter++;
	GXTexCoord2f32_Internal(s, t);
	GXCheckApi32(6, 1, 1, 4);
}


void EmGXTexCoord1u8(u8 s) {
	IncGfxTracker();
	TexIter++;
	GXTexCoord2f32_Internal(s, 0.0);
	GXCheckApi32(6, 1, 0, 0);
}


void EmGXTexCoord1s8(s8 s) {
	IncGfxTracker();
	TexIter++;
	GXTexCoord2f32_Internal(s, 0.0);
	GXCheckApi32(6, 1, 0, 1);
}


void EmGXTexCoord1u16(u16 s) {
	IncGfxTracker();
	TexIter++;
	GXTexCoord2f32_Internal(s, 0.0);
	GXCheckApi32(6, 1, 0, 2);
}


void EmGXTexCoord1s16(s16 s) {
	IncGfxTracker();
	TexIter++;
	GXTexCoord2f32_Internal(s, 0.0);
	GXCheckApi32(6, 1, 0, 3);
}


void EmGXTexCoord1f32(f32 s) {
	IncGfxTracker();
	TexIter++;
	GXTexCoord2f32_Internal(s, 0.0);
	GXCheckApi32(6, 1, 0, 4);
}


void EmGXTexCoord1x8(u8 idx) {
	IncGfxTracker();
	TexIter++;
	GXTexCoordIndex_Internal(idx);
	GXCheckApi16(6, 2);
}


void GXTexCoordIndex_Internal(int idx) {
	float s, t;
	float scale;
	
	CompStruct* header = GfxTracker;
	int trans_arr = TranslateGXAttr_Array(GfxTracker->GetAttr());
	byte* in = GXVtxArray[trans_arr].arr + GXVtxArray[trans_arr].stride * idx;
	
	GXCompType fmt = header->GetCompType();
	switch (fmt) {
		
	case GX_U8:
		VecGetF32FromU8(&s, &t, (float *)0x0, in);
		break;
		
	case GX_S8:
		VecGetF32FromS8(&s, &t, (float *)0x0, (s8*)in);
		break;
		
	case GX_U16:
		VecGetF32FromU16(&s, &t, (float *)0x0, (u16*)in);
		break;
		
	case GX_S16:
		VecGetF32FromS16(&s, &t, (float *)0x0, (s16*)in);
		break;
		
	case GX_F32:
		VecGetF32FromF32(&s, &t, (float *)0x0, (f32*)in);
		break;
		
	default:
		StopByMismatch(6);
		
	}
	if (header->GetCompCnt() == GX_TEX_S) {
		t = 0.0;
	}
	else if (header->GetCompCnt() != GX_TEX_ST) {
		StopByMismatch(6);
	}
	
	TexIter->x = s * header->scale;
	TexIter->y = t * header->scale;
	TexIter->z = 1.0; // added
}


void EmGXTexCoord1x16(u16 idx) {
	IncGfxTracker();
	TexIter++;
	GXTexCoordIndex_Internal((size_t)TexIter);
	GXCheckApi16(6, 3);
}


void EmGXMatrixIndex1u8(u32 idx) {
	CompStruct* header = GfxTracker;
	CompStruct* it1 = header + 1;
	
	if (it1->mode == 2) {
		GXAttr attr = it1->GetAttr();
		IncGfxTracker();
		int trans_idx = TranslateGXAttr_TexMtxIndex(attr);
		
		ASSERTMSG(idx <= 0x3e, "GXMatrixIndex1u8: invalid matrix index");
		
		TexMtxIndex[trans_idx] = idx;
		GXCheckApi16(2, 1);
	}
	else {
		IncGfxTracker();
		ASSERTMSG(idx <= 0x1d, "GXMatrixIndex1u8: invalid matrix index");
		PosMtxIndex = idx;
		GXCheckApi16(1, 1);
	}
}


void CheckVtx16(ushort bitfield) {
	ushort swap = EndianSwapReturn16(GfxTracker);
	
	if (swap == (bitfield | 0x8000))
		DumpVertex();
	else
		StopByMismatch(bitfield >> 8);
}


void CheckVtx32(uint bitfield) {
	u32 swap = EndianSwapReturn32(GfxTracker);
	
	if (swap == (bitfield | 0x80000000))
		DumpVertex();
	else
		StopByMismatch(bitfield >> 0x18);
}


void DumpVertex() {
	if (DisableCurrent == false) {
		TransformVertex();
		
		if (NrmIter != CurrentNormals-1 && GXActiveLights != GX_LIGHT_NULL)
			TransformNormal();
		
		GXPerformLighting();
		GXDoTexGen();
		SelectColorChannel();
		
		/*FinalColor.r = 255;
		FinalColor.g = 255;
		FinalColor.a = 255;*/
		
		if (InQuad == false) {
			WriteGLVertex(&FinalPosition, &FinalColor, &FinalTexCoord[0]);
		}
		else {
			if (QuadCount == 0) {
				QuadPosition = FinalPosition;
				QuadColor = FinalColor;
				for(int i = 0; i < 8; i++)
					QuadTexCoord[i] = FinalTexCoord[i];
			}
			else
				WriteGLVertex(&FinalPosition, &FinalColor, &FinalTexCoord[0]);
			
			if (QuadCount == 3)
				WriteGLVertex(&QuadPosition, &QuadColor, QuadTexCoord);
			
			QuadCount++;
			if (QuadCount < 0)
				QuadCount = 0;
		}
	}
	RemainingVerts--;
	GfxTracker = GfxTrack - 1;
	NrmIter = CurrentNormals - 1;
	ClrIter = CurrentColors - 1;
	TexIter = CurrentTexCoords - 1;
}


void WriteGLVertex(Vec *vec, GXColor *clr, Vec vtx[8]) {
	Gfx::TexArb tex;
	for(int stage = 0; stage < GXNumTevStages; stage++) {
		GXTexCoordID coord = GXTevStatus[stage].coord;
		if (coord != GX_TEXCOORD_NULL) {
			switch (stage) {
				case 0:		tex = Gfx::TEXTURE0_ARB; break;
				case 1:		tex = Gfx::TEXTURE1_ARB; break;
				case 2:		tex = Gfx::TEXTURE2_ARB; break;
				case 3:		tex = Gfx::TEXTURE3_ARB; break;
				case 4:		tex = Gfx::TEXTURE4_ARB; break;
				case 5:		tex = Gfx::TEXTURE5_ARB; break;
				case 6:		tex = Gfx::TEXTURE6_ARB; break;
				case 7:		tex = Gfx::TEXTURE7_ARB; break;
				case 8:		tex = Gfx::TEXTURE8_ARB; break;
				case 9:		tex = Gfx::TEXTURE9_ARB; break;
				case 10:	tex = Gfx::TEXTURE10_ARB; break;
				case 11:	tex = Gfx::TEXTURE11_ARB; break;
				case 12:	tex = Gfx::TEXTURE12_ARB; break;
				case 13:	tex = Gfx::TEXTURE13_ARB; break;
				case 14:	tex = Gfx::TEXTURE14_ARB; break;
				case 15:	tex = Gfx::TEXTURE15_ARB; break;
				default:	PANIC("WriteGLVertex: unknown tev stage No.");
			}
			int trans_tex = TranslateGXTexCoord(coord);
			MultiTexCoord(
				tex,
				vtx[trans_tex].x,
				vtx[trans_tex].y,
				0.0,
				vtx[trans_tex].z);
		}
	}
	
	Static::Color(clr->r, clr->g, clr->b, clr->a);
	Static::Vertex(vec->x, vec->y, vec->z);
}


void TransformVertex() {
	int trans_vcd = TranslateGXAttr_VCD(GX_VA_PNMTXIDX);
	
	u32 pos;
	if (VCD[trans_vcd] == 0)
		pos = DefaultPosMatrix;
	else
		pos = PosMtxIndex;
	
	MtxPtr m = GXPosTexMatrix[pos];
	MTXMultVec(m, &CurrentPosition, &FinalPosition);
}


void TransformNormal() {
	int trans_vcd = TranslateGXAttr_VCD(GX_VA_PNMTXIDX);
	
	u32 pos;
	if (VCD[trans_vcd] == 0)
		pos = DefaultPosMatrix;
	else
		pos = PosMtxIndex;
	
	ASSERT(pos < MAT_LIMIT);
	MtxPtr m = GXNrmMatrix[pos];
	MTXMultVecSR(m, &CurrentNormals[0], &FinalNormal);
	GXVECNormalize(&FinalNormal, &FinalNormal);
	
	if (NrmIter == &FinalBinormal) {
		PANIC("Not really solved");
		MTXMultVecSR(m, &CurrentTangents[0], &FinalTangent);
		MTXMultVecSR(m, &FinalBinormal, &FinalBinormal);
	}
}


void SelectColorChannel() {
	// TODO check
	//if (GXNumTevStages == 4)
	if (GXNumTevStages == 1)
		FinalColor = FinalColors[0];
	
	//else if (GXNumTevStages == 5)
	else if (GXNumTevStages == 2)
		FinalColor = FinalColors[1];
	
	else
		memset(&FinalColor, 0, sizeof(FinalColor));
}


void GXDoTexGen() {
	GXTexGenType func;
	for(int i = 0; i < GXNumTexGenCount; i++) {
		switch (GXTexCoordGen[i].func) {
			case GX_TG_MTX3x4:	TexGenMtx3x4(i); break;
			case GX_TG_MTX2x4:	TexGenMtx2x4(i); break;
			case GX_TG_BUMP0:	TexGenBump(i, 0); break;
			case GX_TG_BUMP1:	TexGenBump(i, 1); break;
			case GX_TG_BUMP2:	TexGenBump(i, 2); break;
			case GX_TG_BUMP3:	TexGenBump(i, 3); break;
			case GX_TG_BUMP4:	TexGenBump(i, 4); break;
			case GX_TG_BUMP5:	TexGenBump(i, 5); break;
			case GX_TG_BUMP6:	TexGenBump(i, 6); break;
			case GX_TG_BUMP7:	TexGenBump(i, 7); break;
			case GX_TG_SRTG:	TexGenSRTG(i); break;
			default: PANIC("GXDoTexGen: unknown texgen type");
		}
	}
}


void TexGenMtx2x4(int i) {
	ASSERT(i < 4);
	GXAttr attr = UntranslateGXAttr_TexMtxIndex(i);
	int trans_vcd = TranslateGXAttr_VCD(attr);
	
	uint mtx_idx;
	if (VCD[trans_vcd] == 0)
		mtx_idx = GXTexCoordGen[i].mtx;
	else
		mtx_idx = TexMtxIndex[i];
	MtxPtr m = GXPosTexMatrix[mtx_idx];
	
	GXTexGenSrc src = GXNumTexGens[i].src;
	float dst_vec, src_vec;
	int trans_coord;
	
	switch (src) {
		
	case GX_TG_POS:
		MTXMultVec2x4(m, CurrentPosition.x, CurrentPosition.y, CurrentPosition.z, &src_vec, &dst_vec);
		break;
		
	case GX_TG_NRM:
		MTXMultVec2x4(m, CurrentNormals[0].x, CurrentNormals[0].y, CurrentNormals[0].z, &src_vec, &dst_vec);
		break;
		
	case GX_TG_TEX0:
	case GX_TG_TEX1:
	case GX_TG_TEX2:
	case GX_TG_TEX3:
	case GX_TG_TEX4:
	case GX_TG_TEX5:
	case GX_TG_TEX6:
	case GX_TG_TEX7:
		trans_coord = TranslateGXTexGenSrc_TexCoord(GXNumTexGens[i].src);
		ASSERTMSG(trans_coord < NumTexCoordSrc, "TexGenMtx2x4: referred to empty TexCoord");
		MTXMultVec2x4(m, CurrentTexCoords[0].x, CurrentTexCoords[0].y, 1.0, &src_vec, &dst_vec);
		break;
		
	default:
		PANIC("GXSetTexCoordGen(): Invalid source parameter");
		break;
		
	}
	
	FinalTexCoord[i].x = src_vec;
	FinalTexCoord[i].y = dst_vec;
	FinalTexCoord[i].z = 1.0;
}



void TexGenMtx3x4(int i) {
	Vec dst;
	int trans_coord;
	GXAttr attr = UntranslateGXAttr_TexMtxIndex(i);
	int vcd_idx = TranslateGXAttr_VCD(attr);
	
	uint mtx_idx;
	if (VCD[vcd_idx] == 0)
		mtx_idx = GXTexCoordGen[i].mtx;
	else
		mtx_idx = TexMtxIndex[i];
	MtxPtr m = GXPosTexMatrix[mtx_idx];
	
	GXTexGenSrc src = GXNumTexGens[i].src;
	
	switch (src) {
	case GX_TG_POS:	MTXMultVec(m, &CurrentPosition, &dst); break;
	case GX_TG_NRM:	MTXMultVec(m, &CurrentNormals[0], &dst); break;
	case GX_TG_TEX0:
	case GX_TG_TEX1:
	case GX_TG_TEX2:
	case GX_TG_TEX3:
	case GX_TG_TEX4:
	case GX_TG_TEX5:
	case GX_TG_TEX6:
	case GX_TG_TEX7:
		trans_coord = TranslateGXTexGenSrc_TexCoord(GXNumTexGens[i].src);
		ASSERTMSG(trans_coord < NumTexCoordSrc, "TexGenMtx3x4: referred to empty TexCoord");
		
		dst.x = CurrentTexCoords[trans_coord].x;
		dst.y = CurrentTexCoords[trans_coord].y;
		dst.z = 1.0;
		MTXMultVec(m, &dst, &dst);
		break;
		
	default:
		PANIC("GXSetTexCoordGen(): Invalid source parameter");
		break;
	}
	
	FinalTexCoord[i] = dst;
}


void TexGenBump(int i, int j) {
	ASSERTMSG(NrmIter == &FinalBinormal, "TexGenBump: no binormal/tangent component loaded");
	ASSERTMSG(GXActiveLights & (1 << j), "TexGenBump: no active light assigned for bumpmap");
	
	// Unknown 0000be78
	float dot0 = VECDotProduct(&CurrentTangents[j], &FinalTangent);
	float dot1 = VECDotProduct(&CurrentBinormals[j], &FinalBinormal);
	
	int trans_bump = TranslateGXTexGenSrc_BumpTexCoord(GXNumTexGens[i].src);
	
	ASSERTMSG(i > trans_bump, "TexGenBump: base coord should be transformed before bump map coord");
	
	FinalTexCoord[i].x = dot0 + FinalTexCoord[trans_bump].x;
	FinalTexCoord[i].y = dot1 + FinalTexCoord[trans_bump].y;
	FinalTexCoord[i].z = 1.0;
}


void TexGenSRTG(int i) {
	GXTexGenSrc src = GXNumTexGens[i].src;
	
	if (src == GX_TG_COLOR0) {
		FinalTexCoord[i].x = FinalColors[0].r / 255.0;
		FinalTexCoord[i].y = FinalColors[0].g / 255.0;
	}
	else if (src == GX_TG_COLOR1) {
		FinalTexCoord[i].x = FinalColors[1].r / 255.0;
		FinalTexCoord[i].y = FinalColors[1].g / 255.0;
	}
	else {
		PANIC("TexGenSRTG: Incorrect TexGen source");
	}
	
	FinalTexCoord[i].z = 1.0;
}


void MTXMultVec2x4(MtxPtr m, float srcbase_x, float srcbase_y, float srcbase_z, float *src, float *dst) {
	*src = srcbase_z * m[0][2] + srcbase_y * m[0][1] + srcbase_x * m[0][0] + m[0][3];
	*dst = srcbase_z * m[1][2] + srcbase_y * m[1][1] + srcbase_x * m[1][0] + m[1][3];
}


void GXVECNormalize(Vec *a, Vec *b) {
	float dist_pow2 =
		a->z * a->z +
		a->y * a->y +
		a->x * a->x;
	
	if (dist_pow2 == 0.0) {
		b->z = 0.0;
		b->y = 0.0;
		b->x = 0.0;
	}
	else {
		double len = sqrt(dist_pow2);
		float unit_mul = 1.0 / len;
		b->x = unit_mul * a->x;
		b->y = unit_mul * a->y;
		b->z = unit_mul * a->z;
	}
}


void StopByMismatch(int i) {
	const char* api_str  = TranslateGXApiID_Name((ApiID)i);
	
	CompStruct* compcnt = GfxTracker;
	
	if (!compcnt)
		OSReport("GX%s[n][t]: vertex command must be in the scope between GXBegin and GXEnd", api_str);
	
	else {
		const char* attr_str		= TranslateGXAttr_Name(compcnt->GetAttr());
		const char* attrtype_str	= TranslateGXAttrType_Name(compcnt->GetAttrType());
		const char* compcnt_str		= TranslateGXCompCnt_Name(compcnt->GetAttr(), compcnt->GetCompCnt());
		const char* comptype_str	= TranslateGXCompType_Name(compcnt->GetChannelID(), compcnt->GetCompType());
		
		OSReport("GX%s[n][t]: type mismatch\n    Vtx Desc = ( %s %s %s %s )",
			attr_str,
			attrtype_str,
			compcnt_str,
			comptype_str);
	}
	
	PANIC("Mismatch");
}


