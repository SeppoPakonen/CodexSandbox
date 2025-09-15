/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     geoPalette.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/geoPalette/src/displayObject.c $
    
    24    12/04/00 7:58p John
    Added multitexturing ability (work in progress).
    
    23    12/05/00 12:20p Ryan
    Added texture state override commands
    
    22    9/16/00 12:26p Ryan
    shader optimization update
    
    21    8/14/00 6:25p John
    Added user-defined data to ANM, ACT, and GPL formats.
    
    20    8/04/00 5:08p John
    No longer computes extra modelview matrix for stitched objects in
    DOVARender since they are loaded in SetState.
    
    19    7/18/00 4:38p Tian
    Initialized colorSrc in DOVARender
    
    18    7/19/00 3:58p Ryan
    update to work with new shader API
    
    Bug fix for TexGen reset
    
    17    7/18/00 11:38a Tian
    Fixed bug in TranslateVCD so it recognizes empty vertex descriptor is
    different from current VCD.
    
    16    7/10/00 5:40p John
    Added optimization to not send VCD if current GX VCD is the same.
    Added optimization to compute enumeration value with arithmetic instead
    of using a switch statement.
    
    15    6/27/00 6:13p John
    Substituted quantize.h macros.
    Added optimization to set a register color if only one color is
    supplied for the display object.
    Cleaned up code and detabified.
    
    14    6/23/00 5:06p Ryan
    updated for correct decal shader
    
    13    5/06/00 5:10p Ryan
    update for shader integration
    
    12    4/26/00 11:39p Ryan
    update for shader implementation
    
    11    4/21/00 5:12a Ryan
    update to reset the # of TEV stages for each Display Object rendered
    
    10    4/12/00 6:28a Ryan
    Update for color/alpha and texgen shader revision
    
    9     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <CharPipe/GeoPalette.h>
#include <Dolphin/gx.h>
#include <CharPipe/texPalette.h>
#include <CharPipe/light.h>
#include <CharPipe/normalTable.h>
#include <CharPipe/shader.h>

/********************************/

#define QUANT_TYPE(a)  ((a)>>4)
#define QUANT_SHIFT(a) ((a)&0x0F)

/********************************/
static MtxPtr       SkinForwardArray = 0; // Also tells whether we are currently stitching
static MtxPtr       SkinInverseArray = 0;

static GXTexObj     DOTexObj[DO_MAX_TEXTURES];
static GXTlutObj    DOTlut[DO_MAX_TEXTURES];

static u32          LastShader;
static u8           ShaderSet = 0;

//flag to keep track of wheather a vertex color channel is used for a shader
static BOOL         ColorChanUsed = FALSE;

//pointer to texture currently used by a shader - NULL if the shader doesn't use a texture
static GXTexObj     *CurrentTexObj = 0;

//texture state overried variables
static u32          TextureStateOverrides[11] = { DOTS_USEDEFAULT,
                                                  DOTS_USEDEFAULT, 
                                                  DOTS_USEDEFAULT,
                                                  DOTS_USEDEFAULT,
                                                  DOTS_USEDEFAULT,
                                                  DOTS_USEDEFAULT,
                                                  DOTS_USEDEFAULT,
                                                  DOTS_USEDEFAULT,
                                                  DOTS_USEDEFAULT,
                                                  DOTS_USEDEFAULT,
                                                  DOTS_USEDEFAULT};
static float        OverrideMaxLOD;
static float        OverrideMinLOD;
static float        OverrideLODBias;

/*>*******************************(*)*******************************<*/
static void      SetState             ( DODisplayStatePtr state, 
                                        DODisplayObjPtr dispObj, 
                                        Mtx camera );

static void      TranslateVCD         ( u32 VCD );
static void      LoadLights           ( u8 numLights, va_list ptr, float scale, 
                                        GXColorSrc colorSrc );
static void      DisableLights        ( GXColorSrc colorSrc );

static GXCompCnt TranslateCmpCount    ( GXAttr attr, u8 numComponents );
static void      GetColorFromQuant    ( void* src, u8 srcCode, 
                                        u8* r, u8* g, u8* b, u8* a );
static u16       DisplayDataSize      ( u8 quantFlags );
static u16       DisplayDataSizeColor ( u8 quantFlags );

