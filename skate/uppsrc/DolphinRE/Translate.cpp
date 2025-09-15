#include "DolphinRE.h"



int TranslateGXAttr_VCD(GXAttr attr) {
	int ret;
	
	switch (attr) {
	case GX_VA_PNMTXIDX:
		ret = 0;
		break;
	case GX_VA_TEX0MTXIDX:
		ret = 1;
		break;
	case GX_VA_TEX1MTXIDX:
		ret = 2;
		break;
	case GX_VA_TEX2MTXIDX:
		ret = 3;
		break;
	case GX_VA_TEX3MTXIDX:
		ret = 4;
		break;
	case GX_VA_TEX4MTXIDX:
		ret = 5;
		break;
	case GX_VA_TEX5MTXIDX:
		ret = 6;
		break;
	case GX_VA_TEX6MTXIDX:
		ret = 7;
		break;
	case GX_VA_TEX7MTXIDX:
		ret = 8;
		break;
	case GX_VA_POS:
		ret = 9;
		break;
	case GX_VA_NRM:
		ret = 0xA;
		break;
	case GX_VA_NBT:
		ret = 0xB;
		break;
	case GX_VA_CLR0:
		ret = 0xC;
		break;
	case GX_VA_CLR1:
		ret = 0xD;
		break;
	case GX_VA_TEX0:
		ret = 0xE;
		break;
	case GX_VA_TEX1:
		ret = 0xF;
		break;
	case GX_VA_TEX2:
		ret = 0x10;
		break;
	case GX_VA_TEX3:
		ret = 0x11;
		break;
	case GX_VA_TEX4:
		ret = 0x12;
		break;
	case GX_VA_TEX5:
		ret = 0x13;
		break;
	case GX_VA_TEX6:
		ret = 0x14;
		break;
	case GX_VA_TEX7:
		ret = 0x15;
		break;
	default:
		ret = -1;
		break;
	}
	return ret;
}


GXAttr UntranslateGXAttr_VCD(int vcd) {
	GXAttr ret;
	
	switch (vcd) {
	case 0:
		ret = GX_VA_PNMTXIDX;
		break;
	case 1:
		ret = GX_VA_TEX0MTXIDX;
		break;
	case 2:
		ret = GX_VA_TEX1MTXIDX;
		break;
	case 3:
		ret = GX_VA_TEX2MTXIDX;
		break;
	case 4:
		ret = GX_VA_TEX3MTXIDX;
		break;
	case 5:
		ret = GX_VA_TEX4MTXIDX;
		break;
	case 6:
		ret = GX_VA_TEX5MTXIDX;
		break;
	case 7:
		ret = GX_VA_TEX6MTXIDX;
		break;
	case 8:
		ret = GX_VA_TEX7MTXIDX;
		break;
	case 9:
		ret = GX_VA_POS;
		break;
	case 10:
		ret = GX_VA_NRM;
		break;
	case 0xb:
		ret = GX_VA_NBT;
		break;
	case 0xc:
		ret = GX_VA_CLR0;
		break;
	case 0xd:
		ret = GX_VA_CLR1;
		break;
	case 0xe:
		ret = GX_VA_TEX0;
		break;
	case 0xf:
		ret = GX_VA_TEX1;
		break;
	case 0x10:
		ret = GX_VA_TEX2;
		break;
	case 0x11:
		ret = GX_VA_TEX3;
		break;
	case 0x12:
		ret = GX_VA_TEX4;
		break;
	case 0x13:
		ret = GX_VA_TEX5;
		break;
	case 0x14:
		ret = GX_VA_TEX6;
		break;
	case 0x15:
		ret = GX_VA_TEX7;
		break;
	default:
		ret = GX_VA_NULL;
	}
	return ret;
}



