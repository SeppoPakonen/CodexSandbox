#include "DolphinRE.h"



void GXInvalidateVtxCache() {}
void GXLoadLightObjIndx( u32 lt_obj_indx, GXLightID light ) {}
void GXSetClipMode( GXClipMode mode ) {}

void GXSetTevColorIn(
    GXTevStageID    stage,
    GXTevColorArg   a,
    GXTevColorArg   b,
    GXTevColorArg   c,
    GXTevColorArg   d ) {}

void GXSetTevAlphaIn(
    GXTevStageID    stage,
    GXTevAlphaArg   a,
    GXTevAlphaArg   b,
    GXTevAlphaArg   c,
    GXTevAlphaArg   d ) {}

void GXSetTevColorOp(
    GXTevStageID    stage,
    GXTevOp         op,
    GXTevBias       bias,
    GXTevScale      scale,
    GXBool          clamp,
    GXTevRegID      out_reg ) {}

void GXSetTevAlphaOp(
    GXTevStageID    stage,
    GXTevOp         op,
    GXTevBias       bias,
    GXTevScale      scale,
    GXBool          clamp,
    GXTevRegID      out_reg ) {}

void GXSetTevColor( GXTevRegID id, GXColor color );
void GXSetTevColorS10( GXTevRegID id, GXColorS10 color );

void GXSetTevClampMode(GXTevStageID stage, GXTevClampMode mode) {}
void GXSetZTexture(GXZTexOp op, GXTexFmt fmt, u32 bias) {}
void GXSetZCompLoc(GXBool before_tex) {}
void GXInitFogAdjTable(GXFogAdjTable* table, u16 width, f32 projmtx[4][4]) {}
void GXSetFogRangeAdj(GXBool enable, u16 center, GXFogAdjTable *table) {}
void GXSetColorUpdate(GXBool update_enable) {}
void GXSetAlphaUpdate(GXBool update_enable) {}
void GXSetDstAlpha(GXBool enable, u8 alpha) {}
void GXSetFieldMask(GXBool odd_mask, GXBool even_mask) {}
void GXSetTexCoordScaleManually(GXTexCoordID texcoord, GXBool enable, u16 ss, u16 ts) {}
void GXSetTexCoordCylWrap(GXTexCoordID texcoord, GXBool s_enable, GXBool t_enable) {}
void GXEnableTexOffsets(GXTexCoordID coord, GXBool line_enable, GXBool point_enable) {}
void GXSetNumIndStages(u8 nstages) {}
void GXSetIndTexOrder(GXIndTexStageID ind_stage, GXTexCoordID tex_coord, GXTexMapID tex_map) {}
void GXSetIndTexCoordScale(GXIndTexStageID ind_stage, GXIndTexScale scale_s, GXIndTexScale scale_t) {}
void GXSetIndTexMtx(GXIndTexMtxID mtx_sel, f32 offset_mtx [2] [3], s8 scale_exp) {}
void GXSetTevIndirect(GXTevStageID tev_stage, GXIndTexStageID ind_stage, GXIndTexFormat format,
	GXIndTexBiasSel bias_sel, GXIndTexMtxID matrix_sel, GXIndTexWrap wrap_s,
	GXIndTexWrap wrap_t, GXBool add_prev, GXBool utc_lod, GXIndTexAlphaSel alpha_sel) {}
void GXSetTevDirect(GXTevStageID tev_stage) {}
void GXSetTevIndWarp(GXTevStageID tev_stage, GXIndTexStageID ind_stage, GXBool signed_offsets,
	GXBool replace_mode, GXIndTexMtxID matrix_sel) {}
void GXSetTevIndTile(GXTevStageID tev_stage, GXIndTexStageID ind_stage, u16 tilesize_s, u16 tilesize_t,
	u16 tilespacing_s, u16 tilespacing_t, GXIndTexFormat format,
	GXIndTexMtxID matrix_sel, GXIndTexBiasSel bias_sel, GXIndTexAlphaSel alpha_sel) {}
void GXSetTevIndBumpST(GXTevStageID tev_stage, GXIndTexStageID ind_stage, GXIndTexMtxID matrix_sel) {}
void GXSetTevIndBumpXYZ(GXTevStageID tev_stage, GXIndTexStageID ind_stage, GXIndTexMtxID matrix_sel) {}
void GXSetTevIndRepeat(GXTevStageID tev_stage) {}
void GXSetFieldMode(GXBool field_mode, GXBool half_aspect_ratio) {}
void GXSetDispCopyDst(u16 wd, u16 ht) {}
void GXSetDispCopyFrame2Field(GXCopyMode mode) {}
void GXSetCopyClamp(GXFBClamp clamp) {}
u32 GXSetDispCopyYScale(f32 yscale) {return 0;}
void GXSetCopyFilter(GXBool aa, const u8 sample_pattern [12] [2], GXBool vf, const u8 vfilter [7]) {}
void GXSetDispCopyGamma(GXGamma gamma) {}
void GXClearBoundingBox() {}
void GXReadBoundingBox(u16 *left, u16 *top, u16 *right, u16 *bottom) {}

void GXInitFifoBase(
	GXFifoObj*  fifo,
    void*       base,
    u32         size) {}
