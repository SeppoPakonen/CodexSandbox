#include "DolphinRE.h"

ushort		VirtualTlut[2080];
bool		EfbAlpha;
byte		TileSizeTable[0x10];
UnpackFunc	UnpackFuncTable[0x10];
PackFunc	PackFuncTable[0x10];



void TexConvertAndLoadTexture(GXTexObj *obj) {
	uint buf_size = TexGetGLBufferSize(obj->width, obj->height, obj->format, obj->mipmap);
	
	GXImage img;
	img.data = (byte *)malloc(buf_size);
	if (img.data == (byte *)0x0) {
		OSReport("Emulator internal:\n");
		OSReport("Texture conversion buffer allocation failed.\n");
		OSReport("Width=%d  Height=%d\n", obj->width, obj->height);
		OSReport("Size = %d\n", buf_size);
		PANIC("null ptr");
	}
	memset(img.data, 0, buf_size);
	img.width = obj->width;
	img.height = obj->height;
	img.stride = 4;
	
	if (obj->mipmap == false)
		TexConvertMain(obj, &img);
	else
		TexConvertMainMipmap(obj, &img);
	
	int offset = 0;
	int w = obj->width;
	int h = obj->height;
	for(int i = 0;; i++) {
		byte* pixels = img.data + offset;
		uint h_top = TexRoundUpSize(h);
		uint w_top = TexRoundUpSize(w);
		TexImage2D(i, INF_4, w_top, h_top, 0, PF_RGBA, PT_UNSIGNED_BYTE, img.data + offset);
		
		if ((w == 1 && h == 1) || !obj->mipmap)
			break;
		
		offset += w * h * 4;
		
		if (w < 2)
			w = 1;
		else
			w = w >> 1;
		
		if (h < 2)
			h = 1;
		else
			h = h >> 1;
		
	}
	free(img.data);
}


uint TexGetGLBufferSize(uint width, uint height, GXTexFmt fmt, bool mipmap) {
	int ret = 0;
	uint w = TexRoundUpSize(width);
	uint h = TexRoundUpSize(height);
	
	if (!mipmap)
		ret = w * h * 4;
	else {
		w <<= 1;
		h <<= 1;
		
		do {
			if (w < 2)
				w = 1;
			else
				w >>= 1;
			
			if (h < 2)
				h = 1;
			else
				h >>= 1;
			
			ret += w * h * 4;
		}
		while (1 < w || 1 < h);
	}
	
	return ret;
}



void TexLoadTlut(ushort *data, u32 idx, ushort count, uint mem_off, GXTlutFmt tlut_fmt) {
	ushort *offset = data;
	GXTlutRegion *region = CurrentTlutRegionCallback(idx);
	
	if (region == 0) {
		PANIC("GXLoadTlut: Invalid TLUT id");
	}
	
	uint vmem_addr = (((size_t)region->tmem_addr - 0xc0000U) >> 5) + (mem_off & 0xffff);
	uint trans_tlut_sz = TranslateGXTlutSize(region->tlut_size);
	
	region->tlut_fmt = tlut_fmt;
	
	if (((mem_off & 0xffff) < trans_tlut_sz) && ((mem_off & 0xffff) + count <= trans_tlut_sz)) {
		while (count != 0) {
			VirtualTlut[vmem_addr] = *offset;
			vmem_addr++;
			offset++;
			count--;
		}
	}
}


void TexCopyFBImage(byte *dst, int x_orig, int y_orig, uint width, uint height, GXTexFmt fmt, bool is_bitfield) {
	EfbAlpha = CurrentPixFmt == GX_PF_RGBA6_Z24;
	bool is_zfmt = TexIsZFmt(fmt);
	
	if (is_zfmt == false) {
		if (is_bitfield == false)
			TexCopyFBImageMain(dst, x_orig, y_orig, width, height, fmt);
		else
			TexCopyFBImageMainBF(dst, x_orig, y_orig, width, height, fmt);
	}
	else {
		ASSERTMSG(fmt == GX_TF_Z24X8, "GXCopyTex: Only GX_TF_Z24X8 is available for Z copy at HW1");
		
		if (is_bitfield == false)
			TexCopyFBImageMainZ(dst, x_orig, y_orig, width, height, fmt);
		else
			TexCopyFBImageMainZB(dst, x_orig, y_orig, width, height, fmt);
	}
}


bool TexIsZFmt(GXTexFmt fmt) {
	return fmt == GX_TF_Z16 || fmt == GX_TF_Z16 || fmt == GX_TF_Z24X8;
}


uint TexRoundUpSize(uint size) {
	uint ret = 1;
	while (ret < size)
		ret <<= 1;
	return ret;
}