int UntranslateApiID(int i) {
	int ret;
	uint high = (uint)i  & 0xFFFFFF00;
	#undef COMBINE
	#define COMBINE(a, b) (high | (uint)b)
	switch (i) {
	case 0:
		ret = COMBINE(high, 1);
		break;
	case 1:
		ret = COMBINE(high, 2);
		break;
	case 2:
		ret = COMBINE(high, 2);
		break;
	case 3:
		ret = COMBINE(high, 2);
		break;
	case 4:
		ret = COMBINE(high, 2);
		break;
	case 5:
		ret = COMBINE(high, 2);
		break;
	case 6:
		ret = COMBINE(high, 2);
		break;
	case 7:
		ret = COMBINE(high, 2);
		break;
	case 8:
		ret = COMBINE(high, 2);
		break;
	case 9:
		ret = COMBINE(high, 3);
		break;
	case 10:
		ret = COMBINE(high, 4);
		break;
	case 0xb:
		ret = COMBINE(high, 4);
		break;
	case 0xc:
		ret = COMBINE(high, 5);
		break;
	case 0xd:
		ret = COMBINE(high, 5);
		break;
	case 0xe:
		ret = COMBINE(high, 6);
		break;
	case 0xf:
		ret = COMBINE(high, 6);
		break;
	case 0x10:
		ret = COMBINE(high, 6);
		break;
	case 0x11:
		ret = COMBINE(high, 6);
		break;
	case 0x12:
		ret = COMBINE(high, 6);
		break;
	case 0x13:
		ret = COMBINE(high, 6);
		break;
	case 0x14:
		ret = COMBINE(high, 6);
		break;
	case 0x15:
		ret = COMBINE(high, 6);
		break;
	default:
		ret = high;
	}
	#undef COMBINE
	return ret;
}


int TranslateGXAttr_VAT(GXAttr attr) {
	int ret;
	switch (attr) {
	case GX_VA_POS:
		ret = 0;
		break;
	case GX_VA_NRM:
		ret = 1;
		break;
	case GX_VA_CLR0:
		ret = 2;
		break;
	case GX_VA_CLR1:
		ret = 3;
		break;
	case GX_VA_TEX0:
		ret = 4;
		break;
	case GX_VA_TEX1:
		ret = 5;
		break;
	case GX_VA_TEX2:
		ret = 6;
		break;
	case GX_VA_TEX3:
		ret = 7;
		break;
	case GX_VA_TEX4:
		ret = 8;
		break;
	case GX_VA_TEX5:
		ret = 9;
		break;
	case GX_VA_TEX6:
		ret = 10;
		break;
	case GX_VA_TEX7:
		ret = 0xb;
		break;
	case GX_VA_NBT:
		ret = 1;
		break;
	default:
		ret = -1;
		break;
	}
	return ret;
}


GXAttr UntranslateGXAttr_VAT(int vat) {
	GXAttr ret;
	switch (vat) {
	case 0:
		ret = GX_VA_POS;
		break;
	case 1:
		ret = GX_VA_NRM;
		break;
	case 2:
		ret = GX_VA_CLR0;
		break;
	case 3:
		ret = GX_VA_CLR1;
		break;
	case 4:
		ret = GX_VA_TEX0;
		break;
	case 5:
		ret = GX_VA_TEX1;
		break;
	case 6:
		ret = GX_VA_TEX2;
		break;
	case 7:
		ret = GX_VA_TEX3;
		break;
	case 8:
		ret = GX_VA_TEX4;
		break;
	case 9:
		ret = GX_VA_TEX5;
		break;
	case 10:
		ret = GX_VA_TEX6;
		break;
	case 0xb:
		ret = GX_VA_TEX7;
		break;
	default:
		ret = GX_VA_NULL;
	}
	return ret;
}


int TranslateGXAttr_Array(GXAttr attr) {
	int ret;
	switch (attr) {
	case GX_VA_POS:
		ret = 0;
		break;
	case GX_VA_NRM:
		ret = 1;
		break;
	case GX_VA_CLR0:
		ret = 2;
		break;
	case GX_VA_CLR1:
		ret = 3;
		break;
	case GX_VA_TEX0:
		ret = 4;
		break;
	case GX_VA_TEX1:
		ret = 5;
		break;
	case GX_VA_TEX2:
		ret = 6;
		break;
	case GX_VA_TEX3:
		ret = 7;
		break;
	case GX_VA_TEX4:
		ret = 8;
		break;
	case GX_VA_TEX5:
		ret = 9;
		break;
	case GX_VA_TEX6:
		ret = 10;
		break;
	case GX_VA_TEX7:
		ret = 0xb;
		break;
	case GX_POS_MTX_ARRAY:
		ret = 0xc;
		break;
	case GX_NRM_MTX_ARRAY:
		ret = 0xd;
		break;
	case GX_TEX_MTX_ARRAY:
		ret = 0xe;
		break;
	case GX_LIGHT_ARRAY:
		ret = 0xf;
		break;
	case GX_VA_NBT:
		ret = 1;
		break;
	default:
		ret = -1;
	}
	return ret;
}


