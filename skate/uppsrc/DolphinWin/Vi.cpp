//============================================================================
// PC Dolphin Emulator Version 2.6
// Nintendo of America, Inc.
// Product Support Group
// Dante Treglia II
// danttr01@noa.nintendo.com
// April 12, 2000
// Copyright (c) 2000 Nintendo of America Inc.
//============================================================================

#include <Dolphin/dolphin.h>
#include <Dolphin/win32.h>

extern u32			ScreenWidth;
extern u32			ScreenHeight;
extern u32			retraceCount;

void VIInit(void) {
}

void VIWaitForRetrace   ( void ) {
}

void VIConfigureTVScreen(u16 xOrg, u16 yOrg, u16 xSize, VITVMode mode) {
}

void VIConfigureXFrameBuffer(u16 xSize, u16 ySize, VIXFBMode mode) {
}

void VIConfigurePan(u16 xOrg, u16 yOrg, u16 width, u16 height) {
}

void VISetNextFrameBuffer (void *fb) {
}

void VISetFrameBuffer (void *fb) {
}

void VISetBlack(BOOL black) {
}

u32 VIGetRetraceCount(void) {
	return retraceCount;
}

u32 VIGetNextField(void) {
	return 0;
}

void VIFlush(void) {
}

void VIConfigure(GXRenderModeObj* rm) {
}

u32 VIGetTvFormat(void) {
	return VI_NTSC;;
}

VIRetraceCallback VISetPreRetraceCallback(VIRetraceCallback cb)
{
    return cb;
}

VIRetraceCallback VISetPostRetraceCallback(VIRetraceCallback cb)
{
    return cb;
}
