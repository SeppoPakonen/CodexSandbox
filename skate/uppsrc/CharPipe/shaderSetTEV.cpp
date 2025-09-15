/*---------------------------------------------------------------------------*
  Project:  Dolphin
  File:     shaderSetTEV.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.
    
  $Log: /Dolphin/build/charPipeline/shader/src/shaderSetTEV.c $    
    
    8     12/08/00 2:04p John
    Disabled ShaderSetTEVClampMode for > HW1 compatibility.
    
    7     7/27/00 10:28a Ryan
    cleanup to remove warnings in the MAC build
    
    6     7/19/00 3:59p Ryan
    update for precompiled shaders
    
    5     6/13/00 11:09a Ryan
    bug fixes found while working on cardemo
    
    4     4/12/00 6:28a Ryan
    Update for color/alpha and texgen shader revision
    
    3     3/16/00 2:44p Ryan
    bug fix for TEV setting output register
    
    2     3/14/00 11:51a Ryan
    Added include<Dolphin/gx.h>
    
    1     3/06/00 2:32p Ryan

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <Dolphin/os.h>
#include <Dolphin/gx.h>

#include "shaderSetTEV.h"
#include "shaderResManage.h"
#include "shaderTCInternals.h"

/*>*******************************(*)*******************************<*/
static void LoadTextures	        ( CompiledShaderResources *res );
static void LoadColors	            ( CompiledShaderResources *res );
static void ShaderSetTEVOrder	    ( CompiledTEVStage *colorStage, CompiledTEVStage *alphaStage, CompiledShaderResources *res, u8 id );
static void ShaderSetTEVClampMode	( CompiledTEVStage *color, u8 id );
static void ShaderSetColorTEVStage	( CompiledTEVStage *stage, CompiledShaderResources *res, u8 id );
static void ShaderSetAlphaTEVStage	( CompiledTEVStage *stage, CompiledShaderResources *res, u8 id );

static GXTevStageID TranslateStageID		( u8 id );
static GXTevColorArg TranslateTEVColorArg	( SHADER_TEVArg arg, CompiledShaderResources *res );
static GXTevAlphaArg TranslateTEVAlphaArg	( SHADER_TEVArg arg, CompiledShaderResources *res );

//static void ShaderCheckRasColors	( u8 id );

/*>*******************************(*)*******************************<*/
//temporary variables to set number of color channels used
static BOOL RAS0used;
static BOOL RAS1used;

GXTexCoordID    coordIDArray[] = {GX_TEXCOORD0, GX_TEXCOORD1, GX_TEXCOORD2, GX_TEXCOORD3, GX_TEXCOORD4, GX_TEXCOORD5, GX_TEXCOORD6, GX_TEXCOORD7};
GXTexMapID      mapIDArray[] = {GX_TEXMAP0, GX_TEXMAP1, GX_TEXMAP2, GX_TEXMAP3, GX_TEXMAP4, GX_TEXMAP5, GX_TEXMAP6, GX_TEXMAP7};
//GXChannelID     colorIDArray[] = {GX_COLOR0A0, GX_COLOR1A1, GX_COLOR_NULL};