int TranslateGXAttr_TexMtxIndex(GXAttr attr) {
	int ret;
	switch (attr) {
	case GX_VA_TEX0MTXIDX:
		ret = 0;
		break;
	case GX_VA_TEX1MTXIDX:
		ret = 1;
		break;
	case GX_VA_TEX2MTXIDX:
		ret = 2;
		break;
	case GX_VA_TEX3MTXIDX:
		ret = 3;
		break;
	case GX_VA_TEX4MTXIDX:
		ret = 4;
		break;
	case GX_VA_TEX5MTXIDX:
		ret = 5;
		break;
	case GX_VA_TEX6MTXIDX:
		ret = 6;
		break;
	case GX_VA_TEX7MTXIDX:
		ret = 7;
		break;
	default:
		ret = 0;
	}
	return ret;
}


GXAttr UntranslateGXAttr_TexMtxIndex(int i) {
	GXAttr ret;
	switch (i) {
	case 0:
		ret = GX_VA_TEX0MTXIDX;
		break;
	case 1:
		ret = GX_VA_TEX1MTXIDX;
		break;
	case 2:
		ret = GX_VA_TEX2MTXIDX;
		break;
	case 3:
		ret = GX_VA_TEX3MTXIDX;
		break;
	case 4:
		ret = GX_VA_TEX4MTXIDX;
		break;
	case 5:
		ret = GX_VA_TEX5MTXIDX;
		break;
	case 6:
		ret = GX_VA_TEX6MTXIDX;
		break;
	case 7:
		ret = GX_VA_TEX7MTXIDX;
		break;
	default:
		ret = GX_VA_PNMTXIDX;
	}
	return ret;
}


int TranslateGXVtxFmt(GXVtxFmt fmt) {
	int ret;
	switch (fmt) {
	case GX_VTXFMT0:
		ret = 0;
		break;
	case GX_VTXFMT1:
		ret = 1;
		break;
	case GX_VTXFMT2:
		ret = 2;
		break;
	case GX_VTXFMT3:
		ret = 3;
		break;
	case GX_VTXFMT4:
		ret = 4;
		break;
	case GX_VTXFMT5:
		ret = 5;
		break;
	case GX_VTXFMT6:
		ret = 6;
		break;
	case GX_VTXFMT7:
		ret = 7;
		break;
	default:
		ret = -1;
	}
	return ret;
}


int TranslateGXLightID(GXLightID i) {
	int ret;
	switch (i) {
	case GX_LIGHT0:
		ret = 0;
		break;
	case GX_LIGHT1:
		ret = 1;
		break;
	case GX_LIGHT2:
		ret = 2;
		break;
	case GX_LIGHT3:
		ret = 3;
		break;
	case GX_LIGHT4:
		ret = 4;
		break;
	case GX_LIGHT5:
		ret = 5;
		break;
	case GX_LIGHT6:
		ret = 6;
		break;
	case GX_LIGHT7:
		ret = 7;
		break;
	default:
		ret = -1;
	}
	return ret;
}


byte UntranslateGXLightID(GXLightID lid) {
	byte ret;
	switch ((int)lid) {
	case GX_LIGHT_NULL:
		ret = 1;
		break;
	case GX_LIGHT0:
		ret = 2;
		break;
	case GX_LIGHT1:
		ret = 4;
		break;
	case GX_LIGHT0 | GX_LIGHT1:
		ret = 8;
		break;
	case GX_LIGHT2:
		ret = 0x10;
		break;
	case GX_LIGHT0 | GX_LIGHT2:
		ret = 0x20;
		break;
	case GX_LIGHT1 | GX_LIGHT2:
		ret = 0x40;
		break;
	case GX_LIGHT0 | GX_LIGHT1 | GX_LIGHT2:
		ret = 0x80;
		break;
	default:
		ret = 0;
	}
	return ret;
}


int TranslateGXChannelID(GXChannelID chan) {
	int ret;
	switch (chan) {
	case GX_COLOR0:
		ret = 0;
		break;
	case GX_COLOR1:
		ret = 1;
		break;
	case GX_ALPHA0:
		ret = 2;
		break;
	case GX_ALPHA1:
		ret = 3;
		break;
	case GX_COLOR0A0:
		ret = 4;
		break;
	case GX_COLOR1A1:
		ret = 5;
		break;
	default:
		ret = -1;
	}
	return ret;
}


