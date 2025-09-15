#include "DolphinSDL.h"




BOOL DVDOpen (const char* fileName, DVDFileInfo* fileInfo) {
	struct stat fileStat;

	stat(fileName, &fileStat);
	fileInfo->length = fileStat.st_size;
	fileInfo->file = fopen(fileName, "rbw");
	assert(fileInfo->file != NULL && "DVDOpen: Couldn't open file");
	return (fileInfo->file != NULL);
}


s32 DVDReadPrio (DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset, s32 prio) {
	u32 bytesRead;

	assert(fileInfo->file != NULL && "DVDRead: File not open");
	fseek(fileInfo->file, offset, SEEK_SET);
	bytesRead = fread(addr, sizeof(char), length, fileInfo->file);
	return bytesRead;
}

BOOL DVDClose (DVDFileInfo* fileInfo) {
	fclose(fileInfo->file);
	return 1;
}

void DVDSetRoot(const char *rootPath) {
	chdir(rootPath);
}

BOOL DVDChangeDir(char* dirName) {
	return !chdir(dirName);
}

void DVDInit() {
	
}

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
