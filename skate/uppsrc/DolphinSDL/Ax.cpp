#include "DolphinSDL.h"


void AXSetVoiceState(AXVPB *p, u16 state) {
	TODO
}

void AXSetVoiceVe(AXVPB *p, AXPBVE *ve) {
	TODO
}

void AXFreeVoice(AXVPB *p) {
	TODO
}

void AXRegisterCallback(AXUserCallback callback) {
	TODO
}

void AXSetVoiceAdpcm(AXVPB *p, AXPBADPCM *adpcm) {
	TODO
}

void AXSetVoiceAddr(AXVPB *p, AXPBADDR *addr) {
	TODO
}

void AXSetVoiceSrcType(AXVPB *p, u32 type) {
	TODO
}

AXVPB*  AXAcquireVoice(
	u32                priority,
	AXVoiceCallback    callback,
	u32                userContext) {
	TODO
}

void AXSetVoiceType(AXVPB *p, u16 type) {
	TODO
}

void AXSetVoiceSrc(AXVPB *p, AXPBSRC *src) {
	TODO
}

void AXSetVoiceMix(AXVPB *p, AXPBMIX *mix) {
	TODO
}

void AXInit() {
	TODO
}

void AXQuit() {
	TODO
}

void AXSetVoiceSrcRatio(AXVPB *p, f32 ratio) {
	TODO
}

void AXSetMode(int mode) {
	TODO
}

void AXSetCompressor(int mode) {
	TODO
}

void AXSetVoiceAdpcmLoop(AXVPB *p, AXPBADPCMLOOP *adpcmloop) {
	TODO
}

