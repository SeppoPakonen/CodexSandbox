/*---------------------------------------------------------------------------*
  Project:  Character Pipeline
  File:     shaderTCInternals.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/shader/src/shaderTCInternals.c $    
    
    2     7/19/00 3:59p Ryan
    update for precompiled shaders
    
    1     4/12/00 10:20p Ryan
    

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <Dolphin/os.h>
#include <Dolphin/gx.h>
#include <CharPipe/shader.h>

#include "shaderTCInternals.h"
#include "shaderInternals.h"

/*>*******************************(*)*******************************<*/
ShaderTexCoordResources TCResources = {
                                                GX_TEXCOORD0,
                                                GX_TEXCOORD1,
                                                GX_TEXCOORD2,
                                                GX_TEXCOORD3,
                                                GX_TEXCOORD4,
                                                GX_TEXCOORD5,
                                                GX_TEXCOORD6,
                                                GX_TEXCOORD7,

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
                                                0
                                            };

/*
SHDRShader      *TextureShaderArray[16];
SHDRShader      *TextureShaderTree[16];*/

u32 MtxIDArray[8] = {   GX_TEXMTX0,
                        GX_TEXMTX1, 
                        GX_TEXMTX2, 
                        GX_TEXMTX3, 
                        GX_TEXMTX4, 
                        GX_TEXMTX5, 
                        GX_TEXMTX6, 
                        GX_TEXMTX7  };
/*>*******************************(*)*******************************<*/
static SHDRTexCoord   *TexCoordArray[8];
static u8              numExpressions;

/*>*******************************(*)*******************************<*
static void ResetTexCoordResources	( void );
static void CleanUpTCTree	        ( void );
static BOOL BuildTexShaderList      ( SHDRShader *shader );
static BOOL BuildTCGenShaderTree    ( void );
static BOOL ProcessTCGenShaderTree  ( void );

static BOOL AllocateTexGen              ( SHDRShader *shader );
static BOOL CheckTCResourceCollision    ( u8 index, SHDRShader *shader );

static GXTexGenSrc TranslateGenSrc      ( GXTexCoordID id );
static GXTexMtxType TranslateTexMtxType ( ShaderTexMtxType type );

/*>*******************************(*)*******************************<*/
static void FindUniqueTexCoordExpressions   ( SHDRShader *shader );
static void AddTexCoordShaderToList         ( SHDRTexCoord *shader );
static void CopyExp                         ( SHDRTexCoord *shader, TexCoordExp *exp );

static GXTexMtxType TranslateTexMtxType ( ShaderTexGenType type );

/*>*******************************(*)*******************************<*/
void CompileTexGen  ( SHDRShader *shader,  OptimizedTexCoordExpressions **exp)
{
    u8 i;

    //reset counter
    numExpressions = 0;

    FindUniqueTexCoordExpressions(shader);

    //allocate memory to save list
    (*exp) = (OptimizedTexCoordExpressions *)OSAlloc(sizeof(OptimizedTexCoordExpressions));

    if(numExpressions)
        (*exp)->expressionArray = (TexCoordExp *)OSAlloc(sizeof(TexCoordExp) * numExpressions);
    else
        (*exp)->expressionArray = 0;

    (*exp)->numExpressions = numExpressions;
    //initialize Mtx data
    for(i = 0; i < 8; i ++ )
    {
        (*exp)->mtxArray[i] = 0;
        (*exp)->mtxUsed[i] = 0;
    }

    //copy the texture coord expressions
    for(i = 0; i < numExpressions; i ++ )
    {
        CopyExp(TexCoordArray[i], &((*exp)->expressionArray[i]));

        //set up matrix data if necessary
        if(TexCoordArray[i]->mtxInput != SHADER_IDENTITY)
        {
            if(!((*exp)->mtxUsed[(u32)(TexCoordArray[i]->mtxInput)])) //if a matrix hasn't been allocated
            {
                (*exp)->mtxUsed[(u32)(TexCoordArray[i]->mtxInput)] = 1;
                (*exp)->mtxArray[(u32)(TexCoordArray[i]->mtxInput)] = OSAlloc(sizeof(Mtx));
            }
        }
    }
}

