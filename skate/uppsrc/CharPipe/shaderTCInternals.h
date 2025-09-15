/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     shaderTCInternals.h

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/shader/include/shaderTCInternals.h $
    
    2     7/19/00 3:59p Ryan
    update for precompiled shaders
    
    1     4/12/00 10:20p Ryan
    
   
  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#ifndef __SHADERTCINTERNALS_H__
#define __SHADERTCINTERNALS_H__

#if 0
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
#include <CharPipe/shader.h>

/*---------------------------------------------------------------------------*/
typedef struct TexCoordExp
{
    ShaderTexGenSrc     genSrc;
    u8                  srcShaderIdx;

    ShaderTexGenType    type;
    SHADER_MTXInput     mtxInput;

} TexCoordExp;

/*---------------------------------------------------------------------------*/
typedef struct
{
    TexCoordExp *expressionArray;
    u8           numExpressions;

    void        *mtxArray[8];
    u8           mtxUsed[8];

} OptimizedTexCoordExpressions;

/*---------------------------------------------------------------------------*/
typedef struct
{
    GXTexCoordID    coord[8];
    SHDRTexCoord    *tcShader[8];
    SHDRShader      *textureShader[8];

} ShaderTexCoordResources;

/*---------------------------------------------------------------------------*/
void CompileTexGen  ( SHDRShader *shader,  OptimizedTexCoordExpressions **exp);
void SetTCGenState  ( SHDRCompiled *shader );

/*---------------------------------------------------------------------------*/
#if 0
}
#endif

#endif