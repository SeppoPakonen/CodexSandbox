/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     shaderResManage.h

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/shader/include/shaderResManage.h $
    
    4     7/19/00 3:59p Ryan
    update for precompiled shaders
    
    3     3/06/00 2:33p Ryan
    Optimization update 3/6/00
    
    2     2/28/00 3:50p Ryan
    optimization update for shader lib
    
    1     2/28/00 11:23a Ryan

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#ifndef __SHADERRESMANAGE_H__
#define __SHADERRESMANAGE_H__

#if 0
extern "C" {
#endif

/*---------------------------------------------------------------------------*/

#include <CharPipe/shader.h>
#include "shaderInternals.h"

/*---------------------------------------------------------------------------*/
extern ShaderTEVStage	TEVPool[50];
extern u32				TEVCounter;

extern ShaderTEVStage	*AlphaInstructions[16];
extern ShaderTEVStage	*ColorInstructions[16];

extern ShaderTEVStage	BestAlphaInstructions[16];
extern ShaderTEVStage	BestColorInstructions[16];
extern u8				BestNumTEVStages;

extern ShaderResources  Resources;

/*---------------------------------------------------------------------------*/

void ResetShaderResources	( void );

SHADER_TEVArg AllocateColorResource ( SHADER_ColorInput color );
SHADER_TEVArg AllocateAlphaResource ( SHADER_ColorInput color );
GXTexMapID AllocateTextureResource ( GXTexObj *tex );

u8	CheckResourceCollision	( ShaderTEVStage *stage, ShaderTEVStage *parent );

u8 CheckResourceAllocation	( void );

void InitCompiledResourceInfo    ( CompiledShaderResources *res );

/*---------------------------------------------------------------------------*/
#if 0
}
#endif

#endif