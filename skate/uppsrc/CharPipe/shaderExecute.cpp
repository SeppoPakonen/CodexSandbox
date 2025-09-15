/*---------------------------------------------------------------------------*
  Project:  Character Pipeline
  File:     shaderExecute.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/shader/src/shaderExecute.c $    
    
    2     7/27/00 10:28a Ryan
    cleanup to remove warnings in the MAC build
    
    1     7/19/00 4:03p Ryan
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <Dolphin/os.h>
#include <CharPipe/shader.h>

#include "shaderInternals.h"
#include "shaderTCInternals.h"
#include "shaderExecute.h"
#include "shaderSetTEV.h"

/*>*******************************(*)*******************************<*/

//TexCoordExp *TexExpListHeads[8];    // a list of pointers to keep 
                                    // track of where the different heads 
                                    // of texture expression lists are 
                                    // in the tex exp pool

TexCoordExp  TexExpPool[8];         // pool of expressions to allocate

u8           CurrentTexExp;         // next free entry in the pool

void        *MtxDataArray[8];       // Array of pointers to point to remapped
                                    // matrix data
u8           MtxUsed[8];

CompiledTEVStage        CompiledTEVColorPool[16];
CompiledTEVStage        CompiledTEVAlphaPool[16];
u8                      CompiledTEVCounter;

CompiledShaderResources CurrentShaderResources;
SHADER_RegisterState	CurrentColorState[4];
SHADER_RegisterState	CurrentAlphaState[4];

/*>*******************************(*)*******************************<*/
static void InitExecute    ( void );
static void CopyTexShaders ( SHDRCompiled *shader );

static void CopyTEVStages  ( SHDRCompiled *shader );

static void FindRegisterUsage ( SHADER_RegisterState *colorRegisters, 
                                SHADER_RegisterState *alphaRegisters, 
                                CompiledTEVStage *stageArray, 
                                u8 numStages );

static void AssignConstantColorRegs ( SHADER_ColorInput *colorRemap, 
                                      CompiledTEVStage *stageArray, 
                                      u8 numStages );

static void FindParents ( u8 *earliest, u8 *latest, SHDRCompiled *shader );
static void FindOpenRegisters   ( u8 pos, SHADER_RegisterState *color, SHADER_RegisterState *alpha );
static BOOL StageNotEmpty ( CompiledTEVStage *stage );

static void FindOutputRegs  ( u8 earliest, u8 latest, SHADER_TEVArg *regRemap,
                              SHADER_RegisterState *startColor, SHADER_RegisterState *startAlpha );

static BOOL RemapRegisters  ( SHADER_TEVArg *regRemap, 
                              SHADER_RegisterState *tempColorRegUsage, 
                              SHADER_RegisterState *tempAlphaRegUsage, 
                              SHADER_RegisterState *colorRegUsage, 
                              SHADER_RegisterState *alphaRegUsage );

static void PrepareTevPool  ( u8 earliest, u8 numStages );

static void CopyCompiledStage   ( CompiledTEVStage *src, CompiledTEVStage *dst );

static void InsertTevStages ( u8 pos, SHDRCompiled *shader, 
                              SHADER_TexInput *texRemap, SHADER_RasInput *rasRemap, 
                              SHADER_ColorInput *colorRemap, 
                              SHADER_ComplexInput *complexRemap, SHADER_TEVArg *regRemap,
                              u8 texOffset );

static void RemapStage(CompiledTEVStage *src, CompiledTEVStage *dst,
                       SHADER_TexInput *texRemap, SHADER_RasInput *rasRemap, 
                       SHADER_ColorInput *colorRemap, 
                       SHADER_ComplexInput *complexRemap, SHADER_TEVArg *regRemap,
                       u8 texOffset );

static void RemapComplexReferences  ( SHDRCompiled *shader, SHADER_TEVArg *regRemap );


/*>*******************************(*)*******************************<*/
void CheckShaderBindings    ( SHDRCompiled *shader )
{
    CompiledShaderResources *res = (CompiledShaderResources *)(shader->shaderResources);
    OptimizedTexCoordExpressions *tcExp = (OptimizedTexCoordExpressions *)(shader->texGen);
    u8 i;

    for ( i = 0; i < 2; i ++ )
    {
        if(res->rasUsed[i] == 1)
            OSHalt("Error - rasterized color not bound to any data - CheckShaderBindings");
    }

    for ( i = 0; i < 4; i ++ )
    {
        if(res->colorUsed[i] == 1)
            OSHalt("Error - constant color not bound to any data - CheckShaderBindings");
    }

    for ( i = 0; i < 8; i ++ )
    {
        if(res->textureUsed[i] == 1)
            OSHalt("Error - texture not bound to any data - CheckShaderBindings");
       
        if(res->complexUsed[i] == 1)
            OSHalt("Error - complex input not bound to any data - CheckShaderBindings");

        if(tcExp->mtxUsed[i] == 1)
            OSHalt("Error - mtx input not bound to any data - CheckShaderBindings");
    }

    //check complex inputs
    for ( i = 0; i < 8; i ++ )
    {
        if(res->complexUsed[i] == 2)
            CheckShaderBindings(res->complexData[i]);
    }
}

