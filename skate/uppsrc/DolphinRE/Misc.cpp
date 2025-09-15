#include "DolphinRE.h"

GXFifoObj	fifo;
u16			DrawSyncToken;


GXFifoObj * GXInitShort(void *base, u32 size) {
	Gxf gl_clr [4];
	gl_clr[0] = 1.0;
	gl_clr[1] = 1.0;
	gl_clr[2] = 1.0;
	gl_clr[3] = 1.0;
	
	MatrixMode(MODELVIEW);
	LoadIdentity();
	TexEnv(TEXTGT_TEXTURE_ENV, TEXPARAM_TEXTURE_ENV_COLOR, gl_clr);
	ShadeModel(SM_SMOOTH);
	
	return &fifo;
}


void GXDrawDone() {
	Gfx::Static::Finish();
}


void GXFlush() {
	Flush();
}


void GXSetDrawSync(u16 token) {
	DrawSyncToken = token;
}


u16 GXReadDrawSync() {
	return DrawSyncToken;
}


