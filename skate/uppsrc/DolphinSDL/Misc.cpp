#include "DolphinSDL.h"

void std::OurPrintf(const char* txt) {
	TODO
}

void LCDisable() {
	TODO
}

unsigned int PtrIndex_FindAdd(void* ptr) {
	TODO
}

void snPause() {
	TODO
}

void __hwInitForEmu() {
	TODO
}

void LCEnable() {
	TODO
}



extern "C" {
	
	void TransformSingle(ROMtx m, s16 * srcBase, s16 * dstBase, u32 count) {
		TODO
	}

	void TransformDouble(ROMtx m0, ROMtx m1, s16 * wtBase, s16 * srcBase, s16 * dstBase, u32 count) {
		TODO
	}
	
	void TransformAcc(ROMtx m, u16 count, s16 * srcBase, s16 * dstBase, u16 * indices, s16 * weights) {
		TODO
	}

}

void AIInit(u8* stack) {
	TODO
}

void AIReset() {
	TODO
}






GDLObj *__GDCurrentDL;

void GDInitGDLObj(GDLObj *dl, void *start, u32 length) {
	TODO
}

void GDFlushCurrToMem() {
	TODO
}

void GDPadCurr32() {
	TODO
}

void GDOverflowed() {
	TODO
}




void GXSetTevSwapModeTable(
	GXTevSwapSel    table,
	GXTevColorChan  red,
	GXTevColorChan  green,
	GXTevColorChan  blue,
	GXTevColorChan  alpha) {
	TODO
}

void GXSetTevKColorSel(GXTevStageID stage, GXTevKColorSel sel) {
	TODO
}

void GXSetScissorBoxOffset(s32 x_off, s32 y_off) {
	TODO
}

GXBreakPtCallback GXSetBreakPtCallback(GXBreakPtCallback cb) {
	TODO
}

void GXSetTevKAlphaSel(GXTevStageID stage, GXTevKAlphaSel sel) {
	TODO
}





u32 VIGetDTVStatus() {
	TODO
}



void DTKInit() {
	TODO
}
void DTKSetRepeatMode(int mode) {
	TODO
}

void DTKSetRepeatMode(u32 repeat) {
	TODO
}

