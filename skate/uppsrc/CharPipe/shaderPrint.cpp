
/*---------------------------------------------------------------------------*
  Project:  Dolphin
  File:     shaderPrint.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.
    
  $Log: /Dolphin/build/charPipeline/shader/src/shaderPrint.c $    
    
    4     7/19/00 3:59p Ryan
    update for precompiled shaders
    
    3     3/06/00 2:33p Ryan
    Optimization update 3/6/00
    
    2     2/28/00 3:50p Ryan
    optimization update for shader lib
    
    1     2/28/00 11:23a Ryan

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <stdio.h>

#include "shaderPrint.h"
#include "shaderResManage.h"
#include "shaderTCInternals.h"

/*>*******************************(*)*******************************<*/
#ifdef MAC
static void PrintTEVStage	( CompiledTEVStage *stage, FILE *fp );
static void PrintTEVArgs	( SHADER_TEVArg arg, FILE *fp );
static void PrintResources  ( CompiledShaderResources *res, OptimizedTexCoordExpressions *tcExp, FILE *fp );
#endif

#ifdef MAC
/*>*******************************(*)*******************************<*/
void PrintTEVTree	( SHDRCompiled *shader )
{
    CompiledTEVStage *color = (CompiledTEVStage *)(shader->colorStages);
    CompiledTEVStage *alpha = (CompiledTEVStage *)(shader->alphaStages);
    CompiledShaderResources *resources = (CompiledShaderResources *)(shader->shaderResources);
    OptimizedTexCoordExpressions *tcExp = (OptimizedTexCoordExpressions *)(shader->texGen);

	FILE *fp = fopen("TEVTree.txt", "w");
	u8 i;

	for ( i = 0; i < shader->numStages; i ++ )
	{
		
		fprintf(fp, "------------------------------------------\n");	
		fprintf(fp, "Stage number %d \n", i);	
		fprintf(fp, "------------------------------------------\n\n");

		fprintf(fp, "COLOR STAGE: \n");	
		PrintTEVStage(&(color[i]), fp);
		
		fprintf(fp, "ALPHA STAGE: \n");	
		PrintTEVStage(&(alpha[i]), fp);
	}	

    PrintResources(resources, tcExp, fp);

	fclose(fp);
}
#endif

#ifdef MAC
/*>*******************************(*)*******************************<*/
static void PrintTEVStage	( CompiledTEVStage *stage, FILE *fp )
{
	u8 i;
	fprintf(fp, "stage ID = %x \n\n", (u32)stage );	

	for ( i = 0; i < 4; i ++ )
	{
		fprintf(fp, "Shader Argument %d is ", i);	
		PrintTEVArgs(stage->TEVArg[i], fp);

		//fprintf(fp, "Shader TEV stage %d is %x\n\n", i, stage->tevStage[i]);
	}

	fprintf(fp, "op = %d\n", stage->op);
	fprintf(fp, "bias = %d\n", stage->bias);
	fprintf(fp, "scale = %d\n", stage->scale);
	fprintf(fp, "clamp = %d\n", stage->clamp);
	fprintf(fp, "clamp mode = %d\n", stage->mode);

	fprintf(fp, "out_reg = ");
	PrintTEVArgs(stage->out_reg, fp);

	fprintf(fp, "tex exp index = %d\n", stage->texGenIdx);
	fprintf(fp, "tex input = %d\n", stage->texInput);
	fprintf(fp, "ras input = %d\n\n\n\n", stage->rasInput);
}