static void      SetDispObjShader     ( DODisplayObjPtr dispObj, u32 setting );


/*>*******************************(*)*******************************<*/
void    DORelease   ( DODisplayObjPtr *dispObj )
{
    if((*dispObj)->positionData)
        OSFree((*dispObj)->positionData);

    if((*dispObj)->colorData)
        OSFree((*dispObj)->colorData);

    if((*dispObj)->textureData)
        OSFree((*dispObj)->textureData);

    if((*dispObj)->lightingData)
        OSFree((*dispObj)->lightingData);

    if((*dispObj)->displayData)
        OSFree((*dispObj)->displayData);

    OSFree(*dispObj);
    *dispObj = 0;
}

/*>*******************************(*)*******************************<*/
MtxPtr  DOGetWorldMatrix    ( DODisplayObjPtr dispObj )
{
    return dispObj->worldMatrix;
}

/*>*******************************(*)*******************************<*/
void    DOHide          ( DODisplayObjPtr dispObj )
{
    dispObj->visibility = DO_DISPLAY_OFF;
}

/*>*******************************(*)*******************************<*/
void    DORender        ( DODisplayObjPtr dispObj, Mtx camera, u8 numLights, ... )
{
    va_list  ptr;

    va_start(ptr, numLights);

    DOVARender(dispObj, camera, numLights, ptr);

    va_end(ptr);
}

/*>*******************************(*)*******************************<*/
void    DOVARender ( DODisplayObjPtr dispObj, Mtx camera, u8 numLights, va_list list )
{
    u32               i;
    DODisplayStatePtr cursor;
    GXColor           matColor;
    GXColorSrc        colorSrc = GX_SRC_VTX;
    Mtx               mv, tempMtx;

    if(dispObj->visibility == DO_DISPLAY_OFF) return;

    // Load the modelview matrix if we are not stitching
    if(SkinForwardArray == 0)
    {
        MTXConcat(camera, dispObj->worldMatrix, mv);
        GXLoadPosMtxImm(mv, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
    }

    // Set position data
    ASSERTMSG( dispObj->positionData, "DOVARender: Must provide position data" );
    ASSERTMSG( dispObj->colorData || dispObj->textureData,
               "DOVARender: Must either supply color or texture coordinates.\n" );

    GXSetArray(GX_VA_POS, dispObj->positionData->positionArray, 
               (u8)(DisplayDataSize(dispObj->positionData->quantizeInfo) * dispObj->positionData->compCount));

    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, 
                    TranslateCmpCount(GX_VA_POS, dispObj->positionData->compCount), 
                   (GXCompType)(QUANT_TYPE(dispObj->positionData->quantizeInfo)),
                   (u8)(QUANT_SHIFT(dispObj->positionData->quantizeInfo)));

    // Set color data
    if(dispObj->colorData)
    {   
        // Check to see if there is only one color so we can use a color register
        if(dispObj->colorData->numColors == 1)
        {
            colorSrc = GX_SRC_REG;
            GetColorFromQuant(dispObj->colorData->colorArray,
                              dispObj->colorData->quantizeInfo,
                              &matColor.r, &matColor.g, &matColor.b, &matColor.a);
            GXSetChanMatColor(GX_COLOR0A0, matColor);
        }
        // Otherwise set a color array and expect color indices in display list
        else
        {
            GXSetArray(GX_VA_CLR0, dispObj->colorData->colorArray, 
                       (u8)(DisplayDataSizeColor(dispObj->colorData->quantizeInfo)));

            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, 
                            TranslateCmpCount(GX_VA_CLR0, dispObj->colorData->compCount), 
                           (GXCompType)(QUANT_TYPE(dispObj->colorData->quantizeInfo)),
                            0);
        }

        GXSetNumChans(1);

        ColorChanUsed = TRUE;
    }
    else
    {
        GXSetNumChans(0);

        ColorChanUsed = FALSE;
    }

    // Set texture data
    if(dispObj->textureData)
    {
        for( i = 0; i < dispObj->numTextureChannels; i++ )
        {
            GXSetArray((GXAttr)(GX_VA_TEX0 + i), dispObj->textureData[i].textureCoordArray, 
                      (u8)(DisplayDataSize(dispObj->textureData[i].quantizeInfo) * dispObj->textureData[i].compCount));

            GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)(GX_VA_TEX0 + i), 
                            TranslateCmpCount((GXAttr)(GX_VA_TEX0 + i), dispObj->textureData[i].compCount), 
                           (GXCompType)(QUANT_TYPE(dispObj->textureData[i].quantizeInfo)),
                           (u8)(QUANT_SHIFT(dispObj->textureData[i].quantizeInfo)));

            GXSetTexCoordGen((GXTexCoordID)(GX_TEXCOORD0 + i), GX_TG_MTX2x4, (GXTexGenSrc)(GX_TG_TEX0 + i), GX_IDENTITY);
        }
    }

    // Set lighting data
    if(dispObj->lightingData && dispObj->colorData && numLights > 0)
    {
        if(dispObj->lightingData->normalArray)
        {
            GXSetArray(GX_VA_NRM, dispObj->lightingData->normalArray, 
                  (u8)(DisplayDataSize(dispObj->lightingData->quantizeInfo) * dispObj->lightingData->compCount));

            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, 
                            TranslateCmpCount(GX_VA_NRM, dispObj->lightingData->compCount), 
                           (GXCompType)(QUANT_TYPE(dispObj->lightingData->quantizeInfo)),
                           (u8)(QUANT_SHIFT(dispObj->lightingData->quantizeInfo)));
        }
        else
        {
            GXSetArray(GX_VA_NRM, normalTable, 
                  (u8)(DisplayDataSize(normalTableQuantizeInfo) * normalTableNumComponents));

            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, 
                            TranslateCmpCount(GX_VA_NRM, normalTableNumComponents), 
                           (GXCompType)(QUANT_TYPE(normalTableQuantizeInfo)),
                           (u8)(QUANT_SHIFT(normalTableQuantizeInfo)));
        }
                 
        // Calculate inverse transpose matrix if this is not a stitched model
        if(SkinForwardArray == 0)
        {
            MTXInverse(mv, tempMtx);
            MTXTranspose(tempMtx, mv);
            GXLoadNrmMtxImm(mv, GX_PNMTX0);
        }

        // Load the lights into hardware
        LoadLights(numLights, list, dispObj->lightingData->ambientPercentage * 0.01F, colorSrc);
    }
    else
    {
        DisableLights(colorSrc);
    }

    cursor = dispObj->displayData->displayStateList;
    for ( i = 0; i < dispObj->displayData->numStateEntries; i++ )
    {
        // Set graphics state
        SetState(&cursor[i], dispObj, camera);
            
        // Call a display list if it exists
        if(cursor[i].primitiveList)
        {
            if(!ShaderSet) // if there has been no shader set, set it now
                SetDispObjShader(dispObj, LastShader);

            GXCallDisplayList(cursor[i].primitiveList, cursor[i].listSize);
        }
        
    }
}

