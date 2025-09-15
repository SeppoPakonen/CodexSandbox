/*---------------------------------------------------------------------------*
  Project:  Character Pipeline
  File:     shader.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/shader/src/shader.c $    
    
    7     7/19/00 3:59p Ryan
    update for precompiled shaders
    
    6     6/07/00 1:08p Ryan
    checkin for shader optimization v1.0
    
    5     4/12/00 6:28a Ryan
    Update for color/alpha and texgen shader revision
    
    4     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
    3     3/06/00 2:33p Ryan
    Optimization update 3/6/00
    
    2     2/28/00 3:50p Ryan
    optimization update for shader lib
    
    1     2/28/00 11:23a Ryan

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <Dolphin/os.h>
#include <CharPipe/shader.h>

#include "shaderInternals.h"
#include "shaderPrint.h"
#include "shaderResManage.h"
#include "shaderBuildTree.h"
#include "shaderPruneTree.h"
#include "shaderFlattenTree.h"
#include "shaderSetTEV.h"
#include "shaderTCInternals.h"
#include "shaderExecute.h"

#include "shaderPredefined.h"

/*>*******************************(*)*******************************<*/
static SHDRShader internalShader1 = {	SHADER_TYPE_CONSTANT, 
							    		SHADER_CHANNEL_TRIVIAL, 
							    		0,
							    		0,
							    		SHADER_ONE	};

static SHDRShader internalShader2 = {	SHADER_TYPE_CONSTANT, 
								    	SHADER_CHANNEL_TRIVIAL, 
								    	0,
								    	0,
								    	SHADER_ZERO	};

static SHDRShader internalShader3 = {	SHADER_TYPE_CONSTANT, 
								    	SHADER_CHANNEL_TRIVIAL, 
								    	0,
								    	0,
								    	SHADER_HALF	};

static SHDRShader internalShader4 = {	SHADER_TYPE_CONSTANT, 
								    	SHADER_CHANNEL_TRIVIAL, 
								    	0,
								    	0,
								    	SHADER_QUARTER	};

SHDRShader *ShaderOne = &internalShader1;
SHDRShader *ShaderZero = &internalShader2;
SHDRShader *ShaderHalf = &internalShader3;
SHDRShader *ShaderQuarter = &internalShader4;

/*>*******************************(*)*******************************<*/
SHDRShader *SHDRCreateTexture	( SHADER_TexInput tex, SHDRTexCoord *texCoordShader, 
								  SHADER_CHANNEL channel )
{
	SHDRShader *temp;

	temp = (SHDRShader *)OSAlloc(sizeof(SHDRShader));

	temp->type = SHADER_TYPE_TEXTURE;
	temp->channel = channel;
	temp->referenceCount = 0;
	temp->TEVStage = 0;

	temp->shaderParams.textureShader.tex = tex;
	temp->shaderParams.textureShader.texCoordShader = texCoordShader;

    texCoordShader->referenceCount++;

	return temp;
}

/*>*******************************(*)*******************************<*/
SHDRShader *SHDRCreateRasterized	( SHADER_RasInput rasColor, SHADER_CHANNEL channel )
{
	SHDRShader *temp; 

	temp = (SHDRShader *)OSAlloc(sizeof(SHDRShader));

	temp->type = SHADER_TYPE_RASTERIZED;
	temp->channel = channel;
	temp->referenceCount = 0;
	temp->TEVStage = 0;

	temp->shaderParams.rasterizedShader.rasColor = rasColor;

	return temp;
}

/*>*******************************(*)*******************************<*/
SHDRShader *SHDRCreateColor		( SHADER_ColorInput color, SHADER_CHANNEL channel )
{
	SHDRShader *temp;

	temp = (SHDRShader *)OSAlloc(sizeof(SHDRShader));

	temp->type = SHADER_TYPE_COLOR;
	temp->channel = channel;
	temp->referenceCount = 0;
	temp->TEVStage = 0;

	temp->shaderParams.colorShader.color = color;

	return temp;
}

/*>*******************************(*)*******************************<*/
SHDRShader *SHDRCreateComplexInput		( SHADER_ComplexInput input, SHADER_CHANNEL channel )
{
	SHDRShader *temp;

	temp = (SHDRShader *)OSAlloc(sizeof(SHDRShader));

	temp->type = SHADER_TYPE_COMPLEXINPUT;
	temp->channel = channel;
	temp->referenceCount = 0;
	temp->TEVStage = 0;

	temp->shaderParams.complexInputShader.input = input;

	return temp;
}