/*>*******************************(*)*******************************<*/
static void InitExecute    ( void )
{
    u8 i;

    CurrentTexExp = 0;

    for ( i = 0; i < 8; i ++ )
    {
        //TexExpListHeads[i] = 0;
        MtxDataArray[i] = 0;
        MtxUsed[i] = 0;
    }
}

/*>*******************************(*)*******************************<*/
static void CopyTexShaders ( SHDRCompiled *shader )
{
    OptimizedTexCoordExpressions *tex = (OptimizedTexCoordExpressions *)(shader->texGen);
    CompiledShaderResources *res = (CompiledShaderResources *)(shader->shaderResources);

    u8 i, j;
    u8 offset = CurrentTexExp;

    // store the start point for the expression shader
    /*for ( i = 0; i < 8; i ++ )
    {
        if(!(TexExpListHeads[i]))
        {
            TexExpListHeads[i] = &(TexExpPool[CurrentTexExp]);
            break;
        }
    }
    if(i == 8) OSHalt("too many texture coord expressions - CopyTexShaders");*/

    for ( i = 0; i < tex->numExpressions; i ++ )
    {
        if(CurrentTexExp > 7) OSHalt("too many texture coord expressions - CopyTexShaders");

        TexExpPool[CurrentTexExp].genSrc = tex->expressionArray[i].genSrc;
        TexExpPool[CurrentTexExp].type = tex->expressionArray[i].type;

        //remap parent to new expression array by adding offset
        if(tex->expressionArray[i].srcShaderIdx == NO_TEXTURE_COORD_EXP)
            TexExpPool[CurrentTexExp].srcShaderIdx = NO_TEXTURE_COORD_EXP;
        else
            TexExpPool[CurrentTexExp].srcShaderIdx = (u8)(tex->expressionArray[i].srcShaderIdx + offset);

        if(tex->expressionArray[i].mtxInput == SHADER_IDENTITY)
            TexExpPool[CurrentTexExp].mtxInput = SHADER_IDENTITY;
        else //remap texture input
        {
            //find open texture
            for ( j = 0; j < 8; j ++ )
            {
                if(!(MtxDataArray[j]))
                {   
                    MtxDataArray[j] = tex->mtxArray[((u32)(tex->expressionArray[i].mtxInput))];
                    TexExpPool[CurrentTexExp].mtxInput = ((SHADER_MTXInput)(j));
                    MtxUsed[i] = 2;
                    break;
                }
            }
            if(j == 8) OSHalt("too many matrices - CopyTexShaders");
        }

        CurrentTexExp ++;
    }

    //check complex inputs
    //for ( i = 0; i < 8; i ++ )
    //{
    //    if(res->complexUsed[i] == 2)
    //        CopyTexShaders(res->complexData[i]);
    //}
}

/*>*******************************(*)*******************************<*/
void CombineTEVStages  ( SHDRCompiled *shader )
{
    u8 i;

    OptimizedTexCoordExpressions compTC;
    SHDRCompiled comp;

    //init global resources

    for ( i = 0; i < 4; i ++ )
    {
        CurrentShaderResources.regColor[i] = SHADER_COLOR_NONE;
        CurrentShaderResources.regAlpha[i] = SHADER_COLOR_NONE;
        CurrentShaderResources.colorUsed[i] = 0;
    }

    for ( i = 0; i < 8; i ++ )
    {
        CurrentShaderResources.textureUsed[i] = 0;
        CurrentShaderResources.textureData[i] = 0;
        CurrentShaderResources.complexUsed[i] = 0;
        CurrentShaderResources.complexData[i] = 0;
    }

    for ( i = 0; i < 2; i ++ )
    {
        CurrentShaderResources.rasUsed[i] = 0;
    }

    for ( i = 0; i < 4; i ++ )
    {
        CurrentColorState[i] = SHADER_REG_EMPTY;
        CurrentAlphaState[i] = SHADER_REG_EMPTY;
    }

    CompiledTEVCounter = 0;

    InitExecute();

    //check to see if shader has any complex inputs
    //
        CopyTEVStages(shader);


        compTC.expressionArray = TexExpPool;
        compTC.numExpressions = CurrentTexExp;

        for ( i = 0; i < 8; i ++ )
        {
            compTC.mtxArray[i] = MtxDataArray[i];
            compTC.mtxUsed[i] = MtxUsed[i];
        }

        ///testing only - print resulting shader
        comp.colorStages = CompiledTEVColorPool;
        comp.alphaStages = CompiledTEVAlphaPool;
        comp.numStages = CompiledTEVCounter;
        comp.shaderResources = &CurrentShaderResources;
        comp.texGen = &compTC;
    //

    //PrintTEVTree(&comp);

    SetTCGenState(&comp);
    SetTEV(&comp);
}

