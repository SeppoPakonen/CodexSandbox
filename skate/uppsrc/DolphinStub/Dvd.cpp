#include "DolphinStub.h"


BOOL DVDReadAsyncPrio(DVDFileInfo* fileInfo, void* addr, s32 length,
		s32 offset,
		DVDCallback callback, s32 prio) {
	TODO
}
  
s32 DVDCancel(DVDCommandBlock* block) {
	TODO
}

s32  DVDConvertPathToEntrynum(char* pathPtr) {
	TODO
}

s32 DVDGetDriveStatus() {
	TODO
}

BOOL DVDFastOpen(s32 entrynum, DVDFileInfo* fileInfo) {
	TODO
}

/*
BOOL DVDOpen(char* fileName, DVDFileInfo* fileInfo) {
	TODO
}
      
s32 DVDReadPrio(DVDFileInfo* fileInfo, void* addr, s32 length,
				 s32 offset, s32 prio) {
	TODO
}

BOOL DVDClose(DVDFileInfo* fileInfo) {
	TODO
}

void DVDInit() {
	TODO
}
*/