/*>*******************************(*)*******************************<*/
void SetTEV	( SHDRCompiled *shader )
{
	u8 i;
    OptimizedTexCoordExpressions *tcExp = (OptimizedTexCoordExpressions *)(shader->texGen);

    //u16 token = 0;

    //reset ras flags
    RAS0used = FALSE;
    RAS1used = FALSE;

/*#ifndef MAC
    GXSetDrawSync(token);
    while(token != GXReadDrawSync()){}
    token ++;
#endif
*/
	//load in all textures
	LoadTextures((CompiledShaderResources *)(shader->shaderResources));

/*#ifndef MAC
    GXSetDrawSync(token);
    while(token != GXReadDrawSync()){}
    token ++;
#endif*/

	//load in all colors
	LoadColors((CompiledShaderResources *)(shader->shaderResources));

/*#ifndef MAC
    GXSetDrawSync(token);
    while(token != GXReadDrawSync()){}
    token ++;
#endif*/

	//set number of TEV stages
#ifndef MAC
	GXSetNumTevStages(shader->numStages);
#endif

/*#ifndef MAC
    GXSetDrawSync(token);
    while(token != GXReadDrawSync()){}
    token ++;
#endif*/

	for ( i = 0; i < shader->numStages; i ++ )
	{
		//set TEV order for each TEV stage
        ShaderSetTEVOrder( &(((CompiledTEVStage *)(shader->colorStages))[i]), 
                           &(((CompiledTEVStage *)(shader->alphaStages))[i]),
                           (CompiledShaderResources *)(shader->shaderResources),
                           i);

/*#ifndef MAC
        GXSetDrawSync(token);
        while(token != GXReadDrawSync()){}
        token ++;
#endif*/

		// set TEV clamp mode for each TEV stage
        ShaderSetTEVClampMode(&(((CompiledTEVStage *)(shader->colorStages))[i]), i);

/*#ifndef MAC
        GXSetDrawSync(token);
        while(token != GXReadDrawSync()){}
        token ++;
#endif*/
		
		// set Tev Stage for color
		ShaderSetColorTEVStage(&(((CompiledTEVStage *)(shader->colorStages))[i]), 
                               (CompiledShaderResources *)(shader->shaderResources),
                               i);

/*#ifndef MAC
        GXSetDrawSync(token);
        while(token != GXReadDrawSync()){}
        token ++;
#endif*/

		// set Tev Stage for alpha
		ShaderSetAlphaTEVStage(&(((CompiledTEVStage *)(shader->alphaStages))[i]), 
                               (CompiledShaderResources *)(shader->shaderResources),
                               i);

/*#ifndef MAC
        GXSetDrawSync(token);
        while(token != GXReadDrawSync()){}  
        token ++;
#endif*/
        // check to see which color channels are used
        //ShaderCheckRasColors(i);
	}

    if(RAS1used)
        GXSetNumChans(2);
    else if(RAS0used)
        GXSetNumChans(1);
    else
    {
        if((tcExp->numExpressions))
            GXSetNumChans(0);
        else
            GXSetNumChans(1); //need to generate at least 1 color or texture coord from the XF - THANKS ROB!
    }

/*#ifndef MAC
    GXSetDrawSync(token);
    while(token != GXReadDrawSync()){}
    token ++;
#endif*/
}

/*>*******************************(*)*******************************<*/
static void LoadTextures	( CompiledShaderResources *res )
{
	u8 i; 

	for ( i = 0; i < 8; i ++ )
	{
		if(res->textureUsed[i])
        {
#ifndef MAC
			GXLoadTexObj(res->textureData[i], Resources.mapIdList[i]);
#endif
        }
	}
}

/*>*******************************(*)*******************************<*/
static void LoadColors	( CompiledShaderResources *res )
{
	u8 i;
	GXColor color;
	GXTevRegID id;

	for ( i = 0; i < 4; i ++ )
	{
        if(res->regColor[i] != SHADER_COLOR_NONE ||
           res->regAlpha[i] != SHADER_COLOR_NONE)
		{
			if(res->regAlpha[i] != SHADER_COLOR_NONE)
				color.a = res->colorData[((u32)(res->regAlpha[i]))].a;
			if(res->regColor[i] != SHADER_COLOR_NONE)
			{
				color.r = res->colorData[((u32)(res->regColor[i]))].r;
				color.g = res->colorData[((u32)(res->regColor[i]))].g;
				color.b = res->colorData[((u32)(res->regColor[i]))].b;
			}

			switch(i)
			{
				case 0:
					id = GX_TEVPREV;
					break;
				case 1:
					id = GX_TEVREG0;
					break;
				case 2:
					id = GX_TEVREG1;
					break;
				case 3:
					id = GX_TEVREG2;
					break;
			}
#ifndef MAC
			GXSetTevColor(id, color);
#endif
		}
	}
}

/*>*******************************(*)*******************************<*/
static void ShaderSetTEVOrder	( CompiledTEVStage *colorStage, CompiledTEVStage *alphaStage, CompiledShaderResources *res, u8 id )
{
#ifdef MAC
    #pragma unused (id)
#endif

    GXTexCoordID coord;
    GXTexMapID map;
    GXChannelID color;

    if(colorStage->texGenIdx == NO_TEXTURE_COORD_EXP)
    {
        if(alphaStage->texGenIdx == NO_TEXTURE_COORD_EXP)
            coord = GX_TEXCOORD_NULL;
        else
            coord = coordIDArray[alphaStage->texGenIdx];
    }
    else
        coord = coordIDArray[colorStage->texGenIdx];

    if(colorStage->texInput == SHADER_TEXNONE)
    {
        if(alphaStage->texInput == SHADER_TEXNONE)
            map = GX_TEXMAP_NULL;
        else
            map = mapIDArray[(u32)(alphaStage->texInput)];
    }
    else
        map = mapIDArray[(u32)(colorStage->texInput)];

	if(colorStage->rasInput == SHADER_RASNONE)
    {
        if(alphaStage->rasInput == SHADER_RASNONE)
            color = GX_COLOR_NULL;
        else
            color = res->rasData[(u32)(alphaStage->rasInput)];
    }
    else
        color = res->rasData[(u32)(colorStage->rasInput)];

    if(color == GX_COLOR0A0)        RAS0used = TRUE;
    else if(color == GX_COLOR1A1)   RAS1used = TRUE;

#ifndef MAC	
	GXSetTevOrder(TranslateStageID(id), coord, map, color);
#endif
}