/*>*******************************(*)*******************************<*/
static void PrintTEVArgs	( SHADER_TEVArg arg, FILE *fp )
{
	switch(arg)
	{
		case SHADER_C0:
			fprintf(fp, "SHADER_C0\n");
			break;
    	case SHADER_A0:
			fprintf(fp, "SHADER_A0\n");
			break;
    	case SHADER_C1:
			fprintf(fp, "SHADER_C1\n");
			break;
    	case SHADER_A1:
			fprintf(fp, "SHADER_A1\n");
			break;
    	case SHADER_C2:
			fprintf(fp, "SHADER_C2\n");
			break;
    	case SHADER_A2:
			fprintf(fp, "SHADER_A2\n");
			break;
    	case SHADER_CPREV:
			fprintf(fp, "SHADER_CPREV\n");
			break;
    	case SHADER_APREV:
			fprintf(fp, "SHADER_APREV\n");
			break;
    	case SHADER_TEXC:
			fprintf(fp, "SHADER_TEXC\n");
			break;
    	case SHADER_TEXA:
			fprintf(fp, "SHADER_TEXA\n");
			break;
    	case SHADER_RASC:
			fprintf(fp, "SHADER_RASC\n");
			break;
    	case SHADER_RASA:
			fprintf(fp, "SHADER_RASA\n");
			break;

		case SHADER_TEXRRR:
			fprintf(fp, "SHADER_TEXRRR\n");
			break;
    	case SHADER_TEXGGG:
			fprintf(fp, "SHADER_TEXGGG\n");
			break;
    	case SHADER_TEXBBB:
			fprintf(fp, "SHADER_TEXBBB\n");
			break;

		case SHADER_HALF:
			fprintf(fp, "SHADER_HALF\n");
			break;
    	case SHADER_QUARTER:
			fprintf(fp, "SHADER_QUARTER\n");
			break;

		case SHADER_NONTRIVIAL:
			fprintf(fp, "SHADER_NONTRIVIAL\n");
			break;

    	case SHADER_ONE:
			fprintf(fp, "SHADER_ONE\n");
			break;
    	case SHADER_ZERO:
			fprintf(fp, "SHADER_ZERO\n");
			break;

    	case SHADER_COMPLEXINPUT0_RGB:
    	case SHADER_COMPLEXINPUT1_RGB:
    	case SHADER_COMPLEXINPUT2_RGB:
    	case SHADER_COMPLEXINPUT3_RGB:
    	case SHADER_COMPLEXINPUT4_RGB:
        case SHADER_COMPLEXINPUT5_RGB:
        case SHADER_COMPLEXINPUT6_RGB:
    	case SHADER_COMPLEXINPUT7_RGB:
			fprintf(fp, "complex color input\n");
			break;

        case SHADER_COMPLEXINPUT0_A:
        case SHADER_COMPLEXINPUT1_A:
        case SHADER_COMPLEXINPUT2_A:
        case SHADER_COMPLEXINPUT3_A:
        case SHADER_COMPLEXINPUT4_A:
        case SHADER_COMPLEXINPUT5_A:
        case SHADER_COMPLEXINPUT6_A:
        case SHADER_COMPLEXINPUT7_A:
            fprintf(fp, "complex Alpha input\n");
			break;

        case SHADER_COLORINPUT0_RGB:
            fprintf(fp, "constant color input 0\n");
			break;
        case SHADER_COLORINPUT1_RGB:
            fprintf(fp, "constant color input 1\n");
			break;
        case SHADER_COLORINPUT2_RGB:
            fprintf(fp, "constant color input 2\n");
			break;
        case SHADER_COLORINPUT3_RGB:
            fprintf(fp, "constant color input 3\n");
			break;

        case SHADER_COLORINPUT0_A:
            fprintf(fp, "constant alpha input 0\n");
			break;
        case SHADER_COLORINPUT1_A:
            fprintf(fp, "constant alpha input 1\n");
			break;
        case SHADER_COLORINPUT2_A:
            fprintf(fp, "constant alpha input 2\n");
			break;
        case SHADER_COLORINPUT3_A:
            fprintf(fp, "constant alpha input 3\n");
			break;
	}
}

