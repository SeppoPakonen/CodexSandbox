/*---------------------------------------------------------------------------*
  Project:  Dolphin
  File:     tev-emu.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/demos/gxdemo/src/Tev/tev-emu.c $
    
    1     3/06/00 12:12p Alligator
    move from tests/gx and rename
    
    4     2/09/00 7:04p Hirose
    added check for texture swap setting
    
    3     2/07/00 7:48p Hirose
    fixed some bugs
    
    2     2/03/00 11:19p Hirose
    beta version (still under construction)
    
    1     2/02/00 11:31p Hirose
    initial stubs
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
    tev-emu
        This is the apprication-level utility library for emulating some
        feature set of the Dolphin Texture Environment (TEV) under the
        MAC emulator environment which is not able to support actual TEV
        feature.
 *---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
   Header files
 *---------------------------------------------------------------------------*/
#include "tev-emu.h"

/*---------------------------------------------------------------------------*/
// This source is for the emulator only.
#ifndef EPPC 

#define NUM_REGISTERS   4
#define MAX_STAGES      16

#define TE_TEXSWAP_ERRMSG \
    "TEXC, TEXRRR, TEXGGG and TEXBBB should be used exclusively."

/*---------------------------------------------------------------------------*
    Private structure definitions
 *---------------------------------------------------------------------------*/
// Status for each Tev stage
typedef struct
{
    GXTevColorArg   colorIn[4]; // for a, b, c, d
    GXTevAlphaArg   alphaIn[4];
    s16             op[2];      // for color/alpha each
    s16             bias[2];
    s16             scale[2];
    GXBool          clamp[2];
    GXTevRegID      outReg[2];
    GXTevClampMode  clampMode;  // shared by color/alpha
} TETevStage;

// Input color for each Tev stage
typedef struct
{
    GXColor         rasIn; // rasterized color
    GXColor         texIn; // looked-up texel color
} TETevInput;

// Entire Tev status
typedef struct
{
    u32             numStages;
    GXColorS10      reg[NUM_REGISTERS];
    TETevStage      stage[MAX_STAGES];
    TETevInput      input[MAX_STAGES];
    GXAlphaOp       alphaOp;
    GXCompare       alphaComp[2];
    u8              alphaRef[2];
} TETevStatus;

// For textured-image level emulation
typedef struct
{
    u16             width;
    u16             height;
    u8*             rasImg[MAX_STAGES];
    u8*             texImg[MAX_STAGES];
} TEImageHolder;


static TETevStatus    TevSt;
static TEImageHolder  ImgHolder;

/*---------------------------------------------------------------------------*
    Private function definitions
 *---------------------------------------------------------------------------*/
static s32  TETranslateTevStageID( GXTevStageID stage );
static s32  TETranslateTevRegID( GXTevRegID reg );
static s16  TETranslateTevOp( GXTevOp op );
static s16  TETranslateTevBias( GXTevBias bias );
static s16  TETranslateTevScale( GXTevScale scale );
static void TECheckTexSwap( TETevStage* stg );

static void TESelectInput( GXColorS10* color, s32 stgNum, s32 arg );
static s16  TEClamp( s16 val, GXTevClampMode mode, GXBool sw );
static void TEPerformOneStage( s32 stgNum );

/*---------------------------------------------------------------------------*

    Symbol conversion functions

 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*

 *---------------------------------------------------------------------------*/
s32 TETranslateTevStageID( GXTevStageID stage )
{
    switch(stage)
    {
      case GX_TEVSTAGE0:   return 0;
      case GX_TEVSTAGE1:   return 1;
      case GX_TEVSTAGE2:   return 2;
      case GX_TEVSTAGE3:   return 3;
      case GX_TEVSTAGE4:   return 4;
      case GX_TEVSTAGE5:   return 5;
      case GX_TEVSTAGE6:   return 6;
      case GX_TEVSTAGE7:   return 7;
      case GX_TEVSTAGE8:   return 8;
      case GX_TEVSTAGE9:   return 9;
      case GX_TEVSTAGE10:  return 10;
      case GX_TEVSTAGE11:  return 11;
      case GX_TEVSTAGE12:  return 12;
      case GX_TEVSTAGE13:  return 13;
      case GX_TEVSTAGE14:  return 14;
      case GX_TEVSTAGE15:  return 15;
    }
    
    OSHalt("Incorrect Tev stage ID");
    return -1;
}

/*---------------------------------------------------------------------------*

 *---------------------------------------------------------------------------*/
