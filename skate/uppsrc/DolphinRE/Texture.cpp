#include "DolphinRE.h"

GXTexRegionCallback		CurrentTexRegionCallback;
GXTlutRegionCallback	CurrentTlutRegionCallback;
uint					TexCacheSizeCounter;
GXTexObj				TexStateArray[TEXSTATE_COUNT];
TexCacheStruct*			TexCache;


void GXInitTexObj(GXTexObj *obj, void* img, u16 width, u16 height, GXTexFmt format,
				  GXTexWrapMode wrap_s, GXTexWrapMode wrap_t, GXBool mipmap) {
	uint rounded_height, rounded_width;
	
	obj->image_ptr	= img;
	obj->width		= width;
	obj->height		= height;
	obj->format		= format;
	obj->wrap_s		= wrap_s;
	obj->wrap_t		= wrap_t;
	obj->mipmap		= mipmap;
	obj->mag_filt	= GX_LINEAR;
	
	if (mipmap == false)
		obj->min_filt = GX_LINEAR;
	else
		obj->min_filt = GX_LIN_MIP_LIN;
	
	ASSERTMSG(width <= MAX_WIDTH, "Width too large");
	ASSERTMSG(height <= MAX_HEIGHT, "Height too large");
	
	
	if (mipmap == false) {
		ASSERTMSG(wrap_s == GX_CLAMP, "GXInitTexObj: Width should be power of 2 for REPEAT/MIRROR");
		ASSERTMSG(wrap_t == GX_CLAMP, "GXInitTexObj: Height should be power of 2 for REPEAT/MIRROR");
	} else {
		rounded_width =  TexRoundUpSize(width);
		rounded_height = TexRoundUpSize(height);
		
		ASSERTMSG(width  == rounded_width,  "GXInitTexObj: Width should be power of 2 for mipmap");
		ASSERTMSG(height == rounded_height, "GXInitTexObj: Height should be power of 2 for mipmap");
	}
}


void GXInitTexObjCI(GXTexObj *obj, GXImage* img, u16 width, u16 height, GXTexFmt format,
					GXTexWrapMode wrap_s, GXTexWrapMode wrap_t, GXBool mipmap, u32 tlut_name) {
	// GXCITexFmt -> GXTexFmt change
	ASSERTMSG(format >= GX_TF_C4 && format <= GX_TF_C14X2, "Format is not GXCITexFmt");
	
	obj->image_ptr = img;
	obj->width = width;
	obj->height = height;
	obj->format = format;
	obj->wrap_s = wrap_s;
	obj->wrap_t = wrap_t;
	obj->mipmap = mipmap;
	obj->tlut_name = tlut_name;
	obj->mag_filt = GX_LINEAR;
	obj->min_filt = GX_LINEAR;
	
	ASSERTMSG(width <= MAX_WIDTH, "Width too large");
	ASSERTMSG(height <= MAX_HEIGHT, "Height too large");
	
	if (mipmap == false) {
		ASSERTMSG(wrap_s == GX_CLAMP, "GXInitTexObj: Width should be power of 2 for REPEAT/MIRROR");
		ASSERTMSG(wrap_t == GX_CLAMP, "GXInitTexObj: Height should be power of 2 for REPEAT/MIRROR");
	} else {
		uint rounded_width = TexRoundUpSize(width);
		uint rounded_height = TexRoundUpSize(height);
		
		ASSERTMSG(width  == rounded_width,  "GXInitTexObj: Width should be power of 2 for mipmap");
		ASSERTMSG(height == rounded_height, "GXInitTexObj: Height should be power of 2 for mipmap");
	}
}