/*>*******************************(*)*******************************<*/
void    DORenderSkin        ( DODisplayObjPtr dispObj, Mtx camera, MtxPtr mtxArray,
                              MtxPtr invTransposeMtxArray, u8 numLights, ... )
{
    va_list  ptr;

    va_start(ptr, numLights);

    DOVARenderSkin( dispObj, camera, mtxArray, invTransposeMtxArray, numLights, ptr );

    va_end(ptr);
}

/*>*******************************(*)*******************************<*/
void	DOVARenderSkin		    ( DODisplayObjPtr dispObj, Mtx camera, 
						    	  MtxPtr mtxArray, MtxPtr invTransposeMtxArray, 
						    	  u8 numLights, va_list list )
{
    // Set stitching matrix arrays
    ASSERT( mtxArray );
    SkinForwardArray = mtxArray;
    SkinInverseArray = invTransposeMtxArray;

    DOVARender(dispObj, camera, numLights, list);

    // Reset stitching matrix arrays since these are also used as flags
    // to see whether we are stitching or not in DOVARender.
    SkinForwardArray = 0;
    SkinInverseArray = 0;
}

/*>*******************************(*)*******************************<*/
static void SetState ( DODisplayStatePtr state, DODisplayObjPtr dispObj, Mtx camera )
{
    TEXDescriptorPtr tex;
    
    u32    pnMtxIdx;
    Mtx    tempMtx;

    u32    wrapS, wrapT;
    u32    minFilt, magFilt;
    u32    maxAniso;
    GXBool mipmap, biasClamp, edgeLOD;
    f32    minLOD, maxLOD, biasLOD;

    u32    channel;

    switch(state->id)
    {
        case DISPLAY_STATE_TEXTURE:

            channel = (state->setting >> 13) & 0x00000007;
            tex = TEXGet(dispObj->textureData[channel].texturePalette, (state->setting) & 0x00001FFF);

            //Grab wrap s state
            if(TextureStateOverrides[DOTS_WRAP_S] != DOTS_USEDEFAULT)
                wrapS = (u32)(TextureStateOverrides[DOTS_WRAP_S]);
            else
            {
                wrapS = ((state->setting) >> 16) & 0x0000000F;
                switch(wrapS)
                {
                    case DO_CLAMP:  wrapS = GX_CLAMP;  break;
                    case DO_REPEAT: wrapS = GX_REPEAT; break;
                    case DO_MIRROR: wrapS = GX_MIRROR; break;
                }
            }

            //Grab wrap t state
            if(TextureStateOverrides[DOTS_WRAP_T] != DOTS_USEDEFAULT)
                wrapT = (u32)(TextureStateOverrides[DOTS_WRAP_T]);
            else
            {
                wrapT = ((state->setting) >> 20) & 0x0000000F;
                switch(wrapT)
                {
                    case DO_CLAMP:  wrapT = GX_CLAMP;  break;
                    case DO_REPEAT: wrapT = GX_REPEAT; break;
                    case DO_MIRROR: wrapT = GX_MIRROR; break;
                }
            }

            //grab min filter state
            if(TextureStateOverrides[DOTS_MIN_FILTER] != DOTS_USEDEFAULT)
                minFilt = (u32)(TextureStateOverrides[DOTS_MIN_FILTER]);
            else
            {
                minFilt = ((state->setting) >> 24) & 0x0000000F;
                switch(minFilt)
                {
                    case DO_NEAR:          minFilt = GX_NEAR;          break;
                    case DO_LINEAR:        minFilt = GX_LINEAR;        break;
                    case DO_NEAR_MIP_NEAR: minFilt = GX_NEAR_MIP_NEAR; break;
                    case DO_LIN_MIP_NEAR:  minFilt = DO_LIN_MIP_NEAR;  break;
                    case DO_NEAR_MIP_LIN:  minFilt = DO_NEAR_MIP_LIN;  break;
                    case DO_LIN_MIP_LIN:   minFilt = DO_LIN_MIP_LIN;   break;
                }
            }

            //grab mag filter state
            if(TextureStateOverrides[DOTS_MAG_FILTER] != DOTS_USEDEFAULT)
                magFilt = (u32)(TextureStateOverrides[DOTS_MAG_FILTER]);
            else
            {
                magFilt = ((state->setting) >> 28) & 0x0000000F;
                switch(magFilt)
                {
                    case DO_NEAR:          magFilt = GX_NEAR;          break;
                    case DO_LINEAR:        magFilt = GX_LINEAR;        break;
                    case DO_NEAR_MIP_NEAR: magFilt = GX_NEAR_MIP_NEAR; break;
                    case DO_LIN_MIP_NEAR:  magFilt = DO_LIN_MIP_NEAR;  break;
                    case DO_NEAR_MIP_LIN:  magFilt = DO_NEAR_MIP_LIN;  break;
                    case DO_LIN_MIP_LIN:   magFilt = DO_LIN_MIP_LIN;   break;
                }
            }

            //grab mipmap state
            if(TextureStateOverrides[DOTS_MIPMAP] != DOTS_USEDEFAULT)
                mipmap = (GXBool)(TextureStateOverrides[DOTS_MIPMAP]);
            else
            {
                if(tex->textureHeader->minLOD == tex->textureHeader->maxLOD)
                    mipmap = GX_FALSE;
                else
                    mipmap = GX_TRUE;


            }

            //grab min LOD state
            if(TextureStateOverrides[DOTS_MIN_LOD] != DOTS_USEDEFAULT)
                minLOD = OverrideMinLOD;
            else
                minLOD = tex->textureHeader->minLOD;

            //grab max LOD state
            if(TextureStateOverrides[DOTS_MAX_LOD] != DOTS_USEDEFAULT)
                maxLOD = OverrideMaxLOD;
            else
                maxLOD = tex->textureHeader->maxLOD;

            //grab LOD bias state
            if(TextureStateOverrides[DOTS_LOD_BIAS] != DOTS_USEDEFAULT)
                biasLOD = OverrideLODBias;
            else
                biasLOD = tex->textureHeader->LODBias;

            //grab bias clamp state
            if(TextureStateOverrides[DOTS_BIAS_CLAMP] != DOTS_USEDEFAULT)
                biasClamp = (GXBool)(TextureStateOverrides[DOTS_LOD_BIAS]);
            else
                biasClamp = GX_DISABLE;

            //grab edge LOD state
            if(TextureStateOverrides[DOTS_EDGE_LOD] != DOTS_USEDEFAULT)
                edgeLOD = (GXBool)(TextureStateOverrides[DOTS_EDGE_LOD]);
            else
                edgeLOD = tex->textureHeader->edgeLODEnable;

            //grab max aniso state
            if(TextureStateOverrides[DOTS_MAX_ANISO] != DOTS_USEDEFAULT)
                maxAniso = (u32)(TextureStateOverrides[DOTS_MAX_ANISO]);
            else
                maxAniso = (u32)GX_ANISO_1;

            if(tex->CLUTHeader)
            {
                GXInitTexObjCI(&DOTexObj[channel], tex->textureHeader->data, tex->textureHeader->width, 
                               tex->textureHeader->height, (GXCITexFmt)tex->textureHeader->format,
                              (GXTexWrapMode)wrapS, (GXTexWrapMode)wrapT, 
                               mipmap, (u32)GX_TLUT0); 

                GXInitTlutObj(&DOTlut[channel], tex->CLUTHeader->data, 
                              tex->CLUTHeader->format, tex->CLUTHeader->numEntries);

                GXLoadTlut(&DOTlut[channel], (u32)GX_TLUT0);
            }
            else
            {
                GXInitTexObj(&DOTexObj[channel], tex->textureHeader->data, tex->textureHeader->width, 
                             tex->textureHeader->height, (GXTexFmt)tex->textureHeader->format,
                            (GXTexWrapMode)wrapS, (GXTexWrapMode)wrapT, 
                             mipmap); 
            }
             
            GXInitTexObjLOD(&DOTexObj[channel], (GXTexFilter)minFilt, (GXTexFilter)magFilt, 
                            minLOD, maxLOD, biasLOD, biasClamp, 
                            edgeLOD, (GXAnisotropy)maxAniso);


            ShaderSet = 0;
            return;
        case DISPLAY_STATE_VCD:
            TranslateVCD(state->setting);
            return;
        case DISPLAY_STATE_TEXTURE_COMBINE:
            SetDispObjShader(dispObj, state->setting);
            return;
        case DISPLAY_STATE_MTXLOAD:

            // Multiply the index [0 to 10] with 3 to get GX_PNMTX? enumeration value
            pnMtxIdx = (state->setting & 0xFFFF) * 3;
            
            ASSERTMSG( SkinForwardArray, "Display object is stitched but no stitching matrices set through DORenderSkin or DOVARenderSkin." );
            MTXConcat(camera, &(SkinForwardArray[((u16)((state->setting) >> 16)) * MTX_PTR_OFFSET]), tempMtx);
            GXLoadPosMtxImm(tempMtx, pnMtxIdx);

            if(dispObj->lightingData)
            {
                MTXConcat(camera, &(SkinInverseArray[((u16)((state->setting) >> 16)) * MTX_PTR_OFFSET]), tempMtx);
                GXLoadNrmMtxImm(tempMtx, pnMtxIdx);
            }

            return;
        default:
            ASSERTMSG(0, "Unknown state setting for Display Object");
            break;
    }
}

