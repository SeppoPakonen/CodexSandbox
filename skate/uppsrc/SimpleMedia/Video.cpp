#include "SimpleMedia.h"


int VidChunkLen(u8*) {
	TODO
}

VAUDDecoder	VAUDCreateDecoder(VAUDAllocator alloc, VAUDDeallocator free, int max_heap) {
	TODO
}
void VAUDDestroyDecoder(VAUDDecoder dec) {
	TODO
}

bool VAUDInitDecoder(VAUDDecoder dec, const u8* data, int len, int prealloc) {
	TODO
}

int VAUDDecode(VAUDDecoder dec, const u8* begin, int size, u32 channelSelectMask, AudioDecodeCb ptr, void* unknown) {
	TODO
}

BOOL VIDVideoDecode(VidDecoder* dec, const u8* chunk, int chunk_size, u8** dst) {
	TODO
}

void VIDDrawGXYuv2RgbDraw(s16 x, s16 y, s16 w, s16 h, const u8* img) {
	TODO
}

void VIDXFBDraw(const u8* img, void* buf, int w, int h, void* mem) {
	TODO
}

int VIDSetupTEV(int) {
	TODO
}

int VIDAudioDecode(VAUDDecoder dec, const u8* buf, int size, int, AudioDecodeCb, void* arg) {
	TODO
}

int VIDXFBGetLCSize() {
	TODO
}

void VIDSimpleAudioStartPlayback(const u32* mask, int) {
	TODO
}

VAUDDecoder VIDCreateDecoder(VIDDecoderSetup* setup) {
	TODO
}

void VIDDestroyDecoder(VAUDDecoder dec) {
	TODO
}





void GQRSetup6(int x_scale, int x_type, int y_scale, int y_type) {
	LOG("GQRSetup6: pass");
}

void GQRSetup7(int x_scale, int x_type, int y_scale, int y_type) {
	LOG("GQRSetup7: pass");
}