void TexUnpackTileC4(byte *src, GXImage *dst, GXTexObj *obj) {
	int x, y, h, w;
	GXColor tmp;
	
	byte *src_it = src;
	GXColor *dst_it = (GXColor *)dst->data;
	y = 0;
	while (true) {
		h = TexTileHeight(GX_TF_C4);
		if (h <= y)
			break;
		x = 0;
		while (true) {
			w = TexTileWidth(GX_TF_C4);
			if (w <= x)
				break;
			if (x % 2 == 0)
				TexLookUpTlut(tmp, obj->tlut_name, *src_it >> 4);
			else {
				TexLookUpTlut(tmp, obj->tlut_name, *src_it & 0xF);
				src_it++;
			}
			dst_it->r = tmp.r;
			dst_it->g = tmp.g;
			dst_it->b = tmp.b;
			dst_it->a = tmp.a;
			dst_it++;
			x++;
		}
		y++;
	}
}


uint TexTileWidth(GXTexFmt fmt) {
	int i = TranslateGXTexFormat(fmt);
	ASSERT(i >= 0);
	int sz = TileSizeTable[i];
	return sz;
}


uint TexTileHeight(GXTexFmt fmt) {
	int i = TranslateGXTexFormat(fmt);
	ASSERT(i >= 0);
	uint ret = TileSizeTable[i];
	return ret;
}


void TexLookUpTlut(GXColor clr, u32 region_idx, u32 tlut) {
	GXTlutRegion *region = CurrentTlutRegionCallback(region_idx);
	ASSERTMSG(region, "GXLoadTexObj: Invalid TLUT id");
	
	void *tmem_addr = (void*)(((size_t)region->tmem_addr - 0xc0000U) >> 5);
	uint tlut_size = TranslateGXTlutSize(region->tlut_size);
	
	tmem_addr = (void*)((size_t)tmem_addr + (tlut & 0xffff) % tlut_size);
	GXTlutFmt tlut_fmt = region->tlut_fmt;
	
	if (tlut_fmt == GX_TL_IA8) {
		clr.r = clr.g;
		clr.g = clr.b;
		clr.b = (char)VirtualTlut[(size_t)tmem_addr];
		clr.a = (char)(VirtualTlut[(size_t)tmem_addr] >> 8);
	}
	else {
		if (tlut_fmt == GX_TL_RGB5A3) {
			if ((VirtualTlut[(size_t)tmem_addr] & 0x8000) == 0) {
				clr.r = ((((uint)VirtualTlut[(size_t)tmem_addr] >>  8 & 0xfU) * 0xff) / 0xf);
				clr.g = ((((uint)VirtualTlut[(size_t)tmem_addr] >>  4 & 0xfU) * 0xff) / 0xf);
				clr.b = ((((uint)VirtualTlut[(size_t)tmem_addr]       & 0xfU) * 0xff) / 0xf);
				clr.a = ((((uint)VirtualTlut[(size_t)tmem_addr] >> 12 & 7U)   * 0xff) / 7);
			}
			else {
				clr.r = ((((uint)VirtualTlut[(size_t)tmem_addr] >> 10 & 0x1fU) * 0xff) / 0x1f);
				clr.g = ((((uint)VirtualTlut[(size_t)tmem_addr] >>  5 & 0x1fU) * 0xff) / 0x1f);
				clr.b = ((((uint)VirtualTlut[(size_t)tmem_addr]       & 0x1fU) * 0xff) / 0x1f);
				clr.a = 0xff;
			}
		} else {
			clr.r = ((((uint)VirtualTlut[(size_t)tmem_addr] >> 11        ) * 0xff) / 0x1f);
			clr.g = ((((uint)VirtualTlut[(size_t)tmem_addr] >> 5  & 0x3fU) * 0xff) / 0x3f);
			clr.b = ((((uint)VirtualTlut[(size_t)tmem_addr]       & 0x1fU) * 0xff) / 0x1f);
			clr.a = 0xff;
		}
	}
}


void TexUnpackTileC8(byte *src, GXImage *dst, GXTexObj *obj) {
	uint x, y, w, h;
	GXColor tmp;
	byte *src_it = src;
	GXColor *dst_it = (GXColor *)dst->data;
	
	x = 0;
	while (true) {
		h = TexTileHeight(GX_TF_C8);
		if (h <= x)
			break;
		y = 0;
		while (true) {
			w = TexTileWidth(GX_TF_C8);
			if (w <= y)
				break;
			TexLookUpTlut(tmp, obj->tlut_name, w & 0xffff0000 | (uint)*src_it);
			src_it++;
			dst_it->r = tmp.r;
			dst_it->g = tmp.g;
			dst_it->b = tmp.b;
			dst_it->a = tmp.a;
			dst_it++;
			y++;
		}
		x++;
	}
}