/*>*******************************(*)*******************************<*/
void    DOSetWorldMatrix    ( DODisplayObjPtr dispObj, Mtx m )
{
    MTXCopy(m, dispObj->worldMatrix);
}

/*>*******************************(*)*******************************<*/
void    DOShow          ( DODisplayObjPtr dispObj )
{
    dispObj->visibility = DO_DISPLAY_ON;
}

/*>*******************************(*)*******************************<*/
f32     DOSetAmbientPercentage  ( DODisplayObjPtr dispObj, f32 percent )
{
    f32 temp = 0.0F;

    if(dispObj->lightingData)
    {
        temp = dispObj->lightingData->ambientPercentage;
        dispObj->lightingData->ambientPercentage = percent;
    }

    return temp;
}

/*>*******************************(*)*******************************<*/
static void TranslateVCD    ( u32 VCD )
{   
    GXAttrType    type;
    u32           i, shift;
    GXAttr        attr;
    GXVtxDescList attrList[GX_MAX_VTXDESCLIST_SZ];

    // Check to see if VCD has changed to prevent redundant states
    GXGetVtxDescv(attrList);
    for( i = 0; i < GX_MAX_VTXDESCLIST_SZ; i++ )
    {
        if( attrList[i].attr == GX_VA_NULL )
        {
            if( i == 0 )
            {
                // empty, so we need to load ours.
                break;
            }
            else
            {
                // Current VCD is same as what we want so don't send VCD
                return;
            }

        }
        
        else if( attrList[i].attr == GX_VA_PNMTXIDX )
            // Shift for GX_VA_PNMTXIDX is 0
            shift = 0;
        else
            // Shift for GX_VA_POS is 2, GX_VA_NRM is 4, GX_VA_CLR0, is 6, etc.
            shift = (u32)(attrList[i].attr - GX_VA_POS + 1) << 1;

        // If VCD is not the same, then break, so we can send new VCD
        if( attrList[i].type != ((VCD >> shift) & 0x3) )
            break;
    }

    // Set the necessary vertex descriptors
    GXClearVtxDesc();
    i = 0;
    type = (GXAttrType)(VCD & 0x3);
    if( type != GX_NONE )
    {
        attrList[i].attr = GX_VA_PNMTXIDX;
        attrList[i].type = type;
        i++;
    }

    for( attr = GX_VA_POS, shift = 2; attr <= GX_VA_TEX7; attr++, shift += 2 )
    {
        type = (GXAttrType)((VCD >> shift) & 0x3);
        if( type != GX_NONE )
        {
            attrList[i].attr = attr;
            attrList[i].type = type;
            i++;
        }
    }

    // End the attribute list
    attrList[i].attr = GX_VA_NULL;

    // Set the VCD
    GXSetVtxDescv( attrList );
}

