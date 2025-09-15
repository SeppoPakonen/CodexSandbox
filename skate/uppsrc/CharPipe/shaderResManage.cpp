
/*---------------------------------------------------------------------------*
  Project:  Dolphin
  File:     shaderResManage.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.
    
  $Log: /Dolphin/build/charPipeline/shader/src/shaderResManage.c $    
    
    6     7/19/00 3:59p Ryan
    update for precompiled shaders
    
    5     6/07/00 1:08p Ryan
    checkin for shader optimization v1.0
    
    4     4/12/00 6:28a Ryan
    Update for color/alpha and texgen shader revision
    
    3     3/06/00 2:33p Ryan
    Optimization update 3/6/00
    
    2     2/28/00 3:50p Ryan
    optimization update for shader lib
    
    1     2/28/00 11:23a Ryan

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <Dolphin/os.h>

#include "shaderResManage.h"

/*>*******************************(*)*******************************<*/
static u8 FindInstructionLifetime		( ShaderTEVStage *stage, u8 index );

static u8 AllocateIntermediateRegister	( SHADER_RegisterState	*regState,
										  u8 *regLifetime, u8 time, u8 lifetime );

/*>*******************************(*)*******************************<*/
ShaderTEVStage	TEVPool[50];
u32				TEVCounter;

ShaderTEVStage	*AlphaInstructions[16];
ShaderTEVStage	*ColorInstructions[16];

ShaderTEVStage	BestAlphaInstructions[16];
ShaderTEVStage	BestColorInstructions[16];
u8				BestNumTEVStages;

ShaderResources  Resources =		{	SHADER_REG_EMPTY, 
										SHADER_REG_EMPTY,
										SHADER_REG_EMPTY,
										SHADER_REG_EMPTY,

										SHADER_COLOR_NONE, 
										SHADER_COLOR_NONE, 
										SHADER_COLOR_NONE,
										SHADER_COLOR_NONE,

										SHADER_CPREV,
										SHADER_C0,				
										SHADER_C1,
										SHADER_C2,

										0,
										0,
										0,
										0,

										0,
										0,
										0,
										0,

										SHADER_REG_EMPTY, 
										SHADER_REG_EMPTY,
										SHADER_REG_EMPTY,
										SHADER_REG_EMPTY,

										SHADER_COLOR_NONE, 
										SHADER_COLOR_NONE, 
										SHADER_COLOR_NONE,
										SHADER_COLOR_NONE,

										SHADER_APREV,
										SHADER_A0,
										SHADER_A1,
										SHADER_A2,

										0,
										0,
										0,
										0,

										0,
										0,
										0,
										0,

										0,
										0,
										0,
										0,
										0,
										0,
										0,
										0,

										GX_TEXMAP0,
										GX_TEXMAP1,
										GX_TEXMAP2,
										GX_TEXMAP3,
										GX_TEXMAP4,
										GX_TEXMAP5,
										GX_TEXMAP6,
										GX_TEXMAP7	};

/*>*******************************(*)*******************************<*/
void ResetShaderResources	( void )
{
	u8 i;

	for ( i = 0; i < 4; i ++ )
	{
		Resources.regColorState[i] = SHADER_REG_EMPTY;
        Resources.regColor[i] = SHADER_COLOR_NONE;
        Resources.colorSrc[i] = 0;
        Resources.regAlphaState[i] = SHADER_REG_EMPTY;
        Resources.regAlpha[i] = SHADER_COLOR_NONE;
        Resources.alphaSrc[i] = 0;
	}

	for ( i = 0; i < 8; i ++ )
	{
		Resources.texObj[i] = 0;
	}

	TEVCounter = 0;
	BestNumTEVStages = 99;

	for ( i = 0; i < 16; i ++ )
	{
		AlphaInstructions[i] = 0;
		ColorInstructions[i] = 0;
	}
}