/*>*******************************(*)*******************************<*/
static void ShaderSetTEVClampMode	( CompiledTEVStage *color, u8 id )
{
#ifndef HW1
    #pragma unused (color)
    #pragma unused (id)
#endif

#ifdef HW1
	GXSetTevClampMode(TranslateStageID(id), color->mode);
#endif
}

/*>*******************************(*)*******************************<*/
static void ShaderSetColorTEVStage	( CompiledTEVStage *stage, CompiledShaderResources *res, u8 id )
{
#ifdef MAC
    #pragma unused (res)
    #pragma unused (id)
#endif

	GXTevRegID reg;
#ifndef MAC
	GXSetTevColorIn(TranslateStageID(id),
					TranslateTEVColorArg(stage->TEVArg[0], res),
					TranslateTEVColorArg(stage->TEVArg[1], res),
					TranslateTEVColorArg(stage->TEVArg[2], res),
					TranslateTEVColorArg(stage->TEVArg[3], res) );
#endif

	switch(stage->out_reg)
	{
		case SHADER_CPREV:
			reg = GX_TEVPREV;
			break;
		case SHADER_C0:
			reg = GX_TEVREG0;
			break;
		case SHADER_C1:
			reg = GX_TEVREG1;
			break;
		case SHADER_C2:
			reg = GX_TEVREG2;
			break;
	}

#ifndef MAC
	GXSetTevColorOp(TranslateStageID(id),
					stage->op,
					stage->bias,
					stage->scale,
					stage->clamp,
					reg);
#endif
}

/*>*******************************(*)*******************************<*/
static void ShaderSetAlphaTEVStage	( CompiledTEVStage *stage, CompiledShaderResources *res, u8 id )
{
#ifdef MAC
    #pragma unused (res)
    #pragma unused (id)
#endif

	GXTevRegID reg;
#ifndef MAC
	GXSetTevAlphaIn(TranslateStageID(id),
					TranslateTEVAlphaArg(stage->TEVArg[0], res),
					TranslateTEVAlphaArg(stage->TEVArg[1], res),
					TranslateTEVAlphaArg(stage->TEVArg[2], res),
					TranslateTEVAlphaArg(stage->TEVArg[3], res) );
#endif

	switch(stage->out_reg)
	{
		case SHADER_APREV:
			reg = GX_TEVPREV;
			break;
		case SHADER_A0:
			reg = GX_TEVREG0;
			break;
		case SHADER_A1:
			reg = GX_TEVREG1;
			break;
		case SHADER_A2:
			reg = GX_TEVREG2;
			break;
	}

#ifndef MAC
	GXSetTevAlphaOp(TranslateStageID(id),
					stage->op,
					stage->bias,
					stage->scale,
					stage->clamp,
					reg);
#endif
}

/*>*******************************(*)*******************************<*/
static GXTevStageID TranslateStageID	( u8 id )
{
	switch(id)
	{
		case 0:
			return GX_TEVSTAGE0;
		case 1:
			return GX_TEVSTAGE1;
		case 2:
			return GX_TEVSTAGE2; 
		case 3:
			return GX_TEVSTAGE3;
		case 4:
			return GX_TEVSTAGE4; 
		case 5:
			return GX_TEVSTAGE5; 
		case 6:
			return GX_TEVSTAGE6; 
		case 7:
			return GX_TEVSTAGE7; 
		case 8:
			return GX_TEVSTAGE8; 
		case 9:
			return GX_TEVSTAGE9; 
		case 10:
			return GX_TEVSTAGE10; 
		case 11:
			return GX_TEVSTAGE11; 
		case 12:
			return GX_TEVSTAGE12; 
		case 13:
			return GX_TEVSTAGE13; 
		case 14:
			return GX_TEVSTAGE14; 
		case 15:
			return GX_TEVSTAGE15; 
	}
	return GX_TEVSTAGE0;
}

