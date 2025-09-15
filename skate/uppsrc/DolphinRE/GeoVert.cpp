#include "DolphinRE.h"
/*
bool		InBegin;
bool		InQuad;
u32			QuadCount;
int			NumTexCoordSrc;
u32			RemainingVerts;
Vec*		NrmIter;
GXColor*	ClrIter;
Vec*		TexIter;
int			ColorRef_ColorRef;
int			ColorRef_plus4;
uint		GXNumTexGenCount;
*/
bool		InBegin;
bool		InQuad;
u32		QuadCount;
int		NumTexCoordSrc;
u32		RemainingVerts;
Vec*		NrmIter;
GXColor*	ClrIter;
Vec*		TexIter;
int		ColorRef_Color0;
int		ColorRef_Color1;
uint		GXNumTexGenCount;

void IncGfxTracker() {
	ASSERT((GfxTracker->mode & 0x80) == 0);
	GfxTracker ++;
}

GXAttrType IntToAttrType(int i) {
	ASSERT(i >= 0 && i <= GX_INDEX16);
	return (GXAttrType)i;
}

GXCompType IntToCompType(int i) {
	ASSERT(i >= 0 && i <= GX_RGBA8);
	return (GXCompType)i;
}

GXCompCnt IntToCompCnt(int i) {
	ASSERT(i >= 0 && i <= 2);
	return (GXCompCnt)i;
}


void GXSetVtxDesc(GXAttr attr, GXAttrType type) {
	{
		int trans_vcd = TranslateGXAttr_VCD(attr);
		ASSERTMSG(trans_vcd >= 0 && trans_vcd < VCD_SIZE, "Invalid vertex array");
		
		VCD[trans_vcd] = type;
	}
	
	if (type != GX_NONE) {
		if (attr == GX_VA_NRM) {
			int trans_vcd = TranslateGXAttr_VCD(GX_VA_NBT);
			ASSERTMSG(trans_vcd >= 0 && trans_vcd < VCD_SIZE, "Invalid vertex array");
			VCD[trans_vcd] = GX_NONE;
		}
		else if (attr == GX_VA_NBT) {
			int trans_vcd = TranslateGXAttr_VCD(GX_VA_NRM);
			ASSERTMSG(trans_vcd >= 0 && trans_vcd < VCD_SIZE, "Invalid vertex array");
			VCD[trans_vcd] = GX_NONE;
		}
	}
}


void GXSetVtxDescv(GXVtxDescList *attr_list) {
	ASSERTMSG(attr_list != 0, "attr ptr is null");
	
	for (GXVtxDescList *it = attr_list; it->attr != GX_VA_NULL; it++)
		GXSetVtxDesc(it->attr, it->type);
}

void GXGetVtxDesc(GXAttr attr, GXAttrType *type) {
	int trans_vcd = TranslateGXAttr_VCD(attr);
	ASSERTMSG(trans_vcd >= 0 && trans_vcd < VCD_SIZE, "Invalid vertex array");
	
	*type = VCD[trans_vcd];
}


void GXGetVtxDescv(GXVtxDescList *attr_list) {
	ASSERTMSG(attr_list != 0, "attr ptr is null");
	
	for (int i = 0; i < VCD_SIZE; i++) {
		GXAttr untrans_attr = UntranslateGXAttr_VCD(i);
		attr_list[i].attr = untrans_attr;
		attr_list[i].type = VCD[i];
	}
	attr_list[VCD_SIZE].attr = GX_VA_NULL;
}


void GXClearVtxDesc() {
	for (int i = 0; i < VCD_SIZE; i++)
		VCD[i] = GX_NONE;
}


void GXSetVtxAttrFmt(GXVtxFmt vtxfmt, GXAttr attr, GXCompCnt cnt, GXCompType type, u8 frac) {
	int trans_fmt = TranslateGXVtxFmt(vtxfmt);
	int trans_attr = TranslateGXAttr_VAT(attr);
	ASSERT(trans_fmt >= 0 && trans_fmt < GX_VA_MAX_ATTR);
	ASSERT(trans_attr >= 0 && trans_attr < VCD_SIZE);
	ASSERT(frac < 0x20);
	
	VatStruct& vat = VAT[trans_fmt][trans_attr];
	vat.cnt = cnt;
	vat.type = type;
	
	int bitshift;
	float vcd_value;
	if (type == GX_RGBA6) {
		vcd_value = 1.0;
	}
	else {
		int bitshift = 1 << (frac & 0x1f);
		vcd_value = 1.0 / (float)bitshift;
	}
	
	vat.frac = frac;
	vat.vcd_value = vcd_value;
}
           
           
void GXSetVtxAttrFmtv(GXVtxFmt vtxfmt, GXVtxAttrFmtList *list) {
	ASSERTMSG(list != 0, "list ptr is null");
	
	for (GXVtxAttrFmtList *it = list; it->attr != GX_VA_NULL; it++)
		GXSetVtxAttrFmt(vtxfmt, it->attr, it->cnt, it->type, it->frac);
}


