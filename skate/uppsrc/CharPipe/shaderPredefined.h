/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     shaderPredefined.h

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/shader/include/shaderPredefined.h $
    
    1     7/19/00 4:04p Ryan

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#ifndef __SHADERPREDEFINED_H__
#define __SHADERPREDEFINED_H__

#if 0
extern "C" {
#endif

/*---------------------------------------------------------------------------*/

//#include <CharPipe/shader.h>

/*---------------------------------------------------------------------------*/
static CompiledTEVStage
ModulateColorTEVStage =    {  
                                SHADER_ZERO,    //TEVArg[0]
                                SHADER_TEXC,    //TEVArg[1]
                                SHADER_RASC,    //TEVArg[2]
                                SHADER_ZERO,    //TEVArg[3]
                                GX_TEV_ADD,     //TEV op
                                GX_TB_ZERO,     //TEV Bias
                                GX_CS_SCALE_1,  //TEV Scale
                                GX_TRUE,        //TEV Clamp
                                GX_TC_LINEAR,   //TEV Clamp Mode
                                SHADER_CPREV,   //Out Reg
                                SHADER_RAS0,    //RAS input
                                0,              //Tex Gen Indx
                                SHADER_TEX0     //Tex Input
                            };

static CompiledTEVStage 
ModulateAlphaTEVStage =    {  
                                SHADER_ZERO,    //TEVArg[0]
                                SHADER_TEXA,    //TEVArg[1]
                                SHADER_RASA,    //TEVArg[2]
                                SHADER_ZERO,    //TEVArg[3]
                                GX_TEV_ADD,     //TEV op
                                GX_TB_ZERO,     //TEV Bias
                                GX_CS_SCALE_1,  //TEV Scale
                                GX_TRUE,        //TEV Clamp
                                GX_TC_LINEAR,   //TEV Clamp Mode
                                SHADER_APREV,   //Out Reg
                                SHADER_RAS0,    //RAS input
                                0,              //Tex Gen Indx
                                SHADER_TEX0     //Tex Input
                            };

static CompiledShaderResources
ModulateResources   =       {
                                SHADER_COLOR_NONE,  //regColor[0]
                                SHADER_COLOR_NONE,  //regColor[1]
                                SHADER_COLOR_NONE,  //regColor[2]
                                SHADER_COLOR_NONE,  //regColor[3]

                                SHADER_COLOR_NONE,  //regAlpha[0]
                                SHADER_COLOR_NONE,  //regAlpha[1]
                                SHADER_COLOR_NONE,  //regAlpha[2]
                                SHADER_COLOR_NONE,  //regAlpha[3]

                                0, 0, 0, 0,         //colorUsed

                                0, 0, 0, 0,         //colorData[0]
                                0, 0, 0, 0,         //colorData[1]
                                0, 0, 0, 0,         //colorData[2]
                                0, 0, 0, 0,         //colorData[3]

                                1, 0, 0, 0, 0, 0, 0, 0, //textureUsed
                                0, 0, 0, 0, 0, 0, 0, 0, //textureData

                                1, 0,               //rasUsed
                                GX_COLOR_NULL,      //rasData[0] 
                                GX_COLOR_NULL,      //rasData[1]

                                0, 0, 0, 0, 0, 0, 0, 0, //complexUsed
                                0, 0, 0, 0, 0, 0, 0, 0  //complexData
                            };

static TexCoordExp
ModulateTexCoord    =       {
                                SHADER_TG_TEX0,     //genSrc
                                99,                 //srcShaderIdx
                                SHADER_TG_MTX2x4,   //type
                                SHADER_IDENTITY     //mtxInput
                            };

static OptimizedTexCoordExpressions
ModulateTexCoordExp =       {
                                &ModulateTexCoord,      //expressionArray
                                1,                      //num expressions
                                0, 0, 0, 0, 0, 0, 0, 0, //mtx array
                                0, 0, 0, 0, 0, 0, 0, 0  //mtxUsed
                            };  

static SHDRCompiled 
ModulateShader      =       {
                                (void *)(&ModulateColorTEVStage),   //color stages
                                (void *)(&ModulateAlphaTEVStage),   //alpha stages
                                1,                                  //num stages
                                (void *)(&ModulateResources),       //shader resources
                                (void *)(&ModulateTexCoordExp)      //texGen
                            };

/*---------------------------------------------------------------------------*/
static CompiledTEVStage 
ReplaceColorTEVStage =    {  
                                SHADER_TEXC,    //TEVArg[0]
                                SHADER_ZERO,    //TEVArg[1]
                                SHADER_ZERO,    //TEVArg[2]
                                SHADER_ZERO,    //TEVArg[3]
                                GX_TEV_ADD,     //TEV op
                                GX_TB_ZERO,     //TEV Bias
                                GX_CS_SCALE_1,  //TEV Scale
                                GX_TRUE,        //TEV Clamp
                                GX_TC_LINEAR,   //TEV Clamp Mode
                                SHADER_CPREV,   //Out Reg
                                SHADER_RASNONE, //RAS input
                                0,              //Tex Gen Indx
                                SHADER_TEX0     //Tex Input
                            };

static CompiledTEVStage 
ReplaceAlphaTEVStage =    {  
                                SHADER_TEXA,    //TEVArg[0]
                                SHADER_ZERO,    //TEVArg[1]
                                SHADER_ZERO,    //TEVArg[2]
                                SHADER_ZERO,    //TEVArg[3]
                                GX_TEV_ADD,     //TEV op
                                GX_TB_ZERO,     //TEV Bias
                                GX_CS_SCALE_1,  //TEV Scale
                                GX_TRUE,        //TEV Clamp
                                GX_TC_LINEAR,   //TEV Clamp Mode
                                SHADER_APREV,   //Out Reg
                                SHADER_RASNONE, //RAS input
                                0,              //Tex Gen Indx
                                SHADER_TEX0     //Tex Input
                            };

static CompiledShaderResources
ReplaceResources   =       {
                                SHADER_COLOR_NONE,  //regColor[0]
                                SHADER_COLOR_NONE,  //regColor[1]
                                SHADER_COLOR_NONE,  //regColor[2]
                                SHADER_COLOR_NONE,  //regColor[3]

                                SHADER_COLOR_NONE,  //regAlpha[0]
                                SHADER_COLOR_NONE,  //regAlpha[1]
                                SHADER_COLOR_NONE,  //regAlpha[2]
                                SHADER_COLOR_NONE,  //regAlpha[3]

                                0, 0, 0, 0,         //colorUsed

                                0, 0, 0, 0,         //colorData[0]
                                0, 0, 0, 0,         //colorData[1]
                                0, 0, 0, 0,         //colorData[2]
                                0, 0, 0, 0,         //colorData[3]

                                1, 0, 0, 0, 0, 0, 0, 0, //textureUsed
                                0, 0, 0, 0, 0, 0, 0, 0, //textureData

                                0, 0,               //rasUsed
                                GX_COLOR_NULL,      //rasData[0] 
                                GX_COLOR_NULL,      //rasData[1]

                                0, 0, 0, 0, 0, 0, 0, 0, //complexUsed
                                0, 0, 0, 0, 0, 0, 0, 0  //complexData
                            };

static TexCoordExp
ReplaceTexCoord    =       {
                                SHADER_TG_TEX0,     //genSrc
                                99,                 //srcShaderIdx
                                SHADER_TG_MTX2x4,   //type
                                SHADER_IDENTITY     //mtxInput
                            };

static OptimizedTexCoordExpressions
ReplaceTexCoordExp =       {
                                &ReplaceTexCoord,      //expressionArray
                                1,                      //num expressions
                                0, 0, 0, 0, 0, 0, 0, 0, //mtx array
                                0, 0, 0, 0, 0, 0, 0, 0  //mtxUsed
                            };  

static SHDRCompiled 
ReplaceShader      =       {
                                (void *)(&ReplaceColorTEVStage),   //color stages
                                (void *)(&ReplaceAlphaTEVStage),   //alpha stages
                                1,                                 //num stages
                                (void *)(&ReplaceResources),       //shader resources
                                (void *)(&ReplaceTexCoordExp)      //texGen
                            };

/*---------------------------------------------------------------------------*/
static CompiledTEVStage 
PassThruColorTEVStage =    {  
                                SHADER_RASC,    //TEVArg[0]
                                SHADER_ZERO,    //TEVArg[1]
                                SHADER_ZERO,    //TEVArg[2]
                                SHADER_ZERO,    //TEVArg[3]
                                GX_TEV_ADD,     //TEV op
                                GX_TB_ZERO,     //TEV Bias
                                GX_CS_SCALE_1,  //TEV Scale
                                GX_TRUE,        //TEV Clamp
                                GX_TC_LINEAR,   //TEV Clamp Mode
                                SHADER_CPREV,   //Out Reg
                                SHADER_RAS0,    //RAS input
                                0,              //Tex Gen Indx
                                SHADER_TEXNONE  //Tex Input
                            };

static CompiledTEVStage 
PassThruAlphaTEVStage =    {  
                                SHADER_RASA,    //TEVArg[0]
                                SHADER_ZERO,    //TEVArg[1]
                                SHADER_ZERO,    //TEVArg[2]
                                SHADER_ZERO,    //TEVArg[3]
                                GX_TEV_ADD,     //TEV op
                                GX_TB_ZERO,     //TEV Bias
                                GX_CS_SCALE_1,  //TEV Scale
                                GX_TRUE,        //TEV Clamp
                                GX_TC_LINEAR,   //TEV Clamp Mode
                                SHADER_APREV,   //Out Reg
                                SHADER_RAS0,    //RAS input
                                0,              //Tex Gen Indx
                                SHADER_TEXNONE  //Tex Input
                            };

static CompiledShaderResources
PassThruResources   =       {
                                SHADER_COLOR_NONE,  //regColor[0]
                                SHADER_COLOR_NONE,  //regColor[1]
                                SHADER_COLOR_NONE,  //regColor[2]
                                SHADER_COLOR_NONE,  //regColor[3]

                                SHADER_COLOR_NONE,  //regAlpha[0]
                                SHADER_COLOR_NONE,  //regAlpha[1]
                                SHADER_COLOR_NONE,  //regAlpha[2]
                                SHADER_COLOR_NONE,  //regAlpha[3]

                                0, 0, 0, 0,         //colorUsed

                                0, 0, 0, 0,         //colorData[0]
                                0, 0, 0, 0,         //colorData[1]
                                0, 0, 0, 0,         //colorData[2]
                                0, 0, 0, 0,         //colorData[3]

                                0, 0, 0, 0, 0, 0, 0, 0, //textureUsed
                                0, 0, 0, 0, 0, 0, 0, 0, //textureData

                                1, 0,               //rasUsed
                                GX_COLOR_NULL,      //rasData[0] 
                                GX_COLOR_NULL,      //rasData[1]

                                0, 0, 0, 0, 0, 0, 0, 0, //complexUsed
                                0, 0, 0, 0, 0, 0, 0, 0  //complexData
                            };

static OptimizedTexCoordExpressions
PassThruTexCoordExp =       {
                                0,                      //expressionArray
                                0,                      //num expressions
                                0, 0, 0, 0, 0, 0, 0, 0, //mtx array
                                0, 0, 0, 0, 0, 0, 0, 0  //mtxUsed
                            };  

static SHDRCompiled 
PassThruShader      =       {
                                (void *)(&PassThruColorTEVStage),   //color stages
                                (void *)(&PassThruAlphaTEVStage),   //alpha stages
                                1,                                  //num stages
                                (void *)(&PassThruResources),       //shader resources
                                (void *)(&PassThruTexCoordExp)      //texGen
                            };

/*---------------------------------------------------------------------------*/

SHDRCompiled *SHDRPassThruShader = &PassThruShader;
SHDRCompiled *SHDRReplaceShader = &ReplaceShader;
SHDRCompiled *SHDRModulateShader = &ModulateShader;

/*---------------------------------------------------------------------------*/
#if 0
}
#endif

#endif