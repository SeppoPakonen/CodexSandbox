/*---------------------------------------------------------------------------*
  Project:  Dolphin GX library
  File:     GXStruct.h

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/dolphin/gx/GXStruct.h $
    
    9     5/10/01 9:43p Hirose
    added tag for each structure definition
    
    8     1/24/01 5:09p Carl
    Fixed texobj, texregion for emulator.
    
    7     1/19/01 7:54p Carl
    Optimized TexObjs and TexRegions.
    
    6     10/30/00 6:26p Carl
    Had to enlarge GXTlutRegion to accomodate change in tlut data.
    
    5     4/04/00 11:17a Alligator
    moved constants from GXStruct.h to GXEnum.h since they are dependent on
    enumerated values.
    
    4     3/13/00 6:18p Danm
    Fixed AA sampling locations to use x,y pairs.
    
    3     1/26/00 4:06p Hashida
    Include vitypes.h instead of vi.h
    
    2     1/13/00 5:55p Alligator
    integrate with ArtX GX library code
    
    18    11/17/99 10:23p Hirose
    changed size of some structures
    
    17    11/16/99 6:17p Alligator
    
    16    11/15/99 4:42p Hirose
    changed size of GXTexObj and GXTexRegion
    
    15    10/26/99 8:04p Alligator
    added s10-bit color
    
    14    10/24/99 7:42a Yasu
    Add GX_MAX_TEXMAP
    
    13    10/22/99 4:07p Yasu
    Add definitions of HW information
    
    12    10/11/99 10:20a Hirose
    changed size of GXTexObj
    
    11    10/04/99 2:42p Yasu
    Add GXInitFogAdjTable
    
    10    9/21/99 2:33p Alligator
    add aa flag, if aa set 16b pix format
    
    9     9/21/99 11:49a Hirose
    changed GXTlutRegion size
    
    8     9/17/99 3:38p Hirose
    changed GXTlutObj size
    
    7     9/16/99 3:49p Alligator
    
    6     9/16/99 3:42p Alligator
    update render mode api
    
    5     9/09/99 3:04p Alligator
    move GXSetRenderMode to GX lib from emu
    
    4     9/02/99 3:18p Ryan
    Made Frame Buffer Api changes
    
    3     9/01/99 2:27p Ryan
    Added temp GXTesRegion and GXTlutRegion structs
    
    2     7/20/99 6:10p Alligator
    added GXGetVtxDescv, GXGetVtxAttrFmtv
    
    1     7/14/99 4:20p Alligator
    split gx.h into individual header files for each major section of API
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GXSTRUCT_H__
#define __GXSTRUCT_H__


/********************************/
#if 0
extern "C" {
#endif

/********************************/
#include "types.h"
#include "GXEnum.h"
#include "vitypes.h"

/********************************/
typedef struct _GXColor
{
    u8  r,
        g, 
        b, 
        a;

} GXColor;

struct GXImage {
	byte* data;
	uint width;
	uint height;
	uint stride;
};

struct ColorVec {
	GXColor x, y, z;
};

typedef struct _GXColorS10
{
    s16    r, g, b, a; // s10-bit components for Tev constant color
} GXColorS10;

/********************************/

struct TexCacheStruct {
    void* image_ptr;
    uint node;
    Gfx::TexArb activetex_tex_arb;
    struct TexCacheStruct* next;
};

struct GXTexObj {
    void* image_ptr;
    u16 width;
    u16 height;
    GXTexFmt format;
    GXTexWrapMode wrap_s;
    GXTexWrapMode wrap_t;
    uint tlut_name;
    GXTexFilter min_filt;
    GXTexFilter mag_filt;
    f32 min_lod;
    f32 max_lod;
    f32 lod_bias;
    GXAnisotropy max_aniso;
    GXBool bias_clamp;
    GXBool do_edge_lod;
    GXBool unused;
    GXBool mipmap;
    void * user_data;
};

/********************************/

struct GXTlutObj {
    void * lut;
    GXTlutFmt fmt;
    u16 entry_count;
    u16 field_0xa;
};

/********************************/
struct GXLightObj {
	Vec normal;
	Vec halfangle;
	GXColor color;
	Vec spotlight_type;
	Vec lt_dist_attn;
	int pad;
};

struct LightState {
	GXLightObj lt;
	float el_pow;
	float vec_len;
	Vec unit;
};

/********************************/
typedef struct _GXVtxAttrFmtList
{
    GXAttr        attr;
    GXCompCnt     cnt;
    GXCompType    type;
    u8            frac;
    
} GXVtxAttrFmtList;

/********************************/

struct GXTexRegion {
    u32 image_ptr_even;
    u32 image_ptr_odd;
    uint field_0x8;
    bool loaded;
    bool is_32b_mipmap;
    bool unused[2];
    void* image_ptr;
    uint trans_size_even;
    uint trans_size_odd;
    uint unused1;
};


/********************************/


struct GXTlutRegion {
    u32 tmem_addr;
    GXTlutFmt tlut_fmt;
    uint tlut_size;
    uint pad1;
};

/********************************/
typedef struct _GXVtxDescList
{
    GXAttr        attr;
    GXAttrType    type;
    
} GXVtxDescList;

/********************************/
typedef struct _GXRenderModeObj
{
    VITVMode          viTVmode;
    u16               fbWidth;   // no xscale from efb to xfb
    u16               efbHeight; // embedded frame buffer
    // 8
    
    u16               xfbHeight; // external frame buffer, may yscale efb
    u16               viXOrigin;
    
    // 12
    
    u16               viYOrigin;
    u16               viWidth;
    // 16
    
    u16               viHeight;
    VIXFBMode         xFBmode;   // whether single-field or double-field in
                                 // XFB.
	// 20
    u8                field_rendering;    // rendering fields or frames?
    u8                aa;                 // antialiasing on?
    // 22
    u8                sample_pattern[12][2]; // aa sample pattern
    // 46
    u8                vfilter[7];         // vertical filter coefficients
} GXRenderModeObj;

/********************************/
typedef struct _GXFogAdjTable
{
    u16			r[10];
} GXFogAdjTable;

/********************************/




struct TevStatus {
    GXTexCoordID coord;
    GXTexMapID map;
    GXChannelID color;
    GXTevMode mode;
};


typedef enum ApiID {
    API_ID_UNKNOWN		= 0,
    API_ID_POS_MTX_IDX	= 0x100,
    API_ID_TEX_MTX_IDX	= 0x200,
    API_ID_POSITION		= 0x300,
    API_ID_NORMAL		= 0x400,
    API_ID_COLOR		= 0x500,
    API_ID_TEXCOORD		= 0x600,
} ApiID;



/*---------------------------------------------------------------------------*/
#if 0
}
#endif

#endif