/*>*******************************(*)*******************************<*/
static void PrintResources  ( CompiledShaderResources *res, OptimizedTexCoordExpressions *tcExp, FILE *fp )
{
    u8 i;

    fprintf(fp, "ras colors used\n");
    for ( i = 0; i < 2; i ++ )
    {
        if(res->rasUsed[i])
            fprintf(fp, "%d ", i);
    }
    fprintf(fp, "\n");

    fprintf(fp, "constant colors used\n");
    for ( i = 0; i < 4; i ++ )
    {
        if(res->colorUsed[i])
            fprintf(fp, "%d ", i);
    }
    fprintf(fp, "\n");

    fprintf(fp, "textures used\n");
    for ( i = 0; i < 8; i ++ )
    {
        if(res->textureUsed[i])
            fprintf(fp, "%d ", i);
    }
    fprintf(fp, "\n");

    fprintf(fp, "complex inputs used\n");
    for ( i = 0; i < 8; i ++ )
    {
        if(res->complexUsed[i])
            fprintf(fp, "%d ", i);
    }
    fprintf(fp, "\n");

    fprintf(fp, "Mtx inputs used\n");
    for ( i = 0; i < 8; i ++ )
    {
        if(tcExp->mtxUsed[i])
            fprintf(fp, "%d ", i);
    }
    fprintf(fp, "\n");
}
#endif
/*>*******************************(*)*******************************<*/




#ifndef MAC
static void PrintTEVStage	( CompiledTEVStage *stage );
static void PrintTEVArgs	( SHADER_TEVArg arg );
static void PrintResources  ( CompiledShaderResources *res, OptimizedTexCoordExpressions *tcExp );
#endif

#ifndef MAC
/*>*******************************(*)*******************************<*/
void PrintTEVTree	( SHDRCompiled *shader )
{
    CompiledTEVStage *color = (CompiledTEVStage *)(shader->colorStages);
    CompiledTEVStage *alpha = (CompiledTEVStage *)(shader->alphaStages);
    CompiledShaderResources *resources = (CompiledShaderResources *)(shader->shaderResources);
    OptimizedTexCoordExpressions *tcExp = (OptimizedTexCoordExpressions *)(shader->texGen);

	u8 i;

	for ( i = 0; i < shader->numStages; i ++ )
	{
		
		OSReport("------------------------------------------\n");	
		OSReport("Stage number %d \n", i);	
		OSReport("------------------------------------------\n\n");

		OSReport("COLOR STAGE: \n");	
		PrintTEVStage(&(color[i]));
		
		OSReport("ALPHA STAGE: \n");	
		PrintTEVStage(&(alpha[i]));
	}	

    PrintResources(resources, tcExp);
}
#endif

#ifndef MAC
/*>*******************************(*)*******************************<*/
static void PrintTEVStage	( CompiledTEVStage *stage )
{
	u8 i;
	OSReport("stage ID = %x \n\n", (u32)stage );	

	for ( i = 0; i < 4; i ++ )
	{
		OSReport("Shader Argument %d is ", i);	
		PrintTEVArgs(stage->TEVArg[i]);
	}

	OSReport("op = %d\n", stage->op);
	OSReport("bias = %d\n", stage->bias);
	OSReport("scale = %d\n", stage->scale);
	OSReport("clamp = %d\n", stage->clamp);
	OSReport("clamp mode = %d\n", stage->mode);

	OSReport("out_reg = ");
	PrintTEVArgs(stage->out_reg);

	OSReport("tex exp index = %d\n", stage->texGenIdx);
	OSReport("tex input = %d\n", stage->texInput);
	OSReport("ras input = %d\n\n\n\n", stage->rasInput);
}