/*>*******************************(*)*******************************<*/
static void CopyTEVStages  ( SHDRCompiled *shader )
{
    SHADER_TexInput texRemap[8];
    SHADER_RasInput rasRemap[2];
    SHADER_ColorInput colorRemap[4];
    SHADER_ComplexInput complexRemap[8];
    SHADER_TEVArg regRemap[8];

    SHADER_RegisterState colorRegUsage[4] = {SHADER_REG_EMPTY, SHADER_REG_EMPTY, SHADER_REG_EMPTY, SHADER_REG_EMPTY};
    SHADER_RegisterState alphaRegUsage[4] = {SHADER_REG_EMPTY, SHADER_REG_EMPTY, SHADER_REG_EMPTY, SHADER_REG_EMPTY};

    SHADER_RegisterState tempColorRegUsage[4] = {SHADER_REG_EMPTY, SHADER_REG_EMPTY, SHADER_REG_EMPTY, SHADER_REG_EMPTY};
    SHADER_RegisterState tempAlphaRegUsage[4] = {SHADER_REG_EMPTY, SHADER_REG_EMPTY, SHADER_REG_EMPTY, SHADER_REG_EMPTY};

    CompiledShaderResources *res = (CompiledShaderResources *)(shader->shaderResources);
    s8 i, j;
    u8 earliest, latest;

    u8 texOffset = CurrentTexExp;

    //remap shader's data into global resource pool

    //remap textures
    for ( i = 0; i < 8; i ++ )
    {
        if(res->textureUsed[i])
        {
            for ( j = 0; j < 8; j ++ )
            {
                if(CurrentShaderResources.textureUsed[j])
                {
                    if(CurrentShaderResources.textureData[j] == res->textureData[i])
                    {
                        //texRemap[((SHADER_TexInput)(i))] = ((SHADER_TexInput)(j));
                        texRemap[i] = ((SHADER_TexInput)(j));
                        break;
                    }
                }        
                else
                {
                    //texRemap[((SHADER_TexInput)(i))] = ((SHADER_TexInput)(j));
                    texRemap[i] = ((SHADER_TexInput)(j));
                    CurrentShaderResources.textureData[j] = res->textureData[i];
                    CurrentShaderResources.textureUsed[j] = 2;
                    break;
                }
            }
            if(j == 8)
                OSHalt("Cannot find empty texture - CopyTEVStages");
        }
    }

    //remap complex inputs
    for ( i = 0; i < 8; i ++ )
    {
        if(res->complexUsed[i])
        {
            for ( j = 0; j < 8; j ++ )
            {
                if(CurrentShaderResources.complexUsed[j])
                {
                    if(CurrentShaderResources.complexData[j] == res->complexData[i])
                    {
                        //complexRemap[((SHADER_ComplexInput)(i))] = ((SHADER_ComplexInput)(j));
                        complexRemap[i] = ((SHADER_ComplexInput)(j));
                        break;
                    }
                }
                else
                {
                    //complexRemap[((SHADER_ComplexInput)(i))] = ((SHADER_ComplexInput)(j));
                    complexRemap[i] = ((SHADER_ComplexInput)(j));
                    CurrentShaderResources.complexData[j] = res->complexData[i];
                    CurrentShaderResources.complexUsed[j] = 2;
                    break;
                }
            }
            if(j == 8)
                OSHalt("Cannot find empty complex input - CopyTEVStages");
        }
    }

    //remap ras inputs
    for ( i = 0; i < 2; i ++ )
    {
        if(res->rasUsed[i])
        {
            for ( j = 0; j < 2; j ++ )
            {
                if(CurrentShaderResources.rasUsed[j])
                {
                    if(CurrentShaderResources.rasData[j] == res->rasData[i])
                    {
                        rasRemap[i] = ((SHADER_RasInput)(j));
                        break;
                    }
                }
                else
                {
                    //rasRemap[((SHADER_RasInput)(i))] = ((SHADER_RasInput)(j));
                    rasRemap[i] = ((SHADER_RasInput)(j));
                    CurrentShaderResources.rasData[j] = res->rasData[i];
                    CurrentShaderResources.rasUsed[j] = 2;
                    break;
                }
            }
            if(j == 2)
                OSHalt("Cannot find empty ras color - CopyTEVStages");
        }
    }

    //remap color inputs
    for ( i = 0; i < 4; i ++ )
    {
        if(res->colorUsed[i])
        {
            for ( j = 0; j < 4; j ++ )
            {
                if(CurrentShaderResources.colorUsed[j])
                {
                    if(res->colorData[i].r == CurrentShaderResources.colorData[j].r &&
                       res->colorData[i].g == CurrentShaderResources.colorData[j].g &&
                       res->colorData[i].b == CurrentShaderResources.colorData[j].b &&
                       res->colorData[i].a == CurrentShaderResources.colorData[j].a )
                    {
                        colorRemap[i] = ((SHADER_ColorInput)(j));
                        break;
                    }
                }
                else
                {
                    //colorRemap[((SHADER_ColorInput)(i))] = ((SHADER_ColorInput)(j));
                    colorRemap[i] = ((SHADER_ColorInput)(j));
                    CurrentShaderResources.colorData[j] = res->colorData[i];
                    CurrentShaderResources.colorUsed[j] = 2;
                    break;
                }
            }
            if(j == 4)
                OSHalt("Cannot find empty color - CopyTEVStages");
        }
    }

    //assign constant color inputs registers in CurrentShader
    AssignConstantColorRegs(colorRemap, (CompiledTEVStage *)(shader->colorStages), shader->numStages);
    AssignConstantColorRegs(colorRemap, (CompiledTEVStage *)(shader->alphaStages), shader->numStages);

    //find out which registers the input shader uses
    FindRegisterUsage(colorRegUsage, alphaRegUsage, (CompiledTEVStage *)(shader->colorStages), shader->numStages);
    FindRegisterUsage(colorRegUsage, alphaRegUsage, (CompiledTEVStage *)(shader->alphaStages), shader->numStages);

    //find location of earliest parent and latest parent
    //if there are no shaders in the list, earliest = 99
    FindParents(&earliest, &latest, shader);

    while(1)
    {
        //find out which registers are available at desired insertion index
        FindOpenRegisters(earliest, tempColorRegUsage, tempAlphaRegUsage);

        //find out which register is available for shader's output
        FindOutputRegs(earliest, latest, regRemap, tempColorRegUsage, tempAlphaRegUsage);

        //try and remap all remaining registers
        if(RemapRegisters(regRemap, tempColorRegUsage, tempAlphaRegUsage, colorRegUsage, alphaRegUsage))
            break;

        if(earliest == 0)
            OSHalt("cannot insert shader - CopyTEVStages");

        earliest --;
    }

    //copy over the texture shaders
    CopyTexShaders(shader);

    //update complex references
    if(earliest != 99)
        RemapComplexReferences(shader, regRemap);
    
    PrepareTevPool(earliest, shader->numStages);

    //copy current TEV stages and remap them
    if(earliest == 99) earliest = 0;

    InsertTevStages(earliest, shader, texRemap, rasRemap, colorRemap, complexRemap, regRemap, texOffset);

    //recur for input shaders
    for ( i = 0; i < 8; i ++ )
    {
        if(res->complexUsed[i])
        {
            CopyTEVStages(res->complexData[i]);
        }
    }

    
}

