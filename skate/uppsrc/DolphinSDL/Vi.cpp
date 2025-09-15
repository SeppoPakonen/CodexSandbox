#include "DolphinSDL.h"


u32			ScreenWidth  = 640;
u32			ScreenHeight = 480;
u32			retraceCount;

void VIInit() {
	TODO //GetScreen().Initialize();
}

void VIWaitForRetrace() {}
void VIConfigureTVScreen(u16 xOrg, u16 yOrg, u16 xSize, VITVMode mode) {}
void VIConfigureXFrameBuffer(u16 xSize, u16 ySize, VIXFBMode mode) {}
void VIConfigurePan(u16 xOrg, u16 yOrg, u16 width, u16 height) {}
void VISetNextFrameBuffer (void *fb) {}
void VISetFrameBuffer (void *fb) {}
void VISetBlack(BOOL black) {}

u32 VIGetRetraceCount() {
	return retraceCount;
}

u32 VIGetNextField() {
	return 0;
}

void VIFlush() {
	
}

void VIConfigure(GXRenderModeObj* rm) {
	
}

u32 VIGetTvFormat() {
	return VI_PAL;
}

VIRetraceCallback VISetPreRetraceCallback(VIRetraceCallback cb) {
    return cb;
}

VIRetraceCallback VISetPostRetraceCallback(VIRetraceCallback cb) {
    return cb;
}
