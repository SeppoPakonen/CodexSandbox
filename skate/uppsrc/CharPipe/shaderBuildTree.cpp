/*---------------------------------------------------------------------------*
  Project:  Dolphin
  File:     shaderBuildTree.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.
    
  $Log: /Dolphin/build/charPipeline/shader/src/shaderBuildTree.c $    
    
    8     7/19/00 3:59p Ryan
    update for precompiled shaders
    
    7     6/07/00 1:08p Ryan
    checkin for shader optimization v1.0
    
    6     4/12/00 6:28a Ryan
    Update for color/alpha and texgen shader revision
    
    5     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
    4     3/06/00 2:33p Ryan
    Optimization update 3/6/00
    
    3     3/02/00 11:31a Ryan
    removed duplicate code for optimization purposes
    
    2     2/28/00 3:50p Ryan
    optimization update for shader lib
    
    1     2/28/00 11:23a Ryan

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <Dolphin/os.h>

#include "shaderBuildTree.h"
#include "shaderResManage.h"
#include "shaderTCInternals.h"

/*>*******************************(*)*******************************<*/
static SHDRShader *CheckShaderRoot	( SHDRShader *rgbShader, SHDRShader *aShader );

static ShaderTEVStage *AllocateStage( SHDRShader *shader );
static void LinkParentToChild		( ShaderTEVStage *parent, u8 index, ShaderTEVStage *child );
static void AllocateEmptyStage	    ( SHDRShader *shader );
static void AllocateComplexStage	( ShaderTEVStage *parent, u8 index, SHDRShader *shader );
static void AllocateRasterizedStage	( ShaderTEVStage *parent, u8 index, SHDRShader *shader );
static void AllocateTextureStage	( ShaderTEVStage *parent, u8 index, SHDRShader *shader );

static void	CollapseShaders	            ( ShaderTEVStage *parent, u8 index, SHDRShader *shader, u8 numTex, u8 numRas );
static void CollapseConstantShader	    ( ShaderTEVStage *parent, u8 index, SHDRShader *shader );
static void CollapseComplexInputStage	( ShaderTEVStage *parent, u8 index, SHDRShader *shader );
static void CollapseColorShader		    ( ShaderTEVStage *parent, u8 index, SHDRShader *shader );
static void CollapseRasterizedShader    ( ShaderTEVStage *parent, u8 index, SHDRShader *shader, u8 numRas );
static void CollapseTextureShader	    ( ShaderTEVStage *parent, u8 index, SHDRShader *shader, u8 numTex );

static SHADER_TEVArg TranslateChannel	( SHADER_CHANNEL channel );

static void TranslateClampMode	( SHADER_CLAMP clamp, GXTevClampMode *mode, GXBool *clampEnable );

static void CleanUpTree			( SHDRShader *shader );

static void InitNonAllocatedTevStages ( void );

static void AddAlphaParent      ( ShaderTEVStage *stage, ShaderTEVStage *parent );

static void CheckAlphaStage     ( ShaderTEVStage *stage );
static BOOL CheckColorInput     ( SHADER_TEVArg arg );

static void CountTexNRasStages  ( SHDRShader *shader, u8 *numTex, u8 *numRas );

/*>*******************************(*)*******************************<*/
static SHDRShader tempRGBRoot = {	SHADER_TYPE_COMPLEX, 
		    					    SHADER_CHANNEL_RGB, 
			    				    0,
				    			    0,
					    		    SHADER_ONE	};

static SHDRShader tempARoot =   {	SHADER_TYPE_COMPLEX, 
		    					    SHADER_CHANNEL_A, 
			    				    0,
				    			    0,
					    		    SHADER_ONE	};

static SHDRShader connector =   {	SHADER_TYPE_EMPTY, 
		    					    SHADER_CHANNEL_RGB, 
			    				    0,
				    			    0,
					    		    SHADER_ONE	};

SHDRShader *RGBRoot;

/*>*******************************(*)*******************************<*/
BOOL BuildTEVTree ( SHDRShader *rgbShader, SHDRShader *aShader, void **compiledTexGen )
{
	SHDRShader *shadeTree;

	shadeTree = CheckShaderRoot(rgbShader, aShader);

    CompileTexGen(shadeTree,  (OptimizedTexCoordExpressions **)compiledTexGen);
    
    // allocate a dummy parent stage since we know the parent shader is EMPTY
    AllocateEmptyStage(shadeTree);

	//AllocateComplexStage(0, 0, shadeTree);

	InitNonAllocatedTevStages();

    //parent Alpha stages
    //AddAlphaParent(TEVPool[0].tevStage[0], TEVPool[0].tevStage[1]);

	CleanUpTree(shadeTree);

    return TRUE;
}

