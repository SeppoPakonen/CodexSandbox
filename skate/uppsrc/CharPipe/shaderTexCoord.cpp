/*---------------------------------------------------------------------------*
  Project:  Character Pipeline
  File:     shaderTexCoord.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/shader/src/shaderTexCoord.c $    
    
    2     7/19/00 3:59p Ryan
    update for precompiled shaders
    
    1     4/12/00 10:20p Ryan
    

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <Dolphin/os.h>
#include <CharPipe/shader.h>

/*>*******************************(*)*******************************<*/
SHDRTexCoord *SHDRCreateTexCoordExpression ( ShaderTexGenSrc src,
                                             SHDRTexCoord *shadSrc,
                                             ShaderTexGenType texGenType, 
                                             SHADER_MTXInput mtxInput )
{
    SHDRTexCoord *temp;
    //u8 i;

	temp = (SHDRTexCoord *)OSAlloc(sizeof(SHDRTexCoord));

    temp->shaderSrc = shadSrc;
    temp->genSrc = src;
	temp->type = texGenType;
    temp->mtxInput = mtxInput;

    temp->texCoordExpIdx = 0;
    temp->referenceCount = 0;

    //for ( i = 0; i < 4; i ++ )
    //{
    //    temp->shaderDependencies[i] = 0;
    //}

	return temp;
}


/*>*******************************(*)*******************************<*/
SHDRTexCoord *SHDRCreateTCPassThrough   ( ShaderTexGenSrc src )
{
    return SHDRCreateTexCoordExpression(src, 
                                        0, 
                                        SHADER_TG_MTX3x4, 
                                        SHADER_IDENTITY);
}

/*>*******************************(*)*******************************<*/
void SHDRFreeTC ( SHDRTexCoord *texCoord )
{
    texCoord->referenceCount --;
	if(texCoord->referenceCount < 1) 
		OSFree(texCoord);
}

/*>*******************************(*)*******************************<*/
