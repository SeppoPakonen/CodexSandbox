#include "DolphinRE.h"

GXTexRegion		gx_tex1[8];
GXTexRegion		gx_tex2[4];
GXTlutRegion	gx_tlut1[0x10];
GXTlutRegion	gx_tlut2[4];
int				tex1n;
int				tex2n;



GXFifoObj * GXInit(void *base, u32 size) {
	Mtx arr;
	GXColor white	{0xff,	0xff,	0xff,	0xff};
	GXColor gray	{0x40,	0x40,	0x40,	0xff};
	GXColor black	{0x0,	0x0,	0x0,	0xff};
	
	//GXInit2();
	
	u32 vi = VIGetTvFormat();
	ASSERTMSG(vi == VI_NTSC || vi == VI_PAL || vi == VI_MPAL, "GXInit: invalid TV format\n");
	
	GXRenderModeObj *rend_mode = NULL;
	if (vi == VI_NTSC)
		rend_mode = &GXNtsc480IntDf;
	else if (vi == VI_PAL)
		rend_mode = &GXPal528IntDf;
	else if (vi == VI_MPAL)
		rend_mode = &GXMpal480IntDf;
		
	GXSetCopyClear(gray, 0xffffff);
	GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c);
	GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x3c);
	GXSetTexCoordGen(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX2, 0x3c);
	GXSetTexCoordGen(GX_TEXCOORD3, GX_TG_MTX2x4, GX_TG_TEX3, 0x3c);
	GXSetTexCoordGen(GX_TEXCOORD4, GX_TG_MTX2x4, GX_TG_TEX4, 0x3c);
	GXSetTexCoordGen(GX_TEXCOORD5, GX_TG_MTX2x4, GX_TG_TEX5, 0x3c);
	GXSetTexCoordGen(GX_TEXCOORD6, GX_TG_MTX2x4, GX_TG_TEX6, 0x3c);
	GXSetTexCoordGen(GX_TEXCOORD7, GX_TG_MTX2x4, GX_TG_TEX7, 0x3c);
	GXSetNumTexGens(1);
	GXClearVtxDesc();
	GXInvalidateVtxCache();
	GXSetLineWidth(6, GX_TO_ZERO);
	GXSetPointSize(6, GX_TO_ZERO);
	GXEnableTexOffsets(GX_TEXCOORD0, 0, 0);
	GXEnableTexOffsets(GX_TEXCOORD1, 0, 0);
	GXEnableTexOffsets(GX_TEXCOORD2, 0, 0);
	GXEnableTexOffsets(GX_TEXCOORD3, 0, 0);
	GXEnableTexOffsets(GX_TEXCOORD4, 0, 0);
	GXEnableTexOffsets(GX_TEXCOORD5, 0, 0);
	GXEnableTexOffsets(GX_TEXCOORD6, 0, 0);
	GXEnableTexOffsets(GX_TEXCOORD7, 0, 0);
	GXLoadPosMtxImm(arr, 0);
	GXLoadNrmMtxImm(arr, 0);
	GXSetCurrentMtx(0);
	GXLoadTexMtxImm(arr, 0x3c, GX_MTX3x4);
	GXSetViewport(0.0, 0.0, rend_mode->fbWidth, rend_mode->xfbHeight, 0.0, 1.0);
	GXSetCoPlanar(0);
	GXSetCullMode(GX_CULL_BACK);
	GXSetClipMode(GX_CLIP_ENABLE);
	GXSetScissor(0, 0, rend_mode->fbWidth, rend_mode->efbHeight);
	GXSetNumChans(0);
	
	GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
	GXSetChanAmbColor(GX_COLOR0A0, black);
	GXSetChanMatColor(GX_COLOR0A0, white);
	
	GXSetChanCtrl(GX_COLOR1A1, 0, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
	GXSetChanAmbColor(GX_COLOR1A1, black);
	GXSetChanMatColor(GX_COLOR1A1, white);
	
	GXInvalidateTexAll();
	
	
	tex1n = 0;
	for(int i = 0; i < 8; i++)
		GXInitTexCacheRegion(
			&gx_tex1[i],
			0,
			i << 0xf,
			GX_TEXCACHE_NONE,
			i * 0x8000 + 0x80000,
			GX_TEXCACHE_NONE);
	
	tex2n = 0;
	for(int i = 0; i < 4; i++)
		GXInitTexCacheRegion(
			&gx_tex2[i],
			0,
			(i * 2 + 8) * 0x8000,
			GX_TEXCACHE_NONE,
			(i * 2 + 9) * 0x8000,
			GX_TEXCACHE_NONE);
	
	for(int i = 0; i < 0x10; i++)
		GXInitTlutRegion(
			&gx_tlut1[i],
			i * 0x2000 + 0xc0000,
			GX_TLUT_256);
	
	for(int i = 0; i < 4; i++)
		GXInitTlutRegion(
			&gx_tlut2[i],
			i * 0x8000 + 0xe0000,
			GX_TLUT_1K);
	
	GXSetTexRegionCallback(GXDefaultTexRegionCallback);
	GXSetTlutRegionCallback(GXDefaultTlutRegionCallback);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
	GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
	GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD2, GX_TEXMAP2, GX_COLOR0A0);
	GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD3, GX_TEXMAP3, GX_COLOR0A0);
	GXSetTevOrder(GX_TEVSTAGE4, GX_TEXCOORD4, GX_TEXMAP4, GX_COLOR0A0);
	GXSetTevOrder(GX_TEVSTAGE5, GX_TEXCOORD5, GX_TEXMAP5, GX_COLOR0A0);
	GXSetTevOrder(GX_TEVSTAGE6, GX_TEXCOORD6, GX_TEXMAP6, GX_COLOR0A0);
	GXSetTevOrder(GX_TEVSTAGE7, GX_TEXCOORD7, GX_TEXMAP7, GX_COLOR0A0);
	GXSetTevOrder(GX_TEVSTAGE8, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE9, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE10, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE11, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE12, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE13, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE14, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE15, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
	GXSetNumTevStages(1);
	GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
	GXSetZTexture(GX_ZT_DISABLE, GX_TF_Z16, 0);
	
	for(int tev_stage = 0; tev_stage < GX_MAX_TEVSTAGE; tev_stage++)
		GXSetTevDirect((GXTevStageID)tev_stage);
	
	GXSetNumIndStages(0);
	GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
	GXSetIndTexCoordScale(GX_INDTEXSTAGE1, GX_ITS_1, GX_ITS_1);
	GXSetIndTexCoordScale(GX_INDTEXSTAGE2, GX_ITS_1, GX_ITS_1);
	GXSetIndTexCoordScale(GX_INDTEXSTAGE3, GX_ITS_1, GX_ITS_1);
	GXSetFog(GX_FOG_NONE, 0.0, 1.0, 0.1, 1.0, black);
	GXSetFogRangeAdj(0, 0, 0);
	GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
	GXSetColorUpdate(1);
	GXSetAlphaUpdate(1);
	GXSetZMode(1, GX_LEQUAL, 1);
	GXSetZCompLoc(1);
	GXSetDither(1);
	GXSetDstAlpha(0, 0);
	GXSetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
	GXSetFieldMask(1, 1);
	GXSetFieldMode(rend_mode->xFBmode, rend_mode->xfbHeight == rend_mode->viXOrigin * 2);
	GXSetDispCopySrc(0, 0, rend_mode->fbWidth, rend_mode->efbHeight);
	GXSetDispCopyDst(rend_mode->fbWidth, rend_mode->efbHeight);
	GXSetDispCopyYScale((float)rend_mode->fbWidth / (float)rend_mode->efbHeight);
	GXSetCopyClamp((GXFBClamp)(GX_CLAMP_TOP | GX_CLAMP_BOTTOM));
	GXSetCopyFilter(rend_mode->aa, rend_mode->sample_pattern, true, rend_mode->vfilter);
	GXSetDispCopyGamma(GX_GM_1_0);
	GXSetDispCopyFrame2Field(GX_COPY_PROGRESSIVE);
	GXClearBoundingBox();
	GXPokeColorUpdate(1);
	GXPokeAlphaUpdate(1);
	GXPokeDither(0);
	GXPokeBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ONE, GX_LO_SET);
	GXPokeAlphaMode(GX_ALWAYS, 0);
	GXPokeAlphaRead(GX_READ_FF);
	GXPokeDstAlpha(0, 0);
	GXPokeZMode(1, GX_ALWAYS, 1);
	
	return 0;
}


GXTexRegion * GXDefaultTexRegionCallback(GXTexObj *t_obj, GXTexMapID id) {
	ASSERTMSG(t_obj, "Texture Object Pointer is null");
	
	if (t_obj->do_edge_lod == false)
		tex2n++;
	else
		tex1n++;
	
	return 0;
}


GXTlutRegion * GXDefaultTlutRegionCallback(u32 idx) {
	GXTlutRegion* region = 0;
	
	if (idx < 0x10)
		region = &gx_tlut1[idx];
	else if (idx < 0x14)
		region = &gx_tlut2[idx - 0x10];
	
	return region;
}