int TranslateGXTexMapID(GXTexMapID map) {
	int ret;
	switch (map) {
	case GX_TEXMAP0:
		ret = 0;
		break;
	case GX_TEXMAP1:
		ret = 1;
		break;
	case GX_TEXMAP2:
		ret = 2;
		break;
	case GX_TEXMAP3:
		ret = 3;
		break;
	case GX_TEXMAP4:
		ret = 4;
		break;
	case GX_TEXMAP5:
		ret = 5;
		break;
	case GX_TEXMAP6:
		ret = 6;
		break;
	case GX_TEXMAP7:
		ret = 7;
		break;
	default:
		ret = -1;
	}
	return ret;
}


int TranslateGXTexCoord(GXTexCoordID coord) {
	int ret;
	switch (coord) {
	case GX_TEXCOORD0:
		ret = 0;
		break;
	case GX_TEXCOORD1:
		ret = 1;
		break;
	case GX_TEXCOORD2:
		ret = 2;
		break;
	case GX_TEXCOORD3:
		ret = 3;
		break;
	case GX_TEXCOORD4:
		ret = 4;
		break;
	case GX_TEXCOORD5:
		ret = 5;
		break;
	case GX_TEXCOORD6:
		ret = 6;
		break;
	case GX_TEXCOORD7:
		ret = 7;
		break;
	default:
		ret = -1;
	}
	return ret;
}


int TranslateGXTexFormat(GXTexFmt fmt) {
	int ret;
	switch (fmt) {
	case GX_TF_I4:
		ret = 3;
		break;
	case GX_TF_I8:
		ret = 4;
		break;
	case GX_TF_IA4:
		ret = 5;
		break;
	case GX_TF_IA8:
		ret = 6;
		break;
	case GX_TF_RGB565:
		ret = 7;
		break;
	case GX_TF_RGB5A3:
		ret = 8;
		break;
	case GX_TF_RGBA8:
		ret = 9;
		break;
		
	// From GXTexFilter
	case GX_TF_C4:
		ret = 0;
		break;
	case GX_TF_C8:
		ret = 1;
		break;
	case GX_TF_C14X2:
		ret = 2;
		break;
		
	case GX_TF_CMPR:
		ret = 10;
		break;
	case GX_TF_Z16:
		ret = 0xc;
		break;
		
	/*case 0x13:
		ret = 0xd;
		break;*/
	case 0x16:
		ret = 0xe;
		break;
	/*case 0x21:
		ret = 0xb;
		break;*/
		
	default:
		ret = -1;
	}
	return ret;
}



uint TranslateGXTlutSize(uint lut) {
	uint ret = 0;
	if (lut < 0x21) {
		switch (lut) {
		case GX_TLUT_16:
			ret = 0x10;
			break;
		case GX_TLUT_32:
			ret = 0x20;
			break;
		case GX_TLUT_16 | GX_TLUT_32:
		case GX_TLUT_16 | GX_TLUT_64:
		case GX_TLUT_32 | GX_TLUT_64:
		case GX_TLUT_16 | GX_TLUT_32 | GX_TLUT_64:
		case GX_TLUT_16 | GX_TLUT_128:
		case GX_TLUT_32 | GX_TLUT_128:
		case GX_TLUT_16 | GX_TLUT_32 | GX_TLUT_128:
		case GX_TLUT_64 | GX_TLUT_128:
		case GX_TLUT_16 | GX_TLUT_64 | GX_TLUT_128:
		case GX_TLUT_32 | GX_TLUT_64 | GX_TLUT_128:
		case GX_TLUT_16 | GX_TLUT_32 | GX_TLUT_64 | GX_TLUT_128:
			break;
		case GX_TLUT_64:
			ret = 0x40;
			break;
		case GX_TLUT_128:
			ret = 0x80;
			break;
		case GX_TLUT_256:
			ret = 0x100;
			break;
		default:
			ret = 0x200;
		}
	}
	else if (lut < 0x101) {
		if (lut == GX_TLUT_4K) {
			ret = 0x1000;
		}
		else if (lut == GX_TLUT_1K) {
			ret = 0x400;
		}
		else if (lut == GX_TLUT_2K) {
			ret = 0x800;
		}
	}
	else {
		if (lut == GX_TLUT_8K) {
			ret = 0x2000;
		}
		else if (lut == GX_TLUT_16K) {
			ret = 0x4000;
		}
	}
	
	return ret;
}