/*>*******************************(*)*******************************<*/
SHADER_TEVArg AllocateColorResource ( SHADER_ColorInput color )
{
	u8 i;

	for(i = 3; i > 0; i -- )
	{
		if(Resources.regColorState[i] == SHADER_REG_EMPTY)
		{
			Resources.regColorState[i] = SHADER_REG_CONSTANTCOLOR;
			Resources.regColor[i] = color;
            return (SHADER_TEVArg)(((u32)(SHADER_COLORINPUT0_RGB)) + ((u32)(color)));
			//return Resources.colorIdList[i];
		}

		if(Resources.regColor[i] == color)
		{
            return (SHADER_TEVArg)(((u32)(SHADER_COLORINPUT0_RGB)) + ((u32)(color)));
			//return Resources.colorIdList[i];
		}
	}

	ASSERTMSG(0, "not enough available color registers for this shader - ExecuteShader");

	//never reached
    return (SHADER_TEVArg)(((u32)(SHADER_COLORINPUT0_RGB)) + ((u32)(color)));
	//return Resources.colorIdList[3];
}

/*>*******************************(*)*******************************<*/
SHADER_TEVArg AllocateAlphaResource ( SHADER_ColorInput color )
{
	u8 i;

	for(i = 3; i > 0; i -- )
	{
		if(Resources.regAlphaState[i] == SHADER_REG_EMPTY)
		{
			Resources.regAlphaState[i] = SHADER_REG_CONSTANTCOLOR;
			Resources.regAlpha[i] = color;
            return (SHADER_TEVArg)(((u32)(SHADER_COLORINPUT0_A)) + ((u32)(color)));
			//return Resources.alphaIdList[i];
		}

		if(Resources.regAlpha[i] == color)
		{
            return (SHADER_TEVArg)(((u32)(SHADER_COLORINPUT0_A)) + ((u32)(color)));
			//return Resources.alphaIdList[i];
		}
	}

	ASSERTMSG(0, "not enough available alpha registers for this shader - ExecuteShader");

	//never reached
    return (SHADER_TEVArg)(((u32)(SHADER_COLORINPUT0_A)) + ((u32)(color)));
	//return Resources.alphaIdList[3];
}

/*>*******************************(*)*******************************<*/
GXTexMapID AllocateTextureResource ( GXTexObj *tex )
{
	u8 i;

	for(i = 0; i < 8; i ++ )
	{
		if(!(Resources.texObj[i]))
		{
			Resources.texObj[i] = tex;
			return Resources.mapIdList[i];
		}

		if(Resources.texObj[i] == tex)
		{
			return Resources.mapIdList[i];
		}
	}

	ASSERTMSG(0, "not enough available texture resources for this shader - ExecuteShader");

	//never reached
	return Resources.mapIdList[0];
}

/*>*******************************(*)*******************************<*/
u8	CheckResourceCollision	( ShaderTEVStage *stage, ShaderTEVStage *parent )
{
	if(!(stage->texGenIdx == NO_TEXTURE_COORD_EXP || parent->texGenIdx == NO_TEXTURE_COORD_EXP || parent->texGenIdx == stage->texGenIdx))
		return 0;

	if(!(stage->texInput == SHADER_TEXNONE || parent->texInput == SHADER_TEXNONE || parent->texInput == stage->texInput))
		return 0;

	if(!(stage->rasInput == SHADER_RASNONE || parent->rasInput == SHADER_RASNONE || parent->rasInput == stage->rasInput))
		return 0;

	if(stage->op != parent->op)
		return 0;
	if(stage->bias != parent->bias)
		return 0;
	if(stage->scale != parent->scale)
		return 0;
    if(stage->mode != parent->mode)
		return 0;
	if(stage->clamp != parent->clamp)
    {
        if(stage->clamp != GX_FALSE ||
           parent->clamp != GX_TRUE ||
           stage->mode != GX_TC_LINEAR)
		return 0;
    }
	
	return 1; 
}

