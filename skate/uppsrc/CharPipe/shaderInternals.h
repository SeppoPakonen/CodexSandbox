/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     shaderInternals.h

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/shader/include/shaderInternals.h $
    
    4     7/19/00 3:59p Ryan
    update for precompiled shaders
    
    3     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
    2     2/28/00 3:50p Ryan
    optimization update for shader lib
    
    1     2/28/00 11:23a Ryan

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#ifndef __SHADERINTERNALS_H__
#define __SHADERINTERNALS_H__

#if 0
extern "C" {
#endif

/*---------------------------------------------------------------------------*/

#define NO_TEXTURE_COORD_EXP    99

/*---------------------------------------------------------------------------*/
typedef enum
{
	SHADER_REG_EMPTY, 
	SHADER_REG_CONSTANTCOLOR,
	SHADER_REG_INUSE

} SHADER_RegisterState;

/*---------------------------------------------------------------------------*/
typedef enum
{
	SHADER_TEV_COLOR,
	SHADER_TEV_ALPHA

} SHADER_TEVStageChannel;

/*---------------------------------------------------------------------------*/
typedef struct
{
	SHADER_RegisterState	regColorState[4];
	SHADER_ColorInput		regColor[4];
	SHADER_TEVArg			colorIdList[4];
	SHDRShader				*colorSrc[4];
	u8						colorLifetime[4];

	SHADER_RegisterState	regAlphaState[4];
	SHADER_ColorInput		regAlpha[4];
	SHADER_TEVArg			alphaIdList[4];
	SHDRShader				*alphaSrc[4];
	u8						alphaLifetime[4];

	GXTexObj				*texObj[8];
	GXTexMapID				mapIdList[8];

} ShaderResources;

/*---------------------------------------------------------------------------*/
typedef struct
{
	SHADER_ColorInput	regColor[4];
	SHADER_ColorInput   regAlpha[4];

    u8                  colorUsed[4];
    GXColor             colorData[4];

    u8                  textureUsed[8];
    GXTexObj		   *textureData[8];

    u8                  rasUsed[2];
    GXChannelID         rasData[2];

    u8                  complexUsed[8];
    SHDRCompiled       *complexData[8];


} CompiledShaderResources;

/*---------------------------------------------------------------------------*/
typedef enum 
{
	SHADER_TRIVIAL,
	SHADER_SIMPLE,
	SHADER_LERP

} SHADER_LERPTypes;

/*---------------------------------------------------------------------------*/
typedef struct ShaderTEVStage
{
	SHADER_TEVStageChannel	channel;
	SHADER_TYPE				type;

	struct ShaderTEVStage  *parent[8];
	u8                      numParents;
	u8                      numNonAllocatedChildren;	//used in tree collapse stage

	SHADER_TEVArg           TEVArg[4];
	struct ShaderTEVStage  *tevStage[4];
	
    GXTevOp                 op;
    GXTevBias               bias;
    GXTevScale              scale;
    GXBool                  clamp;
	GXTevClampMode	        mode;
    SHADER_TEVArg           out_reg;

    SHADER_RasInput         rasInput;
    u8                      texGenIdx;
    SHADER_TexInput         texInput;
    
	//GXTexCoordID            coord;
    //GXTexMapID              map;
    //GXChannelID             color;

	SHADER_LERPTypes	    LERPType;
	u8					    outputIndex;
	SHADER_TEVArg		    outputArg;

} ShaderTEVStage;

/*---------------------------------------------------------------------------*/
typedef struct
{
	SHADER_TEVArg           TEVArg[4];
	
    GXTevOp                 op;
    GXTevBias               bias;
    GXTevScale              scale;
    GXBool                  clamp;
	GXTevClampMode	        mode;
    SHADER_TEVArg           out_reg;

    SHADER_RasInput         rasInput;
    u8                      texGenIdx;
    SHADER_TexInput         texInput;
    
} CompiledTEVStage;

/*---------------------------------------------------------------------------*/

#if 0
}
#endif

#endif