void TexUnpackTileC14X2(byte *src, GXImage *dst, GXTexObj *obj) {
	uint x, y, w, h;
	GXColor tmp;
	ushort *src_it = (ushort *)src;
	GXColor *dst_it = (GXColor *)dst->data;
	
	y = 0;
	while (true) {
		h = TexTileHeight(GX_TF_C14X2);
		if (h <= y)
			break;
		x = 0;
		while (true) {
			w = TexTileWidth(GX_TF_C14X2);
			if (w <= x)
				break;
			TexLookUpTlut(tmp, obj->tlut_name, (uint)*src_it & 0x3fff);
			src_it++;
			dst_it->r = tmp.r;
			dst_it->g = tmp.g;
			dst_it->b = tmp.b;
			dst_it->a = tmp.a;
			dst_it++;
			x++;
		}
		y++;
	}
}


void TexUnpackTileI4(byte *src, GXImage *dst, GXTexObj *obj) {
	byte value;
	uint i;
	byte *src_it = src;
	byte *dst_it = dst->data;
	i = 0;
	while (i < dst->height * dst->width) {
		byte value = ((((int)(uint) * src_it >> 4) * 0xff) / 0xf);
		*dst_it = value;
		dst_it[1] = value;
		dst_it[2] = value;
		dst_it[3] = value;
		
		value = (byte)((int)(((uint) * src_it & 0xf) * 0xff) / 0xf);
		dst_it[4] = value;
		dst_it[5] = value;
		dst_it[6] = value;
		dst_it[7] = value;
		
		dst_it += 8;
		src_it++;
		i++;
	}
}


void TexUnpackTileI8(byte *src, GXImage *dst, GXTexObj *obj) {
	byte *src_it = src;
	byte *dst_it = dst->data;
	int size = dst->height * dst->width;
	for(int i = 0; i < size; i++) {
		byte value = *src_it;
		
		dst_it[0] = value;
		dst_it[1] = value;
		dst_it[2] = value;
		dst_it[3] = value;
		
		dst_it += 4;
		src_it++;
	}
}


void TexUnpackTileIA4(byte *src, GXImage *dst, GXTexObj *obj) {
	byte value;
	uint i;
	byte *src_it = src;
	byte *dst_it = dst->data;
	int sz = dst->height * dst->width;
	for(int i = 0; i < sz; i++) {
		byte value = (((uint)*src_it & 0xf) * 0xff) / 0xf;
		
		dst_it[0] = value;
		dst_it[1] = value;
		dst_it[2] = value;
		dst_it[3] = (((uint)*src_it >> 4) * 0xff) / 0xf;
		
		dst_it += 4;
		src_it++;
	}
}


void TexUnpackTileIA8(byte *src, GXImage *dst, GXTexObj *obj) {
	byte *src_it = src;
	byte *dst_it = dst->data;
	int sz = dst->height * dst->width;
	for(int i = 0; i < sz; i++) {
		byte value = *src_it;
		byte new_value = *(src_it + 1);
		
		dst_it[0] = new_value;
		dst_it[1] = new_value;
		dst_it[2] = new_value;
		dst_it[3] = value;
		
		dst_it += 4;
		src_it += 2;
	}
}


void TexUnpackTileRGB565(byte *src, GXImage *dst, GXTexObj *obj) {
	byte *src_it = src;
	byte *dst_it = dst->data;
	int sz = dst->height * dst->width;
	for(int i = 0; i < sz; i++) {
		int src_it_swap16 = EndianSwapReturn16(src_it);
		
		dst_it[0] = (((src_it_swap16 >> 11        ) * 0xff) / 0x1f);
		dst_it[1] = (((src_it_swap16 >> 5  & 0x3fU) * 0xff) / 0x3f);
		dst_it[2] = (((src_it_swap16       & 0x1fU) * 0xff) / 0x1f);
		dst_it[3] = 0xff;
		
		dst_it += 4;
		src_it += 2;
	}
}



void TexUnpackTileRGB5A3(byte *src, GXImage *dst, GXTexObj *obj) {
	byte *src_it = src;
	byte *dst_it = dst->data;
	int sz = dst->height * dst->width;
	for(int i = 0; i < sz; i++) {
		int src_it_swap16 = EndianSwapReturn16(src_it);
		
		if ((src_it_swap16 & 0x8000) == 0) {
			dst_it[0] = (((src_it_swap16 >>  8 & 0xfU) * 0xff) / 0xf);
			dst_it[1] = (((src_it_swap16 >>  4 & 0xfU) * 0xff) / 0xf);
			dst_it[2] = (((src_it_swap16       & 0xfU) * 0xff) / 0xf);
			dst_it[3] = (((src_it_swap16 >> 12 & 0x7U) * 0xff) / 0x7);
		} else {
			dst_it[0] = (((src_it_swap16 >> 10 & 0x1fU) * 0xff) / 0x1f);
			dst_it[1] = (((src_it_swap16 >>  5 & 0x1fU) * 0xff) / 0x1f);
			dst_it[2] = (((src_it_swap16       & 0x1fU) * 0xff) / 0x1f);
			dst_it[3] = 0xff;
		}
		
		dst_it += 4;
		src_it += 2;
	}
}