/*>*******************************(*)*******************************<*
u8	CheckResourceCollision	( ShaderTEVStage *stage, ShaderTEVStage *parent )
{
	GXTexCoordID    coord;
    GXTexMapID      map;
    GXChannelID     color;

	if(stage->coord == GX_TEXCOORD_NULL)
		coord = parent->coord;
	else if (parent->coord == GX_TEXCOORD_NULL || parent->coord == stage->coord)
		coord = stage->coord;
	else
		return 0;

	if(stage->map == GX_TEXMAP_NULL)
		map = parent->map;
	else if (parent->map == GX_TEXMAP_NULL || parent->map == stage->map)
		map = stage->map;
	else
		return 0;

	if(stage->color == GX_COLOR_NULL)
		color = parent->color;
	else if (parent->color == GX_COLOR_NULL || parent->color == stage->color)
		color = stage->color;
	else
		return 0;

	if(stage->op != parent->op)
		return 0;
	if(stage->bias != parent->bias)
		return 0;
	if(stage->scale != parent->scale)
		return 0;
	if(stage->clamp != parent->clamp)
		return 0;
	if(stage->mode != parent->mode)
		return 0;

	parent->coord = coord;
	parent->map = map;
	parent->color = color;
	return 1; 
}

/*>*******************************(*)*******************************<*/
u8 CheckResourceAllocation	( void )
{
	s8 i, j;
	u8 outputIndex;
	u8 time;
	u8 cost = 0;

	//check cost
	for ( i = 0; i < 16; i ++ )
	{
		if(ColorInstructions[i] || AlphaInstructions[i])
			cost ++;
		else
			break;
	}
#ifdef MAC
    OSReport("compiled cost is %d\n", cost);
#endif

	if(cost < BestNumTEVStages)
	{
		//clear intermediate resources
		for ( i = 0; i < 4; i ++ )
		{
			Resources.colorLifetime[i] = 99;
			Resources.alphaLifetime[i] = 99;
		}

		for ( i = (s8)(cost - 1); i >= 0; i -- )
		{
			if(ColorInstructions[i])
			{
				time = FindInstructionLifetime(ColorInstructions[i], (u8)i);
				outputIndex = AllocateIntermediateRegister(Resources.regColorState, Resources.colorLifetime, (u8)i, time);
				if(outputIndex != 99)
					ColorInstructions[i]->out_reg = Resources.colorIdList[outputIndex];
				else // no allocation possible - return an error
					return 0;
			}
			
			if(AlphaInstructions[i])
			{
				time = FindInstructionLifetime(AlphaInstructions[i], (u8)i);
				outputIndex = AllocateIntermediateRegister(Resources.regAlphaState, Resources.alphaLifetime, (u8)i, time);
				if(outputIndex != 99)
					AlphaInstructions[i]->out_reg = Resources.alphaIdList[outputIndex];
				else // no allocation possible - return an error
					return 0;
			} 
		}

        //go through an inherit child TEV stages' output registers
        for ( i = (s8)(cost - 1); i >= 0; i -- )
		{
			if(ColorInstructions[i])
			{
				for ( j = 0; j < 4; j ++ )
				{
                    //assign child's output reg to parent's input TEV ARG
					if(ColorInstructions[i]->tevStage[j])
						ColorInstructions[i]->TEVArg[j] = ColorInstructions[i]->tevStage[j]->out_reg;
				}
			}
			
			if(AlphaInstructions[i])
			{
				for ( j = 0; j < 4; j ++ )
				{
					if(AlphaInstructions[i]->tevStage[j])
						AlphaInstructions[i]->TEVArg[j] = AlphaInstructions[i]->tevStage[j]->out_reg;
				}
			} 
		}
	}
	
	return cost;
}