s32 TETranslateTevRegID( GXTevRegID reg )
{
    switch(reg)
    {
      case GX_TEVREG0:   return 0;
      case GX_TEVREG1:   return 1;
      case GX_TEVREG2:   return 2;
      case GX_TEVPREV:   return 3;
    }
    
    OSHalt("Incorrect Tev register ID");
    return -1;
}

/*---------------------------------------------------------------------------*

 *---------------------------------------------------------------------------*/
s16 TETranslateTevOp( GXTevOp op )
{
    switch(op)
    {
      case GX_TEV_ADD:   return 1;
      case GX_TEV_SUB:   return -1;
    }
    
    OSHalt("Incorrect Tev operator");
    return 0;
}

/*---------------------------------------------------------------------------*

 *---------------------------------------------------------------------------*/
s16 TETranslateTevBias( GXTevBias bias )
{
    switch(bias)
    {
      case GX_TB_ZERO:     return 0;
      case GX_TB_ADDHALF:  return 128;
      case GX_TB_SUBHALF:  return -128;
    }
    
    OSHalt("Incorrect Tev bias");
    return -1;
}

/*---------------------------------------------------------------------------*

 *---------------------------------------------------------------------------*/
s16 TETranslateTevScale( GXTevScale scale )
{
    switch(scale)
    {
      case GX_CS_SCALE_1:   return 2;
      case GX_CS_SCALE_2:   return 4;
      case GX_CS_SCALE_4:   return 8;
      case GX_CS_DIVIDE_2:  return 1;
    }
    
    OSHalt("Incorrect Tev scale");
    return 0;
}

/*---------------------------------------------------------------------------*

    State check functions

 *---------------------------------------------------------------------------*/
void TECheckTexSwap( TETevStage* stg )
{
#define IsTexC(arg)                                 \
    ( arg == GX_CC_TEXC   || arg == GX_CC_TEXRRR || \
      arg == GX_CC_TEXGGG || arg == GX_CC_TEXBBB )

    GXTevColorArg  tca;
    s32  i;
    
    for ( i = 3 ; i >= 0 ; --i )
    {
        if ( IsTexC(stg->colorIn[i]) )
        {
            tca = stg->colorIn[i];
            break;
        }
    }

    for ( ; i >= 0 ; --i )
    {
        if ( IsTexC(stg->colorIn[i]) && ( stg->colorIn[i] != tca ) )
        {
            ASSERTMSG(0, TE_TEXSWAP_ERRMSG); // Debug mode fails
            stg->colorIn[i] = tca;           // for non-debug mode
        }
    }

#undef IsTexC
}

/*---------------------------------------------------------------------------*

    Tev state control functions (similar to GX function set)

 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*

 *---------------------------------------------------------------------------*/
void TESetTevColorIn( GXTevStageID stage, GXTevColorArg a,
                      GXTevColorArg b, GXTevColorArg c, GXTevColorArg d )
{
    s32 i = TETranslateTevStageID(stage);

    TevSt.stage[i].colorIn[0] = a;
    TevSt.stage[i].colorIn[1] = b;
    TevSt.stage[i].colorIn[2] = c;
    TevSt.stage[i].colorIn[3] = d;

    TECheckTexSwap(&TevSt.stage[i]);
}

/*---------------------------------------------------------------------------*

 *---------------------------------------------------------------------------*/
void TESetTevAlphaIn( GXTevStageID stage, GXTevAlphaArg a,
                      GXTevAlphaArg b, GXTevAlphaArg c, GXTevAlphaArg d )
{
    s32 i = TETranslateTevStageID(stage);

    TevSt.stage[i].alphaIn[0] = a;
    TevSt.stage[i].alphaIn[1] = b;
    TevSt.stage[i].alphaIn[2] = c;
    TevSt.stage[i].alphaIn[3] = d;
}

/*---------------------------------------------------------------------------*

 *---------------------------------------------------------------------------*/
void TESetTevColorOp( GXTevStageID stage, GXTevOp op, GXTevBias bias,
                      GXTevScale scale, GXBool clamp, GXTevRegID out_reg )
{
    s32 i = TETranslateTevStageID(stage);
    
    TevSt.stage[i].op[0]     = TETranslateTevOp(op);
    TevSt.stage[i].bias[0]   = TETranslateTevBias(bias);
    TevSt.stage[i].scale[0]  = TETranslateTevScale(scale);
    TevSt.stage[i].clamp[0]  = clamp;
    TevSt.stage[i].outReg[0] = out_reg;
}