void TexUnpackTileRGBA8(byte *src, GXImage *dst, GXTexObj *obj) {
	byte *src_it = src;
	byte *dst_it = dst->data;
	int sz = dst->height * dst->width;
	for(int i = 0; i < sz; i++) {
		dst_it[0] = src_it[1];
		dst_it[1] = src_it[0x20];
		dst_it[2] = src_it[0x21];
		dst_it[3] = src_it[0];
		
		dst_it += 4;
		src_it += 2;
	}
}


void TexUnpackTileCmpr(byte *src, GXImage *dst, GXTexObj *obj) {
	/*byte value;
	ushort v;
	ushort v_swap;
	uint h;
	uint w;
	int useless0;
	undefined4 *useless1;
	undefined4 useless_arr [16];
	uint useless;
	uint h_mask0x11110000;
	int dst_i;
	uint local_18;
	uint i;
	uint y;
	uint x;*/
	
	uint x, y, w, h;
	GXColor clr1 [2];
	GXColor clr2 [2];
	
	y = 0;
	while (true) {
		h = TexTileHeight(GX_TF_CMPR);
		if (h <= y)
			break;
		x = 0;
		while (true) {
			w = TexTileWidth(GX_TF_CMPR);
			if (w <= x)
				break;
			
			byte *src_next = src + 4;
			for(int i = 0; i < 2; i++) {
				h = EndianSwapReturn16(src + i * 2);
				
				clr1[i].r = ((h >> 11       ) * 0xff) / 0x1f;
				clr1[i].g = ((h >>  5 & 0x3f) * 0xff) / 0x3f;
				clr1[i].b = ((h       & 0x1f) * 0xff) / 0x1f;
				clr1[i].a = 0xFF;
				
				i++;
			}
			
			uint v_swap0		= EndianSwapReturn16(src);
			uint v_swap2		= EndianSwapReturn16(src + 2);
			byte* b10 = &clr1[0].r;
			byte* b11 = &clr1[1].r;
			byte* b20 = &clr2[0].r;
			byte* b21 = &clr2[1].r;
			
			if (v_swap2 < v_swap0) {
				for(int i = 0; i < 3; i++) {
					b20[i] = ((uint)b10[i] * 5 + (uint)b11[i] * 3) >> 3;
					b21[i] = ((uint)b11[i] * 5 + (uint)b10[i] * 3) >> 3;
				}
				clr2[0].a = 0xFF;
				clr2[1].a = 0xFF;
			}
			else {
				for(int i = 0; i < 3; i++) {
					b20[i] = ((uint)b10[i] + (uint)b11[i]) / 2;
					b21[i] = ((uint)b10[i] + (uint)b11[i]) / 2;
				}
				clr2[0].a = 0xFF;
				clr2[1].a = 0;
			}
			clr2[0].a = 0XFF;
			
			for(int i = 0; i < 4; i++) {
				byte *dst_it = dst->data + (x + (y + i) * dst->width) * dst->stride;
				for(int j = 6; j >= 0; j -= 2) {
					uint value = *src_next;
					byte* b = &clr1[value >> (j & 0x1f) & 3].r;
					for(int k = 0; k < 4; k++)
						*dst_it++ = *b++;
				}
				src_next++;
			}
			src += 8;
			x += 4;
		}
		y += 4;
	}
}


void TexPackTileError(GXImage *src, byte *dst) {
	PANIC("GXCopyTex: Illigal texture format for framebuffer copy");
}


void TexPackTileI4(GXImage *src, byte *dst) {
	byte *g_;
	byte *b_;
	int iVar1;
	byte *src_it_;
	byte r_;
	
	byte *src_it = src->data;
	byte *dst_it = dst;
	int sz = src->height * src->width;
	for(int i = 0; i < sz; i += 2) {
		uint r0 = src_it[0];
		uint g0 = src_it[1];
		uint b0 = src_it[2];
		src_it += src->stride;
		
		uint r1 = src_it[0];
		uint g1 = src_it[1];
		uint b1 = src_it[2];
		src_it += src->stride;
		
		byte v0 = (r0 * 0x42 + g0 * 0x81 + 0x1080 + b0 * 0x19) >> 8;
		byte v1 = (r1 * 0x42 + g1 * 0x81 + 0x1080 + b1 * 0x19) >> 8;
		*dst_it++ = (v0 & 0xf0) | ((v1 >> 4) & 0x0f);
	}
}