uint TranslateGXTexCacheSize(GXTexCacheSize i) {
	uint ret;
	
	if (i == GX_TEXCACHE_32K)
		ret = 0x08000;
	else if (i == GX_TEXCACHE_128K)
		ret = 0x20000;
	else if (i == GX_TEXCACHE_512K)
		ret = 0x80000;
	else
		ret = 0;
	
	return ret;
}


int TranslateGXTevStageID(GXTevStageID i) {
	int ret;
	switch (i) {
	case GX_TEVSTAGE0:
		ret = 0;
		break;
	case GX_TEVSTAGE1:
		ret = 1;
		break;
	case GX_TEVSTAGE2:
		ret = 2;
		break;
	case GX_TEVSTAGE3:
		ret = 3;
		break;
	case GX_TEVSTAGE4:
		ret = 4;
		break;
	case GX_TEVSTAGE5:
		ret = 5;
		break;
	case GX_TEVSTAGE6:
		ret = 6;
		break;
	case GX_TEVSTAGE7:
		ret = 7;
		break;
	case GX_TEVSTAGE8:
		ret = 8;
		break;
	case GX_TEVSTAGE9:
		ret = 9;
		break;
	case GX_TEVSTAGE10:
		ret = 10;
		break;
	case GX_TEVSTAGE11:
		ret = 0xb;
		break;
	case GX_TEVSTAGE12:
		ret = 0xc;
		break;
	case GX_TEVSTAGE13:
		ret = 0xd;
		break;
	case GX_TEVSTAGE14:
		ret = 0xe;
		break;
	case GX_TEVSTAGE15:
		ret = 0xf;
		break;
	default:
		ret = -1;
	}
	return ret;
}


int TranslateGXTexGenSrc_TexCoord(GXTexGenSrc in) {
	int ret;
	switch (in) {
	case GX_TG_TEX0:
		ret = 0;
		break;
	case GX_TG_TEX1:
		ret = 1;
		break;
	case GX_TG_TEX2:
		ret = 2;
		break;
	case GX_TG_TEX3:
		ret = 3;
		break;
	case GX_TG_TEX4:
		ret = 4;
		break;
	case GX_TG_TEX5:
		ret = 5;
		break;
	case GX_TG_TEX6:
		ret = 6;
		break;
	case GX_TG_TEX7:
		ret = 7;
		break;
	default:
		ret = -1;
	}
	return ret;
}


int TranslateGXTexGenSrc_BumpTexCoord(GXTexGenSrc i) {
	int ret;
	switch (i) {
	case GX_TG_TEXCOORD0:
		ret = 0;
		break;
	case GX_TG_TEXCOORD1:
		ret = 1;
		break;
	case GX_TG_TEXCOORD2:
		ret = 2;
		break;
	case GX_TG_TEXCOORD3:
		ret = 3;
		break;
	case GX_TG_TEXCOORD4:
		ret = 4;
		break;
	case GX_TG_TEXCOORD5:
		ret = 5;
		break;
	case GX_TG_TEXCOORD6:
		ret = 6;
		break;
	default:
		ret = -1;
	}
	return ret;
}


const char* TranslateGXAttr_Name(GXAttr attr) {
	const char *ret;
	switch (attr) {
	case GX_VA_PNMTXIDX:
		ret = "GX_VA_PNMTXIDX";
		break;
	case GX_VA_TEX0MTXIDX:
		ret = "GX_VA_TEX0MTXIDX";
		break;
	case GX_VA_TEX1MTXIDX:
		ret = "GX_VA_TEX1MTXIDX";
		break;
	case GX_VA_TEX2MTXIDX:
		ret = "GX_VA_TEX2MTXIDX";
		break;
	case GX_VA_TEX3MTXIDX:
		ret = "GX_VA_TEX3MTXIDX";
		break;
	case GX_VA_TEX4MTXIDX:
		ret = "GX_VA_TEX4MTXIDX";
		break;
	case GX_VA_TEX5MTXIDX:
		ret = "GX_VA_TEX5MTXIDX";
		break;
	case GX_VA_TEX6MTXIDX:
		ret = "GX_VA_TEX6MTXIDX";
		break;
	case GX_VA_TEX7MTXIDX:
		ret = "GX_VA_TEX7MTXIDX";
		break;
	case GX_VA_POS:
		ret = "GX_VA_POS";
		break;
	case GX_VA_NRM:
		ret = "GX_VA_NRM";
		break;
	case GX_VA_CLR0:
		ret = "GX_VA_CLR0";
		break;
	case GX_VA_CLR1:
		ret = "GX_VA_CLR1";
		break;
	case GX_VA_TEX0:
		ret = "GX_VA_TEX0";
		break;
	case GX_VA_TEX1:
		ret = "GX_VA_TEX1";
		break;
	case GX_VA_TEX2:
		ret = "GX_VA_TEX2";
		break;
	case GX_VA_TEX3:
		ret = "GX_VA_TEX3";
		break;
	case GX_VA_TEX4:
		ret = "GX_VA_TEX4";
		break;
	case GX_VA_TEX5:
		ret = "GX_VA_TEX5";
		break;
	case GX_VA_TEX6:
		ret = "GX_VA_TEX6";
		break;
	case GX_VA_TEX7:
		ret = "GX_VA_TEX7";
		break;
	case GX_VA_NBT:
		ret = "GX_VA_NBT";
		break;
	default:
		ret = "Unknown";
	}
	return ret;
}


