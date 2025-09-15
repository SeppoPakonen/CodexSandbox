#include "DolphinRE.h"


void GXCmd1u8(u8 x) {
	if (EmDisplayListInProgress == false) {
		EmGXCmd1u8(x);
	}
	else {
		*EmBuffPtr++ = x;
	}
}


void GXCmd1u16(u16 x) {
	if (EmDisplayListInProgress == false) {
		EmGXCmd1u16(x);
	}
	else {
		*(u16 *)EmBuffPtr = x;
		EmBuffPtr += 2;
	}
}


void GXCmd1u32(u32 x) {
	if (EmDisplayListInProgress == false) {
		EmGXCmd1u32(x);
	}
	else {
		*(u32 *)EmBuffPtr = x;
		EmBuffPtr += 4;
	}
}


void GXParam1u8(u8 x) {
	if (EmDisplayListInProgress == false) {
		EmGXParam1u8((uint)x);
	}
	else {
		*EmBuffPtr = x;
		EmBuffPtr++;
	}
}


void GXParam1u16(u16 x) {
	if (EmDisplayListInProgress == false) {
		EmGXParam1u16((uint)x);
	}
	else {
		*(u16 *)EmBuffPtr = x;
		EmBuffPtr += 2;
	}
}


void GXParam1u32(u32 x) {
	if (EmDisplayListInProgress == false) {
		EmGXParam1u32(x);
	}
	else {
		*(u32 *)EmBuffPtr = x;
		EmBuffPtr += 4;
	}
}


void GXParam1s8(s8 x) {
	if (EmDisplayListInProgress == false) {
		EmGXParam1s8(x);
	}
	else {
		*EmBuffPtr = x;
		EmBuffPtr++;
	}
}


void GXParam1s16(s16 x) {
	if (EmDisplayListInProgress == false) {
		EmGXParam1s16(x);
	}
	else {
		*(s16 *)EmBuffPtr = x;
		EmBuffPtr += 2;
	}
}


void GXParam1s32(s32 x) {
	if (EmDisplayListInProgress == false) {
		EmGXParam1s32(x);
	}
	else {
		*(s32 *)EmBuffPtr = x;
		EmBuffPtr += 4;
	}
}


void GXParam1f32(f32 x) {
	if (EmDisplayListInProgress == false) {
		EmGXParam1f32(x);
	}
	else {
		*(f32 *)EmBuffPtr = x;
		EmBuffPtr += 4;
	}
}


void GXParam3f32(f32 x, f32 y, f32 z) {
	if (EmDisplayListInProgress == false) {
		EmGXParam3f32(x, y, z);
	}
	else {
		*(f32 *)EmBuffPtr = x;
		EmBuffPtr += 4;
		*(f32 *)EmBuffPtr = y;
		EmBuffPtr += 4;
		*(f32 *)EmBuffPtr = z;
		EmBuffPtr += 4;
	}
}


void GXParam4f32(f32 x, f32 y, f32 z, f32 w) {
	if (EmDisplayListInProgress == false) {
		EmGXParam4f32(x, y, z, w);
	}
	else {
		*(f32 *)EmBuffPtr = x;
		EmBuffPtr += 4;
		*(f32 *)EmBuffPtr = y;
		EmBuffPtr += 4;
		*(f32 *)EmBuffPtr = z;
		EmBuffPtr += 4;
		*(f32 *)EmBuffPtr = w;
		EmBuffPtr += 4;
	}
}


void GXPosition3f32(float x, float y, float z) {
	if (EmDisplayListInProgress == false) {
		EmGXPosition3f32(x, y, z);
	}
	else {
		*(float *)EmBuffPtr = x;
		EmBuffPtr += 4;
		*(float *)EmBuffPtr = y;
		EmBuffPtr += 4;
		*(float *)EmBuffPtr = z;
		EmBuffPtr += 4;
	}
}


void GXPosition3u8(u8 x, u8 y, u8 z) {
	if (EmDisplayListInProgress == false) {
		EmGXPosition3u8(x, y, z);
	}
	else {
		*EmBuffPtr = x;
		EmBuffPtr++;
		*EmBuffPtr = y;
		EmBuffPtr++;
		*EmBuffPtr = z;
		EmBuffPtr++;
	}
}


void GXPosition3s8(s8 x, s8 y, s8 z) {
	if (EmDisplayListInProgress == false) {
		EmGXPosition3s8(x, y, z);
	}
	else {
		*EmBuffPtr = x;
		EmBuffPtr++;
		*EmBuffPtr = y;
		EmBuffPtr++;
		*EmBuffPtr = z;
		EmBuffPtr++;
	}
}


void GXPosition3u16(u16 x, u16 y, u16 z) {
	if (EmDisplayListInProgress == false) {
		EmGXPosition3u16(x, y, z);
	}
	else {
		*(u16 *)EmBuffPtr = x;
		EmBuffPtr += 2;
		*(u16 *)EmBuffPtr = y;
		EmBuffPtr += 2;
		*(u16 *)EmBuffPtr = z;
		EmBuffPtr += 2;
	}
}