/*>*******************************(*)*******************************<*/
// checks to see if there is a complex shader at the root of the tree.  
// If not, add one since we assume a complex root during the build 
// process.
/*>*******************************(*)*******************************<*/
static SHDRShader *CheckShaderRoot			( SHDRShader *rgbShader, SHDRShader *aShader )
{
    SHDRShader *rgb = rgbShader;
    SHDRShader *alpha = aShader;

	if(rgbShader->type != SHADER_TYPE_COMPLEX)
    {
        tempRGBRoot.TEVStage = 0;
	    tempRGBRoot.shaderParams.complexShader.input1 = rgbShader;
	    tempRGBRoot.shaderParams.complexShader.input2 = ShaderZero;
	    tempRGBRoot.shaderParams.complexShader.input3 = ShaderZero;
	    tempRGBRoot.shaderParams.complexShader.input4 = ShaderZero;

	    tempRGBRoot.shaderParams.complexShader.op = SHADER_OP_ADD;
	    tempRGBRoot.shaderParams.complexShader.clamp = SHADER_CLAMP_LINEAR_255;
        tempRGBRoot.shaderParams.complexShader.bias = SHADER_BIAS_ZERO;
	    tempRGBRoot.shaderParams.complexShader.scale = SHADER_SCALE_1;

        rgb = &tempRGBRoot;
    }

    if(aShader->type != SHADER_TYPE_COMPLEX)
    {
        tempARoot.TEVStage = 0;
	    tempARoot.shaderParams.complexShader.input1 = aShader;
	    tempARoot.shaderParams.complexShader.input2 = ShaderZero;
	    tempARoot.shaderParams.complexShader.input3 = ShaderZero;
	    tempARoot.shaderParams.complexShader.input4 = ShaderZero;

	    tempARoot.shaderParams.complexShader.op = SHADER_OP_ADD;
	    tempARoot.shaderParams.complexShader.clamp = SHADER_CLAMP_LINEAR_255;
        tempARoot.shaderParams.complexShader.bias = SHADER_BIAS_ZERO;
	    tempARoot.shaderParams.complexShader.scale = SHADER_SCALE_1;

        alpha = &tempARoot;
    }

	connector.shaderParams.complexShader.input1 = rgb;
	connector.shaderParams.complexShader.input2 = alpha;
	connector.shaderParams.complexShader.input3 = 0;
	connector.shaderParams.complexShader.input4 = 0;

    //set RGBRoot to check for alpha stage later!
    RGBRoot = rgb;

	return &connector;
}

/*>*******************************(*)*******************************<*/
static ShaderTEVStage *AllocateStage	( SHDRShader *shader )
{
	ShaderTEVStage *currentStage = &(TEVPool[TEVCounter++]);
	u8 i;
	
	shader->TEVStage = currentStage;

	if(shader->channel == SHADER_CHANNEL_A)
		currentStage->channel = SHADER_TEV_ALPHA;
	else
		currentStage->channel = SHADER_TEV_COLOR;

    if(shader->type == SHADER_TYPE_EMPTY)
        currentStage->type = shader->type;
    else
	    currentStage->type = SHADER_TYPE_COMPLEX;

	for( i = 0; i < 4; i ++)
	{
		currentStage->TEVArg[i] = SHADER_ZERO;
		currentStage->tevStage[i] = 0;
	}

	currentStage->op = GX_TEV_ADD;
	currentStage->bias = GX_TB_ZERO;
	currentStage->scale = GX_CS_SCALE_1;

	TranslateClampMode(SHADER_CLAMP_LINEAR_1023, 
					   &currentStage->mode, 
					   &currentStage->clamp);

    currentStage->texGenIdx = NO_TEXTURE_COORD_EXP;
	currentStage->texInput = SHADER_TEXNONE;
	currentStage->rasInput = SHADER_RASNONE;

	//currentStage->coord = GX_TEXCOORD_NULL;
	//currentStage->map = GX_TEXMAP_NULL;
	//currentStage->color = GX_COLOR_NULL;

	currentStage->numParents = 0;
    currentStage->numNonAllocatedChildren = 0;

	return currentStage;
}

