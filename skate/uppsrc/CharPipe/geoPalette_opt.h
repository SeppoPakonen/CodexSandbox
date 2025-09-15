/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     GeoPalette.h

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/charPipeline/GeoPalette.h $
    
    13    12/04/00 7:58p John
    Added multitexturing ability (work in progress).
    Changed GPL version number.
    
    12    12/05/00 12:20p Ryan
    Added new texture state override functionality.
    
    11    9/16/00 12:27p Ryan
    shader optimization update
    
    10    8/14/00 6:22p John
    Added user-defined data to ANM, ACT, and GPL formats.
    
    9     8/04/00 5:13p John
    Eliminated screenMatrix and inverseTransposeMatrix fields.
    
    8     7/19/00 4:20p Ryan
    update to work with precompiled shaders
    
    7     4/26/00 11:39p Ryan
    update for shader implementation
    
    6     4/12/00 6:28a Ryan
    Update for color/alpha and texgen shader revision
    
    5     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
    4     3/06/00 2:41p Ryan
    temporary update to set framework for shader lib
    
    3     1/20/00 4:17p Ryan
    update to add stitching functionality
    
    2     12/08/99 12:16p Ryan
    added function descriptions
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  The geoPalette library provides an interface between the character pipeline
  and .gpl files on disk.  Also, it provides methods to display and manipulate
  display objects which have been extracted from a .gpl file.
 *---------------------------------------------------------------------------*/

#ifndef GEOPALETTE_H
#define GEOPALETTE_H