/*---------------------------------------------------------------------------*

 *---------------------------------------------------------------------------*/
void TESetTevAlphaOp( GXTevStageID stage, GXTevOp op, GXTevBias bias,
                      GXTevScale scale, GXBool clamp, GXTevRegID out_reg )
{
    s32 i = TETranslateTevStageID(stage);
    
    TevSt.stage[i].op[1]     = TETranslateTevOp(op);
    TevSt.stage[i].bias[1]   = TETranslateTevBias(bias);
    TevSt.stage[i].scale[1]  = TETranslateTevScale(scale);
    TevSt.stage[i].clamp[1]  = clamp;
    TevSt.stage[i].outReg[1] = out_reg;
}

/*---------------------------------------------------------------------------*

 *---------------------------------------------------------------------------*/
void TESetTevOp( GXTevStageID id, GXTevMode mode )
{
	GXTevColorArg carg = GX_CC_RASC;
	GXTevAlphaArg aarg = GX_CA_RASA;

    if (id != GX_TEVSTAGE0)
    {
        carg = GX_CC_CPREV;
        aarg = GX_CA_APREV;
    }

    switch (mode)
    {
      case GX_MODULATE:
        TESetTevColorIn(id, GX_CC_ZERO, GX_CC_TEXC, carg, GX_CC_ZERO);
        TESetTevAlphaIn(id, GX_CA_ZERO, GX_CA_TEXA, aarg, GX_CA_ZERO);
        break;
      case GX_DECAL:
        TESetTevColorIn(id, carg, GX_CC_TEXC, GX_CC_TEXA, GX_CC_ZERO);
        TESetTevAlphaIn(id, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, aarg);
        break;
      case GX_BLEND:
        TESetTevColorIn(id, carg, GX_CC_ONE, GX_CC_TEXC, GX_CC_ZERO);
        TESetTevAlphaIn(id, GX_CA_ZERO, GX_CA_TEXA, aarg, GX_CA_ZERO);
        break;
      case GX_REPLACE:
        TESetTevColorIn(id, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        TESetTevAlphaIn(id, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        break;
      case GX_PASSCLR:
        TESetTevColorIn(id, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, carg);
        TESetTevAlphaIn(id, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, aarg);
        break;
      default:
        OSHalt("Invalid Tev mode");
        break;
    }
    
    TESetTevColorOp(id, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    TESetTevAlphaOp(id, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
}

/*---------------------------------------------------------------------------*

 *---------------------------------------------------------------------------*/
void TESetTevColor( GXTevRegID id, GXColor color )
{
    s32 i = TETranslateTevRegID(id);
    
    // Convert U8 data into S16
    TevSt.reg[i].r = (s16)color.r;
    TevSt.reg[i].g = (s16)color.g;
    TevSt.reg[i].b = (s16)color.b;
    TevSt.reg[i].a = (s16)color.a;
}

/*---------------------------------------------------------------------------*

 *---------------------------------------------------------------------------*/
void TESetTevColorS10( GXTevRegID id, GXColorS10 color )
{
    s32 i = TETranslateTevRegID(id);

    TevSt.reg[i] = color;
}

/*---------------------------------------------------------------------------*

 *---------------------------------------------------------------------------*/
void TESetTevClampMode( GXTevStageID stage, GXTevClampMode mode )
{
    s32 i = TETranslateTevStageID(stage);

    TevSt.stage[i].clampMode = mode;
}

/*---------------------------------------------------------------------------*

 *---------------------------------------------------------------------------*/
void TESetAlphaCompare( GXCompare comp0, u8 ref0, GXAlphaOp op,
                        GXCompare comp1, u8 ref1 )
{
    TevSt.alphaOp      = op;
    TevSt.alphaComp[0] = comp0;
    TevSt.alphaComp[1] = comp1;
    TevSt.alphaRef[0]  = ref0;
    TevSt.alphaRef[1]  = ref1;
}

/*---------------------------------------------------------------------------*

 *---------------------------------------------------------------------------*/
void TESetNumTevStages( u8 nStages )
{
    if ( nStages < 1 || nStages > MAX_STAGES )
    {
        OSHalt("Invalid number of Tev stages");
    }

    TevSt.numStages = nStages;
}

/*---------------------------------------------------------------------------*

 *---------------------------------------------------------------------------*/
/*
void TESetTevOrder( GXTevStageID stage, GXTexCoordID coord,
                    GXTexMapID map, GXChannelID color )
{
#pragma unused(stage)
#pragma unused(coord)
#pragma unused(map)
#pragma unused(color)
}
*/

/*---------------------------------------------------------------------------*

    Color combining functions

 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*

 *---------------------------------------------------------------------------*/
void TESelectInput( GXColorS10* color, s32 stgNum, s32 arg )
{
    // Get color components
    switch(TevSt.stage[stgNum].colorIn[arg])
    {
      case GX_CC_C0:
        {
            *color = TevSt.reg[0];
        } break;
      case GX_CC_C1:
        {
            *color = TevSt.reg[1];
        } break;
      case GX_CC_C2:
        {
            *color = TevSt.reg[2];
        } break;
      case GX_CC_CPREV:
        {
            *color = TevSt.reg[3];
        } break;
      case GX_CC_A0:
        {
            color->r = color->g = color->b = TevSt.reg[0].a;
        } break;
      case GX_CC_A1:
        {
            color->r = color->g = color->b = TevSt.reg[1].a;
        } break;
      case GX_CC_A2:
        {
            color->r = color->g = color->b = TevSt.reg[2].a;
        } break;
      case GX_CC_APREV:
        {
            color->r = color->g = color->b = TevSt.reg[3].a;
        } break;
      case GX_CC_TEXC:
        {
            color->r = (s16)TevSt.input[stgNum].texIn.r;
            color->g = (s16)TevSt.input[stgNum].texIn.g;
            color->b = (s16)TevSt.input[stgNum].texIn.b;
        } break;
      case GX_CC_TEXA:
        {
            color->r = (s16)TevSt.input[stgNum].texIn.a;
            color->g = (s16)TevSt.input[stgNum].texIn.a;
            color->b = (s16)TevSt.input[stgNum].texIn.a;
        } break;
      case GX_CC_RASC:
        {
            color->r = (s16)TevSt.input[stgNum].rasIn.r;
            color->g = (s16)TevSt.input[stgNum].rasIn.g;
            color->b = (s16)TevSt.input[stgNum].rasIn.b;
        } break;
      case GX_CC_RASA:
        {
            color->r = (s16)TevSt.input[stgNum].rasIn.a;
            color->g = (s16)TevSt.input[stgNum].rasIn.a;
            color->b = (s16)TevSt.input[stgNum].rasIn.a;
        } break;
      case GX_CC_ONE:
        {
            color->r = color->g = color->b = 255;
        } break;    
      case GX_CC_HALF:
        {
            color->r = color->g = color->b = 128;
        } break;    
      case GX_CC_QUARTER:
        {
            color->r = color->g = color->b = 64;
        } break;    
      case GX_CC_ZERO:
        {
            color->r = color->g = color->b = 0;
        } break;    
      case GX_CC_TEXRRR:
        {
            color->r = (s16)TevSt.input[stgNum].texIn.r;
            color->g = (s16)TevSt.input[stgNum].texIn.r;
            color->b = (s16)TevSt.input[stgNum].texIn.r;
        } break;
      case GX_CC_TEXGGG:
        {
            color->r = (s16)TevSt.input[stgNum].texIn.g;
            color->g = (s16)TevSt.input[stgNum].texIn.g;
            color->b = (s16)TevSt.input[stgNum].texIn.g;
        } break;
      case GX_CC_TEXBBB:
        {
            color->r = (s16)TevSt.input[stgNum].texIn.b;
            color->g = (s16)TevSt.input[stgNum].texIn.b;
            color->b = (s16)TevSt.input[stgNum].texIn.b;
        } break;
    }

    // Get alpha component
    switch(TevSt.stage[stgNum].alphaIn[arg])
    {
      case GX_CA_A0:
        {
            color->a = TevSt.reg[0].a;
        } break;
      case GX_CA_A1:
        {
            color->a = TevSt.reg[1].a;
        } break;
      case GX_CA_A2:
        {
            color->a = TevSt.reg[2].a;
        } break;
      case GX_CA_APREV:
        {
            color->a = TevSt.reg[3].a;
        } break;
      case GX_CA_TEXA:
        {
            color->a = (s16)TevSt.input[stgNum].texIn.a;
        } break;
      case GX_CA_RASA:
        {
            color->a = (s16)TevSt.input[stgNum].rasIn.a;
        } break;
      case GX_CA_ONE:
        {
            color->a = 255;
        } break;    
      case GX_CA_ZERO:
        {
            color->a = 0;
        } break;    
    }

    // Argument A,B,C are truncated into U8
    if ( arg < 3 )
    {
        color->r = (s16)(color->r & 0xFF);
        color->g = (s16)(color->g & 0xFF);
        color->b = (s16)(color->b & 0xFF);
        color->a = (s16)(color->a & 0xFF);
    }
}

/*---------------------------------------------------------------------------*

 *---------------------------------------------------------------------------*/
s16 TEClamp( s16 val, GXTevClampMode mode, GXBool sw )
{
    s32  cv, v0, v1;

    switch(mode)
    {
      case GX_TC_LINEAR:
        {
            v0 = sw ? 0 : -1024;
            v1 = sw ? 255 : 1023;
            cv  = ( val < v0 ) ? v0 : ( val > v1 ) ? v1 : val;  
        } break;
      case GX_TC_GE:
        {
            v0 = sw ? 0 : 255;
            v1 = sw ? 255 : 0;
            cv = ( val >= 0 ) ? v0 : v1;
        } break;
      case GX_TC_EQ:
        {
            v0 = sw ? 0 : 255;
            v1 = sw ? 255 : 0;
            cv = ( val == 0 ) ? v0 : v1;
        } break;
      case GX_TC_LE:
        {
            v0 = sw ? 0 : 255;
            v1 = sw ? 255 : 0;
            cv = ( val <= 0 ) ? v0 : v1;
        } break;
    }

    return (s16)cv;
}

/*---------------------------------------------------------------------------*

 *---------------------------------------------------------------------------*/
void TEPerformOneStage( s32 stgNum )
{
    GXColorS10   cwork[11];
    s32          i;
    TETevStage*  ts = &TevSt.stage[stgNum];

    // cwork[0] = A, cwork[1] = B, cwork[2] = C, cwork[3] = D 
    for ( i = 0 ; i < 4 ; ++i )
    {
        TESelectInput(&cwork[i], stgNum, i);
    }

    // cwork[4] = 1 - C
    cwork[4].r = (s16)(255 - cwork[2].r);
    cwork[4].g = (s16)(255 - cwork[2].g);
    cwork[4].b = (s16)(255 - cwork[2].b);
    cwork[4].a = (s16)(255 - cwork[2].a);
    
    // cwork[5] = A(1 - C)
    cwork[5].r = (s16)(cwork[0].r * cwork[4].r / 255);
    cwork[5].g = (s16)(cwork[0].g * cwork[4].g / 255);
    cwork[5].b = (s16)(cwork[0].b * cwork[4].b / 255);
    cwork[5].a = (s16)(cwork[0].a * cwork[4].a / 255);
    
    // cwork[6] = BC
    cwork[6].r = (s16)(cwork[1].r * cwork[2].r / 255);
    cwork[6].g = (s16)(cwork[1].g * cwork[2].g / 255);
    cwork[6].b = (s16)(cwork[1].b * cwork[2].b / 255);
    cwork[6].a = (s16)(cwork[1].a * cwork[2].a / 255);
    
    // cwork[7] = op ( A(1 - C) + BC )
    cwork[7].r = (s16)(ts->op[0] * (cwork[5].r + cwork[6].r));
    cwork[7].g = (s16)(ts->op[0] * (cwork[5].g + cwork[6].g));
    cwork[7].b = (s16)(ts->op[0] * (cwork[5].b + cwork[6].b));
    cwork[7].a = (s16)(ts->op[1] * (cwork[5].a + cwork[6].a));

    // cwork[8] = D op (A(1-C)+BC) + bias
    cwork[8].r = (s16)(cwork[3].r + cwork[7].r + ts->bias[0]);
    cwork[8].g = (s16)(cwork[3].g + cwork[7].g + ts->bias[0]);
    cwork[8].b = (s16)(cwork[3].b + cwork[7].b + ts->bias[0]);
    cwork[8].a = (s16)(cwork[3].a + cwork[7].a + ts->bias[1]);

    // cwork[9] = (D op (A(1-C)+BC) + bias) * scale
    cwork[9].r = (s16)((cwork[8].r * ts->scale[0]) / 2 );
    cwork[9].g = (s16)((cwork[8].g * ts->scale[0]) / 2 );
    cwork[9].b = (s16)((cwork[8].b * ts->scale[0]) / 2 );
    cwork[9].a = (s16)((cwork[8].a * ts->scale[1]) / 2 );

    // clamp
    cwork[10].r = TEClamp(cwork[9].r, ts->clampMode, ts->clamp[0]);
    cwork[10].g = TEClamp(cwork[9].g, ts->clampMode, ts->clamp[0]);
    cwork[10].b = TEClamp(cwork[9].b, ts->clampMode, ts->clamp[0]);
    cwork[10].a = TEClamp(cwork[9].a, ts->clampMode, ts->clamp[1]);
    
    // output
    i = TETranslateTevRegID(ts->outReg[0]);
    TevSt.reg[i].r = cwork[10].r;
    TevSt.reg[i].g = cwork[10].g;
    TevSt.reg[i].b = cwork[10].b;
    i = TETranslateTevRegID(ts->outReg[1]);
    TevSt.reg[i].a = cwork[10].a;
}

/*---------------------------------------------------------------------------*

    Pixel level emulation API

 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*

 *---------------------------------------------------------------------------*/
static void __TESetStageInput( s32 stage, GXColor* rasColor, GXColor* texColor )
{
    TevSt.input[stage].rasIn = *rasColor;
    TevSt.input[stage].texIn = *texColor;
}

void TESetStageInput( GXTevStageID stage, GXColor* rasColor, GXColor* texColor )
{
    __TESetStageInput(TETranslateTevStageID(stage), rasColor, texColor);
}

/*---------------------------------------------------------------------------*

 *---------------------------------------------------------------------------*/
void TECombineOnePixel( GXColor* result )
{
    s32  i;
    
    for ( i = 0 ; i < TevSt.numStages ; ++i )
    {
        TEPerformOneStage(i);
    }

    result->r = (u8)(TevSt.reg[3].r);
    result->g = (u8)(TevSt.reg[3].g);
    result->b = (u8)(TevSt.reg[3].b);
    result->a = (u8)(TevSt.reg[3].a);
}

/*---------------------------------------------------------------------------*

    Image level emulation API

 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*

 *---------------------------------------------------------------------------*/
void TEImgInit( void )
{
    s32  i;

    for ( i = 0 ; i < MAX_STAGES ; ++i )
    {
        ImgHolder.rasImg[i] = NULL;
        ImgHolder.texImg[i] = NULL;
    }
    TEImgSetSize(64, 64);
}

/*---------------------------------------------------------------------------*

 *---------------------------------------------------------------------------*/
void TEImgSetStageInput( GXTevStageID stage, void* rasImg, void* texImg )
{
    s32  i = TETranslateTevStageID(stage);

    ImgHolder.rasImg[i] = (u8*)rasImg;
    ImgHolder.texImg[i] = (u8*)texImg;
}

/*---------------------------------------------------------------------------*

 *---------------------------------------------------------------------------*/
void TEImgSetSize( u16 width, u16 height )
{
    ImgHolder.width  = width;
    ImgHolder.height = height;
}

/*---------------------------------------------------------------------------*

 *---------------------------------------------------------------------------*/
void TEImgCombine( void* resultImg )
{
    s32  i;
    u32  bufSize, tileBase, offset;
    u8*  inPtr;
    u8*  outPtr;
    GXColor  rc, tc, cc;
    
    bufSize = GXGetTexBufferSize(
                  ImgHolder.width,
                  ImgHolder.height,
                  GX_TF_RGBA8,      // All images should be RGBA8 texture
                  GX_FALSE,         // format with no mipmap.
                  0 );
    tileBase = 0;
    
    while ( tileBase < bufSize )
    {
        for ( offset = 0 ; offset < 32 ; offset += 2 )
        {
            for ( i = 0 ; i < TevSt.numStages ; ++i )
            {
                // set rasterized pixel color input
                inPtr = ImgHolder.rasImg[i] + tileBase + offset;
                rc.a = *inPtr;
                rc.r = *(inPtr+1);
                rc.g = *(inPtr+32);
                rc.b = *(inPtr+33);
                
                // set looked up texel color input
                inPtr = ImgHolder.texImg[i] + tileBase + offset;
                tc.a = *inPtr;
                tc.r = *(inPtr+1);
                tc.g = *(inPtr+32);
                tc.b = *(inPtr+33);
                
                // process one Tev operation
                __TESetStageInput(i, &rc, &tc);
                TECombineOnePixel(&cc);
                
                // write combined color into result image
                outPtr = (u8*)resultImg + tileBase + offset;
                *outPtr      = cc.a;
                *(outPtr+1)  = cc.r;
                *(outPtr+32) = cc.g;
                *(outPtr+33) = cc.b;
            }
        }
        tileBase += 64; // AR tile + GB tile
    }
}

/*---------------------------------------------------------------------------*/
#endif  // EPPC

/*============================================================================*/