/*>*******************************(*)*******************************<*/
static void LinkParentToChild	( ShaderTEVStage *parent, u8 index, ShaderTEVStage *child )
{
	if(parent)
	{
		parent->tevStage[index] = child; 

        if(child->channel == SHADER_TEV_ALPHA)
            parent->TEVArg[index] = SHADER_APREV; // placeholder for real intermediate variable
        else
		    parent->TEVArg[index] = SHADER_CPREV; // placeholder for real intermediate variable

		child->parent[child->numParents] = parent;
		child->numParents ++;
		ASSERTMSG(child->numParents < 8, "TEV Stage has too many parents!");
	}
}

/*>*******************************(*)*******************************<*/
static void AllocateEmptyStage	( SHDRShader *shader )
{
	ShaderTEVStage	*currentStage;
    u8 i;
	
	currentStage = AllocateStage(shader);

    for( i = 0; i < 4; i ++)
	{
		currentStage->TEVArg[i] = SHADER_CPREV;
	}

	CollapseShaders(currentStage, 0, shader->shaderParams.complexShader.input1, 0, 0);
	CollapseShaders(currentStage, 1, shader->shaderParams.complexShader.input2, 0, 0);
}

/*>*******************************(*)*******************************<*/
static void AllocateComplexStage	( ShaderTEVStage *parent, u8 index, SHDRShader *shader )
{
	ShaderTEVStage	*currentStage;
    u8 numTextureChildren = 0;
    u8 numRasterizedChildren = 0;

	if(shader->TEVStage)
	{
		currentStage = (ShaderTEVStage *)(shader->TEVStage);
	}
	else
	{
		currentStage = AllocateStage(shader);

		currentStage->op = (GXTevOp)(shader->shaderParams.complexShader.op);
		currentStage->bias = (GXTevBias)(shader->shaderParams.complexShader.bias);
		currentStage->scale = (GXTevScale)(shader->shaderParams.complexShader.scale);

		TranslateClampMode(shader->shaderParams.complexShader.clamp, 
						   &currentStage->mode, 
						   &currentStage->clamp);

        CountTexNRasStages(shader, &numTextureChildren, &numRasterizedChildren);

		CollapseShaders(currentStage, 0, shader->shaderParams.complexShader.input1,
                        numTextureChildren, numRasterizedChildren);
		CollapseShaders(currentStage, 1, shader->shaderParams.complexShader.input2,
                        numTextureChildren, numRasterizedChildren);
		CollapseShaders(currentStage, 2, shader->shaderParams.complexShader.input3,
                        numTextureChildren, numRasterizedChildren);
		CollapseShaders(currentStage, 3, shader->shaderParams.complexShader.input4,
                        numTextureChildren, numRasterizedChildren);

        //check to see if the currently allocated stage is an alpha stage
        CheckAlphaStage(currentStage);
	}

	LinkParentToChild(parent, index, currentStage);
}

/*>*******************************(*)*******************************<*/
static void AllocateRasterizedStage	( ShaderTEVStage *parent, u8 index, SHDRShader *shader )
{
	ShaderTEVStage	*currentStage;

	if(shader->TEVStage)
	{
		currentStage = (ShaderTEVStage *)(shader->TEVStage);
	}
	else
	{
		currentStage = AllocateStage(shader);

		if(shader->channel == SHADER_CHANNEL_RGB)
			currentStage->TEVArg[0] = SHADER_RASC;
		else
			currentStage->TEVArg[0] = SHADER_RASA;

		currentStage->rasInput = shader->shaderParams.rasterizedShader.rasColor;
	}

	LinkParentToChild(parent, index, currentStage);
}

/*>*******************************(*)*******************************<*/
static void AllocateTextureStage	( ShaderTEVStage *parent, u8 index, SHDRShader *shader )
{
	ShaderTEVStage	*currentStage;

	if(shader->TEVStage)
	{
		currentStage = (ShaderTEVStage *)(shader->TEVStage);
	}
	else
	{
		currentStage = AllocateStage(shader);

		currentStage->TEVArg[0] = TranslateChannel(shader->channel);

		currentStage->texGenIdx = shader->shaderParams.textureShader.texCoordShader->texCoordExpIdx;
		currentStage->texInput = shader->shaderParams.textureShader.tex;
	}

	LinkParentToChild(parent, index, currentStage);
}