void GXPosition3s16(s16 x, s16 y, s16 z) {
	if (EmDisplayListInProgress == false) {
		EmGXPosition3s16(x, y, z);
	}
	else {
		*(s16 *)EmBuffPtr = x;
		EmBuffPtr += 2;
		*(s16 *)EmBuffPtr = y;
		EmBuffPtr += 2;
		*(s16 *)EmBuffPtr = z;
		EmBuffPtr += 2;
	}
}


void GXPosition2f32(float x, float y) {
	if (EmDisplayListInProgress == false) {
		EmGXPosition2f32(x, y);
	}
	else {
		*(float *)EmBuffPtr = x;
		EmBuffPtr += 4;
		*(float *)EmBuffPtr = y;
		EmBuffPtr += 4;
	}
}


void GXPosition2u8(u8 x, u8 y) {
	if (EmDisplayListInProgress == false) {
		EmGXPosition2u8(x, y);
	}
	else {
		*EmBuffPtr = x;
		EmBuffPtr++;
		*EmBuffPtr = y;
		EmBuffPtr++;
	}
}


void GXPosition2s8(s8 x, s8 y) {
	if (EmDisplayListInProgress == false) {
		EmGXPosition2s8(x, y);
	}
	else {
		*EmBuffPtr = x;
		EmBuffPtr++;
		*EmBuffPtr = y;
		EmBuffPtr++;
	}
}


void GXPosition2u16(u16 x, u16 y) {
	if (EmDisplayListInProgress == false) {
		EmGXPosition2u16(x, y);
	}
	else {
		*(u16 *)EmBuffPtr = x;
		EmBuffPtr += 2;
		*(u16 *)EmBuffPtr = y;
		EmBuffPtr += 2;
	}
}


void GXPosition2s16(s16 x, s16 y) {
	if (EmDisplayListInProgress == false) {
		EmGXPosition2s16(x, y);
	}
	else {
		*(s16 *)EmBuffPtr = x;
		EmBuffPtr += 2;
		*(s16 *)EmBuffPtr = y;
		EmBuffPtr += 2;
	}
}


void GXPosition1x16(u16 idx) {
	if (EmDisplayListInProgress == false) {
		EmGXPosition1x16(idx);
	}
	else {
		*(u16 *)EmBuffPtr = idx;
		EmBuffPtr += 2;
	}
}


void GXPosition1x8(u8 idx) {
	if (EmDisplayListInProgress == false) {
		EmGXPosition1x8(idx);
	}
	else {
		*EmBuffPtr = idx;
		EmBuffPtr++;
	}
}


void GXNormal3f32(float x, float y, float z) {
	if (EmDisplayListInProgress == false) {
		EmGXNormal3f32(x, y, z);
	}
	else {
		*(float *)EmBuffPtr = x;
		EmBuffPtr += 4;
		*(float *)EmBuffPtr = y;
		EmBuffPtr += 4;
		*(float *)EmBuffPtr = z;
		EmBuffPtr += 4;
	}
}


void GXNormal3s16(s16 x, s16 y, s16 z) {
	if (EmDisplayListInProgress == false) {
		EmGXNormal3s16(y, z, z);
	}
	else {
		*(s16 *)EmBuffPtr = x;
		EmBuffPtr += 2;
		*(s16 *)EmBuffPtr = y;
		EmBuffPtr += 2;
		*(s16 *)EmBuffPtr = z;
		EmBuffPtr += 2;
	}
}


void GXNormal3s8(s8 x, s8 y, s8 z) {
	if (EmDisplayListInProgress == false) {
		EmGXNormal3s8(y, z, z);
	}
	else {
		*EmBuffPtr = x;
		EmBuffPtr++;
		*EmBuffPtr = y;
		EmBuffPtr++;
		*EmBuffPtr = z;
		EmBuffPtr++;
	}
}


void GXNormal1x16(u16 idx) {
	if (EmDisplayListInProgress == false) {
		EmGXNormal1x16(idx);
	}
	else {
		*(u16 *)EmBuffPtr = idx;
		EmBuffPtr += 2;
	}
}


void GXNormal1x8(u8 idx) {
	if (EmDisplayListInProgress == false) {
		EmGXNormal1x8(idx);
	}
	else {
		*EmBuffPtr = idx;
		EmBuffPtr++;
	}
}


void GXColor4u8(u8 r, u8 g, u8 b, u8 a) {
	if (EmDisplayListInProgress == false) {
		EmGXColor4u8(r, g, b, a);
	}
	else {
		*EmBuffPtr = r;
		EmBuffPtr++;
		*EmBuffPtr = g;
		EmBuffPtr++;
		*EmBuffPtr = b;
		EmBuffPtr++;
		*EmBuffPtr = a;
		EmBuffPtr++;
	}
}