void GXGetVtxAttrFmt(GXVtxFmt vtxfmt, GXAttr attr, GXCompCnt *cnt, GXCompType *type, u8 *frac) {
	int trans_fmt = TranslateGXVtxFmt(vtxfmt);
	int trans_attr = TranslateGXAttr_VAT(attr);
	ASSERT(trans_fmt >= 0 && trans_fmt < GX_VA_MAX_ATTR);
	ASSERT(trans_attr >= 0 && trans_attr < VCD_SIZE);
	
	VatStruct& vat = VAT[trans_fmt][trans_attr];
	*cnt  = vat.cnt;
	*type = vat.type;
	*frac = vat.frac;
}


void GXGetVtxAttrFmtv(GXVtxFmt vtxfmt, GXVtxAttrFmtList *list) {
	int trans_fmt = TranslateGXVtxFmt(vtxfmt);
	
	ASSERT(list != 0);
	ASSERT(trans_fmt >= 0 && trans_fmt < GX_VA_MAX_ATTR);
	
	for (int i = 0; i < VCD_SIZE; i++) {
		GXAttr untrans_vat = UntranslateGXAttr_VAT(i);
		
		VatStruct& vat = VAT[trans_fmt][i];
		list[i].attr = untrans_vat;
		list[i].cnt  = vat.cnt;
		list[i].type = vat.type;
		list[i].frac = vat.frac;
	}
	list[VCD_SIZE].attr = GX_VA_NULL;
}


void GXSetArray(GXAttr attr, void* base_ptr, u8 stride) {
	int i = TranslateGXAttr_Array(attr);
	ASSERTMSG(i >= 0 && i < VTXARR_SIZE, "Invalid vertex attribute");
	
	GXVtxArray[i].arr		= (byte*)base_ptr;
	GXVtxArray[i].stride	= stride;
}


void GXGetArray(GXAttr attr, void** base_ptr, u8* stride) {
	int i = TranslateGXAttr_Array(attr);
	ASSERTMSG(i >= 0 && i < VTXARR_SIZE, "Invalid vertex attribute");
	
	*base_ptr	= GXVtxArray[i].arr;
	*stride		= GXVtxArray[i].stride;
}