/*>*******************************(*)*******************************<*/
static void	CollapseShaders	( ShaderTEVStage *parent, u8 index, SHDRShader *shader, u8 numTex, u8 numRas )
{	
	switch(shader->type)
	{
		case SHADER_TYPE_TEXTURE :
			CollapseTextureShader(parent, index, shader, numTex);
			break;
		case SHADER_TYPE_COLOR :
			CollapseColorShader(parent, index, shader);
			break;
		case SHADER_TYPE_RASTERIZED :
			CollapseRasterizedShader(parent, index, shader, numRas);
			break;
		case SHADER_TYPE_CONSTANT :
			CollapseConstantShader(parent, index, shader);
			break;
		case SHADER_TYPE_COMPLEXINPUT :
			CollapseComplexInputStage(parent, index, shader);
            break;
        case SHADER_TYPE_COMPLEX :
			AllocateComplexStage(parent, index, shader);
            break;
	}
}

/*>*******************************(*)*******************************<*/
static void CollapseConstantShader	( ShaderTEVStage *parent, u8 index, SHDRShader *shader )
{
	parent->tevStage[index] = 0; 
	parent->TEVArg[index] = shader->shaderParams.constantShader.arg;
}

/*>*******************************(*)*******************************<*/
static void CollapseComplexInputStage	( ShaderTEVStage *parent, u8 index, SHDRShader *shader )
{
	parent->tevStage[index] = 0; 

    if(shader->channel == SHADER_CHANNEL_RGB)
	    parent->TEVArg[index] = (SHADER_TEVArg)(((u32)(shader->shaderParams.complexInputShader.input)) +
                                ((u32)(SHADER_COMPLEXINPUT0_RGB)));
    else
	    parent->TEVArg[index] = (SHADER_TEVArg)(((u32)(shader->shaderParams.complexInputShader.input)) +
                                ((u32)(SHADER_COMPLEXINPUT0_A)));
}

/*>*******************************(*)*******************************<*/
static void CollapseColorShader	( ShaderTEVStage *parent, u8 index, SHDRShader *shader )
{
	if(shader->channel == SHADER_CHANNEL_RGB)
		parent->TEVArg[index] = AllocateColorResource(shader->shaderParams.colorShader.color);
	else
		parent->TEVArg[index] = AllocateAlphaResource(shader->shaderParams.colorShader.color);

	parent->tevStage[index] = 0; 
}

/*>*******************************(*)*******************************<*/
static void CollapseRasterizedShader	( ShaderTEVStage *parent, u8 index, SHDRShader *shader, u8 numRas )
{
	//GXChannelID	colorChannel = shader->shaderParams.rasterizedShader.colorChannel;
    SHADER_RasInput rasInput = shader->shaderParams.rasterizedShader.rasColor;
	SHADER_TEVArg arg;

    if(numRas <= 1)
    {
	    if(parent->rasInput == SHADER_RASNONE || parent->rasInput == rasInput)
	    {
            if(shader->channel == SHADER_CHANNEL_RGB)
		        arg = SHADER_RASC;
	        else
		        arg = SHADER_RASA;

		    parent->rasInput = rasInput;
		    parent->tevStage[index] = 0; 
		    parent->TEVArg[index] = arg;
		    return;
	    }
    }

	//cannot collapse - allocate a new TEV stage
	AllocateRasterizedStage(parent, index, shader);
}

/*>*******************************(*)*******************************<*/
static void CollapseTextureShader	( ShaderTEVStage *parent, u8 index, SHDRShader *shader, u8 numTex )
{
	//GXTexMapID mapID = AllocateTextureResource(shader->shaderParams.textureShader.texObj);
    SHADER_TexInput texInput = shader->shaderParams.textureShader.tex;

    if(numTex <= 1)
    {
	    if((parent->texInput == SHADER_TEXNONE && parent->texGenIdx == NO_TEXTURE_COORD_EXP) ||
	       (parent->texInput == texInput && parent->texGenIdx == shader->shaderParams.textureShader.texCoordShader->texCoordExpIdx))
	    {
		    parent->texInput = texInput;
		    parent->texGenIdx = shader->shaderParams.textureShader.texCoordShader->texCoordExpIdx;
		    parent->tevStage[index] = 0; 

		    parent->TEVArg[index] = TranslateChannel(shader->channel);

		    return;
	    }
    }

	//cannot collapse - allocate a new TEV stage
	AllocateTextureStage(parent, index, shader);
}