void TexPackTileI8(GXImage *src, byte *dst) {
	byte *src_it = src->data;
	byte *dst_it = dst;
	int sz = src->height * src->width;
	for(int i = 0; i < sz; i++) {
		uint r = src_it[0];
		uint g = src_it[1];
		uint b = src_it[2];
		src_it += src->stride;
		
		*dst_it++ = (r * 0x42 + g * 0x81 + 0x1080 + b * 0x19) >> 8;
	}
}


void TexPackTileIA4(GXImage *src, byte *dst) {
	byte alpha;
	byte *src_it = src->data;
	byte *dst_it = dst;
	int sz = src->height * src->width;
	for(int i = 0; i < sz; i++) {
		if (EfbAlpha == false)
			alpha = 0xff;
		else
			alpha = src_it[3];
		
		int r = src_it[0];
		int g = src_it[1];
		int b = src_it[2];
		byte v = (r * 0x42 + g * 0x81 + 0x1080 + b * 0x19) >> 8;
		*dst_it = alpha & 0xf0 | (v >> 4);
		dst_it++;
		src_it += src->stride;
	}
}


void TexPackTileIA8(GXImage *src, byte *dst) {
	byte alpha;
	byte *src_it = src->data;
	byte *dst_it = dst;
	int sz = src->height * src->width;
	for(int i = 0; i < sz; i++) {
		if (EfbAlpha == false)
			alpha = 0xff;
		else
			alpha = src_it[3];
		
		int r = src_it[0];
		int g = src_it[1];
		int b = src_it[2];
		
		dst_it[0] = alpha;
		dst_it[1] = (r * 0x42 + g * 0x81 + 0x1080 + b * 0x19) >> 8;
		dst_it += 2;
		src_it += src->stride;
	}
}


void TexPackTileRGB565(GXImage *src, byte *dst) {
	byte *src_it = src->data;
	ushort *dst_it = (ushort *)dst;
	int sz = src->height * src->width;
	for(int i = 0; i < sz; i++) {
		int r = src_it[0];
		int g = src_it[1];
		int b = src_it[2];
		
		*dst_it = ((r & 0xf8) << 8) | ((g & 0xfc) << 3) | ((b & 0xf8) >> 3);
		
		EndianSwap16(dst_it);
		dst_it++;
		src_it += src->stride;
	}
}


void TexPackTileRGB5A3(GXImage *src, byte *dst) {
	byte *src_it = src->data;
	ushort *dst_it = (ushort *)dst;
	int sz = src->height * src->width;
	for(int i = 0; i < sz; i++) {
		int r = src_it[0];
		int g = src_it[1];
		int b = src_it[2];
		int alpha = EfbAlpha ? src_it[3] : 0xFF;
		
		ushort v;
		if (alpha < 0xe0)
			v = ((r & 0xf0) << 4) | g & 0xf0 | ((b & 0xf0) >> 4) | ((alpha & 0xe0) << 7);
		else
			v = ((r & 0xf8) << 7) | ((g & 0xf8) << 2) | ((b & 0xf8) >> 3) | 0x8000;
		
		*dst_it = v;
		dst_it++;
		src_it += src->stride;
	}
}


void TexPackTileRGBA8(GXImage *src, byte *dst) {
	byte alpha;
	byte *src_it = src->data;
	byte *dst_it = dst;
	int sz = src->height * src->width;
	for(int i = 0; i < sz; i++) {
		dst_it[0] = EfbAlpha ? src_it[3] : 0xFF;
		dst_it[1]    = src_it[0];
		dst_it[0x20] = src_it[1];
		dst_it[0x21] = src_it[2];
		
		src_it += 4;
		dst_it += 2;
	}
}


void TexPackTileA8(GXImage *src, byte *dst) {
	byte *src_it = src->data;
	byte *dst_it = dst;
	int sz = src->height * src->width;
	for(int i = 0; i < sz; i++) {
		dst_it[0] = EfbAlpha ? src_it[3] : 0xFF;
		dst_it++;
		src_it += src->stride;
		i++;
	}
}


void TexPackTileZ24X8(GXImage *src, byte *dst) {
	uint *src_it = (uint*)src->data;
	byte *dst_it = dst;
	
	int sz = src->height * src->width;
	for(int i = 0; i < sz; i++) {
		float f = *src_it * 1000000.0;
		uint v = f;
		dst_it[0] = 0;
		dst_it[1] = v >> 0x10;
		dst_it[0x20] = v >> 8;
		dst_it[0x21] = v;
		dst_it += 2;
		src_it += 4;
		i++;
	}
}