/*>*******************************(*)*******************************<*/
static void LoadLights ( u8 numLights, va_list ptr, float scale, GXColorSrc colorSrc )
{
    LITLightPtr light;
    GXLightID   lightNum, lightMask = (GXLightID)0;
    GXColor     color;
    u32         i;

    ASSERTMSG( numLights <= 8, "LoadLights: Maximum number of lights is 8" );
    ASSERTMSG( scale >= 0.0F && scale <= 1.0F, "LoadLights: Scale needs to be normalized" );
    ASSERTMSG( colorSrc == GX_SRC_VTX || colorSrc == GX_SRC_REG, "LoadLights: Color source incorrect" );

    //set the ambient color
    color.r = color.g = color.b = (u8)(255 * scale);
    color.a = 255;
    GXSetChanAmbColor(GX_COLOR0A0, color);

    scale = 1.0F - scale;
    for( i = 0; i < numLights; i++ )
    {
        light = va_arg(ptr, LITLightPtr);

        //scale the light color
        color.r = (u8)(light->color.r * scale);
        color.g = (u8)(light->color.g * scale);
        color.b = (u8)(light->color.b * scale);

        GXInitLightColor(&(light->lt_obj), color);

        GXInitLightPos(&(light->lt_obj), light->worldPosition.x, 
                       light->worldPosition.y, light->worldPosition.z);

        GXInitLightDir(&(light->lt_obj), light->worldDirection.x, 
                       light->worldDirection.y, light->worldDirection.z);

        //get value of GX_LIGHT? enumeration from i
        lightNum = (GXLightID)(0x1 << i);

        GXLoadLightObjImm(&(light->lt_obj), lightNum);

        lightMask |= lightNum;
    }

    GXSetChanCtrl(GX_COLOR0A0, GX_ENABLE, GX_SRC_REG, colorSrc, lightMask,
                  GX_DF_CLAMP, GX_AF_SPOT );
}

