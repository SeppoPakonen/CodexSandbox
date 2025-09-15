#pragma once

#define COMMON_SIZE		0x1A
#define VCD_SIZE		COMMON_SIZE
#define VTXARR_SIZE		COMMON_SIZE

// 0xC
/*struct GfxTrackStruct {
	byte mode_and_arg;
	byte mode;
	byte cnt;
	byte gts_unknown1;
	uint gts_unknown2;
	uint gts_unknown3;
	
};*/

struct CompStruct {
	byte mode;
	byte attr_type;
	byte cnt;
	byte fmt;
	float scale;
	uint attr;
	
	GXChannelID	GetChannelID()	const {return (GXChannelID)mode;}
	GXAttrType	GetAttrType()	const {return (GXAttrType)attr_type;}
	GXCompCnt	GetCompCnt()	const {return (GXCompCnt)cnt;}
	GXCompType	GetCompType()	const {return (GXCompType)fmt;}
	GXAttr		GetAttr()		const {return (GXAttr)attr;}
	
};

struct VatStruct {
	GXCompCnt cnt;
	GXCompType type;
	uint frac;
	float vcd_value;
	
};

struct NTGStruct {
	GXTexGenSrc src;
	uint unknown0;
	uint unknown1;
};

extern bool			EmDisplayListInProgress;
extern byte*		EmBuffPtr;
extern void*		EmBuffStart;
extern void*		EmBuffTop;
extern CompStruct	GfxTrack[VTXARR_SIZE];
extern CompStruct	some_gxvert_var[VTXARR_SIZE];
extern GXAttrType	VCD[VCD_SIZE];
extern VatStruct	VAT[GX_VA_MAX_ATTR][VCD_SIZE];
extern VtxArray		GXVtxArray[VTXARR_SIZE];
extern NTGStruct	GXNumTexGens[VTXARR_SIZE];
extern TexCoordGen	GXTexCoordGen[VTXARR_SIZE];
extern GXColor		CurrentColors[VTXARR_SIZE];
extern uint			NBTCount;


enum {
	PRIM_DATA_INVALID,
	PRIM_DATA_MAT_IDX0,
	PRIM_DATA_MAT_IDX1,
	PRIM_DATA_POS,
	PRIM_DATA_NORM,
	PRIM_DATA_CLR,
	PRIM_DATA_TEX,
};

enum {
	ARGTYPE_U8,
	ARGTYPE_S8,
	ARGTYPE_U16,
	ARGTYPE_S16,
	ARGTYPE_F32,
	
};

void SendPrimitiveData(byte **data, int size);
void SendPositionDirect(int arg, byte **data, int *size);
void SendNormalDirect(int arg, byte **data, int *size);
void SendColorDirect(int arg, byte **data, int *size);
void SendTexCoordDirect(int arg, byte **data, int *size);
float GetF32(byte **data, int *size_left);
u16 GetU16(byte **data, int *size_left);
u8 GetU8(byte **data, int *size_left);
uint GetVertexLength(void);
uint VCDComponentLength(uint vcd, uint i);

