/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     shader.h

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/charPipeline/shader.h $
    
    6     7/19/00 4:05p Ryan
    Update to work with precompiled shaders
    
    5     4/12/00 6:28a Ryan
    Update for color/alpha and texgen shader revision
    
    4     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
    3     3/14/00 11:51a Ryan
    removed gx.h
    
    2     3/06/00 2:33p Ryan
    Optimization update 3/6/00
    
    1     2/28/00 11:41a Ryan

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#ifndef __SHADER_H__
#define __SHADER_H__

#if 0
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
#include <Dolphin/GXEnum.h>
#include <Dolphin/GXStruct.h>

/*---------------------------------------------------------------------------*/
typedef struct SHDRShader SHDRShader, *SHDRShaderPtr;

/*---------------------------------------------------------------------------*/
#include <CharPipe/shaderTexCoord.h>

/*---------------------------------------------------------------------------*/
typedef enum
{
	SHADER_OP_ADD = GX_TEV_ADD,
    SHADER_OP_SUB = GX_TEV_SUB

} SHADER_OP;

/*---------------------------------------------------------------------------*/
typedef enum 
{
	SHADER_CLAMP_LINEAR_1023, 
	SHADER_CLAMP_LINEAR_255, 
	SHADER_CLAMP_GE_255, 
	SHADER_CLAMP_GE_0, 
	SHADER_CLAMP_EQ_255, 
	SHADER_CLAMP_EQ_0, 
	SHADER_CLAMP_LE_255, 
	SHADER_CLAMP_LE_0

} SHADER_CLAMP;

/*---------------------------------------------------------------------------*/
typedef enum 
{
    SHADER_BIAS_ZERO = GX_TB_ZERO,
    SHADER_BIAS_ADDHALF = GX_TB_ADDHALF,
    SHADER_BIAS_SUBHALF = GX_TB_SUBHALF

} SHADER_BIAS;

/*---------------------------------------------------------------------------*/
typedef enum
{
    SHADER_SCALE_1 = GX_CS_SCALE_1,
    SHADER_SCALE_2 = GX_CS_SCALE_2,
    SHADER_SCALE_4 = GX_CS_SCALE_4,
    SHADER_SCALE_DIVIDE_2 = GX_CS_DIVIDE_2

} SHADER_SCALE;

/*---------------------------------------------------------------------------*/
typedef enum
{
	SHADER_CHANNEL_RGB,
	SHADER_CHANNEL_RRR,
	SHADER_CHANNEL_GGG,
	SHADER_CHANNEL_BBB,
	SHADER_CHANNEL_A,

	SHADER_CHANNEL_TRIVIAL	// only used internally

}SHADER_CHANNEL;

/*---------------------------------------------------------------------------*/
typedef enum
{
	SHADER_TYPE_TEXTURE, 
	SHADER_TYPE_COLOR, 
	SHADER_TYPE_RASTERIZED, 
	SHADER_TYPE_CONSTANT,
    SHADER_TYPE_COMPLEXINPUT,
	SHADER_TYPE_COMPLEX,
	SHADER_TYPE_EMPTY

}SHADER_TYPE;

/*---------------------------------------------------------------------------*/
typedef enum
{
	SHADER_CPREV = 0,
    SHADER_C0 = 1,
	SHADER_C1 = 2,
	SHADER_C2 = 3,
	
	SHADER_APREV = 4,
    SHADER_A0 = 5,
    SHADER_A1 = 6,
    SHADER_A2 = 7,
    
    SHADER_TEXC = 8,
    SHADER_TEXA = 9,
    SHADER_RASC = 10,
    SHADER_RASA = 11,

	SHADER_TEXRRR = 12,
    SHADER_TEXGGG = 13,
    SHADER_TEXBBB = 14,

	SHADER_HALF = 15,
    SHADER_QUARTER = 16,

    SHADER_COMPLEXINPUT0_RGB = 25,
    SHADER_COMPLEXINPUT1_RGB = 26,
    SHADER_COMPLEXINPUT2_RGB = 27,
    SHADER_COMPLEXINPUT3_RGB = 28,
    SHADER_COMPLEXINPUT4_RGB = 29,
    SHADER_COMPLEXINPUT5_RGB = 30,
    SHADER_COMPLEXINPUT6_RGB = 31,
    SHADER_COMPLEXINPUT7_RGB = 32,

    SHADER_COMPLEXINPUT0_A = 35,
    SHADER_COMPLEXINPUT1_A = 36,
    SHADER_COMPLEXINPUT2_A = 37,
    SHADER_COMPLEXINPUT3_A = 38,
    SHADER_COMPLEXINPUT4_A = 39,
    SHADER_COMPLEXINPUT5_A = 40,
    SHADER_COMPLEXINPUT6_A = 41,
    SHADER_COMPLEXINPUT7_A = 42,

    SHADER_COLORINPUT0_RGB = 50,
    SHADER_COLORINPUT1_RGB = 51,
    SHADER_COLORINPUT2_RGB = 52,
    SHADER_COLORINPUT3_RGB = 53,

    SHADER_COLORINPUT0_A = 60,
    SHADER_COLORINPUT1_A = 61,
    SHADER_COLORINPUT2_A = 62,
    SHADER_COLORINPUT3_A = 63,

	SHADER_NONTRIVIAL = 100,	//only used in shader compilation step

    SHADER_ONE = 200,    
    SHADER_ZERO = 201

} SHADER_TEVArg;

/*---------------------------------------------------------------------------*/
typedef enum
{	
    SHADER_TEX0 = 0, 
    SHADER_TEX1 = 1, 
    SHADER_TEX2 = 2, 
    SHADER_TEX3 = 3, 
    SHADER_TEX4 = 4, 
    SHADER_TEX5 = 5, 
    SHADER_TEX6 = 6, 
    SHADER_TEX7 = 7,
    SHADER_TEXNONE = 8

} SHADER_TexInput;