/*>*******************************(*)*******************************<*/
static void PrintTEVArgs	( SHADER_TEVArg arg)
{
	switch(arg)
	{
		case SHADER_C0:
			OSReport("SHADER_C0\n");
			break;
    	case SHADER_A0:
			OSReport("SHADER_A0\n");
			break;
    	case SHADER_C1:
			OSReport("SHADER_C1\n");
			break;
    	case SHADER_A1:
			OSReport("SHADER_A1\n");
			break;
    	case SHADER_C2:
			OSReport("SHADER_C2\n");
			break;
    	case SHADER_A2:
			OSReport("SHADER_A2\n");
			break;
    	case SHADER_CPREV:
			OSReport("SHADER_CPREV\n");
			break;
    	case SHADER_APREV:
			OSReport("SHADER_APREV\n");
			break;
    	case SHADER_TEXC:
			OSReport("SHADER_TEXC\n");
			break;
    	case SHADER_TEXA:
			OSReport("SHADER_TEXA\n");
			break;
    	case SHADER_RASC:
			OSReport("SHADER_RASC\n");
			break;
    	case SHADER_RASA:
			OSReport("SHADER_RASA\n");
			break;

		case SHADER_TEXRRR:
			OSReport("SHADER_TEXRRR\n");
			break;
    	case SHADER_TEXGGG:
			OSReport("SHADER_TEXGGG\n");
			break;
    	case SHADER_TEXBBB:
			OSReport("SHADER_TEXBBB\n");
			break;

		case SHADER_HALF:
			OSReport("SHADER_HALF\n");
			break;
    	case SHADER_QUARTER:
			OSReport("SHADER_QUARTER\n");
			break;

		case SHADER_NONTRIVIAL:
			OSReport("SHADER_NONTRIVIAL\n");
			break;

    	case SHADER_ONE:
			OSReport("SHADER_ONE\n");
			break;
    	case SHADER_ZERO:
			OSReport("SHADER_ZERO\n");
			break;

    	case SHADER_COMPLEXINPUT0_RGB:
    	case SHADER_COMPLEXINPUT1_RGB:
    	case SHADER_COMPLEXINPUT2_RGB:
    	case SHADER_COMPLEXINPUT3_RGB:
    	case SHADER_COMPLEXINPUT4_RGB:
        case SHADER_COMPLEXINPUT5_RGB:
        case SHADER_COMPLEXINPUT6_RGB:
    	case SHADER_COMPLEXINPUT7_RGB:
			OSReport("complex color input\n");
			break;

        case SHADER_COMPLEXINPUT0_A:
        case SHADER_COMPLEXINPUT1_A:
        case SHADER_COMPLEXINPUT2_A:
        case SHADER_COMPLEXINPUT3_A:
        case SHADER_COMPLEXINPUT4_A:
        case SHADER_COMPLEXINPUT5_A:
        case SHADER_COMPLEXINPUT6_A:
        case SHADER_COMPLEXINPUT7_A:
            OSReport("complex Alpha input\n");
			break;

        case SHADER_COLORINPUT0_RGB:
            OSReport("constant color input 0\n");
			break;
        case SHADER_COLORINPUT1_RGB:
            OSReport("constant color input 1\n");
			break;
        case SHADER_COLORINPUT2_RGB:
            OSReport("constant color input 2\n");
			break;
        case SHADER_COLORINPUT3_RGB:
            OSReport("constant color input 3\n");
			break;

        case SHADER_COLORINPUT0_A:
            OSReport("constant alpha input 0\n");
			break;
        case SHADER_COLORINPUT1_A:
            OSReport("constant alpha input 1\n");
			break;
        case SHADER_COLORINPUT2_A:
            OSReport("constant alpha input 2\n");
			break;
        case SHADER_COLORINPUT3_A:
            OSReport("constant alpha input 3\n");
			break;
	}
}

/*>*******************************(*)*******************************<*/
static void PrintResources  ( CompiledShaderResources *res, OptimizedTexCoordExpressions *tcExp)
{
    u8 i;

    OSReport("ras colors used\n");
    for ( i = 0; i < 2; i ++ )
    {
        if(res->rasUsed[i])
            OSReport("%d ", i);
    }
    OSReport("\n");

    OSReport("constant colors used\n");
    for ( i = 0; i < 4; i ++ )
    {
        if(res->colorUsed[i])
            OSReport("%d ", i);
    }
    OSReport("\n");

    OSReport("textures used\n");
    for ( i = 0; i < 8; i ++ )
    {
        if(res->textureUsed[i])
            OSReport("%d ", i);
    }
    OSReport("\n");

    OSReport("complex inputs used\n");
    for ( i = 0; i < 8; i ++ )
    {
        if(res->complexUsed[i])
            OSReport("%d ", i);
    }
    OSReport("\n");

    OSReport("Mtx inputs used\n");
    for ( i = 0; i < 8; i ++ )
    {
        if(tcExp->mtxUsed[i])
            OSReport("%d ", i);
    }
    OSReport("\n");
}
#endif
/*>*******************************(*)*******************************<*/