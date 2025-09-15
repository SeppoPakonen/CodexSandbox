#pragma once


#define MAX_WIDTH			1024
#define MAX_HEIGHT			1024

#define TEXSTATE_COUNT		8
#define GX_TEXCACHE_SIZE	0x800000

/*struct CacheStruct {
    void * image_ptr;
    int node_i;
    GLenum tex_arb;
    //struct CacheStruct * sub;
};*/



extern GXTexRegionCallback CurrentTexRegionCallback;
extern GXTlutRegionCallback CurrentTlutRegionCallback;
extern uint TexCacheSizeCounter;
extern GXTexObj TexStateArray[TEXSTATE_COUNT];
extern TexCacheStruct* TexCache;


uint TexRoundUpSize(uint size);
void LoadGLTexture(GXTexObj *obj,GXTexMapID id);
void SetGLTexAttrs(GXTexObj *obj);
void TexLoadTlut(ushort *data, u32 idx, ushort count, uint mem_off, GXTlutFmt tlut_fmt);
void ClearAllTexCacheNode();
GXTexRegionCallback GXSetTexRegionCallback(GXTexRegionCallback f);
GXTlutRegionCallback GXSetTlutRegionCallback(GXTlutRegionCallback f);
bool CheckPreloadSize(GXTexObj *tex_obj, int trans_size_even, int trans_size_odd);
Gxu FindTexCacheNode(void* image_ptr, TexArb tex_arb);
uint TexGetGLBufferSize (uint width, uint height, GXTexFmt fmt, bool mipmap);
void AddTexCacheNode(void* image_ptr, Gxu deltex_tex_arb, Gfx::TexArb activetex_tex_arb);
void TexConvertAndLoadTexture(GXTexObj *obj);

/*
GXTexCacheSize TranslateGXTexCacheSize(int i);
void GXInitTexObj(GXTexObj *obj, void* image_ptr,u16 width,u16 height,GXTexFmt format,GXTexWrapMode wrap_s,GXTexWrapMode wrap_t,GXBool mipmap);
void GXInitTexObjCI(GXTexObj *obj, void* image_ptr,u16 width,u16 height, GXTexFmt format,GXTexWrapMode wrap_s,GXTexWrapMode wrap_t,GXBool mipmap,u32 tlut_name);
void GXInitTexObjLOD(GXTexObj *obj,GXTexFilter min_filt,GXTexFilter mag_filt,f32 min_lod,f32 max_lod,f32 lod_bias,GXBool bias_clamp,GXBool do_edge_lod,GXAnisotropy max_aniso);
void GXInitTexObjUserData(GXTexObj *obj,void *user_data);
void GXLoadTexObj(GXTexObj *obj,GXTexMapID id);
void GXInitTlutObj(GXTlutObj *tlut_obj,void *lut,GXTlutFmt fmt,u16 n_entries);
void GXLoadTlut(GXTlutObj *tlut_obj,u32 tlut_name);
void GXInitTexRegion(void);
void GXInitTexCacheRegion(GXTexRegion *region,GXBool is_32b_mipmap,void *tmem_even,GXTexCacheSize size_even,void *tmem_odd,GXTexCacheSize size_odd);
void GXInitTexPreLoadRegion(GXTexRegion *region,void *tmem_even, GXTexCacheSize size_even,void *tmem_odd, GXTexCacheSize size_odd);
void GXInvalidateTexRegion(GXTexRegion *region);
void GXInvalidateTexAll(void);
void GXPreLoadEntireTexture(GXTexObj *tex_obj,GXTexRegion *region);
void GXLoadTexObjPreLoaded(GXTexObj *obj,GXTexRegion *region,GXTexMapID id);
void * GXGetTexObjData(GXTexObj *obj);
u16 GXGetTexObjWidth(GXTexObj *obj);
u16 GXGetTexObjHeight(GXTexObj *obj);
GXTexFmt GXGetTexObjFmt(GXTexObj *obj);
GXTexWrapMode GXGetTexObjWrapS(GXTexObj *obj);
GXTexWrapMode GXGetTexObjWrapT(GXTexObj *obj);
GXBool GXGetTexObjMipMap(GXTexObj *obj);
void * GXGetTexObjUserData(GXTexObj *obj);
void GXGetTexObjAll(GXTexObj *obj,void **image_ptr,u16 *width,u16 *height,GXTexFmt *format,GXTexWrapMode *wrap_s,GXTexWrapMode *wrap_t,GXBool *mipmap);
void * GXGetTlutObjData(GXTlutObj *tlut_obj);
GXTlutFmt GXGetTlutObjFmt(GXTlutObj *tlut_obj);
u16 GXGetTlutObjNumEntries(GXTlutObj *tlut_obj);
void GXGetTlutObjAll(GXTlutObj *tlut_obj,void **lut,GXTlutFmt *fmt,u16 *n_entries);
void OSPanic(char * param_1, int param_2, char * param_3)
undefined __chkesp()
int TranslateGXTexCacheSize(int i)
undefined TexGetGLBufferSize(void)
undefined OSReport(void)
int TranslateGXTexMapID(int i)
u32 GXGetTexBufferSize(u16 width, u16 height, u32 format, GXBool mipmap, u8 max_lod)
*/