/*>*******************************(*)*******************************<*/
static void FindRegisterUsage ( SHADER_RegisterState *colorRegisters, 
                                SHADER_RegisterState *alphaRegisters, 
                                CompiledTEVStage *stageArray, 
                                u8 numStages )
{
    u8 i, j;

    for ( i = 0; i < numStages; i ++ ) //for each stages
    {
        for ( j = 0; j < 4; j ++ ) //for each TEV arg
        {
            if(stageArray[i].TEVArg[j] >= SHADER_CPREV && //TEV arg is a color register
               stageArray[i].TEVArg[j] <= SHADER_C2)
            {   //set the register to be in use
                colorRegisters[((u32)(stageArray[i].TEVArg[j]))] = SHADER_REG_INUSE;
            }

            else if (stageArray[i].TEVArg[j] >= SHADER_APREV && //TEV arg is an alpha register
                     stageArray[i].TEVArg[j] <= SHADER_A2)
            {   //set the register to be in use
                alphaRegisters[((u32)(stageArray[i].TEVArg[j])) - ((u32)(SHADER_APREV))] = SHADER_REG_INUSE;
            }                
        }
        
        if(stageArray[i].out_reg >= SHADER_CPREV && //TEV arg is a color register
           stageArray[i].out_reg <= SHADER_C2)
        {   //set the register to be in use
            colorRegisters[((u32)(stageArray[i].out_reg))] = SHADER_REG_INUSE;
        }

        else if (stageArray[i].out_reg >= SHADER_APREV && //TEV arg is an alpha register
                 stageArray[i].out_reg <= SHADER_A2)
        {   //set the register to be in use
            alphaRegisters[((u32)(stageArray[i].out_reg)) - ((u32)(SHADER_APREV))] = SHADER_REG_INUSE;
        }                
    }
}