void GXInitTexObjLOD(GXTexObj *obj, GXTexFilter min_filt, GXTexFilter mag_filt, f32 min_lod, f32 max_lod
		, f32 lod_bias, GXBool bias_clamp, GXBool do_edge_lod, GXAnisotropy max_aniso)
{
	obj->min_filt = min_filt;
	obj->mag_filt = mag_filt;
	obj->min_lod = min_lod;
	obj->max_lod = max_lod;
	obj->lod_bias = lod_bias;
	obj->bias_clamp = bias_clamp;
	obj->do_edge_lod = do_edge_lod;
	obj->max_aniso = max_aniso;
}


void GXInitTexObjUserData(GXTexObj *obj, void *user_data) {
	obj->user_data = user_data;
}


void GXLoadTexObj(GXTexObj *obj, GXTexMapID id) {
	char *ret_ptr;
	
	// check that ret (GXTexRegion *) is NULL
	GXTexRegion* reg = (*CurrentTexRegionCallback)(obj, id);
	ASSERTMSG(reg, "GXLoadTexObj: region allocation failed.");
	ASSERTMSG(reg->loaded, "GXLoadTexObj: bad region (not for cache) is allocated.");
	
	LoadGLTexture(obj, id);
	SetGLTexAttrs(obj);
}


void GXInitTlutObj(GXTlutObj *tlut_obj, void *lut, GXTlutFmt fmt, u16 n_entries) {
	tlut_obj->lut = lut;
	tlut_obj->entry_count = n_entries;
	tlut_obj->fmt = fmt;
}


void GXLoadTlut(GXTlutObj *tlut_obj, u32 tlut_name) {
	TexLoadTlut(
		(ushort *)tlut_obj->lut,
		tlut_name,
		tlut_obj->entry_count,
		0,
		tlut_obj->fmt);
}


void GXInitTexRegion() {
	PANIC("Warning - This GXInitTexRegion will be deleted soon.");
}


void GXInitTexCacheRegion(
	GXTexRegion *region,
	GXBool is_32b_mipmap,
	u32 tmem_even,
	GXTexCacheSize size_even,
	u32 tmem_odd,
	GXTexCacheSize size_odd) {
	uint trans_size_even = TranslateGXTexCacheSize(size_even);
	uint trans_size_odd = TranslateGXTexCacheSize(size_odd);
	
	ASSERTMSG(((size_t)tmem_even & 0x7ff) == 0, "GXInitTexCacheRegion: Alignment mismatch");
	ASSERTMSG(((size_t)tmem_odd & 0x7ff) == 0, "GXInitTexCacheRegion: Alignment mismatch");
	//ASSERTMSG(trans_size_even, "GXInitTexCacheRegion: Illigal size argument");
	//ASSERTMSG(trans_size_odd, "GXInitTexCacheRegion: Illigal size argument");
	
	region->loaded = true;
	region->image_ptr_even = tmem_even;
	region->image_ptr_odd = tmem_odd;
	region->trans_size_even = trans_size_even;
	region->trans_size_odd = trans_size_odd;
	region->is_32b_mipmap = is_32b_mipmap;
}


void GXInitTexPreLoadRegion(GXTexRegion *region, u32 tmem_even, GXTexCacheSize size_even, u32 tmem_odd, GXTexCacheSize size_odd) {
	ASSERTMSG(((size_t)tmem_even & 0x1f) == 0, "GXInitTexPreLoadRegion: Alignment mismatch");
	ASSERTMSG(((size_t)tmem_odd & 0x1f) == 0, "GXInitTexPreLoadRegion: Alignment mismatch");
	
	region->loaded = false;
	region->image_ptr_even = tmem_even;
	region->image_ptr_odd = tmem_odd;
	region->trans_size_even = size_even;
	region->trans_size_odd = size_odd;
	region->is_32b_mipmap = false;
}


