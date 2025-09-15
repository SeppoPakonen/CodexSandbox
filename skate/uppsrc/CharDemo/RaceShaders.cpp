/*---------------------------------------------------------------------------*
  Project:  Dolphin
  File:     raceShaders.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/demos/charPipeline/src/raceShaders.c $
    
    2     9/16/00 12:26p Ryan
    shader optimization update
    
    1     7/29/00 11:17a Ryan
    initial checkin
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <charPipeline.h>
#include <demo.h>

#include "raceShaders.h"

/****************************************************************************/
static SHDRCompiled    *CreateCarShader_REF_DIFF        ( void );
static SHDRCompiled    *CreateCarShader_DIFF            ( void );
static SHDRCompiled    *CreateCarShader_GLOSS           ( void );
static SHDRCompiled    *CreateCarShader_REF             ( void );
static SHDRCompiled    *CreateCarShader                 ( void );

static SHDRShader *AddShaders           ( SHDRShader *a, SHDRShader *b );
static SHDRShader *MultiplyShaders      ( SHDRShader *a, SHDRShader *b );
static SHDRShader *MultiplyAddShaders   ( SHDRShader *a, SHDRShader *b, SHDRShader *c );

/****************************************************************************/
SHDRCompiled *CarShader_REF_DIFF;
SHDRCompiled *CarShader_DIFF;
SHDRCompiled *CarShader_GLOSS;
SHDRCompiled *CarShader_REF;
SHDRCompiled *CarShader;

/****************************************************************************/
void InitShaders ( void )
{
    CarShader_REF_DIFF = CreateCarShader_REF_DIFF();
    CarShader_DIFF = CreateCarShader_DIFF();
    CarShader_GLOSS = CreateCarShader_GLOSS();
    CarShader_REF = CreateCarShader_REF();
    CarShader = CreateCarShader();
}


