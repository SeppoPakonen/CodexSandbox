#include "DolphinRE.h"


uint CopyRectLeft;
uint CopyRectTop;
uint CopyRectWd;
uint CopyRectHt;
int TexCopyRectLeft;
int TexCopyRectTop;
uint TexCopyRectSrcWd;
uint TexCopyRectSrcHt;
uint TexCopyRectDstWd;
uint TexCopyRectDstHt;
bool TexCopyMipmap;
GXTexFmt TexCopyFmt;

GXPixelFmt CurrentPixFmt;


void GXSetDispCopySrc(u16 left, u16 top, u16 wd, u16 ht) {
	CopyRectLeft = left;
	CopyRectTop = top;
	CopyRectWd = wd;
	CopyRectHt = ht;
}


void GXCopyDisp(void *dest, GXBool clear) {
	Gfx::Static::Finish();
	WinSwapBuffers();
	if (clear)
		ClearFrameBuffer(CopyRectLeft, CopyRectTop, CopyRectWd, CopyRectHt);
}


void GXSetCopyClear(GXColor clear_clr, u32 clear_z) {
	float red	= clear_clr.r;
	float green	= clear_clr.g;
	float blue	= clear_clr.b;
	float alpha	= clear_clr.a;
	Gfx::Static::ClearColor(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);
	Gfx::Static::ClearDepth(clear_z / 16777215.0);
}


void GXSetTexCopySrc(u16 left, u16 top, u16 wd, u16 ht) {
	ASSERTMSG((left & 1) == 0, "Left pos align");
	ASSERTMSG((top & 1)  == 0, "Top pos align");
	ASSERTMSG((wd & 3)   == 0, "Width pos align");
	ASSERTMSG((ht & 1)   == 0, "Height pos align");
	
	TexCopyRectLeft = left;
	TexCopyRectTop = top;
	TexCopyRectSrcWd = wd;
	TexCopyRectSrcHt = ht;
}


void GXSetTexCopyDst(u16 wd, u16 ht, GXTexFmt fmt, GXBool mipmap) {
	TexCopyFmt = fmt;
	TexCopyRectDstWd = wd;
	TexCopyRectDstHt = ht;
	TexCopyMipmap = mipmap;
}


void GXCopyTex(void *dest, GXBool clear) {
	ASSERT(CurrentPixFmt >= 0 && CurrentPixFmt < 3);
	
	int ret = TranslateGXTexFormat(TexCopyFmt);
	if (ret < 0) {
		PANIC("Wrong texture format");
	}
	
	if (TexCopyMipmap == false) {
		ASSERTMSG(TexCopyRectDstWd == TexCopyRectSrcWd, "Wrong destination width");
		ASSERTMSG(TexCopyRectDstHt == TexCopyRectSrcHt, "Wrong destination height");
	}
	else {
		ASSERTMSG(TexCopyRectDstWd == (TexCopyRectSrcWd >> 1), "Wrong destination width");
		ASSERTMSG(TexCopyRectDstHt == (TexCopyRectSrcHt >> 1), "Wrong destination height");
	}
	
	TexCopyFBImage(
		(byte *)dest,
		TexCopyRectLeft,
		ScreenHeight - TexCopyRectTop,
		TexCopyRectSrcWd,
		TexCopyRectSrcHt,
		TexCopyFmt,
		TexCopyMipmap);
	
	if (clear)
		ClearFrameBuffer(TexCopyRectLeft, TexCopyRectTop, TexCopyRectSrcWd, TexCopyRectSrcHt);
}



void ClearFrameBuffer(u32 left, u32 top, u32 width, u32 height) {
	u32 sc_height;
	u32 sc_top;
	u32 sc_width;
	u32 sc_left;
	Gxb data[4];
	
	Gfx::Static::GetBoolean(PARAM_DEPTH_WRITEMASK, data);
	GXGetScissor(&sc_left, &sc_top, &sc_width, &sc_height);
	
	Gfx::Static::DepthMask(true);
	GXSetScissor(left, top, width, height);
	
	Gfx::Static::Clear(BF_COLOR_BUFFER_BIT | BF_DEPTH_BUFFER_BIT);
	Gfx::Static::DepthMask(data[0]);
	GXSetScissor(sc_left, sc_top, sc_width, sc_height);
}