/*>*******************************(*)*******************************<*/
SHDRShader *SHDRCreateComplex		( SHDRShader *input1, SHDRShader *input2, 
    								  SHDRShader *input3, SHDRShader *input4, 
	    							  SHADER_OP op, SHADER_CLAMP clamp,
                                      SHADER_BIAS bias, SHADER_SCALE scale, 
			    					  SHADER_CHANNEL channel )
{
	SHDRShader *temp;

	//check for .25 and .5 shaders too!
	if(channel == SHADER_CHANNEL_A)
	{
		if(input1->channel < SHADER_CHANNEL_A ||
		   input2->channel < SHADER_CHANNEL_A ||
		   input3->channel < SHADER_CHANNEL_A ||
		   input4->channel < SHADER_CHANNEL_A )
		{
			ASSERTMSG(0, "all input into a complex shader of type SHADER_CHANNEL_A must also be of type SHADER_CHANNEL_A"); 	
		}
	}

	temp = (SHDRShader *)OSAlloc(sizeof(SHDRShader));

	temp->type = SHADER_TYPE_COMPLEX;
	temp->channel = channel;
	temp->referenceCount = 0;
	temp->TEVStage = 0;

	temp->shaderParams.complexShader.input1 = input1;
	temp->shaderParams.complexShader.input2 = input2;
	temp->shaderParams.complexShader.input3 = input3;
	temp->shaderParams.complexShader.input4 = input4;

	temp->shaderParams.complexShader.op = op;
	temp->shaderParams.complexShader.clamp = clamp;
    temp->shaderParams.complexShader.bias = bias;
	temp->shaderParams.complexShader.scale = scale;

	if(input1->type != SHADER_TYPE_CONSTANT)
		input1->referenceCount++;
	if(input2->type != SHADER_TYPE_CONSTANT)
		input2->referenceCount++;
	if(input3->type != SHADER_TYPE_CONSTANT)
		input3->referenceCount++;
	if(input4->type != SHADER_TYPE_CONSTANT)
		input4->referenceCount++;

	return temp;
}

/*>*******************************(*)*******************************<*/
void	SHDRFree				( SHDRShader *shader )
{
    if(!shader) return;
    if(shader->type == SHADER_TYPE_CONSTANT) return;

    shader->referenceCount --;

    if(shader->referenceCount < 1)
    {
	    if(shader->type == SHADER_TYPE_COMPLEX)
	    {
		    SHDRFree(shader->shaderParams.complexShader.input1);
		    SHDRFree(shader->shaderParams.complexShader.input2);
		    SHDRFree(shader->shaderParams.complexShader.input3);
		    SHDRFree(shader->shaderParams.complexShader.input4);
	    }

        if(shader->type == SHADER_TYPE_TEXTURE && shader->referenceCount < 1)
            SHDRFreeTC(shader->shaderParams.textureShader.texCoordShader);
		    	
		OSFree(shader);
    }
}

/*>*******************************(*)*******************************<*/
SHDRCompiled    *SHDRCompile    ( SHDRShader *rgbShader, SHDRShader *aShader )
{
    SHDRCompiled *compiledShader = 0;

    if(!rgbShader) rgbShader = ShaderOne;
    if(!aShader) aShader = ShaderOne;

    ASSERTMSG(rgbShader->channel != SHADER_CHANNEL_A, "rgbShader needs to be of color type");
    ASSERTMSG(aShader->channel == SHADER_CHANNEL_A || aShader->type == SHADER_TYPE_CONSTANT, "aShader needs to be of alpha type");

	if(rgbShader->type != SHADER_TYPE_CONSTANT)
		rgbShader->referenceCount++;

    if(aShader->type != SHADER_TYPE_CONSTANT)
		aShader->referenceCount++;

	ResetShaderResources();

    //allocate a compiled shader
    compiledShader = (SHDRCompiled *)OSAlloc(sizeof(SHDRCompiled));
    compiledShader->texGen = 0;
    compiledShader->colorStages = 0;
    compiledShader->alphaStages = 0;

    //build TEV tree and compile tex gen
	if(!(BuildTEVTree(rgbShader, aShader, &(compiledShader->texGen))))
    {
        if(compiledShader->texGen)
            OSFree(compiledShader->texGen);
        OSFree(compiledShader);
        return 0;
    }

	PruneTEVTree();

    if(BestNumTEVStages == 99)
    {
        if(compiledShader->texGen)
            OSFree(compiledShader->texGen);
        OSFree(compiledShader);
        return 0;
    }

    //SetTCGenState();
	//SetTEV();

    //allocate compiled TEV stages and resources
    compiledShader->colorStages = OSAlloc(sizeof(CompiledTEVStage) * BestNumTEVStages);
    compiledShader->alphaStages = OSAlloc(sizeof(CompiledTEVStage) * BestNumTEVStages);
    compiledShader->numStages = BestNumTEVStages;
    compiledShader->shaderResources = OSAlloc(sizeof(CompiledShaderResources));

    //initialize resource information
    InitCompiledResourceInfo((CompiledShaderResources *)(compiledShader->shaderResources));

    //copy color stages
    CopyCompiledTEVStages((CompiledTEVStage *)(compiledShader->colorStages), 
                          BestColorInstructions, 
                          BestNumTEVStages, 
                          (CompiledShaderResources *)(compiledShader->shaderResources));

    CopyCompiledTEVStages((CompiledTEVStage *)(compiledShader->alphaStages), 
                          BestAlphaInstructions, 
                          BestNumTEVStages, 
                          (CompiledShaderResources *)(compiledShader->shaderResources));

    return compiledShader;
}