void GXInitTlutRegion(GXTlutRegion *region, u32 tmem_addr, GXTlutSize tlut_size) {
	ASSERTMSG(((size_t)tmem_addr & 0x80000), "GXInitTlutRegion: Region should be allocated on high TMEM bank");
	ASSERTMSG(((size_t)tmem_addr & 0x40000), "GXInitTlutRegion: Only last half of high bank is available on this emulator");
	
	int trans_lut_size = TranslateGXTlutSize(tlut_size);
	int lot_offset = trans_lut_size * 0x20;
	
	ASSERTMSG(((size_t)tmem_addr & lot_offset - 1U) == 0, "GXInitTlutRegion: TLUT Region must be aligned to TLUT size");
	
	region->tmem_addr = tmem_addr;
	region->tlut_size = tlut_size;
}


void GXInvalidateTexRegion(GXTexRegion *region) {
	GXInvalidateTexAll();
}


void GXInvalidateTexAll() {
	ClearAllTexCacheNode();
	TexCacheSizeCounter = 0;
}


GXTexRegionCallback GXSetTexRegionCallback(GXTexRegionCallback f) {
	GXTexRegionCallback prev_cb = CurrentTexRegionCallback;
	CurrentTexRegionCallback = f;
	return prev_cb;
}


GXTlutRegionCallback GXSetTlutRegionCallback(GXTlutRegionCallback f) {
	GXTlutRegionCallback prev_cb = CurrentTlutRegionCallback;
	CurrentTlutRegionCallback = f;
	return prev_cb;
}


void GXPreLoadEntireTexture(GXTexObj *tex_obj, GXTexRegion *region) {
	ASSERTMSG(region->loaded == false, "GXPreLoadEntireTexture: this region is not available for preloading.");
	
	bool ok = CheckPreloadSize(tex_obj, region->trans_size_even, region->trans_size_odd);
	ASSERTMSG(ok, "GXPreLoadEntireTexture: data size is lager than region size");
	
	region->image_ptr = tex_obj->image_ptr;
}


void GXLoadTexObjPreLoaded(GXTexObj *obj, GXTexRegion *region, GXTexMapID id) {
	ASSERTMSG(!region->loaded, "GXLoadTexObjPreLoaded: this region is not available for preloading.");
	ASSERTMSG(obj->image_ptr == region->image_ptr, "GXLoadTexObjPreLoaded: wrong TexObj for preloaded data.");
	
	LoadGLTexture(obj, id);
	SetGLTexAttrs(obj);
}


void* GXGetTexObjData(GXTexObj *obj) {
	return obj->image_ptr;
}


u16 GXGetTexObjWidth(GXTexObj *obj) {
	return obj->width;
}


u16 GXGetTexObjHeight(GXTexObj *obj) {
	return obj->height;
}


GXTexFmt GXGetTexObjFmt(GXTexObj *obj) {
	return obj->format;
}


GXTexWrapMode GXGetTexObjWrapS(GXTexObj *obj) {
	return obj->wrap_s;
}


GXTexWrapMode GXGetTexObjWrapT(GXTexObj *obj) {
	return obj->wrap_t;
}


GXBool GXGetTexObjMipMap(GXTexObj *obj) {
	return obj->mipmap;
}


void * GXGetTexObjUserData(GXTexObj *obj) {
	return obj->user_data;
}



void GXGetTexObjAll(GXTexObj *obj, void **image_ptr, u16 *width, u16 *height, GXTexFmt *format,
					GXTexWrapMode *wrap_s, GXTexWrapMode *wrap_t, GXBool *mipmap)
{
	*image_ptr = obj->image_ptr;
	*width = obj->width;
	*height = obj->height;
	*format = obj->format;
	*wrap_s = obj->wrap_s;
	*wrap_t = obj->wrap_t;
	*mipmap = obj->mipmap;
}


void * GXGetTlutObjData(GXTlutObj *tlut_obj) {
	return tlut_obj->lut;
}


GXTlutFmt GXGetTlutObjFmt(GXTlutObj *tlut_obj) {
	return tlut_obj->fmt;
}



u16 GXGetTlutObjNumEntries(GXTlutObj *tlut_obj) {
	return tlut_obj->entry_count;
}