void GXColor1u32(u32 rgba) {
	if (EmDisplayListInProgress == false) {
		EmGXColor1u32(rgba);
	}
	else {
		*(u32 *)EmBuffPtr = rgba;
		EmBuffPtr += 4;
	}
}


void GXColor3u8(u8 r, u8 g, u8 b) {
	if (EmDisplayListInProgress == false) {
		EmGXColor3u8(r, g, b);
	}
	else {
		*EmBuffPtr = r;
		EmBuffPtr++;
		*EmBuffPtr = g;
		EmBuffPtr++;
		*EmBuffPtr = b;
		EmBuffPtr++;
	}
}


void GXColor1u16(u16 rgba) {
	if (EmDisplayListInProgress == false) {
		EmGXColor1u16(rgba);
	}
	else {
		*(u16 *)EmBuffPtr = rgba;
		EmBuffPtr += 2;
	}
}


void GXColor1x16(u16 idx) {
	if (EmDisplayListInProgress == false) {
		EmGXColor1x16(idx);
	}
	else {
		*(u16 *)EmBuffPtr = idx;
		EmBuffPtr += 2;
	}
}


void GXColor1x8(u8 idx) {
	if (EmDisplayListInProgress == false) {
		EmGXColor1x8(idx);
	}
	else {
		*EmBuffPtr = idx;
		EmBuffPtr++;
	}
}


void GXTexCoord2f32(float s, float t) {
	if (EmDisplayListInProgress == false) {
		EmGXTexCoord2f32(s, t);
	}
	else {
		*(float *)EmBuffPtr = s;
		EmBuffPtr += 4;
		*(float *)EmBuffPtr = t;
		EmBuffPtr += 4;
	}
}


void GXTexCoord2s16(s16 s, s16 t) {
	if (EmDisplayListInProgress == false) {
		EmGXTexCoord2s16(s, t);
	}
	else {
		*(s16 *)EmBuffPtr = s;
		EmBuffPtr += 2;
		*(s16 *)EmBuffPtr = t;
		EmBuffPtr += 2;
	}
}


void GXTexCoord2u16(u16 s, u16 t) {
	if (EmDisplayListInProgress == false) {
		EmGXTexCoord2u16(s, t);
	}
	else {
		*(u16 *)EmBuffPtr = s;
		EmBuffPtr += 2;
		*(u16 *)EmBuffPtr = t;
		EmBuffPtr += 2;
	}
}


void GXTexCoord2s8(s8 s, s8 t) {
	if (EmDisplayListInProgress == false) {
		EmGXTexCoord2s8(s, t);
	}
	else {
		*EmBuffPtr = s;
		EmBuffPtr++;
		*EmBuffPtr = t;
		EmBuffPtr++;
	}
}


void GXTexCoord2u8(u8 s, u8 t) {
	if (EmDisplayListInProgress == false) {
		EmGXTexCoord2u8(s, t);
	}
	else {
		*EmBuffPtr = s;
		EmBuffPtr++;
		*EmBuffPtr = t;
		EmBuffPtr++;
	}
}


void GXTexCoord1f32(f32 s) {
	if (EmDisplayListInProgress == false) {
		EmGXTexCoord1f32(s);
	}
	else {
		*(f32 *)EmBuffPtr = s;
		EmBuffPtr += 4;
	}
}


void GXTexCoord1s16(s16 s) {
	if (EmDisplayListInProgress == false) {
		EmGXTexCoord1s16(s);
	}
	else {
		*(s16 *)EmBuffPtr = s;
		EmBuffPtr += 2;
	}
}


void GXTexCoord1u16(u16 s) {
	if (EmDisplayListInProgress == false) {
		EmGXTexCoord1u16(s);
	}
	else {
		*(u16 *)EmBuffPtr = s;
		EmBuffPtr += 2;
	}
}


void GXTexCoord1s8(s8 s) {
	if (EmDisplayListInProgress == false) {
		EmGXTexCoord1s8(s);
	}
	else {
		*EmBuffPtr = s;
		EmBuffPtr++;
	}
}


void GXTexCoord1u8(u8 s) {
	if (EmDisplayListInProgress == false) {
		EmGXTexCoord1u8(s);
	}
	else {
		*EmBuffPtr = s;
		EmBuffPtr++;
	}
}


void GXTexCoord1x16(u16 idx) {
	if (EmDisplayListInProgress == false) {
		EmGXTexCoord1x16(idx);
	}
	else {
		*(u16 *)EmBuffPtr = idx;
		EmBuffPtr += 2;
	}
}


void GXTexCoord1x8(u8 idx) {
	if (EmDisplayListInProgress == false) {
		EmGXTexCoord1x8(idx);
	}
	else {
		*EmBuffPtr = idx;
		EmBuffPtr++;
	}
}


void GXMatrixIndex1u8(u8 idx) {
	if (EmDisplayListInProgress == false) {
		EmGXMatrixIndex1u8(idx);
	}
	else {
		*EmBuffPtr = idx;
		EmBuffPtr++;
	}
}