void TexConvertMain(GXTexObj *obj, GXImage* img) {
	byte* it = (byte *)obj->image_ptr;
	uint height = TexRoundUpSize((uint)obj->width);
	uint width = TexRoundUpSize((uint)obj->height);
	
	GXImage tmp_img;
	tmp_img.width = TexTileWidth(obj->format);
	tmp_img.height = TexTileHeight(obj->format);
	tmp_img.stride = 4;
	tmp_img.data = (byte *)malloc(tmp_img.width * tmp_img.height * 4);
	
	GXTexFmt fmt = obj->format;
	int trans_fmt = TranslateGXTexFormat(obj->format);
	ASSERT(trans_fmt >= 0);
	UnpackFunc unpack_fn = UnpackFuncTable[trans_fmt];
	
	int x, y;
	uint w, h;
	y = 0;
	while (y < obj->height) {
		x = 0;
		while (x < obj->width) {
			unpack_fn(it, &tmp_img, obj);
			
			TexPutTileBuffer(&tmp_img, img, x, y);
			
			it += fmt == GX_TF_RGBA8 ? 0x40 : 0x20;
			x += w;
		}
		y += h;
	}
	
	free(tmp_img.data);
	
	if (obj->width < width || obj->height < height)
		TexStretchBuffer(img, obj->width, obj->height);
}


void TexPutTileBuffer(GXImage *src, GXImage *dst, int x, int y) {
	int w, h;
	
	byte *src_it = src->data;
	ASSERTMSG(src->stride == dst->stride, "Emulator Internal: Different component size");
	
	
	int x_diff = dst->width - x;
	if (src->width < x_diff || src->width == x_diff)
		w = src->width;
	else
		w = dst->width - x;
	
	
	int y_diff = dst->height - y;
	if (src->height < y_diff || src->height == y_diff)
		h = src->height;
	else
		h = dst->height - y;
	
	
	u8*		dst_it = dst->data + (y * dst->width + x) * dst->stride;
	uint	dst_width  = dst->width;
	uint	dst_stride = dst->stride;
	uint	src_width  = src->width;
	uint	src_stride = src->stride;
	
	for(int y = 0; y < h; y++) {
		int row = w * src->stride;
		for(int x = 0; x < row; x++)
			*dst_it++ = *src_it++;
		src_it += (src_width - w) * src_stride;
		dst_it += (dst_width - w) * dst_stride;
	}
}


void TexStretchBuffer(GXImage *img, uint width, uint height) {
	for(int y = 0; y < height; y++) {
		byte* it = img->data + y * img->width * img->stride;
		byte* dst_it = it + img->width * img->stride - 1;
		byte* src_it = it + width * img->stride - 1;
		int x = 0;
		while (dst_it != src_it) {
			int pix_pos = 0;
			while (pix_pos < img->stride) {
				*dst_it = *src_it;
				dst_it--;
				src_it--;
				pix_pos++;
			}
			x += width - 1;
			if (img->width - 1 < x)
				x -= img->width - 1;
			else
				src_it += img->stride;
		}
	}
	
	int row_pixels = img->width * img->stride;
	
	for(int x = 0; x < img->width; x++) {
		byte* dst_it = img->data + x * img->stride + (img->height - 1) * row_pixels;
		byte* src_it = img->data + x * img->stride + (height - 1) * row_pixels;
		int y = 0;
		
		while (dst_it != src_it) {
			for(int pix_pos = 0; pix_pos < img->stride; pix_pos++)
				*dst_it++ = *src_it++;
			y += height - 1;
			
			dst_it -= row_pixels + img->stride;
			
			int shift = img->stride;
			if (img->height - 1 < y) {
				y -= img->height - 1;
				shift += row_pixels;
			}
			src_it -= shift;
		}
	}
}



void TexConvertMainMipmap(GXTexObj *obj, GXImage* img) {
	uint obj_w_div2, obj_h_div2;
	int x, y;
	
	byte *image_it = (byte *)obj->image_ptr;
	
	GXImage new_data;
	new_data.width = TexTileWidth(obj->format);
	new_data.height = TexTileHeight(obj->format);
	new_data.stride = 4;
	new_data.data = (byte *)malloc(new_data.width * new_data.height * 4);
	
	GXImage it;
	it.width = (uint)obj->width << 1;
	it.height = (uint)obj->height << 1;
	it.stride = 4;
	it.data = img->data;
	
	int obj_fmt = obj->format;
	int trans_fmt = TranslateGXTexFormat(obj->format);
	ASSERT(trans_fmt >= 0);
	UnpackFunc unpack_fn = UnpackFuncTable[trans_fmt];
	
	float min_lod_padded = obj->min_lod - 1.0;
	float max_lod_padded = obj->max_lod + 1.0;
	
	int i = -1;
	do {
		i++;
		
		if (it.width < 2)
			obj_w_div2 = 1;
		else
			obj_w_div2 = it.width >> 1;
		it.width = obj_w_div2;
		
		
		if (it.height < 2)
			obj_h_div2 = 1;
		else
			obj_h_div2 = it.height >> 1;
		it.height = obj_h_div2;
		
		
		if (i <= min_lod_padded || max_lod_padded <= i)
			it.data += obj_w_div2 * obj_h_div2 * it.stride;
		
		else {
			for(int y = 0; y < it.height; y += new_data.height) {
				for(int x = 0; x < it.width; x += new_data.width) {
					unpack_fn(image_it, &new_data, obj);
					TexPutTileBuffer(&new_data, &it, x, y);
					image_it += obj_fmt == GX_TF_RGBA8 ? 0x40 : 0x20;
				}
			}
			it.data += it.width * it.height * it.stride;
		}
	}
	while (1 < it.width && 1 < it.height);
	
	free(new_data.data);
}


