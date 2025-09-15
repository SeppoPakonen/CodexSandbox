#include "DolphinStub.h"

GXRenderModeObj GXNtsc480IntDf;
GXRenderModeObj GXMpal480IntDf;


void GXSetTexCoordGen(
	GXTexCoordID    dst_coord,
	GXTexGenType    func,
	GXTexGenSrc     src_param,
	u32             mtx) {
	TODO
}
    
void GXSetTevOrder(
	GXTevStageID    stage,
	GXTexCoordID    coord,
	GXTexMapID      map,
	GXChannelID     color) {
	TODO
}
    
void GXSetTevSwapMode(
	GXTevStageID    stage,
	GXTevSwapSel    ras_sel,
	GXTevSwapSel    tex_sel) {
	TODO
}
    
void GXSetChanCtrl(
	GXChannelID   chan,
	GXBool        enable,
	GXColorSrc    amb_src,
	GXColorSrc    mat_src,
	u32           light_mask,
	GXDiffuseFn   diff_fn,
	GXAttnFn      attn_fn) {
	TODO
}
    
void GXSetChanMatColor(GXChannelID chan, GXColor mat_color) {
	TODO
}

void GXSetZMode(GXBool		compare_enable,
				GXCompare	func,
				GXBool		update_enable) {
	TODO
}
                
void GXSetChanAmbColor(GXChannelID chan, GXColor amb_color) {
	TODO
}

void GXSetNumTexGens(u8 nTexGens) {
	TODO
}

void GXSetNumChans(u8 nChans) {
	TODO
}

void GXSetNumTevStages(u8 nStages) {
	TODO
}

void GXEnableTexOffsets(GXTexCoordID coord, GXBool line_enable, GXBool point_enable) {
	TODO
}
        
void GXSetTevOp(GXTevStageID id, GXTevMode mode) {
	TODO
}

void GXSetClipMode(GXClipMode mode) {
	TODO
}

void GXSetVtxAttrFmt(
	GXVtxFmt       vtxfmt,
	GXAttr         attr,
	GXCompCnt      cnt,
	GXCompType     type,
	u8             frac) {
	TODO
}
    
void GXSetTexCoordScaleManually(GXTexCoordID coord,
		GXBool enable,
		u16 ss, u16 ts) {
	TODO
}
        
void GXSetAlphaUpdate(GXBool update_enable) {
	TODO
}

void GXSetTevSwapModeTable(
	GXTevSwapSel    table,
	GXTevColorChan  red,
	GXTevColorChan  green,
	GXTevColorChan  blue,
	GXTevColorChan  alpha) {
	TODO
}
   
void GXSetBlendMode(GXBlendMode	type,
					GXBlendFactor	src_factor,
					GXBlendFactor	dst_factor,
					GXLogicOp	op) {
	TODO
}
                    
                    
void  GXLoadTexObj(GXTexObj* obj, GXTexMapID id) {
	TODO
}

void GXSetAlphaCompare(
	GXCompare       comp0,
	u8              ref0,
	GXAlphaOp       op,
	GXCompare       comp1,
	u8              ref1) {
	TODO
}
    
void GXSetColorUpdate(GXBool update_enable) {
	TODO
}

void GXSetCurrentMtx(u32 id) {
	TODO
}

void GXSetTevKColorSel(GXTevStageID stage, GXTevKColorSel sel) {
	TODO
}

void GXInvalidateTexAll() {
	TODO
}

void GXSetTevKAlphaSel(GXTevStageID stage, GXTevKAlphaSel sel) {
	TODO
}
         
void GXInitTexObj(
	GXTexObj*      obj,
	void*          image_ptr,
	u16            width,
	u16            height,
	GXTexFmt       format,
	GXTexWrapMode  wrap_s,
	GXTexWrapMode  wrap_t,
	GXBool         mipmap) {
	TODO
}
    
void GXInitTexObjLOD(
	GXTexObj*      obj,
	GXTexFilter    min_filt,
	GXTexFilter    mag_filt,
	f32            min_lod,
	f32            max_lod,
	f32            lod_bias,
	GXBool         bias_clamp,
	GXBool         do_edge_lod,
	GXAnisotropy   max_aniso) {
	TODO
}
    
void GXSetCullMode(GXCullMode mode) {
	TODO
}

/*void GXSetVtxDesc(GXAttr attr, GXAttrType type) {
	TODO
}*/

void GXSetViewport(
	f32 left,
	f32 top,
	f32 wd,
	f32 ht,
	f32 nearz,
	f32 farz) {
	TODO
}
    