/*>*******************************(*)*******************************<*/
static void DisableLights ( GXColorSrc colorSrc )
{
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, colorSrc, GX_LIGHT_NULL,
                  GX_DF_NONE, GX_AF_NONE );
}

/*>*******************************(*)*******************************<*/
static GXCompCnt TranslateCmpCount  ( GXAttr attr, u8 numComponents )
{
    switch(attr)
    {
        case GX_VA_POS:
            if(numComponents == 2)
                return GX_POS_XY;
            if(numComponents == 3)
                return GX_POS_XYZ;
            break;

        case GX_VA_NRM:
            if(numComponents == 3)
                return GX_NRM_XYZ;
            break;

        case GX_VA_CLR0:
        case GX_VA_CLR1:
            if(numComponents == 3)
                return GX_CLR_RGB;
            if(numComponents == 4)
                return GX_CLR_RGBA;
            break;

        case GX_VA_TEX0:
        case GX_VA_TEX1:
        case GX_VA_TEX2:
        case GX_VA_TEX3:
        case GX_VA_TEX4:
        case GX_VA_TEX5:
        case GX_VA_TEX6:
        case GX_VA_TEX7:
            if(numComponents == 1)
                return GX_TEX_S;
            if(numComponents == 2)
                return GX_TEX_ST;
            break;
    }

    ASSERTMSG(0, "TranslateCmpCount: Unknown component count value");
    return GX_POS_XY;  // never reached
}