const char * TranslateGXAttrType_Name(GXAttrType type) {
	const char *ret;
	switch (type) {
	case GX_NONE:
		ret = "GX_NONE";
		break;
	case GX_DIRECT:
		ret = "GX_DIRECT";
		break;
	case GX_INDEX8:
		ret = "GX_INDEX8";
		break;
	case GX_INDEX16:
		ret = "GX_INDEX16";
		break;
	default:
		ret = "Unknown";
	}
	return ret;
}


const char * TranslateGXApiID_Name(ApiID api) {
	const char *ret;
	switch ((uint)api & 0xff) {
	case 1:
		ret = "PosMtxIndex";
		break;
	case 2:
		ret = "TexMtxIndex";
		break;
	case 3:
		ret = "Postion";
		break;
	case 4:
		ret = "Normal";
		break;
	case 5:
		ret = "Color";
		break;
	case 6:
		ret = "TexCoord";
		break;
	default:
		ret = "Unknown";
	}
	return ret;
}


const char * TranslateGXCompCnt_Name(GXAttr attr, GXCompCnt cnt) {
	switch (attr & 0xff) {
	case GX_VA_TEX0MTXIDX:
	case GX_VA_TEX1MTXIDX:
		return "________";
	case GX_VA_TEX2MTXIDX:
		if (cnt == GX_TEX_S) {
			return "GX_POS_XY";
		}
		if (cnt == GX_TEX_ST) {
			return "GX_POS_XYZ";
		}
		break;
	case GX_VA_TEX3MTXIDX:
		if (cnt == GX_TEX_S) {
			return "GX_NRM_XYZ";
		}
		if (cnt == GX_TEX_ST) {
			return "GX_NRM_NBT";
		}
		break;
	case GX_VA_TEX4MTXIDX:
		if (cnt == GX_TEX_S) {
			return "GX_CLR_RGB";
		}
		if (cnt == GX_TEX_ST) {
			return "GX_CLR_RGBA";
		}
		break;
	case GX_VA_TEX5MTXIDX:
		if (cnt == GX_TEX_S) {
			return "GX_TEX_S";
		}
		if (cnt == GX_TEX_ST) {
			return "GX_TEX_ST";
		}
	}
	return "Unknown";
}


const char * TranslateGXCompType_Name(GXChannelID chan, GXCompType type) {
	const char *ret;
	switch (chan & 0xff) {
	case GX_COLOR1:
	case GX_ALPHA0:
		return "________";
	case GX_ALPHA1:
	case GX_COLOR0A0:
	case GX_COLOR_ZERO:
		switch (type) {
		case GX_U8:
			ret = "GX_U8";
			break;
		case GX_S8:
			ret = "GX_S8";
			break;
		case GX_U16:
			ret = "GX_U16";
			break;
		case GX_S16:
			ret = "GX_S16";
			break;
		case GX_RGBA6:
			ret = "GX_F32";
			break;
		default:
			ret = "Unknown";
		}
		break;
	case GX_COLOR1A1:
		switch (type) {
		case GX_U8:
			return "GX_RGB565";
		case GX_S8:
			return "GX_RGB8";
		case GX_U16:
			return "GX_RGBX8";
		case GX_S16:
			return "GX_RGBA4";
		case GX_RGBA6:
			return "GX_RGBA6";
		case GX_RGBA8:
			return "GX_RGBA8";
		}
	default:
		ret = "Unknown";
	}
	return ret;
}