/*>*******************************(*)*******************************<*/
static void FindUniqueTexCoordExpressions    ( SHDRShader *shader )
{
    if(!shader)
        return;

    if(shader->type == SHADER_TYPE_TEXTURE)
    {
        AddTexCoordShaderToList(shader->shaderParams.textureShader.texCoordShader);
        return;
    }

    if (shader->type == SHADER_TYPE_COMPLEX || shader->type == SHADER_TYPE_EMPTY)
    {
        FindUniqueTexCoordExpressions(shader->shaderParams.complexShader.input1);
        FindUniqueTexCoordExpressions(shader->shaderParams.complexShader.input2);
        FindUniqueTexCoordExpressions(shader->shaderParams.complexShader.input3);
        FindUniqueTexCoordExpressions(shader->shaderParams.complexShader.input4);
    }
}

/*>*******************************(*)*******************************<*/
static void AddTexCoordShaderToList ( SHDRTexCoord *shader )
{
    u8 i;

    //check to see if shader is already in the list
    // xxx commented temporarily until a better texgen solution can be found
    for ( i = 0; i < numExpressions; i ++ )
    {
        if(TexCoordArray[i] == shader)
            return; 
    }

    //check to see if shader has a parent
    if(shader->shaderSrc)
        AddTexCoordShaderToList(shader->shaderSrc);

    //shader was not found in list - add it
    ASSERTMSG(numExpressions < 8, "Too many texture coordinate shaders - FindUniqueTexCoordExpressions");
    TexCoordArray[numExpressions] = shader;
    shader->texCoordExpIdx = numExpressions;
    numExpressions ++;    

    return;
}

/*>*******************************(*)*******************************<*/
static void CopyExp ( SHDRTexCoord *shader, TexCoordExp *exp )
{
    exp->genSrc = shader->genSrc;
    exp->type = shader->type;
    exp->mtxInput = shader->mtxInput;

    if(shader->shaderSrc)
    {
        exp->srcShaderIdx = shader->shaderSrc->texCoordExpIdx;
    }
    else
        exp->srcShaderIdx = NO_TEXTURE_COORD_EXP;
}