void GXGetTlutObjAll(GXTlutObj *tlut_obj, void **lut, GXTlutFmt *fmt, u16 *n_entries) {
	*lut = tlut_obj->lut;
	*fmt = tlut_obj->fmt;
	*n_entries = tlut_obj->entry_count;
}


void LoadGLTexture(GXTexObj *obj, GXTexMapID id) {
	int many_uses = TranslateGXTexFormat(obj->format);
	ASSERTMSG(many_uses >= 0, "GXLoadTexObj: Invalid texture format");
	
	int trans_texmap = TranslateGXTexMapID(id);
	ASSERT(trans_texmap >= 0 && trans_texmap < TEXSTATE_COUNT);
	
	TexArb tex_arb;
	if (trans_texmap < 0) {
		PANIC("GXLoadTexObj: unknown texmap ID");
	}
	else if (trans_texmap == 0) {
		tex_arb = Gfx::TEXTURE0_ARB;
	}
	else if (trans_texmap == 1) {
		tex_arb = Gfx::TEXTURE1_ARB;
	}
	else {
		const char* msg = "LoadGLTexture: invalid trans texmap index";
		OSReport(msg);
		PANIC(msg);
	}
	
	memcpy(&TexStateArray[trans_texmap], obj, sizeof(GXTexObj));
	
	Gxu node = FindTexCacheNode(obj->image_ptr, tex_arb);
	
	if (node == 0) {
		uint sz = TexGetGLBufferSize(obj->width, obj->height, obj->format, obj->mipmap);
		
		if (GX_TEXCACHE_SIZE < TexCacheSizeCounter + sz)
			GXInvalidateTexAll();
		
		TexCacheSizeCounter += sz;
		
		ActiveTexture(tex_arb);
		GenTextures(1, &node);
		BindTexture(TB_TEXTURE_2D, node);
		AddTexCacheNode(obj->image_ptr, node, tex_arb);
		TexConvertAndLoadTexture(obj);
	}
	else {
		ActiveTexture(tex_arb);
		BindTexture(TB_TEXTURE_2D, node);
	}
}
                                       

void SetGLTexAttrs(GXTexObj *obj) {
	
	if (obj->wrap_s == GX_CLAMP) {
		TexParameter(TB_TEXTURE_2D, PN_TEXTURE_WRAP_S, TW_CLAMP);
	}
	else if (obj->wrap_s == GX_REPEAT) {
		TexParameter(TB_TEXTURE_2D, PN_TEXTURE_WRAP_S, TW_REPEAT);
	}
	else if (obj->wrap_s == GX_MIRROR) {
		// Not supported
		TexParameter(TB_TEXTURE_2D, PN_TEXTURE_WRAP_S, TW_REPEAT);
	}
	else {
		PANIC("Invalid TexWrapMode");
	}
			
			
	if (obj->wrap_t == GX_CLAMP) {
		TexParameter(TB_TEXTURE_2D, PN_TEXTURE_WRAP_T, TW_CLAMP);
	}
	else if (obj->wrap_t == GX_REPEAT) {
		TexParameter(TB_TEXTURE_2D, PN_TEXTURE_WRAP_T, TW_REPEAT);
	}
	else if (obj->wrap_t == GX_MIRROR) {
		// Not supported
		TexParameter(TB_TEXTURE_2D, PN_TEXTURE_WRAP_T, TW_REPEAT);
	}
	else {
		PANIC("Invalid TexWrapMode");
	}
			
			
	if (obj->mag_filt == GX_NEAR) {
		TexParameter(TB_TEXTURE_2D, PN_TEXTURE_MAG_FILTER, MAGF_NEAREST);
	}
	else if (obj->mag_filt == GX_LINEAR) {
		TexParameter(TB_TEXTURE_2D, PN_TEXTURE_MAG_FILTER, MAGF_LINEAR);
	}
	else {
		PANIC("Invalid parameter mag_filt");
	}
	
	if (obj->min_filt == GX_NEAR) {
		TexParameter(TB_TEXTURE_2D, PN_TEXTURE_MIN_FILTER, MINF_NEAREST);
	}
	else if (obj->min_filt == GX_LINEAR) {
		TexParameter(TB_TEXTURE_2D, PN_TEXTURE_MIN_FILTER, MINF_LINEAR);
	}
	else if (obj->min_filt == GX_NEAR_MIP_NEAR) {
		TexParameter(TB_TEXTURE_2D, PN_TEXTURE_MIN_FILTER, MINF_NEAREST_MIPMAP_NEAREST);
	}
	else if (obj->min_filt == GX_LIN_MIP_NEAR) {
		TexParameter(TB_TEXTURE_2D, PN_TEXTURE_MIN_FILTER, MINF_LINEAR_MIPMAP_NEAREST);
	}
	else if (obj->min_filt == GX_NEAR_MIP_LIN) {
		TexParameter(TB_TEXTURE_2D, PN_TEXTURE_MIN_FILTER, MINF_NEAREST_MIPMAP_LINEAR);
	}
	else if (obj->min_filt == GX_LIN_MIP_LIN) {
		TexParameter(TB_TEXTURE_2D, PN_TEXTURE_MIN_FILTER, MINF_LINEAR_MIPMAP_LINEAR);
	}
	else {
		PANIC("Invalid parameter min_fil");
	}
	
	Flush();
}


