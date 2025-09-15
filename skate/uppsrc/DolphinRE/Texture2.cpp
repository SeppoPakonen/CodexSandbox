#include "DolphinRE.h"



u32 GXGetTexBufferSize(u16 width, u16 height, GXTexFmt format, GXBool mipmap, u8 max_lod) {
	uint x_shf, y_shf;
	ASSERTMSG(0x400 >= width, "GXGetTexBufferSize");
	ASSERTMSG(0x400 >= height, "GXGetTexBufferSize");
	
	GXGetTexTileShift(format, &x_shf, &y_shf);
	
	int pixel_bytes;
	if (format == GX_TF_RGBA8 || format == GX_TF_Z24X8)
		pixel_bytes = 0x40;
	else
		pixel_bytes = 0x20;
	
	uint size = 0;
	if (mipmap == 1) {
		uint width_low_bits = BitScanForward(width);
		uint width_pow2 = 1 << (int)(0x1f - width_low_bits & 0x1f);
		ASSERTMSG(width == width_pow2, "%s: width must be a power of 2");
		
		uint height_low_bits = BitScanForward(height);
		uint height_pow2 = 1 << (int)(0x1fU - height_low_bits & 0x1f);
		ASSERTMSG(height == height_pow2, "s_%s:_height_must_be_a_power_of_2");
		
		for(int lod = 0; lod < max_lod; lod++) {
			width_pow2  = (uint)(width  - 1 + (1 << (x_shf & 0x1f))) >> (x_shf & 0x1f);
			height_pow2 = (uint)(height - 1 + (1 << (y_shf & 0x1f))) >> (y_shf & 0x1f);
			
			size += width_pow2 * height_pow2 * pixel_bytes;
			
			if (width == 1 && height == 1)
				break;
			
			if (width < 2)		width = 1;
			else				width >>= 1;
			
			if (height < 2)		height = 1;
			else				height >>= 1;
			
			lod++;
		}
	} else {
		uint width_pow2  = (int)(width  - 1 + (1 << (x_shf & 0x1f))) >> (x_shf & 0x1f);
		uint height_pow2 = (int)(height - 1 + (1 << (y_shf & 0x1f))) >> (y_shf & 0x1f);
		
		size = width_pow2 * height_pow2 * pixel_bytes;
	}
	
	return size;
}


void GXGetTexTileShift(GXTexFmt fmt, uint *x_shift, uint *y_shift) {
	switch (fmt) {
	case GX_TF_I4:
	case GX_TF_C4:
	case GX_TF_CMPR:
		*x_shift = 3;
		*y_shift = 3;
		break;
	case GX_TF_I8:
	case GX_TF_IA4:
	case GX_TF_C8:
	case GX_TF_Z16:
		
	#if GX_REV != 1
	case 0x21:
	#endif
	
		*x_shift = 3;
		*y_shift = 2;
		break;
	case GX_TF_IA8:
	case GX_TF_RGB565:
	case GX_TF_RGB5A3:
	case GX_TF_RGBA8:
	case GX_TF_C14X2:
	//case 0x13:
	case 0x16:
		*x_shift = 2;
		*y_shift = 2;
		break;
	default:
		*y_shift = 0;
		*x_shift = 0;
		PANIC("invalid texture format");
	}
}