/****************************************************************************/
/*---------------------------------------------------------------------------*
    Name:			CreateCarShader
    
    Description:	Defines the Shader for a car
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
static SHDRCompiled    *CreateCarShader_REF_DIFF ( void )
{
	SHDRShader *reflection;
    SHDRShader *diffuse;
    SHDRShader *final;
    SHDRShader *alpha;
    SHDRShader *diffuseShader;
    SHDRShader *vertexColor;
    SHDRShader *gloss;
    
    SHDRTexCoord *texcoord0;
    SHDRTexCoord *texcoord1;

    SHDRCompiled *temp;

    ///////create basic building blocks

    //create reflection texture
    texcoord1 = SHDRCreateTexCoordExpression (SHADER_TG_NRM, 0, SHADER_TG_MTX2x4, SHADER_MTX0);
	reflection = SHDRCreateTexture(SHADER_TEX1, texcoord1, SHADER_CHANNEL_RGB);

    //create diffuse texture
    texcoord0 = SHDRCreateTCPassThrough(SHADER_TG_TEX0);
	diffuse = SHDRCreateTexture(SHADER_TEX0, texcoord0, SHADER_CHANNEL_RGB);

    //create gloss texture
    gloss = SHDRCreateTexture(SHADER_TEX0, texcoord0, SHADER_CHANNEL_A);

    //create vertex color
    vertexColor = SHDRCreateRasterized(SHADER_RAS0, SHADER_CHANNEL_RGB);
    alpha = SHDRCreateRasterized(SHADER_RAS0, SHADER_CHANNEL_A);

    ///////

    //diffuse * vertex color
    diffuseShader = MultiplyShaders(diffuse, vertexColor);

    //reflection * gloss + diffuse
    final =  MultiplyAddShaders(reflection, gloss, diffuseShader);
    
    //compile
    temp = SHDRCompile(final, alpha);

    //free shaders
    SHDRFree(final);
    SHDRFree(alpha);

    return temp;
}

/*---------------------------------------------------------------------------*
    Name:			CreateCarShader
    
    Description:	Defines the Shader for a car
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
static SHDRCompiled    *CreateCarShader_DIFF ( void )
{
	
    SHDRShader *rgbShader;
    SHDRShader *alpha;
    
    SHDRCompiled *temp;

    //create RGB side
    rgbShader = SHDRCreateComplexInput(SHADER_COMPLEX0, SHADER_CHANNEL_RGB);
  
    //create Alpha side
    alpha = SHDRCreateRasterized(SHADER_RAS0, SHADER_CHANNEL_A);

    //compile
    temp = SHDRCompile(rgbShader, alpha);

    //free shaders
    SHDRFree(rgbShader);
    SHDRFree(alpha);

    return temp;
}

/*---------------------------------------------------------------------------*
    Name:			CreateCarShader
    
    Description:	Defines the Shader for a car
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
static SHDRCompiled    *CreateCarShader_GLOSS ( void )
{
	
    SHDRShader *rgbShader;
    SHDRShader *alpha;
    
    SHDRCompiled *temp;

    //create RGB side
    alpha = SHDRCreateComplexInput(SHADER_COMPLEX0, SHADER_CHANNEL_A);

    rgbShader = SHDRCreateComplex(alpha, ShaderZero, ShaderZero, ShaderZero, SHADER_OP_ADD, 
                                SHADER_CLAMP_LINEAR_255, SHADER_BIAS_ZERO, SHADER_SCALE_1, 
							    SHADER_CHANNEL_RGB); 
  
    //compile
    temp = SHDRCompile(rgbShader, ShaderOne);

    //free shaders
    SHDRFree(rgbShader);

    return temp;
}

/*---------------------------------------------------------------------------*
    Name:			CreateCarShader
    
    Description:	Defines the Shader for a car
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
static SHDRCompiled    *CreateCarShader_REF ( void )
{
	SHDRShader *reflection;
    SHDRShader *aShader;
    SHDRShader *final;
    SHDRShader *alpha;
    
    SHDRTexCoord *texcoord0;

    SHDRCompiled *temp;

    //create RGB side
    texcoord0 = SHDRCreateTexCoordExpression (SHADER_TG_NRM, 0, SHADER_TG_MTX2x4, SHADER_MTX0);
	reflection = SHDRCreateTexture(SHADER_TEX0, texcoord0, SHADER_CHANNEL_RGB);

    aShader = SHDRCreateComplexInput(SHADER_COMPLEX0, SHADER_CHANNEL_A);

    final =  MultiplyShaders(reflection, aShader);

    //create Alpha side
    alpha = SHDRCreateRasterized(SHADER_RAS0, SHADER_CHANNEL_A);

    //compile
    temp = SHDRCompile(final, alpha);

    //free shaders
    SHDRFree(final);
    SHDRFree(alpha);

    return temp;
}

/*---------------------------------------------------------------------------*
    Name:			CreateCarShader
    
    Description:	Defines the Shader for a car
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
static SHDRCompiled    *CreateCarShader ( void )
{
    SHDRShader *rgbShader;
    SHDRShader *alpha;
        
    SHDRCompiled *temp;

    //create RGB side
    rgbShader = SHDRCreateRasterized(SHADER_RAS0, SHADER_CHANNEL_RGB);

    //create Alpha side
    alpha = SHDRCreateRasterized(SHADER_RAS0, SHADER_CHANNEL_A);

    //compile
    temp = SHDRCompile(rgbShader, alpha);

    //free shaders
    SHDRFree(rgbShader);
    SHDRFree(alpha);

    return temp;
}

/*---------------------------------------------------------------------------*
    Name:			AddShaders
    
    Description:	Adds the results of 2 shaders
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
static SHDRShader *AddShaders ( SHDRShader *a, SHDRShader *b )
{
    return SHDRCreateComplex(a, ShaderZero, ShaderZero, b, SHADER_OP_ADD, 
                             SHADER_CLAMP_LINEAR_255, SHADER_BIAS_ZERO, SHADER_SCALE_1, 
							 SHADER_CHANNEL_RGB); 
}

/*---------------------------------------------------------------------------*
    Name:			MultiplyShaders
    
    Description:	Multiplies the results of 2 shaders
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
static SHDRShader *MultiplyShaders ( SHDRShader *a, SHDRShader *b )
{
    return SHDRCreateComplex(ShaderZero, a, b, ShaderZero, SHADER_OP_ADD, 
                             SHADER_CLAMP_LINEAR_1023, SHADER_BIAS_ZERO, SHADER_SCALE_1, 
							 SHADER_CHANNEL_RGB); 
}

/*---------------------------------------------------------------------------*
    Name:			MultiplyAddShaders
    
    Description:	Multiplies the results of a and b and then adds c.
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
static SHDRShader *MultiplyAddShaders ( SHDRShader *a, SHDRShader *b, SHDRShader *c )
{
    return SHDRCreateComplex(ShaderZero, a, b, c, SHADER_OP_ADD, 
                             SHADER_CLAMP_LINEAR_255, SHADER_BIAS_ZERO, SHADER_SCALE_1, 
							 SHADER_CHANNEL_RGB); 
}