/********************************/
#if 0
extern "C" {
#endif

/********************************/
#include <stdio.h>
#include <stdarg.h>

#include <Dolphin/os.h>
#include <Dolphin/mtx.h>
#include <CharPipe/fileCache.h>
#include <CharPipe/displayObjectInternals.h>
#include <CharPipe/shader.h>

/********************************/
#define GPL_VERSION     12012000
    
/********************************/
typedef struct 
{
    u32                 versionNumber;

    u32                 userDataSize;
    void*               userData;    

    u32                 numDescriptors;
    GEODescriptorPtr    descriptorArray;

} GEOPalette, *GEOPalettePtr;

/********************************/
typedef struct DODisplayObj
{
    DOPositionHeaderPtr     positionData;
    DOColorHeaderPtr        colorData;
    DOTextureDataHeaderPtr  textureData;
    DOLightingHeaderPtr     lightingData;
    DODisplayHeaderPtr      displayData;
    
    u8                      numTextureChannels;
    u8                      pad8;
    u16                     pad16;
    /////////////////////////////////////////////////
    BOOL                    visibility; 

    Mtx                     worldMatrix;

    void*                   (*shaderFunc)(SHDRCompiled *shader, struct DODisplayObj *dispObj, u32 combineSetting, GXTexObj *texture, BOOL colorChanUsed, Ptr data);
    void*                   shaderData;

#if defined(flagWIN32) && defined(OPENGL_OPTIMIZE)
	u32 displayList;
	u32 displayInit;
#endif

} DODisplayObj, *DODisplayObjPtr;

/********************************/
typedef enum
{
    DOTS_WRAP_S = 0, 
    DOTS_WRAP_T = 1, 
    DOTS_MIPMAP = 2, 
    DOTS_MIN_FILTER = 3,
    DOTS_MAG_FILTER = 4,
    DOTS_MIN_LOD = 5, 
    DOTS_MAX_LOD = 6,
    DOTS_LOD_BIAS = 7,
    DOTS_BIAS_CLAMP = 8,
    DOTS_EDGE_LOD = 9, 
    DOTS_MAX_ANISO = 10

} DOTextureState;

/********************************/
typedef enum
{
    DOTS_USEDEFAULT = 0xFFFFFFFF,

    DOTS_CLAMP = GX_CLAMP,
    DOTS_REPEAT = GX_REPEAT,
    DOTS_MIRROR = GX_MIRROR,
    
    DOTS_MIPMAP_ON = GX_TRUE,
    DOTS_MIPMAP_OFF = GX_FALSE,

    DOTS_NEAR = GX_NEAR,
    DOTS_LINEAR = GX_LINEAR,
    DOTS_NEAR_MIP_NEAR = GX_NEAR_MIP_NEAR,
    DOTS_LIN_MIP_NEAR = GX_LIN_MIP_NEAR,
    DOTS_NEAR_MIP_LIN = GX_NEAR_MIP_LIN,
    DOTS_LIN_MIP_LIN = GX_LIN_MIP_LIN,

    DOTS_LOD_SET = 0,

    DOTS_BIAS_CLAMP_ON = GX_TRUE,
    DOTS_BIAS_CLAMP_OFF = GX_FALSE,

    DOTS_EDGE_LOD_ON = GX_TRUE,
    DOTS_EDGE_LOD_OFF = GX_FALSE,

    DOTS_ANISO_1 = GX_ANISO_1,
    DOTS_ANISO_2 = GX_ANISO_2,
    DOTS_ANISO_4 = GX_ANISO_4 

} DOTextureStateSetting;

/*>*******************************(*)*******************************<*/
/*---------------------------------------------------------------------------*
  GEOGetGeoPalette - Loads and unpacks the specified .gpl file.  This system
  loads files from disk in a cached manner if the display cache has been 
  turned on with a call to the DOInitDisplayCache function.
 *---------------------------------------------------------------------------*/
void    GEOGetPalette           ( GEOPalettePtr *pal, char *name );

/*---------------------------------------------------------------------------*
  GEOReleaseGeoPalette - If the display cache is initialized, this function will
  decrement the reference count of the specified GeoPalette and free its memory 
  if the reference count is 0.  If the display cache is not initialized, the 
  specified GeoPalette is simply freed.
 *---------------------------------------------------------------------------*/
void    GEOReleasePalette       ( GEOPalettePtr *pal );

/*---------------------------------------------------------------------------*
  GEOGetUserDataSize - Returns the size in bytes of the user-defined data.
 *---------------------------------------------------------------------------*/
u32     GEOGetUserDataSize      ( GEOPalettePtr pal );

/*---------------------------------------------------------------------------*
  GEOGetUserData - Returns a pointer to the user-defined data.
 *---------------------------------------------------------------------------*/
Ptr     GEOGetUserData          ( GEOPalettePtr pal );

/*---------------------------------------------------------------------------*
  DOGet - Initializes the given display object structure with 
  unpacked data from the GeoPalette specified.  It is important to note that
  most of the data in the display object is instanced from the GeoPalette and
  therefore the GeoPalette cannot be releasedwhile the display object is still 
  in use.
 *---------------------------------------------------------------------------*/
void    DOGet                   ( DODisplayObjPtr *dispObj, GEOPalettePtr pal, 
                                  u16 id, char *name );

/*>*******************************(*)*******************************<*/
/*---------------------------------------------------------------------------*
  DORelease - Frees all memory allocated to the specified display object.  
  This does not affect the memory allocated to the corresponding GeoPalette.
 *---------------------------------------------------------------------------*/
void    DORelease               ( DODisplayObjPtr *dispObj );

/*>*******************************(*)*******************************<*/
/*---------------------------------------------------------------------------*
  DOGetWorldMatrix - Returns the world matrix associated with the specified
  display object.
 *---------------------------------------------------------------------------*/
MtxPtr  DOGetWorldMatrix        ( DODisplayObjPtr dispObj );

/*---------------------------------------------------------------------------*
  DOHide - Sets the display object's display flag to DO_DISPLAY_OFF so that
  the display object will not render.
 *---------------------------------------------------------------------------*/
void    DOHide                  ( DODisplayObjPtr dispObj );

/*---------------------------------------------------------------------------*
  DORender - Renders the display object to the frame buffer.  If any lights 
  are passed down, the object will be lit as required by them.
 *---------------------------------------------------------------------------*/
void    DORender                ( DODisplayObjPtr dispObj, Mtx camera, 
                                  u8 numLights, ... );

/*---------------------------------------------------------------------------*
  DOVARender - utility function to be called by ACTRender in the actor 
  library.  This performs the same function as DORender.
 *---------------------------------------------------------------------------*/
void    DOVARender              ( DODisplayObjPtr dispObj, Mtx camera, 
                                  u8 numLights, va_list list );

/*---------------------------------------------------------------------------*
  DORenderSkin - Renders a skinned display object to the frame buffer.  If any 
  lights are passed down, the object will be lit as required by them.  This 
  routine uses the specified matrix arrays to transform the points correctly 
  in the skinned mesh.
 *---------------------------------------------------------------------------*/
void    DORenderSkin            ( DODisplayObjPtr dispObj, Mtx camera, 
                                  MtxPtr mtxArray, MtxPtr invTransposeMtxArray, 
                                  u8 numLights, ... );

/*---------------------------------------------------------------------------*
  DOVARender - utility function to be called by ACTRender in the actor 
  library.  This performs the same function as DORenderSkin.
 *---------------------------------------------------------------------------*/
void	DOVARenderSkin		    ( DODisplayObjPtr dispObj, Mtx camera, 
						    	  MtxPtr mtxArray, MtxPtr invTransposeMtxArray, 
						    	  u8 numLights, va_list list );

/*---------------------------------------------------------------------------*
  DOSetWorldMatrix - Sets the World matrix of the specified display object.
 *---------------------------------------------------------------------------*/
void    DOSetWorldMatrix        ( DODisplayObjPtr dispObj, Mtx m );

/*---------------------------------------------------------------------------*
  DOShow - Sets the display object's display flag to DO_DISPLAY_ON so that
  the display object will render.
 *---------------------------------------------------------------------------*/
void    DOShow                  ( DODisplayObjPtr dispObj );

/*---------------------------------------------------------------------------*
  DOSetAmbientPercentage - Sets the percentage of ambient light present on 
  the object.  For display objects without lighting this percentage has no 
  effect.  The percentage is specified between 0.0F and 100.0F.  This function 
  returns the previous value for this setting.
 *---------------------------------------------------------------------------*/
f32     DOSetAmbientPercentage  ( DODisplayObjPtr dispObj, f32 percent );

/*---------------------------------------------------------------------------*
  DOSetEffectsShader - Sets a Shader callback function for the color and Alpha
  channels to be used to properly display the specified Display Object.
 *---------------------------------------------------------------------------*/
void    DOSetEffectsShader      ( DODisplayObjPtr dispObj, Ptr shaderFunc, Ptr data );

/*---------------------------------------------------------------------------*
  DOOverrideTextureState - Overrides default texture state settings in the
  character pipeline.
 *---------------------------------------------------------------------------*/
void    DOOverrideTextureState  ( DOTextureState state, DOTextureStateSetting setting, float LODValue );

/********************************/
#if 0
}
#endif

#if defined(flagWIN32) && defined(OPENGL_OPTIMIZE)
#define OPENGL_STATE_NOT_OPTIMIZED 0
#define OPENGL_STATE_OPTIMIZING 1
#define OPENGL_STATE_OPTIMIZED 2
#endif

#endif