void GXClearVtxDesc() {
	TODO
}

void GXSetScissor(u32 left, u32 top, u32 wd, u32 ht) {
	TODO
}

void GXSetDispCopyGamma(GXGamma gamma) {
	TODO
}

void GXSetPixelFmt(GXPixelFmt pix_fmt, GXZFmt16 z_fmt) {
	TODO
}

void GXSetProjection(const f32 mtx[4][4], GXProjectionType type) {
	TODO
}

void GXInvalidateVtxCache() {
	TODO
}

void GXInitLightAttn(
	GXLightObj*   lt_obj,
	f32           a0,
	f32           a1,
	f32           a2,
	f32           k0,
	f32           k1,
	f32           k2) {
	TODO
}
    
void GXInitLightPos(GXLightObj* lt_obj, f32 x, f32 y, f32 z) {
	TODO
}

void GXInitLightDir(GXLightObj* lt_obj, f32 nx, f32 ny, f32 nz) {
	TODO
}

void GXInitSpecularDir(GXLightObj* lt_obj, f32 nx, f32 ny, f32 nz) {
	TODO
}

void GXInitSpecularDirHA(
	GXLightObj* lt_obj,
	f32         nx,
	f32         ny,
	f32         nz,
	f32         hx,
	f32         hy,
	f32         hz) {
	TODO
}
    
void GXAdjustForOverscan(const GXRenderModeObj *rmin, GXRenderModeObj *rmout,
		u16 hor, u16 ver) {
	TODO
}
        
void GXBegin(GXPrimitive type, GXVtxFmt vtxfmt, u16 nverts) {
	TODO
}

void GXEnd() {
	TODO
}

void GXSetVerifyLevel(GXWarningLevel level) {
	TODO
}

void GXDisableBreakPt() {
	TODO
}

void GXSetScissorBoxOffset(s32 x_off, s32 y_off) {
	TODO
}

GXBreakPtCallback GXSetBreakPtCallback(GXBreakPtCallback cb) {
	TODO
}

void GXFlush(void) {
	TODO
}

void GXGetFifoPtrs(
	GXFifoObj*  fifo,
	void**      readPtr,
	void**      writePtr) {
	TODO
}
    
GXFifoObj* GXGetCPUFifo() {
	TODO
}

GXFifoObj* GXGetGPFifo() {
	TODO
}

void GXEnableBreakPt(void* breakPtr) {
	TODO
}
    
void GXSetGPMetric(GXPerf0       perf0,
				   GXPerf1       perf1) {
	TODO
}
                   
void GXClearGPMetric() {
	TODO
}

void GXReadGPMetric(u32*          cnt0,
					u32*          cnt1) {
	TODO
}
                    
void GXSetDrawSync(u16 token) {
	TODO
}

u16 GXReadDrawSync() {
	TODO
}

void GXSetCopyFilter(GXBool aa, const u8 sample_pattern[12][2], GXBool vf, const u8 vfilter[7]) {
	TODO
}
void GXCopyDisp(void *dest, GXBool clear) {
	TODO
}

void GXSetCopyClear(GXColor clear_clr, u32 clear_z) {
	TODO
}

void GXPokeZ(u16 x, u16 y, u32 z) {
	TODO
}

void GXPeekZ(u16 x, u16 y, u32* z) {
	TODO
}

void GXPokeARGB(u16 x, u16 y, u32 color) {
	TODO
}

void GXPeekARGB(u16 x, u16 y, u32* color) {
	TODO
}

GXFifoObj* GXInit(void* base, u32 size) {
	TODO
}

void GXSetDispCopySrc(u16 left, u16 top, u16 wd, u16 ht) {
	TODO
}

void GXSetDispCopyDst(u16 wd, u16 ht) {
	TODO
}

u32 GXSetDispCopyYScale(f32 vscale) {
	TODO
}

void GXSetViewportJitter(
	f32 left,
	f32 top,
	f32 wd,
	f32 ht,
	f32 nearz,
	f32 farz,
	u32 field) {
	TODO
}
    
    
GXDrawSyncCallback GXSetDrawSyncCallback(GXDrawSyncCallback cb) {
	TODO
}

void GXDrawDone() {
	TODO
}

void GXAbortFrame() {
	TODO
}

void GXSetDrawDone() {
	TODO
}

void GXWaitDrawDone() {
	TODO
}

GXDrawDoneCallback GXSetDrawDoneCallback(GXDrawDoneCallback cb) {
	TODO
}

