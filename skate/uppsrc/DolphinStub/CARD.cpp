#include "DolphinStub.h"

s32 CARDOpen(s32 chan, char* fileName, CARDFileInfo* fileInfo) {
	TODO
}

s32 CARDWrite(CARDFileInfo* fileInfo, void* addr, s32 length, s32 offset) {
	TODO
}

s32 CARDGetStatus(s32 chan, s32 fileNo, CARDStat* stat) {
	TODO
}

s32 CARDCheck(s32 chan) {
	TODO
}

s32 CARDRead(CARDFileInfo* fileInfo, void* addr, s32 length, s32 offset) {
	TODO
}

s32 CARDCreate(s32 chan, char* fileName, u32 size, CARDFileInfo* fileInfo) {
	TODO
}

s32 CARDSetStatus(s32 chan, s32 fileNo, CARDStat* stat) {
	TODO
}

void CARDInit() {
	TODO
}

s32 CARDProbeEx(s32 chan, s32* memSize, s32* sectorSize) {
	TODO
}

s32 CARDMount(s32 chan, void* workArea, CARDCallback detachCallback) {
	TODO
}

s32 CARDFormat(s32 chan) {
	TODO
}

s32 CARDGetEncoding(s32 chan, u16* encode) {
	TODO
}

s32 CARDFreeBlocks(s32 chan, s32* byteNotUsed, s32* filesNotUsed) {
	TODO
}

s32 CARDDelete(s32 chan, char* fileName) {
	TODO
}

s32 CARDClose(CARDFileInfo* fileInfo) {
	TODO
}