/*>*******************************(*)*******************************<*/
static void AssignConstantColorRegs ( SHADER_ColorInput *colorRemap, 
                                      CompiledTEVStage *stageArray, 
                                      u8 numStages )
{
    s8 i, j, k;

    for ( i = 0; i < numStages; i ++ ) //for each stage
    {
        for ( j = 0; j < 4; j ++ ) //for each TEV arg
        {
            if(stageArray[i].TEVArg[j] >= SHADER_COLORINPUT0_RGB && //TEV arg is a constant color
               stageArray[i].TEVArg[j] <= SHADER_COLORINPUT3_RGB)
            {   //find an empty register
                for ( k = 3; k > -1; k -- )
                {
                    if(CurrentColorState[k] == SHADER_REG_INUSE)
                        OSHalt("cannot allocate color register for constant color - AssignConstantColorRegs");
                    else if(CurrentColorState[k] == SHADER_REG_CONSTANTCOLOR)
                    {
                        if(CurrentShaderResources.regColor[k] == colorRemap[((u32)(stageArray[i].TEVArg[j])) - ((u32)(SHADER_COLORINPUT0_RGB))])
                            break;
                    }
                    else //register must be empty
                    {
                        CurrentColorState[k] = SHADER_REG_CONSTANTCOLOR;
                        CurrentShaderResources.regColor[k] = colorRemap[((u32)(stageArray[i].TEVArg[j])) - ((u32)(SHADER_COLORINPUT0_RGB))];
                        break;
                    }
                }              
                if(k == -1)
                    OSHalt("out of registers - AssignConstantColorRegs");
            }

            else if(stageArray[i].TEVArg[j] >= SHADER_COLORINPUT0_A && //TEV arg is a constant alpha
                    stageArray[i].TEVArg[j] <= SHADER_COLORINPUT3_A)
            {   //find an empty register
                for ( k = 3; k > -1; k -- )
                {
                    if(CurrentAlphaState[k] == SHADER_REG_INUSE)
                        OSHalt("cannot allocate alpha register for constant color - AssignConstantColorRegs");
                    else if(CurrentAlphaState[k] == SHADER_REG_CONSTANTCOLOR)
                    {
                        if(CurrentShaderResources.regAlpha[k] == colorRemap[((u32)(stageArray[i].TEVArg[j])) - ((u32)(SHADER_COLORINPUT0_A))])
                            break;
                    }
                    else //register must be empty
                    {
                        CurrentAlphaState[k] = SHADER_REG_CONSTANTCOLOR;
                        CurrentShaderResources.regAlpha[k] = colorRemap[((u32)(stageArray[i].TEVArg[j])) - ((u32)(SHADER_COLORINPUT0_A))];
                        break;
                    }
                }  
                if(k == -1)
                    OSHalt("out of registers - AssignConstantColorRegs");
            }
        }
    }
}

/*>*******************************(*)*******************************<*/
static void FindParents ( u8 *earliest, u8 *latest, SHDRCompiled *shader )
{
    u8 i, j;
    SHADER_ComplexInput inputID;

    *earliest = *latest = 99;

    if(!(CompiledTEVCounter)) //this is the root shader
        return;

    //find out which shader input this is
    for ( i = 0; i < 8; i ++ )
    {
        if(CurrentShaderResources.complexData[i] == shader)
        {
            inputID = ((SHADER_ComplexInput)(i));
            break;
        }
    }

    if(i == 8) OSHalt("cannot find complex input in list - FindParents");

    //find parents in pool
    for ( i = 0; i < CompiledTEVCounter; i ++ )
    {
        for ( j = 0; j < 4; j ++ )
        {
            //if either the color or alpha stage uses this complex input
            if( ((u32)(CompiledTEVColorPool[i].TEVArg[j])) - ((u32)(SHADER_COMPLEXINPUT0_RGB)) == ((u32)(inputID)) ||
                ((u32)(CompiledTEVColorPool[i].TEVArg[j])) - ((u32)(SHADER_COMPLEXINPUT0_A)) == ((u32)(inputID)) ||
                ((u32)(CompiledTEVAlphaPool[i].TEVArg[j])) - ((u32)(SHADER_COMPLEXINPUT0_RGB)) == ((u32)(inputID)) ||
                ((u32)(CompiledTEVAlphaPool[i].TEVArg[j])) - ((u32)(SHADER_COMPLEXINPUT0_A)) == ((u32)(inputID)) )
            {
                if((*earliest) == 99)
                    *earliest = i;

                *latest = i;
            }
        }
    }

    if((*earliest) == 99)
        OSHalt("child not found in list - FindParents");
}