/*---------------------------------------------------------------------------*/
typedef enum
{	
    SHADER_RAS0 = 0, 
    SHADER_RAS1 = 1, 
    SHADER_RASNONE = 2

} SHADER_RasInput;

/*---------------------------------------------------------------------------*/
typedef enum
{	
    SHADER_COLOR0 = 0, 
    SHADER_COLOR1 = 1, 
    SHADER_COLOR2 = 2, 
    SHADER_COLOR3 = 3, 
    SHADER_COLOR_NONE = 4

} SHADER_ColorInput;

/*---------------------------------------------------------------------------*/
typedef enum
{	
    SHADER_COMPLEX0 = 0, 
    SHADER_COMPLEX1 = 1, 
    SHADER_COMPLEX2 = 2, 
    SHADER_COMPLEX3 = 3, 
    SHADER_COMPLEX4 = 4, 
    SHADER_COMPLEX5 = 5, 
    SHADER_COMPLEX6 = 6, 
    SHADER_COMPLEX7 = 7

} SHADER_ComplexInput;

/*---------------------------------------------------------------------------*/
typedef struct
{
	SHADER_TexInput	tex;
	SHDRTexCoord    *texCoordShader;

} SHDRTexture;

/*---------------------------------------------------------------------------*/
typedef struct
{
	SHADER_ColorInput	color;

} SHDRColor;

/*---------------------------------------------------------------------------*/
typedef struct
{
	SHADER_RasInput	rasColor;

} SHDRRasterized;

/*---------------------------------------------------------------------------*/
typedef struct
{
	SHADER_TEVArg arg;

} SHDRConstant;

/*---------------------------------------------------------------------------*/
typedef struct
{
	SHADER_ComplexInput input;

} SHDRComplexInput;

/*---------------------------------------------------------------------------*/
typedef struct
{
	SHDRShader			*input1, 
	    				*input2, 
		    			*input3, 
		    			*input4;

	SHADER_OP 		op;
	SHADER_CLAMP	clamp;
    SHADER_BIAS 	bias;
	SHADER_SCALE 	scale;

} SHDRComplex;

/*---------------------------------------------------------------------------*/
struct SHDRShader
{
	SHADER_TYPE		type;
	SHADER_CHANNEL	channel; //alpha, RGB, RRR, GGG, BBB
	s16				referenceCount;

	void *TEVStage;	// used only during shader compilation

	union
	{
		SHDRConstant	    constantShader;
		SHDRTexture		    textureShader;
		SHDRColor		    colorShader;
		SHDRRasterized	    rasterizedShader;
		SHDRComplex		    complexShader;
        SHDRComplexInput    complexInputShader;

	}shaderParams;
};

/*---------------------------------------------------------------------------*/
typedef struct 
{
    void   *colorStages;
    void   *alphaStages;
    u8      numStages;
    
    void   *shaderResources;

    void   *texGen;

} SHDRCompiled;

/*---------------------------------------------------------------------------*/
extern SHDRShader	*ShaderZero;
extern SHDRShader	*ShaderOne;
extern SHDRShader	*ShaderHalf;
extern SHDRShader	*ShaderQuarter;

/*---------------------------------------------------------------------------*/
//convenience shaders for simple operations
extern SHDRCompiled *SHDRPassThruShader;
extern SHDRCompiled *SHDRReplaceShader;
extern SHDRCompiled *SHDRModulateShader;

/*---------------------------------------------------------------------------*/
SHDRShader *SHDRCreateTexture	    ( SHADER_TexInput tex, SHDRTexCoord *texCoordShader, 
								      SHADER_CHANNEL channel );

SHDRShader *SHDRCreateRasterized	( SHADER_RasInput rasColor, SHADER_CHANNEL channel );
SHDRShader *SHDRCreateColor		    ( SHADER_ColorInput color, SHADER_CHANNEL channel );
SHDRShader *SHDRCreateComplexInput	( SHADER_ComplexInput input, SHADER_CHANNEL channel );

SHDRShader *SHDRCreateComplex		( SHDRShader *input1, SHDRShader *input2, 
								      SHDRShader *input3, SHDRShader *input4, 
    								  SHADER_OP op, SHADER_CLAMP clamp,
                                      SHADER_BIAS bias, SHADER_SCALE scale, 
		    						  SHADER_CHANNEL channel );

void	SHDRFree				    ( SHDRShader *shader );

SHDRCompiled    *SHDRCompile		( SHDRShader *rgbShader, SHDRShader *aShader );

/*---------------------------------------------------------------------------*/
void SHDRBindTexture        ( SHDRCompiled *shader, SHADER_TexInput tex, 
                              GXTexObj *texObj );

void SHDRBindRasterized     ( SHDRCompiled *shader, SHADER_RasInput rasColor, 
                              GXChannelID channel );

void SHDRBindColor          ( SHDRCompiled *shader, SHADER_ColorInput colorInput, 
                              GXColor color );

void SHDRBindComplexInput   ( SHDRCompiled *shader, SHADER_ComplexInput input, 
                              SHDRCompiled *inputShader );

void SHDRBindTexGenMtx      ( SHDRCompiled *shader, SHADER_MTXInput input, Mtx mtxData );

void SHDRExecute            ( SHDRCompiled *shader );

//////////////////////////////////////////////////////////////////////

void PrintTEVTree	( SHDRCompiled *shader );  // for use only with MAC emulation


/*---------------------------------------------------------------------------*/
#if 0
}
#endif

#endif  // __LIGHT_H__