/*>*******************************(*)*******************************<*
u8 CheckResourceAllocation	( void )
{
	u8 i, j;
	u8 outputIndex;
	u8 time;
	u8 cost = 0;

	//check cost
	for ( i = 0; i < 16; i ++ )
	{
		if(ColorInstructions[i] || AlphaInstructions[i])
			cost ++;
		else
			break;
	}

	if(cost < BestNumTEVStages)
	{
		//clear intermediate resources
		for ( i = 0; i < 4; i ++ )
		{
			Resources.colorLifetime[i] = 0;
			Resources.alphaLifetime[i] = 0;
		}

		for ( i = 0; i < cost; i ++ )
		{
			if(ColorInstructions[i])
			{
				time = FindInstructionLifetime(ColorInstructions[i], i);
				outputIndex = AllocateIntermediateRegister(Resources.regColorState, Resources.colorLifetime, i, time);
				if(outputIndex != 99)
					ColorInstructions[i]->out_reg = Resources.colorIdList[outputIndex];
				else // no allocation possible - return an error
					return 0;

				for ( j = 0; j < 4; j ++ )
				{
					if(ColorInstructions[i]->tevStage[j])
						ColorInstructions[i]->TEVArg[j] = ColorInstructions[i]->tevStage[j]->out_reg;
				}
			}
			
			if(AlphaInstructions[i])
			{
				time = FindInstructionLifetime(AlphaInstructions[i], i);
				outputIndex = AllocateIntermediateRegister(Resources.regAlphaState, Resources.alphaLifetime, i, time);
				if(outputIndex != 99)
					AlphaInstructions[i]->out_reg = Resources.alphaIdList[outputIndex];
				else // no allocation possible - return an error
					return 0;

				for ( j = 0; j < 4; j ++ )
				{
					if(AlphaInstructions[i]->tevStage[j])
						AlphaInstructions[i]->TEVArg[j] = AlphaInstructions[i]->tevStage[j]->out_reg;
				}
			} 
		}
		return cost;
	}
	

	return 0;
}

/*>*******************************(*)*******************************<*/
static u8 FindInstructionLifetime	( ShaderTEVStage *stage, u8 index )
{
	u8 i, j;
	u8 longest = 0;

	if(stage->type == SHADER_TYPE_EMPTY)
		return 0;

	for ( i = 0; i < stage->numParents; i ++ )
	{
		for ( j = (u8)(index + 1); j < 16; j ++ )
		{
			if(ColorInstructions[j] == stage->parent[i] ||
			   AlphaInstructions[j] == stage->parent[i])
			{
				if(j > longest) longest = j;
			}
		}
	}

	return longest;
}

/*>*******************************(*)*******************************<*/
static u8 AllocateIntermediateRegister	( SHADER_RegisterState	*regState,
										  u8 *regLifetime, u8 time, u8 lifetime )
{
		u8 i;

		for ( i = 0; i < 4; i ++ )
		{
			if(regState[i] == SHADER_REG_CONSTANTCOLOR) // we know all registers after this are 
				return 99;								// constant color, so break

            if(regLifetime[i] >= lifetime) // register isn't alive
			{
				regLifetime[i] = time;
				return i;
			}

			/*if(regLifetime[i] <= time) // register isn't alive
			{
				regLifetime[i] = lifetime;
				return i;
			}*/
		}

		return 99;
}

/*>*******************************(*)*******************************<*/
void InitCompiledResourceInfo    ( CompiledShaderResources *res )
{
    u8 i;

    for ( i = 0; i < 2; i ++ )
    {
        res->rasUsed[i] = 0;
    }

    for ( i = 0; i < 4; i ++ )
    {
        res->colorUsed[i] = 0;
    }

    for ( i = 0; i < 4; i ++ )
    {
        res->regColor[i] = Resources.regColor[i];
        res->regAlpha[i] = Resources.regAlpha[i];
        if(res->regColor[i] != SHADER_COLOR_NONE)
            res->colorUsed[(u32)(res->regColor[i])] = 1;
        if(res->regAlpha[i] != SHADER_COLOR_NONE)
            res->colorUsed[(u32)(res->regAlpha[i])] = 1;
    }

    for ( i = 0; i < 8; i ++ )
    {
        res->textureUsed[i] = 0;
        res->textureData[i] = 0;

        res->complexUsed[i] = 0;
        res->complexData[i] = 0;
    }
}

/*>*******************************(*)*******************************<*/