/*>*******************************(*)*******************************<*/
static void FindOpenRegisters   ( u8 pos, SHADER_RegisterState *color, SHADER_RegisterState *alpha )
{
    u8 i, j;
    u8 colorAccess[4] = {99, 99, 99, 99};
    u8 alphaAccess[4] = {99, 99, 99, 99};

    //block out constant color regs
    for ( i = 0; i < 4; i ++ )
    {
        if(CurrentColorState[i] == SHADER_REG_CONSTANTCOLOR)
            colorAccess[i] = 0;
        if(CurrentAlphaState[i] == SHADER_REG_CONSTANTCOLOR)
            alphaAccess[i] = 0;
    }
    
    if(pos != 99)
    {
        for ( i = pos; i < CompiledTEVCounter; i ++ )
        {
            //check CompiledTEVColorPool[i] to see if it's empty
            if(StageNotEmpty(&(CompiledTEVColorPool[i])))
            {
                //check to see which regs are consumed
                for ( j = 0; j < 4; j ++ )
                {
                    if(CompiledTEVColorPool[i].TEVArg[j] >= SHADER_CPREV &&
                       CompiledTEVColorPool[i].TEVArg[j] <= SHADER_C2 )
                    {
                        if(colorAccess[((u32)(CompiledTEVColorPool[i].TEVArg[j]))] == 99)
                            colorAccess[((u32)(CompiledTEVColorPool[i].TEVArg[j]))] = 0;
                    }

                    else if(CompiledTEVColorPool[i].TEVArg[j] >= SHADER_APREV &&
                            CompiledTEVColorPool[i].TEVArg[j] <= SHADER_A2 )
                    {
                        if(colorAccess[((u32)(CompiledTEVColorPool[i].TEVArg[j])) - ((u32)(SHADER_APREV))] == 99)
                            colorAccess[((u32)(CompiledTEVColorPool[i].TEVArg[j])) - ((u32)(SHADER_APREV))] = 0;
                    }
                }

                //check to see which reg is output
                if(colorAccess[((u32)(CompiledTEVColorPool[i].out_reg))] == 99)
                    colorAccess[((u32)(CompiledTEVColorPool[i].out_reg))] = 1;
            }

            //check CompiledTEVAlphaPool[i] to see if it's empty
            if(StageNotEmpty(&(CompiledTEVAlphaPool[i])))
            {
                //check to see which regs are consumed
                for ( j = 0; j < 4; j ++ )
                {
                    if(CompiledTEVAlphaPool[i].TEVArg[j] >= SHADER_APREV &&
                       CompiledTEVAlphaPool[i].TEVArg[j] <= SHADER_A2 )
                    {
                        if(alphaAccess[((u32)(CompiledTEVAlphaPool[i].TEVArg[j])) - ((u32)(SHADER_APREV))] == 99)
                            alphaAccess[((u32)(CompiledTEVAlphaPool[i].TEVArg[j])) - ((u32)(SHADER_APREV))] = 0;
                    }
                }

                //check to see which reg is output
                if(alphaAccess[((u32)(CompiledTEVAlphaPool[i].out_reg))] == 99)
                    alphaAccess[((u32)(CompiledTEVAlphaPool[i].out_reg))] = 1;
            }
        }
        //check for APREV and CPREV
        if(colorAccess[0] == 99)
            colorAccess[0] = 0;
        if(alphaAccess[0] == 99)
            alphaAccess[0] = 0;
    }    

    for ( i = 0; i < 4; i ++ )
    {
        if(colorAccess[i])
            color[i] = SHADER_REG_EMPTY;
        else
            color[i] = SHADER_REG_INUSE;

        if(alphaAccess[i])
            alpha[i] = SHADER_REG_EMPTY;
        else
            alpha[i] = SHADER_REG_INUSE;
    }
}

/*>*******************************(*)*******************************<*/
static BOOL StageNotEmpty ( CompiledTEVStage *stage )
{
    if(stage->TEVArg[1] == SHADER_ZERO &&
       stage->TEVArg[2] == SHADER_ZERO &&
       stage->TEVArg[3] == SHADER_ZERO )

        return FALSE;

    //else
    return TRUE;
}

/*>*******************************(*)*******************************<*/
// find an alpha and a color reg which are available for output.
// since we know a compiled shader always outputs to CPREV and APREV, 
// remap CPREV and APREV to these new registers
static void FindOutputRegs  ( u8 earliest, u8 latest, SHADER_TEVArg *regRemap,
                              SHADER_RegisterState *startColor, SHADER_RegisterState *startAlpha )
{
    u8 colorAccess[4] = {99, 99, 99, 99};
    u8 alphaAccess[4] = {99, 99, 99, 99};
    u8 i, j;

    for ( i = 0; i <= 4; i ++ )
    {
        if(startColor[i] != SHADER_REG_EMPTY)
            colorAccess[i] = 0;

        if(startAlpha[i] != SHADER_REG_EMPTY)
            alphaAccess[i] = 0;
    }

    if(earliest != 99)
    {
        for ( i = earliest; i <= latest; i ++ )
        {
            if(StageNotEmpty(&(CompiledTEVColorPool[i])))
            {
                //check to see which regs are consumed
                for ( j = 0; j < 4; j ++ )
                {
                    if(CompiledTEVColorPool[i].TEVArg[j] >= SHADER_CPREV &&
                       CompiledTEVColorPool[i].TEVArg[j] <= SHADER_C2 )
                    {
                        colorAccess[((u32)(CompiledTEVColorPool[i].TEVArg[j]))] = 0;
                    }

                    else if(CompiledTEVColorPool[i].TEVArg[j] >= SHADER_APREV &&
                            CompiledTEVColorPool[i].TEVArg[j] <= SHADER_A2 )
                    {
                        colorAccess[((u32)(CompiledTEVColorPool[i].TEVArg[j])) - ((u32)(SHADER_APREV))] = 0;
                    }
                }

                //check to see which reg is output
                colorAccess[((u32)(CompiledTEVColorPool[i].out_reg))] = 0;
            }

            //check CompiledTEVAlphaPool[i] to see if it's empty
            if(StageNotEmpty(&(CompiledTEVAlphaPool[i])))
            {
                //check to see which regs are consumed
                for ( j = 0; j < 4; j ++ )
                {
                    if(CompiledTEVAlphaPool[i].TEVArg[j] >= SHADER_APREV &&
                       CompiledTEVAlphaPool[i].TEVArg[j] <= SHADER_A2 )
                    {
                       alphaAccess[((u32)(CompiledTEVAlphaPool[i].TEVArg[j])) - ((u32)(SHADER_APREV))] = 0;
                    }
                }

                //check to see which reg is output
                alphaAccess[((u32)(CompiledTEVAlphaPool[i].out_reg))] = 0;
            }
        }
    }

    //find a color output register
    for ( i = 0; i <= 4; i ++ )
    {
        if(colorAccess[i] == 99)
        {
            regRemap[0] = (SHADER_TEVArg)(i);
            startColor[i] =  SHADER_REG_INUSE;
            break;
        }
    }
    if(i == 4) OSHalt("Cannot find color output register");

    //find a color output register
    for ( i = 0; i <= 4; i ++ )
    {
        if(alphaAccess[i] == 99)
        {
            regRemap[4] = (SHADER_TEVArg)(((u32)(i)) + ((u32)(SHADER_APREV)));
            startAlpha[i] =  SHADER_REG_INUSE;
            break;
        }
    }
    if(i == 4) OSHalt("Cannot find alpha output register");
}