/*>*******************************(*)*******************************<*/
static void GetColorFromQuant( void* src, u8 srcCode, u8* r, u8* g, u8* b, u8* a )
{
    u8  quant = (u8)QUANT_TYPE(srcCode);

    ASSERTMSG( src && r && g && b && a, "GetColorFromQuant: One argument is NULL" );

    switch( quant )
    {
        case GX_RGB565:
            *r = (u8)((( *(u16*)src & 0xF800 ) >> 11) << 3);
            *g = (u8)((( *(u16*)src & 0x07E0 ) >> 5 ) << 2);
            *b = (u8)( ( *(u16*)src & 0x001F ) << 3);
            *a = 255;
            break;

        case GX_RGBA4:
            *r = (u8)((( *(u16*)src & 0xF000 ) >> 12) << 4);
            *g = (u8)((( *(u16*)src & 0x0F00 ) >> 8 ) << 4);
            *b = (u8)((( *(u16*)src & 0x00F0 ) >> 4 ) << 4);
            *a = (u8)( ( *(u16*)src & 0x000F ) << 4);
            break;

        case GX_RGBA8:
            *r = (u8)(( *(u32*)src & 0xFF000000 ) >> 24);
            *g = (u8)(( *(u32*)src & 0x00FF0000 ) >> 16);
            *b = (u8)(( *(u32*)src & 0x0000FF00 ) >> 8);
            *a = (u8)(( *(u32*)src & 0x000000FF ));
            break;
  
        case GX_RGB8:
        case GX_RGBX8:
            *r = (u8)(( *(u32*)src & 0xFF000000 ) >> 24);
            *g = (u8)(( *(u32*)src & 0x00FF0000 ) >> 16);
            *b = (u8)(( *(u32*)src & 0x0000FF00 ) >> 8);
            *a = 255;
            break;
            
        case GX_RGBA6:
            *r = (u8)((( *(u16*)src & 0xFC0000 ) >> 18) << 2);
            *g = (u8)((( *(u16*)src & 0x03F000 ) >> 12) << 2);
            *b = (u8)((( *(u16*)src & 0x000FC0 ) >> 6 ) << 2);
            *a = (u8)( ( *(u16*)src & 0x00003F ) << 2);
            break;

        default:
            ASSERTMSG(0, "GetColorFromQuant: Unknown color type" );
            break;
    }
}

/*>*******************************(*)*******************************<*/
static u16 DisplayDataSize ( u8 quantFlags )
{
    u8 flag = (u8)QUANT_TYPE(quantFlags);

    switch(flag)
    {
        case GX_U8:
        case GX_S8:     return 1;
        case GX_U16:
        case GX_S16:    return 2;
        case GX_F32:    return 4;
    }
    
    ASSERTMSG(0, "DisplayDataSize: Unknown quantization type");
    return 0;
}