void GXInitFifoPtrs(
	GXFifoObj*  fifo,
    void*       readPtr,
    void*       writePtr) {}
void GXInitFifoLimits(
    GXFifoObj*  fifo,
    u32         hiWaterMark,
    u32         loWaterMark) {}
    
void GXSetCPUFifo(GXFifoObj *fifo) {}
void GXSetGPFifo(GXFifoObj *fifo) {}

GXFifoObj* GXGetCPUFifo() {return 0;}
GXFifoObj* GXGetGPFifo() {return 0;}

void GXSaveCPUFifo(GXFifoObj* fifo) {}
void GXGetGPStatus(
    GXBool* overhi,
    GXBool* underlow,
    GXBool* readIdle,
    GXBool* cmdIdle,
    GXBool* brkpt) {}
void GXGetFifoStatus(
    GXFifoObj*  fifo,
    GXBool*     overhi,
    GXBool*     underlow,
    u32*        fifoCount,
    GXBool*     cpu_write,
    GXBool*     gp_read,
    GXBool*     fifowrap) {}
    
void GXGetFifoPtrs(
    GXFifoObj*  fifo,
    void**      readPtr,
    void**      writePtr) {}
void* _GXRedirectWriteGatherPipe(void* ptr) {return 0;}
void GXRestoreWriteGatherPipe() {}
void* GXGetFifoBase(const GXFifoObj* fifo) {return 0;}
u32 GXGetFifoSize(const GXFifoObj* fifo) {return 0;}
void GXGetFifoLimits(const GXFifoObj* fifo, u32* hi, u32* lo) {}
void GXEnableBreakPt(void* breakPtr) {}
void GXDisableBreakPt() {}
u32 GXGetOverflowCount() {return 0;}
u32 GXResetOverflowCount() {return 0;}
void GXSetGPMetric(GXPerf0 perf0, GXPerf1 perf1) {}
void GXClearGPMetric() {}
void GXReadGPMetric(u32* cnt0, u32* cnt1) {
	*cnt0 = 0;
	*cnt1 = 0;
}
u32 GXReadGP0Metric() {return 0;}
u32 GXReadGP1Metric() {return 0;}


void GXReadMemMetric(
	u32*           cp_req,
	u32*           tc_req,
	u32*           cpu_rd_req,
	u32*           cpu_wr_req,
	u32*           dsp_req,
	u32*           io_req,
	u32*           vi_req,
	u32*           pe_req,
	u32*           rf_req,
	u32*           fi_req) {
	*cp_req = 0;
	*tc_req = 0;
	*cpu_rd_req = 0;
	*cpu_wr_req = 0;
	*dsp_req = 0;
	*io_req = 0;
	*vi_req = 0;
	*pe_req = 0;
	*rf_req = 0;
	*fi_req = 0;
}

void GXClearMemMetric() {}
void GXReadPixMetric(     u32*          top_pixels_in,
                          u32*          top_pixels_out,
                          u32*          bot_pixels_in,
                          u32*          bot_pixels_out,
                          u32*          clr_pixels_in,
                          u32*          copy_clks) {}
void GXClearPixMetric() {}
void GXSetVCacheMetric(GXVCachePerf attr) {}
void GXReadVCacheMetric(  u32*          check,
                          u32*          miss,
                          u32*          stall ) {}
void GXClearVCacheMetric() {}
void GXInitXfRasMetric() {}
void GXReadXfRasMetric(   u32*          xf_wait_in,
                          u32*          xf_wait_out,
                          u32*          ras_busy,
                          u32*          clocks ) {}
void GXPokeAlphaMode(GXCompare func, u8 threshold) {}
void GXPokeAlphaRead(GXAlphaReadMode mode) {}
void GXPokeAlphaUpdate(GXBool update_enable) {}
void GXPokeBlendMode(GXBlendMode type, GXBlendFactor src_factor,
                          GXBlendFactor dst_factor, GXLogicOp op) {}
void GXPokeColorUpdate(GXBool update_enable) {}
void GXPokeDstAlpha(GXBool enable, u8 alpha) {}
void GXPokeDither(GXBool dither) {}
void GXPokeZMode(GXBool compare_enable, GXCompare func, GXBool update_enable) {}
void GXPeekARGB(u16 x, u16 y, u32* color) {}
void GXPokeARGB(u16 x, u16 y, u32 color) {}
void GXPeekZ(u16 x, u16 y, u32* z) {}
void GXPokeZ(u16 x, u16 y, u32 z) {}
void GXAbortFrame() {}
GXDrawSyncCallback GXSetDrawSyncCallback(GXDrawSyncCallback f) {return 0;}
void GXSetDrawDone() {}
void GXWaitDrawDone() {}
GXDrawDoneCallback GXSetDrawDoneCallback(GXDrawDoneCallback f) {return 0;}
void GXPixModeSync() {}



void GXTexModeSync() {}
void GXSetVerifyLevel(GXWarningLevel level) {}
GXVerifyCallback GXSetVerifyCallback(GXVerifyCallback f) {return 0;}

void GXSetTevSwapMode(
	GXTevStageID    stage,
	GXTevSwapSel    ras_sel,
	GXTevSwapSel    tex_sel) {
	
}