/*>*******************************(*)*******************************<*/
static GXTevColorArg TranslateTEVColorArg	( SHADER_TEVArg arg, CompiledShaderResources *res )
{
    u8 i;

	switch(arg)
	{

		case SHADER_CPREV:
			return GX_CC_CPREV;
		case SHADER_C0:
			return GX_CC_C0;
		case SHADER_C1:
			return GX_CC_C1;
		case SHADER_C2:
			return GX_CC_C2;
		case SHADER_APREV:
			return GX_CC_APREV;
		case SHADER_A0:
			return GX_CC_A0;
		case SHADER_A1:
			return GX_CC_A1;
		case SHADER_A2:
			return GX_CC_A2;
		case SHADER_TEXC:
			return GX_CC_TEXC;
		case SHADER_TEXA:
			return GX_CC_TEXA;
		case SHADER_RASC:
			return GX_CC_RASC;
		case SHADER_RASA:
			return GX_CC_RASA;
		case SHADER_TEXRRR:
			return GX_CC_TEXRRR;
		case SHADER_TEXGGG:
			return GX_CC_TEXGGG;
		case SHADER_TEXBBB:
			return GX_CC_TEXBBB;
		case SHADER_HALF:
			return GX_CC_HALF;
		case SHADER_QUARTER:
			return GX_CC_QUARTER;
		case SHADER_ONE:
			return GX_CC_ONE;  
		case SHADER_ZERO:
			return GX_CC_ZERO;

        //constant colors
        case SHADER_COLORINPUT0_RGB:
        case SHADER_COLORINPUT1_RGB:
        case SHADER_COLORINPUT2_RGB:
        case SHADER_COLORINPUT3_RGB: 
            for ( i = 0; i < 4; i ++ )
            {
                if(res->regColor[i] == (SHADER_ColorInput)(arg - SHADER_COLORINPUT0_RGB))
                    return TranslateTEVColorArg((SHADER_TEVArg)(i), res);
            }
            OSHalt("constant color not found - TranslateTEVColorArg");
            break;

        //constant alphas
        case SHADER_COLORINPUT0_A:
        case SHADER_COLORINPUT1_A:
        case SHADER_COLORINPUT2_A:
        case SHADER_COLORINPUT3_A:
            for ( i = 0; i < 4; i ++ )
            {
                if(res->regAlpha[i] == (SHADER_ColorInput)(arg - SHADER_COLORINPUT0_A))
                    return TranslateTEVColorArg((SHADER_TEVArg)(((u32)i) + ((u32)(SHADER_APREV))), res);
            }
            OSHalt("constant alpha not found - TranslateTEVColorArg");
            break;

		default:
			ASSERTMSG(0, "unknown color arg type in shader compilation");
	}
	return GX_CC_ZERO;
}

/*>*******************************(*)*******************************<*/
static GXTevAlphaArg TranslateTEVAlphaArg	( SHADER_TEVArg arg, CompiledShaderResources *res )
{
    u8 i;

	switch(arg)
	{

		case SHADER_APREV:
			return GX_CA_APREV;
		case SHADER_A0:
			return GX_CA_A0;
		case SHADER_A1:
			return GX_CA_A1;
		case SHADER_A2:
			return GX_CA_A2;
		case SHADER_TEXA:
			return GX_CA_TEXA;
		case SHADER_RASA:
			return GX_CA_RASA;
		case SHADER_ONE:
			return GX_CA_ONE;  
		case SHADER_ZERO:
			return GX_CA_ZERO;

        //constant alphas
        case SHADER_COLORINPUT0_A:
        case SHADER_COLORINPUT1_A:
        case SHADER_COLORINPUT2_A:
        case SHADER_COLORINPUT3_A:
            for ( i = 0; i < 4; i ++ )
            {
                if(res->regAlpha[i] == (SHADER_ColorInput)(arg - SHADER_COLORINPUT0_A))
                    return TranslateTEVAlphaArg((SHADER_TEVArg)(((u32)i) + ((u32)(SHADER_APREV))), res);
            }
            OSHalt("constant alpha not found - TranslateTEVAlphaArg");
            break;

		default:
			ASSERTMSG(0, "unknown alpha arg type in shader compilation");
	}
	return GX_CA_ZERO;
}

/*>*******************************(*)*******************************<*
static void ShaderCheckRasColors	( CompiledTEVStage *color, CompiledTEVStage *alpha )
{
    if(BestColorInstructions[id].color == GX_COLOR0A0 ||
       BestAlphaInstructions[id].color == GX_COLOR0A0 )
    {
        RAS0used = TRUE;
    }

    if(BestColorInstructions[id].color == GX_COLOR1A1 ||
       BestAlphaInstructions[id].color == GX_COLOR1A1 )
    {
        RAS1used = TRUE;
    }
}

/*>*******************************(*)*******************************<*/