/*>*******************************(*)*******************************<*
static void ResetTexCoordResources	( void )
{
	u8 i;

    //reset all tex coord shader pointers to NULL
	for ( i = 0; i < 8; i ++ )
	{
		TexCoordResources.tcShader[i] = 0;
        TexCoordResources.textureShader[i] = 0;
	}

    //reset the texture shader array
    //reset the texture coordinate gen shader array
    for ( i = 0; i < 16; i ++ )
	{
		TextureShaderArray[i] = 0;
        TextureShaderTree[i] = 0;
	}
}

/*>*******************************(*)*******************************<*
static void CleanUpTCTree	( void )
{
    u8 i;
    u8 j;

    SHDRTexCoord *temp;

    //reset the texture shader array
    for ( i = 0; i < 16; i ++ )
	{
        if(!(TextureShaderArray[i])) break;

        temp = (TextureShaderArray[i])->shaderParams.textureShader.texCoordShader;

        for ( j = 0; j < 4; j ++ )
        {
            temp->shaderDependencies[j] = 0;
        }
	}
}

/*>*******************************(*)*******************************<*
BOOL CompileTexGen    ( SHDRShader *shader )
{
    ResetTexCoordResources();

    if(!(BuildTexShaderList(shader)))
        return FALSE;

    if(!(BuildTCGenShaderTree()))
    {
        CleanUpTCTree();
        return FALSE;
    }

    if(!(ProcessTCGenShaderTree()))
    {
        CleanUpTCTree();
        return FALSE;
    }

    CleanUpTCTree();

    return TRUE;
}

/*>*******************************(*)*******************************<*
static BOOL BuildTexShaderList    ( SHDRShader *shader )
{
    u8 i;

    if(!shader)
        return TRUE;

    if(shader->type == SHADER_TYPE_TEXTURE)
    {
        //check to see if shader is already in the list
        for ( i = 0; i < 16; i ++ )
        {
            if(TextureShaderArray[i] == shader)
                return TRUE;

            if(!(TextureShaderArray[i])) //add shader to the list
            {
                TextureShaderArray[i] = shader;
                return TRUE;
            }
        }
        return FALSE;
    }
    else if (shader->type == SHADER_TYPE_COMPLEX || shader->type == SHADER_TYPE_EMPTY)
    {
        if(!(BuildTexShaderList(shader->shaderParams.complexShader.input1)))
            return FALSE;
        if(!(BuildTexShaderList(shader->shaderParams.complexShader.input2)))
            return FALSE;
        if(!(BuildTexShaderList(shader->shaderParams.complexShader.input3)))
            return FALSE;
        if(!(BuildTexShaderList(shader->shaderParams.complexShader.input4)))
            return FALSE;
    }

    return TRUE;
}

/*>*******************************(*)*******************************<*
static BOOL BuildTCGenShaderTree    ( void )
{
    u8 i, j;
    u8 cursor = 0;

    SHDRTexCoord *parent;

    for ( i = 0; i < 16; i ++ )
    {
        if(!(TextureShaderArray[i]))
            return TRUE;

        parent = (TextureShaderArray[i])->shaderParams.textureShader.texCoordShader->shaderSrc;

        // if texcoord gen is dependent on another texcoord gen
        if(parent)
        {
            // add it to its parent's dependency list
            for ( j = 0; j < 4; j ++ )
            {
                if(!(parent->shaderDependencies[j]))
                {
                    parent->shaderDependencies[j] = TextureShaderArray[i];
                    break;
                }
            }
            // no available slots - return false
            if(j == 4)
                return FALSE;
        }
        else
        {
            // add it to the root list
            TextureShaderTree[cursor] = TextureShaderArray[i];
            cursor ++;
        }
    }

    return TRUE;
}

/*>*******************************(*)*******************************<*
static BOOL ProcessTCGenShaderTree    ( void )
{
    u8 i;

    for ( i = 0; i < 16; i ++ )
    {
        if(!(TextureShaderTree[i]))
            return TRUE;

        if(!(AllocateTexGen(TextureShaderTree[i])))
            return FALSE;
    }

    return TRUE;
}

/*>*******************************(*)*******************************<*
static BOOL AllocateTexGen    ( SHDRShader *shader )
{
    u8 i, j;

	for ( i = 0; i < 8; i ++ )
	{
        if(CheckTCResourceCollision(i, shader))
        //if(!(TexCoordResources.tcShader[i]) || // we've reached an empty texture coord - allocate it
        //     TexCoordResources.tcShader[i] == shader->shaderParams.textureShader.texCoordShader) 
        {
            TexCoordResources.tcShader[i] = shader->shaderParams.textureShader.texCoordShader;
            TexCoordResources.textureShader[i] = shader;
            shader->shaderParams.textureShader.coord = TexCoordResources.coord[i];
            shader->shaderParams.textureShader.texCoordShader->shaderOutput = TexCoordResources.coord[i];
            break;
        }   
	}

    // if there are no texcoords left, return false
    if(i == 8)
        return FALSE;

    for ( j = 0; j < 4; j ++ )
	{
        if(TexCoordResources.tcShader[i]->shaderDependencies[j]) // allocate child texcoord shaders
        {
            if(!(AllocateTexGen(TexCoordResources.tcShader[i]->shaderDependencies[j])))
                return FALSE;
        }   
        else break;
	}

    return TRUE;
}

/*>*******************************(*)*******************************<*
static BOOL CheckTCResourceCollision    ( u8 index, SHDRShader *shader )
{
    u16 height1, width1;
    u16 height2, width2;

    if(!(TexCoordResources.tcShader[index])) return TRUE;

    if(TexCoordResources.textureShader[index]->shaderParams.textureShader.texObj ==
       shader->shaderParams.textureShader.texObj) return TRUE;

    // check to see if the textures are of the same dimensions
    height1 = GXGetTexObjHeight(TexCoordResources.textureShader[index]->shaderParams.textureShader.texObj);
    width1 = GXGetTexObjWidth(TexCoordResources.textureShader[index]->shaderParams.textureShader.texObj);

    height2 = GXGetTexObjHeight(shader->shaderParams.textureShader.texObj);
    width2 = GXGetTexObjWidth(shader->shaderParams.textureShader.texObj);

    if(height1 == height2 && width1 == width2)
    {
        // if they're the same expression
        if(TexCoordResources.tcShader[index] == shader->shaderParams.textureShader.texCoordShader)
            return TRUE; 

        // if they're both passthrough expressions resulting in the same coordinate
        if(TexCoordResources.tcShader[index]->type == SHADER_TG_PASSTHROUGH &&
           shader->shaderParams.textureShader.texCoordShader->type == SHADER_TG_PASSTHROUGH &&
           TexCoordResources.tcShader[index]->genSrc == shader->shaderParams.textureShader.texCoordShader->genSrc)
           return TRUE;

            
    }
       

    return FALSE;
}

/*>*******************************(*)*******************************<*/
void SetTCGenState  ( SHDRCompiled *shader )
{
    //xxx need to check for multiple textures sharing tex coords.

    OptimizedTexCoordExpressions *tcExp = (OptimizedTexCoordExpressions *)(shader->texGen);

    u8 i;
    TexCoordExp *tcShader;
    u32 texMtxIdx;

	for ( i = 0; i < tcExp->numExpressions; i ++ )
	{
        tcShader = &(tcExp->expressionArray[i]);

        //deal with matrix loadng
        if(tcShader->mtxInput == SHADER_IDENTITY)
            texMtxIdx = GX_IDENTITY;
        else
        {
            texMtxIdx = MtxIDArray[((u32)(tcShader->mtxInput))];

            GXLoadTexMtxImm(tcExp->mtxArray[((u32)(tcShader->mtxInput))], 
                            texMtxIdx, 
                            TranslateTexMtxType(tcShader->type));
        }

        if(tcShader->srcShaderIdx != NO_TEXTURE_COORD_EXP)
        {
            GXSetTexCoordGen( TCResources.coord[i], 
                              (GXTexGenType)(tcShader->type), 
                              (GXTexGenSrc)(((GXTexGenSrc)(tcShader->srcShaderIdx)) + GX_TG_TEXCOORD0), 
                              texMtxIdx );
        }
        else
        {
            GXSetTexCoordGen( TCResources.coord[i], 
                              (GXTexGenType)(tcShader->type), 
                              (GXTexGenSrc)(tcShader->genSrc), 
                              texMtxIdx );
        }
	}

    GXSetNumTexGens(tcExp->numExpressions);
}

