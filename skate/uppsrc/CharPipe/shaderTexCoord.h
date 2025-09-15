/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     shaderTexCoord.h

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/charPipeline/shader/shaderTexCoord.h $
    
    2     7/19/00 4:05p Ryan
    Update to work with precompiled shaders
    
    1     4/12/00 10:20p Ryan

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#ifndef __SHADERTEXCOORD_H__
#define __SHADERTEXCOORD_H__

#if 0
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
#include <Dolphin/mtx.h>

/*---------------------------------------------------------------------------*/
typedef struct SHDRTexCoord SHDRTexCoord, *SHDRTexCoordPtr;

/*---------------------------------------------------------------------------*/
typedef enum
{
    SHADER_TG_POS = GX_TG_POS, 
    SHADER_TG_NRM = GX_TG_NRM, 
    SHADER_TG_BINRM = GX_TG_BINRM, 
    SHADER_TG_TANGENT = GX_TG_TANGENT, 
    SHADER_TG_TEX0 = GX_TG_TEX0, 
    SHADER_TG_TEX1 = GX_TG_TEX1, 
    SHADER_TG_TEX2 = GX_TG_TEX2, 
    SHADER_TG_TEX3 = GX_TG_TEX3, 
    SHADER_TG_TEX4 = GX_TG_TEX4, 
    SHADER_TG_TEX5 = GX_TG_TEX5, 
    SHADER_TG_TEX6 = GX_TG_TEX6, 
    SHADER_TG_TEX7 = GX_TG_TEX7, 
    SHADER_TG_COLOR0 = GX_TG_COLOR0, 
    SHADER_TG_COLOR1 = GX_TG_COLOR1

} ShaderTexGenSrc;

/*---------------------------------------------------------------------------*/
typedef enum
{
    SHADER_TG_MTX3x4 = GX_TG_MTX3x4, 
    SHADER_TG_MTX2x4 = GX_TG_MTX2x4,
    SHADER_TG_BUMP0 = GX_TG_BUMP0, 
    SHADER_TG_BUMP1 = GX_TG_BUMP1, 
    SHADER_TG_BUMP2 = GX_TG_BUMP2, 
    SHADER_TG_BUMP3 = GX_TG_BUMP3, 
    SHADER_TG_BUMP4 = GX_TG_BUMP4, 
    SHADER_TG_BUMP5 = GX_TG_BUMP5, 
    SHADER_TG_BUMP6 = GX_TG_BUMP6, 
    SHADER_TG_BUMP7 = GX_TG_BUMP7,
    SHADER_TG_SRTG = GX_TG_SRTG

} ShaderTexGenType;

/*---------------------------------------------------------------------------*/
typedef enum
{
    SHADER_MTX0 = 0,
    SHADER_MTX1 = 1,
    SHADER_MTX2 = 2,
    SHADER_MTX3 = 3,
    SHADER_MTX4 = 4,
    SHADER_MTX5 = 5,
    SHADER_MTX6 = 6,
    SHADER_MTX7 = 7,
    SHADER_IDENTITY = 8
    
} SHADER_MTXInput;

/*---------------------------------------------------------------------------*/
struct SHDRTexCoord
{
    s16				    referenceCount;

    ShaderTexGenSrc     genSrc;
    SHDRTexCoord        *shaderSrc;

    ShaderTexGenType    type;
    SHADER_MTXInput     mtxInput;

    u8                  texCoordExpIdx;
};

/*---------------------------------------------------------------------------*/
SHDRTexCoord *SHDRCreateTexCoordExpression ( ShaderTexGenSrc src,
                                             SHDRTexCoord *shadSrc,
                                             ShaderTexGenType texGenType, 
                                             SHADER_MTXInput mtxInput );

SHDRTexCoord *SHDRCreateTCPassThrough   ( ShaderTexGenSrc src );

void SHDRFreeTC ( SHDRTexCoord *texCoord );

/*---------------------------------------------------------------------------*/
#if 0
}
#endif

#endif