#include "DolphinStub.h"

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