void GXBegin(GXPrimitive type, GXVtxFmt vtxfmt, u16 nverts) {
	int trans_fmt = TranslateGXVtxFmt(vtxfmt);
	ASSERTMSG(trans_fmt >= 0 && trans_fmt < GX_VA_MAX_ATTR, "Format index is out of range");
	ASSERTMSG(nverts, "Vertex count must be > 0");
	ASSERTMSG(InBegin == 0, "GXEnd statement is missing");
	
	InBegin = true;
	
	if (EmDisplayListInProgress == false) {
		switch (type) {
		
		case GX_QUADS:
			InQuad = true;
			QuadCount = 0;
			Gfx::Static::Begin(Gfx::PrimType::QUADS);
			break;
			
		case GX_TRIANGLES:
			Gfx::Static::Begin(Gfx::PrimType::TRIANGLES);
			break;
			
		case GX_TRIANGLESTRIP:
			Gfx::Static::Begin(Gfx::PrimType::TRIANGLE_STRIP);
			break;
			
		case GX_TRIANGLEFAN:
			Gfx::Static::Begin(Gfx::PrimType::TRIANGLE_FAN);
			break;
			
		case GX_LINES:
			Gfx::Static::Begin(Gfx::PrimType::LINES);
			break;
			
		case GX_LINESTRIP:
			Gfx::Static::Begin(Gfx::PrimType::LINE_STRIP);
			break;
			
		case GX_POINTS:
			Gfx::Static::Begin(Gfx::PrimType::POINTS);
			break;
			
		default:
			PANIC("Unsupported primitive");
			
		}
		
		int trans_vcd = TranslateGXAttr_VCD(GX_VA_POS);
		ASSERT(trans_vcd >= 0 && trans_vcd < VCD_SIZE);
		ASSERTMSG(VCD[trans_vcd] != 0, "No descriptor for vcd position");
		
		int i = 0;
		NumTexCoordSrc = 0;
		
		for (int j = 0; j < VCD_SIZE; j++) {
			byte mode = UntranslateApiID(j);
			
			if (VCD[j] != 0) {
				while (1) {
					GXAttr untrans_vcd = UntranslateGXAttr_VCD(j);
					int retrans_vat = TranslateGXAttr_VAT(untrans_vcd);
					ASSERT(retrans_vat >= 0 && retrans_vat < VCD_SIZE);
					
					ASSERT((mode & 0x80) == 0);
					GfxTrack[i].mode = mode;
					GfxTrack[i].attr_type = VCD[j];
					GfxTrack[i].attr = untrans_vcd;
					
					if (mode > 2) {
						if (mode > 5) {
							if (mode != 6)
								break;
							NumTexCoordSrc++;
						}
						VatStruct& vat = VAT[trans_fmt][retrans_vat];
						GfxTrack[i].cnt = vat.cnt;
						GfxTrack[i].fmt = vat.type;
						GfxTrack[i].scale = vat.vcd_value;
						
						if (untrans_vcd == GX_VA_NBT && GfxTrack[i].mode) {
							GfxTrack[i].cnt = 0;
							GfxTrack[i+1] = GfxTrack[i];
							GfxTrack[i+2] = GfxTrack[i+1];
							i += 2;
						}
					}
					break;
				}
				i++;
			}
		}
		GfxTrack[i - 1].mode |= 0x80;
		
		RemainingVerts = nverts;
		GfxTracker = GfxTrack - 1;
		NrmIter = CurrentNormals - 1;
		ClrIter = CurrentColors - 1;
		TexIter = CurrentTexCoords - 1;
		NBTCount = 0;
	}
	// EmDisplayListInProgress == true
	else {
	    GXCmd1u8((byte)type | (byte)vtxfmt);
	    GXParam1u16(nverts);
	}
}


void GXEnd() {
	ASSERTMSG(InBegin == true, "GXBegin statement is missing");
	
	InBegin = false;
	InQuad = 0;
	if (EmDisplayListInProgress == false) {
		ASSERTMSG(RemainingVerts == 0, "mismatch of the number of vertices");
		ASSERTMSG(GfxTracker == GfxTrack - 1, "number of components missing");
		End();
		GfxTrack[0].mode = 0;
	}
}


void GXSetTexCoordGen(GXTexCoordID dst_coord, GXTexGenType func, GXTexGenSrc src_param, u32 mtx) {
	int i = TranslateGXTexCoord(dst_coord);
	
	ASSERTMSG(i >= 0, "Invalid coordinate");
	ASSERTMSG(func >= 0 && func <= GX_TG_SRTG, "Invalid function");
	ASSERTMSG(mtx < MAT_LIMIT, "Invalid matrix");
	
	GXTexGenSrc old_src_param = GXTexCoordGen[i].src_param;
	
	if (old_src_param == GX_TG_COLOR0)
		ColorRef_Color0--;
	else if (old_src_param == GX_TG_COLOR1)
		ColorRef_Color1--;
	
	if (src_param == GX_TG_COLOR0)
		ColorRef_Color0++;
	else if (src_param == GX_TG_COLOR1)
		ColorRef_Color1++;
	
	GXTexCoordGen[i].func = func;
	GXTexCoordGen[i].mtx = mtx;
	GXTexCoordGen[i].src_param = src_param;
}


void GXSetNumTexGens(u8 nTexGens) {
	ASSERTMSG(nTexGens <= 8, "Invalid number");
	GXNumTexGenCount = nTexGens;
}


void GXSetLineWidth(u8 width, GXTexOffset tex_offsets) {
	float w = width;
	LineWidth(w / 6.0f);
}


void GXSetPointSize(u8 size, GXTexOffset tex_offsets) {
	float size_f32 = size;
	PointSize(size_f32 / 6.0f);
}