/*>*******************************(*)*******************************<*/
static BOOL RemapRegisters  ( SHADER_TEVArg *regRemap, 
                              SHADER_RegisterState *tempColorRegUsage, 
                              SHADER_RegisterState *tempAlphaRegUsage, 
                              SHADER_RegisterState *colorRegUsage, 
                              SHADER_RegisterState *alphaRegUsage )
{
    u8 i, j;

    for ( i = 1; i < 4; i ++ )
    {
        if(colorRegUsage[i] == SHADER_REG_INUSE) // find empty remapped register
        {
            for ( j = 0; j < 4; j ++ )
            {
               if(tempColorRegUsage[j] == SHADER_REG_EMPTY) // we've found an empty reg - map it!
               {
                    regRemap[i] = (SHADER_TEVArg)(j);
                    tempColorRegUsage[j] = SHADER_REG_INUSE;
                    break;
               }
            }
            if(j == 4) return FALSE;
        }

        if(alphaRegUsage[i] == SHADER_REG_INUSE) // find empty remapped register
        {
            for ( j = 0; j < 4; j ++ )
            {
               if(tempAlphaRegUsage[j] == SHADER_REG_EMPTY) // we've found an empty reg - map it!
               {
                    regRemap[i + 4] = (SHADER_TEVArg)(((u32)(j)) + ((u32)(SHADER_APREV)));
                    tempAlphaRegUsage[j] = SHADER_REG_INUSE;
                    break;
               }
            }
            if(j == 4) return FALSE;
        }
    }
    return TRUE;
}

/*>*******************************(*)*******************************<*/
static void PrepareTevPool  ( u8 earliest, u8 numStages )
{
    s8 i;

    if(numStages + CompiledTEVCounter > 16)
        OSHalt("too many TEV stages - PrepareTevPool");

    if(earliest != 99)
    {
        for ( i = (s8)(CompiledTEVCounter - 1); i >= (s8)earliest; i -- )
        {
            CopyCompiledStage(&(CompiledTEVColorPool[i]), &(CompiledTEVColorPool[i + numStages]));
            CopyCompiledStage(&(CompiledTEVAlphaPool[i]), &(CompiledTEVAlphaPool[i + numStages]));
        }
    }

    CompiledTEVCounter += numStages;
}

/*>*******************************(*)*******************************<*
static void PrepareTevPool  ( u8 earliest, u8 numStages )
{
    u8 i;

    if(numStages + CompiledTEVCounter > 16)
        OSHalt("too many TEV stages - PrepareTevPool");

    if(earliest != 99)
    {
        for ( i = 0; i < numStages; i ++ )
        {
            CopyCompiledStage(&(CompiledTEVColorPool[i + earliest]), &(CompiledTEVColorPool[CompiledTEVCounter + i]));
            CopyCompiledStage(&(CompiledTEVAlphaPool[i + earliest]), &(CompiledTEVAlphaPool[CompiledTEVCounter + i]));
        }
    }

    CompiledTEVCounter += numStages;
}

/*>*******************************(*)*******************************<*/
static void CopyCompiledStage   ( CompiledTEVStage *src, CompiledTEVStage *dst )
{
    u8 i;

    for ( i = 0; i < 4; i ++ )
    {
        dst->TEVArg[i] = src->TEVArg[i];
    }
	
    dst->op = src->op;
    dst->bias = src->bias;
    dst->scale = src->scale;
    dst->clamp = src->clamp;
	dst->mode = src->mode;
    dst->out_reg = src->out_reg;

    dst->rasInput = src->rasInput;
    dst->texGenIdx = src->texGenIdx;
    dst->texInput = src->texInput;
}

/*>*******************************(*)*******************************<*/
static void InsertTevStages ( u8 pos, SHDRCompiled *shader, 
                              SHADER_TexInput *texRemap, SHADER_RasInput *rasRemap, 
                              SHADER_ColorInput *colorRemap, 
                              SHADER_ComplexInput *complexRemap, SHADER_TEVArg *regRemap,
                              u8 texOffset )
{
    CompiledTEVStage *colorStages = (CompiledTEVStage *)(shader->colorStages);
    CompiledTEVStage *alphaStages = (CompiledTEVStage *)(shader->alphaStages);
    u8 i;

    for ( i = 0; i < shader->numStages; i ++ )
    {
        RemapStage(&(colorStages[i]), &(CompiledTEVColorPool[i + pos]),
                   texRemap, rasRemap, colorRemap, complexRemap, regRemap, texOffset);

        RemapStage(&(alphaStages[i]), &(CompiledTEVAlphaPool[i + pos]),
                   texRemap, rasRemap, colorRemap, complexRemap, regRemap, texOffset);
    }
}

