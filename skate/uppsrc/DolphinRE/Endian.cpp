#include "DolphinRE.h"




void EndianSwap32(void* buffer) {
	u32 output;
	((u8*)&output)[0] = ((u8*)buffer)[3];
	((u8*)&output)[1] = ((u8*)buffer)[2];
	((u8*)&output)[2] = ((u8*)buffer)[1];
	((u8*)&output)[3] = ((u8*)buffer)[0];
	*((u32*)buffer) = output;
}

void EndianSwap16(void* buffer) {
	u16 output;
	((u8*)&output)[0] = ((u8*)buffer)[1];
	((u8*)&output)[1] = ((u8*)buffer)[0];
	*((u16*)buffer) = output;
}

u32 EndianSwapReturn32(void* buffer) {
	u32 output;
	((u8*)&output)[0] = ((u8*)buffer)[3];
	((u8*)&output)[1] = ((u8*)buffer)[2];
	((u8*)&output)[2] = ((u8*)buffer)[1];
	((u8*)&output)[3] = ((u8*)buffer)[0];
	return output;
}

u16 EndianSwapReturn16(void* buffer) {
	u16 output;
	((u8*)&output)[0] = ((u8*)buffer)[1];
	((u8*)&output)[1] = ((u8*)buffer)[0];
	return output;
}