/*>*******************************(*)*******************************<*/
static u16 DisplayDataSizeColor ( u8 quantFlags )
{
    u8 flag = (u8)QUANT_TYPE(quantFlags);

    switch(flag)
    {
        case GX_RGB565:
        case GX_RGBA4:  return 2;
        case GX_RGB8:
        case GX_RGBA6:  return 3;
        case GX_RGBX8:
        case GX_RGBA8:  return 4;
    }

    ASSERTMSG(0, "DisplayDataSizeColor: Unknown quantization type");
    return 0;
}

/*>*******************************(*)*******************************<*/
static void SetDispObjShader    ( DODisplayObjPtr dispObj, u32 setting )
{
    SHDRCompiled *shader;

    //this is a hack to include the SetTevOp API which was the old
    //standard before the addition of shaders
    
    if((!(dispObj->shaderFunc)) && 
       (setting == GX_MODULATE ||
       setting == GX_DECAL ||
       setting == GX_REPLACE ||
       setting == GX_PASSCLR))
    {
        switch(setting)
	    {
		    case GX_MODULATE:
			    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
		 	    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
                GXLoadTexObj(&DOTexObj[0], GX_TEXMAP0);
                GXSetNumTexGens(1);
                GXSetNumTevStages(1);
			    break;
		    case GX_DECAL:
			    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
		 	    GXSetTevOp(GX_TEVSTAGE0, GX_DECAL);
                GXLoadTexObj(&DOTexObj[0], GX_TEXMAP0);
                GXSetNumTexGens(1);
                GXSetNumTevStages(1);
			    break;
		    case GX_REPLACE:
			    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
		 	    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
                GXLoadTexObj(&DOTexObj[0], GX_TEXMAP0);
                GXSetNumTexGens(1);
                GXSetNumTevStages(1);
			    break;
		    case GX_PASSCLR:
			    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL,GX_TEXMAP_NULL,GX_COLOR0A0);
		 	    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
                GXSetNumTexGens(0);
                GXSetNumTevStages(1);
			    break;
	    }
    }

    else
    {
	    switch(setting)
	    {
		    case GX_MODULATE:
            case GX_DECAL:

                SHDRBindTexture(SHDRModulateShader, SHADER_TEX0, &DOTexObj[0]);
                SHDRBindRasterized(SHDRModulateShader, SHADER_RAS0, GX_COLOR0A0);

                shader = SHDRModulateShader;

                CurrentTexObj = &DOTexObj[0];
			    break;
		    case GX_REPLACE:    
                SHDRBindTexture(SHDRReplaceShader, SHADER_TEX0, &DOTexObj[0]);

                shader = SHDRReplaceShader;

                CurrentTexObj = &DOTexObj[0];
			    break;
		    case GX_PASSCLR:
                SHDRBindRasterized(SHDRPassThruShader, SHADER_RAS0, GX_COLOR0A0);

                shader = SHDRPassThruShader;

                CurrentTexObj = 0;
			    break;
	    }

        if(dispObj->shaderFunc)
            dispObj->shaderFunc(shader, dispObj, setting, CurrentTexObj, ColorChanUsed, dispObj->shaderData);
        else
            SHDRExecute(shader);
    }

    LastShader = setting;
    ShaderSet = 1;
}

/*>*******************************(*)*******************************<*/
void    DOSetEffectsShader		( DODisplayObjPtr dispObj, Ptr shaderFunc, Ptr data )
{
	dispObj->shaderFunc = (void *(*) (SHDRCompiled *shader, DODisplayObjPtr dispObj, u32 combineSetting, GXTexObj *texture, BOOL colorChanUsed, Ptr data))shaderFunc;
    dispObj->shaderData = data;
}

/*>*******************************(*)*******************************<*/
void    DOOverrideTextureState  ( DOTextureState state, DOTextureStateSetting setting, float LODValue )
{
    TextureStateOverrides[state] = (u32)setting;

    if(state == DOTS_MIN_LOD) OverrideMinLOD = LODValue;
    else if(state == DOTS_MAX_LOD) OverrideMaxLOD = LODValue;
    else if(state == DOTS_LOD_BIAS) OverrideLODBias = LODValue;
}