/*>*******************************(*)*******************************<*/
void SHDRExecute    ( SHDRCompiled *shader )
{
    //check to make sure valid data is bound to all shaders
    CheckShaderBindings(shader);

    CombineTEVStages(shader);

    //set tc state

    //set tev

}

/*>*******************************(*)*******************************<*/
void SHDRBindTexture    ( SHDRCompiled *shader, SHADER_TexInput tex, GXTexObj *texObj )
{
    // xxx insert errors for trying to bind unused data

    CompiledShaderResources *resources = (CompiledShaderResources *)(shader->shaderResources);

    if(!(resources->textureUsed[(u32)(tex)]))
        OSHalt("Error, trying to bind a texture to an unused texture input - SHDRBindTexture");

    if(texObj)
    {
        resources->textureUsed[(u32)(tex)] = 2;
        resources->textureData[(u32)(tex)] = texObj;
    }
    else
    {
        resources->textureUsed[(u32)(tex)] = 1;
        resources->textureData[(u32)(tex)] = 0;
    }
}

/*>*******************************(*)*******************************<*/
void SHDRBindRasterized ( SHDRCompiled *shader, SHADER_RasInput rasColor, GXChannelID channel )
{
    CompiledShaderResources *resources = (CompiledShaderResources *)(shader->shaderResources);

    if(!(resources->rasUsed[(u32)(rasColor)]))
        OSHalt("Error, trying to bind a rasterized color to an unused rasterized input - SHDRBindRasterized");

    resources->rasUsed[(u32)(rasColor)] = 2;
    resources->rasData[(u32)(rasColor)] = channel;
}

/*>*******************************(*)*******************************<*/
void SHDRBindColor      ( SHDRCompiled *shader, SHADER_ColorInput colorInput, GXColor color )
{
    CompiledShaderResources *resources = (CompiledShaderResources *)(shader->shaderResources);

    if(!(resources->colorUsed[(u32)(colorInput)]))
        OSHalt("Error, trying to bind a color to an unused rasterized input - SHDRBindColor");

    resources->colorUsed[(u32)(colorInput)] = 2;
    resources->colorData[(u32)(colorInput)] = color;
}

/*>*******************************(*)*******************************<*/
void SHDRBindComplexInput   ( SHDRCompiled *shader, SHADER_ComplexInput input, SHDRCompiled *inputShader )
{
    CompiledShaderResources *resources = (CompiledShaderResources *)(shader->shaderResources);

    if(!(resources->complexUsed[(u32)(input)]))
        OSHalt("Error, trying to bind a complex shader to an unused complex input - SHDRBindComplexInput");

    if(inputShader)
    {
        resources->complexUsed[(u32)(input)] = 2;
        resources->complexData[(u32)(input)] = inputShader;
    }
    else
    {
        resources->complexUsed[(u32)(input)] = 1;
        resources->complexData[(u32)(input)] = 0;
    }
}

/*>*******************************(*)*******************************<*/
void SHDRBindTexGenMtx   ( SHDRCompiled *shader, SHADER_MTXInput input, Mtx mtxData )
{
    OptimizedTexCoordExpressions *tcExp = (OptimizedTexCoordExpressions *)(shader->texGen);
    void *temp;

    if(!(tcExp->mtxUsed[((u32)(input))]))
        OSHalt("Error, trying to bind a mtx to an unused mtx input - SHDRBindTexGenMtx");


    if(tcExp->mtxUsed[((u32)(input))])
    {
        tcExp->mtxUsed[((u32)(input))] = 2;
        temp = tcExp->mtxArray[((u32)(input))];

        //copy mtx
        MTXCopy(mtxData, (MtxPtr)temp);
    }
}