/*>*******************************(*)*******************************<*/
static SHADER_TEVArg TranslateChannel	( SHADER_CHANNEL channel )
{
	switch(channel)
	{
		case SHADER_CHANNEL_RGB:
			return SHADER_TEXC;
		case SHADER_CHANNEL_A:
			return SHADER_TEXA;
		case SHADER_CHANNEL_RRR:
			return SHADER_TEXRRR;
		case SHADER_CHANNEL_GGG:
			return SHADER_TEXGGG;
		case SHADER_CHANNEL_BBB:
			return SHADER_TEXBBB;
	}

	//never reached
	return SHADER_TEXC;
}

/*>*******************************(*)*******************************<*/
static void TranslateClampMode	( SHADER_CLAMP clamp, GXTevClampMode *mode, GXBool *clampEnable )
{
	switch(clamp)
	{
		case SHADER_CLAMP_LINEAR_1023: 
			*mode = GX_TC_LINEAR;
			*clampEnable = GX_FALSE;
			break;
		case SHADER_CLAMP_LINEAR_255: 
			*mode = GX_TC_LINEAR;
			*clampEnable = GX_TRUE;
			break;
		case SHADER_CLAMP_GE_255: 
			*mode = GX_TC_GE;
			*clampEnable = GX_FALSE;
			break;
		case SHADER_CLAMP_GE_0: 
			*mode = GX_TC_GE;
			*clampEnable = GX_TRUE;
			break;
		case SHADER_CLAMP_EQ_255: 
			*mode = GX_TC_EQ;
			*clampEnable = GX_FALSE;
			break;
		case SHADER_CLAMP_EQ_0: 
			*mode = GX_TC_EQ;
			*clampEnable = GX_TRUE;
			break;
		case SHADER_CLAMP_LE_255: 
			*mode = GX_TC_LE;
			*clampEnable = GX_FALSE;
			break;
		case SHADER_CLAMP_LE_0:
			*mode = GX_TC_LE;
			*clampEnable = GX_TRUE;
			break;
	}
}

/*>*******************************(*)*******************************<*/
static void CleanUpTree	( SHDRShader *shader )
{
    if(!shader) return;

	shader->TEVStage = 0;

	if(shader->type == SHADER_TYPE_COMPLEX || shader->type == SHADER_TYPE_EMPTY)
	{
		CleanUpTree(shader->shaderParams.complexShader.input1);
		CleanUpTree(shader->shaderParams.complexShader.input2);
		CleanUpTree(shader->shaderParams.complexShader.input3);
		CleanUpTree(shader->shaderParams.complexShader.input4);
	}
}

/*>*******************************(*)*******************************<*/
static void InitNonAllocatedTevStages ( void )
{
	u8 i, j;
	ShaderTEVStage *currentStage;

	for ( i = (u8)TEVCounter; i < 50; i ++ )
	{
		currentStage = &(TEVPool[i]);

		currentStage->channel = SHADER_TEV_COLOR;

		currentStage->type = SHADER_TYPE_EMPTY;

		for( j = 0; j < 4; j ++)
		{
			currentStage->TEVArg[j] = SHADER_ZERO;
			currentStage->tevStage[j] = 0;
		}

		currentStage->op = GX_TEV_ADD;
		currentStage->bias = GX_TB_ZERO;
		currentStage->scale = GX_CS_SCALE_1;

		TranslateClampMode(SHADER_CLAMP_LINEAR_255, 
						   &currentStage->mode, 
						   &currentStage->clamp);

        currentStage->texGenIdx = NO_TEXTURE_COORD_EXP;
	    currentStage->texInput = SHADER_TEXNONE;
	    currentStage->rasInput = SHADER_RASNONE;

		//currentStage->coord = GX_TEXCOORD_NULL;
		//currentStage->map = GX_TEXMAP_NULL;
		//currentStage->color = GX_COLOR_NULL;

		currentStage->numParents = 0;
	}
}