void GXReadXfRasMetric(u32*          xf_wait_in,
		u32*          xf_wait_out,
		u32*          ras_busy,
		u32*          clocks) {
	TODO
}
        
void GXGetGPStatus(
	GXBool* overhi,
	GXBool* underlow,
	GXBool* readIdle,
	GXBool* cmdIdle,
	GXBool* brkpt) {
	TODO
}
    
void GXGetFifoStatus(
	GXFifoObj*  fifo,
	GXBool*     overhi,
	GXBool*     underlow,
	u32*        fifoCount,
	GXBool*     cpu_write,
	GXBool*     gp_read,
	GXBool*     fifowrap) {
	TODO
}
    
void* GXGetFifoBase(const GXFifoObj* fifo) {
	TODO
}

u32 GXGetFifoSize(const GXFifoObj* fifo) {
	TODO
}

void GXGetFifoLimits(const GXFifoObj* fifo, u32* hi, u32* lo) {
	TODO
}

u32 GXGetOverflowCount() {
	TODO
}

u32 GXResetOverflowCount() {
	TODO
}

void GXSetTexCopySrc(u16 left, u16 top, u16 wd, u16 ht) {
	TODO
}

void GXSetTexCopyDst(u16 wd, u16 ht, GXTexFmt fmt, GXBool mipmap) {
	TODO
}

void GXCopyTex(void *dest, GXBool clear) {
	TODO
}

void GXPixModeSync() {
	TODO
}

void GXPokeAlphaUpdate(GXBool update_enable) {
	TODO
}

void GXPokeColorUpdate(GXBool update_enable) {
	TODO
}

void GXGetProjectionv(f32* p) {
	TODO
}

void GXSetProjectionv(const f32 *ptr) {
	TODO
}

void GXGetViewportv(f32* viewport) {
	TODO
}

void GXGetScissor(
	u32*    left,
	u32*    top,
	u32*    width,
	u32*    height) {
	TODO
}
    
void GXProject(
	f32  x,          // model coordinates
	f32  y,
	f32  z,
	const f32  mtx[3][4],  // model-view matrix
	const f32* pm,         // projection matrix, as returned by GXGetProjectionv
	const f32* vp,         // viewport, as returned by GXGetViewportv
	f32* sx,         // screen coordinates
	f32* sy,
	f32* sz) {
	TODO
}
    
void GXInitLightAttnA(GXLightObj *lt_obj, f32 a0, f32 a1, f32 a2) {
	TODO
}

void GXInitLightAttnK(GXLightObj *lt_obj, f32 k0, f32 k1, f32 k2) {
	TODO
}

void GXInitLightSpot(
	GXLightObj*   lt_obj,
	f32           cutoff,
	GXSpotFn      spot_func) {
	TODO
}
    
void GXInitLightDistAttn(
	GXLightObj*   lt_obj,
	f32           ref_distance,
	f32           ref_brightness,
	GXDistAttnFn  dist_func) {
	TODO
}
    
void GXInitLightColor(GXLightObj* lt_obj, GXColor color) {
	TODO
}

void GXLoadLightObjImm(const GXLightObj* lt_obj, GXLightID light) {
	TODO
}

void GXLoadLightObjIndx(u32 lt_obj_indx, GXLightID light) {
	TODO
}

void GXCallDisplayList(const void *list, u32 nbytes) {
	TODO
}

void GXSetLineWidth(u8 width, GXTexOffset texOffsets) {
	TODO
}

void GXSetPointSize(u8 pointSize, GXTexOffset texOffsets) {
	TODO
}

void GXSetFog(GXFogType	type,
			  f32		startz,
			  f32		endz,
			  f32		nearz,
			  f32		farz,
			  GXColor		color) {
	TODO
}
              
void GXSetFogRangeAdj(GXBool enable, u16 center, GXFogAdjTable* table) {
	TODO
}

void GXSetZCompLoc(GXBool before_tex) {
	TODO
}

void GXSetDither(GXBool dither) {
	TODO
}

void GXSetDstAlpha(GXBool enable, u8 alpha) {
	TODO
}

u32 GXGetTexBufferSize(
	u16           width,
	u16           height,
	u32           format,
	GXBool        mipmap,
	u8            max_lod) {
	TODO
}
    
    
    
    
    
    
    
    
    
void GXTexCoord2u16(u16 s, u16 t) {
	TODO
}

void GXPosition3s16(s16 x, s16 y, s16 z) {
	TODO
}