void TexCopyFBImageMain(byte *dst, int x_orig, int y_orig, uint width, uint height, GXTexFmt fmt) {
	byte *dst_it = dst;
	
	GXImage src;
	src.width = TexTileWidth(fmt);
	src.height = TexTileHeight(fmt);
	src.stride = 4;
	src.data = (byte *)malloc(src.width * src.height * 4);
	
	int trans_fmt = TranslateGXTexFormat(fmt);
	ASSERT(trans_fmt >= 0);
	PackFunc pack_func = PackFuncTable[trans_fmt];
	
	PixelStore(PA_PACK_SKIP_ROWS,	0);
	PixelStore(PA_PACK_SKIP_PIXELS,	0);
	PixelStore(PA_PACK_ROW_LENGTH,	src.width);
	
	for(int y = 0; y < height; y += src.height) {
		for(int x = 0; x < width; x += src.width) {
			ReadPixels(
				x_orig + x,
				y_orig - y - src.height,
				src.width,
				src.height,
				PF_RGBA,
				PT_UNSIGNED_BYTE,
				src.data);
			 
			TexFlipBufferV(&src);
			pack_func(&src, dst_it);
			
			dst_it += fmt == GX_TF_RGBA8 ? 0x40 : 0x20;
		}
	}
	free(src.data);
}


void TexFlipBufferV(GXImage *img) {
	int w = img->width * img->stride;
	int h = img->height >> 1;
	for(int y = 0; y < h; y++) {
		byte *src = img->data + y * w;
		byte *dst = img->data + (img->height - y - 1) * w;
		for(int x = 0; x < w; x++) {
			byte value = *src;
			*src++ = *dst;
			*dst++ = value;
		}
	}
}


void TexCopyFBImageMainBF(byte *dst, int x_orig, int y_orig, uint width, uint height, GXTexFmt fmt) {
	byte *dst_it = dst;
	
	GXImage img;
	img.width = TexTileWidth(fmt) << 1;
	img.height = TexTileHeight(fmt) << 1;
	img.stride = 4;
	img.data = (byte *)malloc(img.width * img.height * 4);
	
	GXImage src;
	src.width = TexTileWidth(fmt);
	src.height = TexTileHeight(fmt);
	src.stride = 4;
	src.data = (byte *)malloc(src.width * src.height * 4);
	
	int trans_fmt = TranslateGXTexFormat(fmt);
	ASSERT(trans_fmt >= 0);
	PackFunc pack_func = PackFuncTable[trans_fmt];
	
	PixelStore(PA_PACK_SKIP_ROWS,	0);
	PixelStore(PA_PACK_SKIP_PIXELS,	0);
	PixelStore(PA_PACK_ROW_LENGTH,	img.width);
	
	for(int y = 0; y < height; y += img.height) {
		for(int x = 0; x < width; x += img.width) {
			ReadPixels(
				x_orig + x,
				(y_orig - y) - img.height,
				img.width,
				img.height,
				PF_RGBA,
				PT_UNSIGNED_BYTE,
				img.data);
			
			TexFlipBufferV(&img);
			TexBoxFilter(&img, &src);
			pack_func(&src, dst_it);
			
			dst_it += fmt == GX_TF_RGBA8 ? 0x40 : 0x20;
		}
	}
	free(img.data);
	free(src.data);
}


void TexBoxFilter(GXImage *src, GXImage *dst) {
	ASSERTMSG(src->width == dst->width * 2, "Failed assertion inBuffer->width == outBuffer->width * 2");
	ASSERTMSG(src->height == dst->height * 2, "Failed assertion inBuffer->height == outBuffer->height * 2");
	ASSERTMSG(src->stride == dst->stride, "Failed assertion inBuffer->components == outBuffer->components");
	
	uint stride = dst->stride;
	byte *dst_it = dst->data;
	for(int y = 0; y < dst->height; y++) {
		for(int x = 0; x < dst->width; x++) {
			byte* src_it = src->data + (y * src->width + x) * 2 * stride;
			for(int e = 0; e < stride; e++) {
				int v00 = src_it[0];
				int v01 = src_it[stride];
				int v10 = src_it[src->width * stride];
				int v11 = src_it[(src->width + 1) * stride];
				
				*dst_it++ = (v00 + v01 + v10 + v11) >> 2;
				src_it++;
			}
		}
	}
}