/*>*******************************(*)*******************************<*/
static void RemapStage(CompiledTEVStage *src, CompiledTEVStage *dst,
                       SHADER_TexInput *texRemap, SHADER_RasInput *rasRemap, 
                       SHADER_ColorInput *colorRemap, 
                       SHADER_ComplexInput *complexRemap, SHADER_TEVArg *regRemap,
                       u8 texOffset )
{
    u8 i;

    for ( i = 0; i < 4; i ++ )
    {
        if(src->TEVArg[i] <= SHADER_A2) // it's a register - remap it
            dst->TEVArg[i] = regRemap[((u32)(src->TEVArg[i]))];

        else if(src->TEVArg[i] >= SHADER_COMPLEXINPUT0_RGB && // it's a complex input - remap it
                src->TEVArg[i] <= SHADER_COMPLEXINPUT7_A)
        {
            if(src->TEVArg[i] >= SHADER_COMPLEXINPUT0_A)
                dst->TEVArg[i] = (SHADER_TEVArg)(((u32)(SHADER_COMPLEXINPUT0_A)) + ((u32)(complexRemap[((u32)(src->TEVArg[i])) - ((u32)(SHADER_COMPLEXINPUT0_A))])));
            else
                dst->TEVArg[i] = (SHADER_TEVArg)(((u32)(SHADER_COMPLEXINPUT0_RGB)) +((u32)(complexRemap[((u32)(src->TEVArg[i])) - ((u32)(SHADER_COMPLEXINPUT0_RGB))])));
        }

        else if(src->TEVArg[i] >= SHADER_COLORINPUT0_RGB && // it's a color input - remap it
                src->TEVArg[i] <= SHADER_COLORINPUT3_A)
        {
            if(src->TEVArg[i] >= SHADER_COLORINPUT0_A)
                dst->TEVArg[i] = (SHADER_TEVArg)(((u32)(SHADER_COLORINPUT0_A)) + ((u32)(colorRemap[((u32)(src->TEVArg[i])) - ((u32)(SHADER_COLORINPUT0_A))])));
            else
                dst->TEVArg[i] = (SHADER_TEVArg)(((u32)(SHADER_COLORINPUT0_RGB)) + ((u32)(colorRemap[((u32)(src->TEVArg[i])) - ((u32)(SHADER_COLORINPUT0_RGB))])));
        }

        else
            dst->TEVArg[i] = src->TEVArg[i];

    }
	
    dst->op = src->op;
    dst->bias = src->bias;
    dst->scale = src->scale;
    dst->clamp = src->clamp;
	dst->mode = src->mode;

    dst->out_reg = regRemap[((u32)(src->out_reg))];
    //dst->out_reg = src->out_reg;

    if(src->rasInput == SHADER_RASNONE)
        dst->rasInput = SHADER_RASNONE;
    else
        dst->rasInput = rasRemap[((u32)(src->rasInput))];

    if(src->texGenIdx == NO_TEXTURE_COORD_EXP)
        dst->texGenIdx = NO_TEXTURE_COORD_EXP;
    else
        dst->texGenIdx = (u8)(src->texGenIdx + texOffset);

    if(src->texInput == SHADER_TEXNONE)
        dst->texInput = SHADER_TEXNONE;
    else
        dst->texInput = texRemap[((u32)(src->texInput))];
}

/*>*******************************(*)*******************************<*/
static void RemapComplexReferences  ( SHDRCompiled *shader, SHADER_TEVArg *regRemap )
{
    u8 i, j;
    SHADER_ComplexInput input;

    //figure out which input this shader is
    for ( i = 0; i < 8; i ++ )
    {
        if(CurrentShaderResources.complexData[i] == shader)
        {
            input = (SHADER_ComplexInput)(i);
            break;
        }
    }

    if(i == 8) OSHalt("Complex input not found - RemapComplexReferences");

    for ( i = 0; i < CompiledTEVCounter; i ++ )
    {
        for ( j = 0; j < 4; j++ )
        {
            if(CompiledTEVColorPool[i].TEVArg[j] == (SHADER_TEVArg)(((u32)(SHADER_COMPLEXINPUT0_RGB)) + ((u32)(input))))
                CompiledTEVColorPool[i].TEVArg[j] = regRemap[0];
            else if(CompiledTEVColorPool[i].TEVArg[j] == (SHADER_TEVArg)(((u32)(SHADER_COMPLEXINPUT0_A)) + ((u32)(input))))
                CompiledTEVColorPool[i].TEVArg[j] = regRemap[4];

            if(CompiledTEVAlphaPool[i].TEVArg[j] == (SHADER_TEVArg)(((u32)(SHADER_COMPLEXINPUT0_A)) + ((u32)(input))))
                CompiledTEVAlphaPool[i].TEVArg[j] = regRemap[4];
        }
    }

}

/*>*******************************(*)*******************************<*/


