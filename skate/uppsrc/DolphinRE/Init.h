#pragma once

extern GXTexRegion gx_tex1[8];
extern GXTexRegion gx_tex2[4];
extern GXTlutRegion gx_tlut1[0x10];
extern GXTlutRegion gx_tlut2[4];
extern int tex1n;
extern int tex2n;

void GXInit2();
//void GXInitTexCacheRegion(GXTexRegion * region, GXBool is_32b_mipmap, u32 tmem_even, GXTexCacheSize size_even, u32 tmem_odd, GXTexCacheSize size_odd);
void GXInitTlutRegion(GXTlutRegion * region, u32 tmem_addr, GXTlutSize tlut_size);
GXTexRegion * GXDefaultTexRegionCallback(GXTexObj *t_obj,GXTexMapID id);
GXTlutRegion * GXDefaultTlutRegionCallback(u32 idx);
void GXSetTevDirect(GXTevStageID tev_stage);
void GXSetCopyClamp(GXFBClamp clamp);

/*
GXFifoObj * GXInit(void *base,u32 size);
void GXPokeZMode(GXBool compare_enable, GXCompare func, GXBool update_enable)
void GXPokeDstAlpha(GXBool enable, u8 alpha)
void GXPokeAlphaRead(GXAlphaReadMode mode)
void GXPokeAlphaMode(GXCompare func, u8 threshold)
void GXPokeBlendMode(GXBlendMode type, GXBlendFactor src_factor, GXBlendFactor dst_factor, GXLogicOp op)
void GXPokeDither(GXBool dither)
void GXPokeAlphaUpdate(GXBool update_enable)
void GXPokeColorUpdate(GXBool update_enable)
void GXClearBoundingBox(void)
void GXSetDispCopyFrame2Field(GXCopyMode mode)
void GXSetDispCopyGamma(GXGamma gamma)
void GXSetCopyFilter(GXBool aa, u8[12][2] sample_pattern, GXBool vf, u8[7] vfilter)
u32 GXSetDispCopyYScale(f32 vscale)
void GXSetDispCopyDst(u16 wd, u16 ht)
void GXSetDispCopySrc(u16 left, u16 top, u16 wd, u16 ht)
void GXSetFieldMode(GXBool field_mode, GXBool half_aspect_ratio)
void GXSetFieldMask(GXBool odd_mask, GXBool even_mask)
void GXSetPixelFmt(GXPixelFmt pix_fmt, GXZFmt16 z_fmt)
void GXSetDstAlpha(GXBool enable, u8 alpha)
void GXSetDither(GXBool dither)
void GXSetZCompLoc(GXBool before_tex)
void GXSetZMode(GXBool compare_enable, GXCompare func, GXBool update_enable)
void GXSetAlphaUpdate(GXBool update_enable)
void GXSetColorUpdate(GXBool update_enable)
void GXSetBlendMode(GXBlendMode type, GXBlendFactor src_factor, GXBlendFactor dst_factor, GXLogicOp op)
void GXSetFogRangeAdj(GXBool enable, u16 center, GXFogAdjTable * table)
void GXSetFog(GXFogType type, f32 startz, f32 endz, f32 nearz, f32 farz, GXColor color)
void GXSetIndTexCoordScale(GXIndTexStageID ind_stage, GXIndTexScale scale_s, GXIndTexScale scale_t)
void GXSetNumIndStages(u8 nstages)
void GXSetZTexture(GXZTexOp op, GXTexFmt fmt, u32 bias)
void GXSetAlphaCompare(GXCompare comp0, u8 ref0, GXAlphaOp op, GXCompare comp1, u8 ref1)
void GXSetTevOp(GXTevStageID id, GXTevMode mode)
void GXSetNumTevStages(u8 nStages)
void GXSetTevOrder(GXTevStageID stage, GXTexCoordID coord, GXTexMapID map, GXChannelID color)
GXTlutRegionFn * GXSetTlutRegionCallback(GXTlutRegionFn * f)
GXTexRegionFn * GXSetTexRegionCallback(GXTexRegionFn * f)
void GXInvalidateTexAll(void)
void GXSetChanMatColor(GXChannelID chan, GXColor mat_color)
void GXSetChanAmbColor(GXChannelID chan, GXColor amb_color)
void GXSetChanCtrl(GXChannelID chan, GXBool enable, GXColorSrc amb_src, GXColorSrc mat_src, GXLightID light_mask, GXDiffuseFn diff_fn, GXAttnFn attn_fn)
void GXSetNumChans(u8 nChans)
void GXSetScissor(u32 xOrig, u32 yOrig, u32 wd, u32 ht)
void GXSetClipMode(GXClipMode mode)
void GXSetCullMode(GXCullMode mode)
void GXSetCoPlanar(GXBool enable)
void GXSetViewport(f32 xOrig, f32 yOrig, f32 wd, f32 ht, f32 nearZ, f32 farZ)
void GXLoadTexMtxImm(MtxPtr mtxPtr, u32 id, GXTexMtxType type)
void GXSetCurrentMtx(u32 id)
void GXLoadNrmMtxImm(Mtx mtx_ptr, u32 id)
void GXLoadPosMtxImm(MtxPtr m, u32 id)
void GXEnableTexOffsets(GXTexCoordID coord, GXBool line_enable, GXBool point_enable)
void GXSetPointSize(u8 size, GXTexOffset tex_offsets)
void GXSetLineWidth(u8 width, GXTexOffset tex_offsets)
void GXInvalidateVtxCache(void)
void GXClearVtxDesc(void)
void GXSetNumTexGens(u8 nTexGens)
void GXSetTexCoordGen(GXTexCoordID dst_coord, GXTexGenType func, GXTexGenSrc src_param, u32 mtx)
void GXSetCopyClear(GXColor a, GXColor b)
undefined _OSPanic()
u32 VIGetTvFormat(void)
undefined __chkesp()
*/