/*>*******************************(*)*******************************<*
static GXTexGenSrc TranslateGenSrc ( GXTexCoordID id )
{
    switch(id)
    {
        case GX_TEXCOORD0:
            return GX_TG_TEXCOORD0;
        case GX_TEXCOORD1:
            return GX_TG_TEXCOORD1; 
        case GX_TEXCOORD2:
            return GX_TG_TEXCOORD2;
        case GX_TEXCOORD3:
            return GX_TG_TEXCOORD3; 
        case GX_TEXCOORD4:
            return GX_TG_TEXCOORD4; 
        case GX_TEXCOORD5:
            return GX_TG_TEXCOORD5; 
        case GX_TEXCOORD6:
            return GX_TG_TEXCOORD6; 
        default:
            ASSERTMSG(0, "unknown tex coord id in TranslateGenSrc");
    }

    //never reached
    return GX_TG_TEXCOORD0;
}

/*>*******************************(*)*******************************<*/
static GXTexMtxType TranslateTexMtxType ( ShaderTexGenType type )
{
    switch(type)
    {
        case SHADER_TG_MTX3x4:
            return GX_MTX3x4; 
        case SHADER_TG_MTX2x4:
            return GX_MTX2x4;
        default:
            ASSERTMSG(0, "unknown tex mtx type in TranslateTexMtxType");
    }

    //never reached
    return GX_MTX3x4; 
}


/*>*******************************(*)*******************************<*/