/*>*******************************(*)*******************************<*/
//this needs to be done since we need to guarantee that the specified
//alpha shader's output does not get overwritten by intermediate Alpha 
//calculations in the color pipe.
/*>*******************************(*)*******************************<*/
static void AddAlphaParent   ( ShaderTEVStage *stage, ShaderTEVStage *parent )
{
    u8 i;

    if(stage->channel == SHADER_TEV_ALPHA)
    {
		stage->parent[stage->numParents] = parent;
		stage->numParents ++;
		ASSERTMSG(stage->numParents < 8, "TEV Stage has too many parents!");
        return;
    }

    if(stage->type == SHADER_TYPE_COMPLEX)
    {
        for ( i= 0; i < 4; i ++ )
        {
            if(stage->tevStage[i])
                AddAlphaParent(stage->tevStage[i], parent);
        }
    }
}

/*>*******************************(*)*******************************<*/
//This function checks a TEV stage to see if it is only taking ALPHA 
//input.  If so, it tags the stage as an alpha stage.
/*>*******************************(*)*******************************<*/
static void CheckAlphaStage ( ShaderTEVStage *stage )
{
    u32 i;

    for ( i = 0; i < 4; i ++ )
    {
        if(CheckColorInput(stage->TEVArg[i]))
            return;
    }

    if((ShaderTEVStage *)(RGBRoot->TEVStage) == stage)
        return;

    //change stage type to Alpha
    stage->channel = SHADER_TEV_ALPHA;
}

/*>*******************************(*)*******************************<*/
//This function checks a SHADER_TEVArg to see if it is an ALPHA or 
//color arg.
/*>*******************************(*)*******************************<*/
static BOOL CheckColorInput   ( SHADER_TEVArg arg )
{
    //xxx add complex alpha input
    switch(arg)
    {
        case SHADER_CPREV:
        case SHADER_C0:
        case SHADER_C1:
        case SHADER_C2:
        case SHADER_TEXC:
        case SHADER_RASC:
        case SHADER_TEXRRR:
        case SHADER_TEXGGG:
        case SHADER_TEXBBB:
        case SHADER_HALF:
        case SHADER_QUARTER:
        case SHADER_COMPLEXINPUT0_RGB:
        case SHADER_COMPLEXINPUT1_RGB:
        case SHADER_COMPLEXINPUT2_RGB:
        case SHADER_COMPLEXINPUT3_RGB:
        case SHADER_COMPLEXINPUT4_RGB:
        case SHADER_COMPLEXINPUT5_RGB:
        case SHADER_COMPLEXINPUT6_RGB:
        case SHADER_COMPLEXINPUT7_RGB:
        case SHADER_COLORINPUT0_RGB:
        case SHADER_COLORINPUT1_RGB:
        case SHADER_COLORINPUT2_RGB:
        case SHADER_COLORINPUT3_RGB:
            return TRUE;
            break;
    }

    //arg is not a color arg, so return FALSE
    return FALSE;
}

/*>*******************************(*)*******************************<*/
static void CountTexNRasStages  ( SHDRShader *shader, u8 *numTex, u8 *numRas )
{
    if(shader->shaderParams.complexShader.input1->type == SHADER_TYPE_TEXTURE)
        (*numTex)++;
    else if(shader->shaderParams.complexShader.input1->type == SHADER_TYPE_RASTERIZED)
        (*numRas)++;
    else if(shader->shaderParams.complexShader.input1->type == SHADER_TYPE_COMPLEX)
    {
        (*numRas)++;
        (*numTex)++;
    }

    if(shader->shaderParams.complexShader.input2->type == SHADER_TYPE_TEXTURE)
        (*numTex)++;
    else if(shader->shaderParams.complexShader.input2->type == SHADER_TYPE_RASTERIZED)
        (*numRas)++;
    else if(shader->shaderParams.complexShader.input2->type == SHADER_TYPE_COMPLEX)
    {
        (*numRas)++;
        (*numTex)++;
    }

    if(shader->shaderParams.complexShader.input3->type == SHADER_TYPE_TEXTURE)
        (*numTex)++;
    else if(shader->shaderParams.complexShader.input3->type == SHADER_TYPE_RASTERIZED)
        (*numRas)++;
    else if(shader->shaderParams.complexShader.input3->type == SHADER_TYPE_COMPLEX)
    {
        (*numRas)++;
        (*numTex)++;
    }

    if(shader->shaderParams.complexShader.input4->type == SHADER_TYPE_TEXTURE)
        (*numTex)++;
    else if(shader->shaderParams.complexShader.input4->type == SHADER_TYPE_RASTERIZED)
        (*numRas)++;
    else if(shader->shaderParams.complexShader.input4->type == SHADER_TYPE_COMPLEX)
    {
        (*numRas)++;
        (*numTex)++;
    }
}