bool CheckPreloadSize(GXTexObj *tex_obj, int trans_size_even, int trans_size_odd) {
	ASSERT(tex_obj->max_lod >= 0.0f);
	uint max_lod_floor = tex_obj->max_lod;
	float max_lod_frac = tex_obj->max_lod - max_lod_floor;
	
	uint j = max_lod_floor;
	if (max_lod_frac > 0.0f)
		j = max_lod_floor + 1;
	
	uint sz = 0;
	
	if (tex_obj->format == GX_TF_RGBA8) {
		sz = GXGetTexBufferSize(tex_obj->width, tex_obj->height, tex_obj->format, tex_obj->mipmap, j + 1);
	}
	else if (tex_obj->mipmap == false) {
		sz = GXGetTexBufferSize(tex_obj->width, tex_obj->height, tex_obj->format, tex_obj->mipmap, 0);
	}
	else {
		for(int k = 0; k <= j; k++) {
			sz += GXGetTexBufferSize(tex_obj->width, tex_obj->height, tex_obj->format, tex_obj->mipmap, k + 1);
		}
	}
	
	ASSERT(sz);
	return true;
}


void AddTexCacheNode(void* image_ptr, Gxu node, Gfx::TexArb activetex_tex_arb) {
	TexCacheStruct* new_next = (TexCacheStruct*)calloc(1, sizeof(TexCacheStruct));
	
	new_next->image_ptr = image_ptr;
	new_next->node = node;
	new_next->activetex_tex_arb = activetex_tex_arb;
	new_next->next = 0;
	
	if (TexCache) {
		TexCacheStruct* it = TexCache;
		
		while (it->next)
			it = it->next;
		
		it->next = new_next;
	}
	else {
		TexCache = new_next;
	}
}


Gxu FindTexCacheNode(void* image_ptr, TexArb tex_arb) {
	TexCacheStruct* it = TexCache;
	
	while (it) {
		if (it->image_ptr == image_ptr && it->activetex_tex_arb == tex_arb)
			return it->node;
		it = it->next;
	}
	
	return 0;
}


void ClearAllTexCacheNode() {
	TexCacheStruct* it = TexCache;
	
	while (it) {
		TexCacheStruct* next = it->next;
		
		ActiveTexture(it->activetex_tex_arb);
		DeleteTextures(1, &it->node);
		
		free(it);
		it = next;
	}
	
	TexCache = 0;
}