void TexCopyFBImageMainZ(byte *dst, int x_orig, int y_orig, uint width, uint height, GXTexFmt fmt) {
	byte *dst_it = dst;
	
	GXImage tile_buf;
	tile_buf.width = TexTileWidth(fmt);
	tile_buf.height = TexTileHeight(fmt);
	tile_buf.stride = 4;
	tile_buf.data = (byte *)malloc(tile_buf.width * tile_buf.height * 4);
	
	int trans_fmt = TranslateGXTexFormat(fmt);
	ASSERT(trans_fmt >= 0);
	PackFunc pack_func = PackFuncTable[trans_fmt];
	
	PixelStore(PA_PACK_SKIP_ROWS,	0);
	PixelStore(PA_PACK_SKIP_PIXELS,	0);
	PixelStore(PA_PACK_ROW_LENGTH,	tile_buf.width);
	PixelTransfer(PTRANS_DEPTH_SCALE,	1.0);
	PixelTransfer(PTRANS_DEPTH_BIAS,	0.0);
	
	for(int y = 0; y < height; y += tile_buf.height) {
		for(int x = 0; x < width; x += tile_buf.width) {
			ReadPixels(
				x_orig + x,
				y_orig - y - tile_buf.height,
				tile_buf.width,
				tile_buf.height,
				PF_DEPTH_COMPONENT,
				PT_FLOAT,
				tile_buf.data);
			
			TexFlipBufferV(&tile_buf);
			pack_func(&tile_buf, dst_it);
			
			dst_it += fmt == GX_TF_RGBA8 ? 0x40 : 0x20;
		}
	}
	free(tile_buf.data);
}


void TexCopyFBImageMainZB(byte *dst, int x_orig, int y_orig, uint width, uint height, GXTexFmt fmt) {
	byte *dst_it = dst;
	
	GXImage tile_buf1;
	tile_buf1.width = TexTileWidth(fmt) << 1;
	tile_buf1.height = tile_buf1.height << 1;
	tile_buf1.stride = 4;
	tile_buf1.data = (byte *)malloc(tile_buf1.width * tile_buf1.height * 4);
	
	GXImage tile_buf2;
	tile_buf2.width = TexTileWidth(fmt);
	tile_buf2.height = TexTileHeight(fmt);
	tile_buf2.stride = 4;
	tile_buf2.data = (byte *)malloc(tile_buf2.width * tile_buf2.height * 4);
	
	int trans_fmt = TranslateGXTexFormat(fmt);
	ASSERT(trans_fmt >= 0);
	PackFunc pack_func = PackFuncTable[trans_fmt];
	
	PixelStore(PA_PACK_SKIP_ROWS,			0);
	PixelStore(PA_PACK_SKIP_PIXELS,			0);
	PixelStore(PA_PACK_ROW_LENGTH,			tile_buf1.width);
	PixelTransfer(PTRANS_DEPTH_SCALE,		1.0);
	PixelTransfer(PTRANS_DEPTH_BIAS,		0.0);
	
	for(int y = 0; y < height; y += tile_buf1.height) {
		for(int x = 0; x < width; x += tile_buf1.width) {
			ReadPixels(
				x_orig + x,
				y_orig - y - tile_buf1.height,
				tile_buf1.width,
				tile_buf1.height,
				PF_DEPTH_COMPONENT,
				PT_FLOAT,
				tile_buf1.data);
			
			TexFlipBufferV(&tile_buf1);
			TexBoxFilterZ(&tile_buf1, &tile_buf2);
			pack_func(&tile_buf2, dst_it);
			
			dst_it += fmt == GX_TF_RGBA8 ? 0x40 : 0x20;
		}
	}
	
	free(tile_buf1.data);
	free(tile_buf2.data); // tile_buf2 too
}


void TexBoxFilterZ(GXImage *src, GXImage *dst) {
	ASSERTMSG(src->width == dst->width * 2, "Failed assertion inBuffer->width == outBuffer->width * 2");
	ASSERTMSG(src->height == dst->height * 2, "Failed assertion inBuffer->height == outBuffer->height * 2");
	ASSERTMSG(src->stride == dst->stride, "Failed assertion inBuffer->components == outBuffer->components");
	
	uint *dst_it = (uint *)dst->data;
	for(int y = 0; y < dst->height; y++) {
		for(int x = 0; x < dst->width; x++) {
			float *src_it = (float *)(src->data + (y * src->width + x) * 8);
			*dst_it++ = (		src_it[0] +
								src_it[1] +
								src_it[src->width] +
								src_it[src->width + 1])
							/ 4.0;
		}
	}
}


