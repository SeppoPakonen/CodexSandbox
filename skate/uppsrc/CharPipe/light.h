/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     light.h

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/charPipeline/light.h $
    
    3     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
    2     12/08/99 12:16p Ryan
    added function descriptions
    
    1     10/11/99 12:59p Ryan
    

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  The light library provides methods for creating and managing vertex lights 
  in a scene.
 *---------------------------------------------------------------------------*/

#ifndef __LIGHT_H__
#define __LIGHT_H__

#if 0
extern "C" {
#endif

/*---------------------------------------------------------------------------*/

#include <Dolphin/gx.h>
#include <CharPipe/anim.h>
#include <CharPipe/actor.h>
#include <CharPipe/GeoPalette.h>
#include <Dolphin/mtx.h>

/*---------------------------------------------------------------------------*/
typedef enum
{
	PARENT_BONE,
	PARENT_DISP_OBJ,
	PARENT_MTX

} CPParentType;

/*---------------------------------------------------------------------------*/
typedef struct 
{
	GXLightObj	lt_obj;
	
	Vec			position;
	Vec			worldPosition;

	Vec			direction;
	Vec			worldDirection;

	GXColor		color;

	CTRLControl control;
	MtxPtr		parent;

	ANIMPipePtr	animPipe;

} LITLight, *LITLightPtr;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  LITAlloc - allocates and initializes a light structure.
 *---------------------------------------------------------------------------*/
void LITAlloc		( LITLightPtr *light );

/*---------------------------------------------------------------------------*
  LITFree - frees a light structure.
 *---------------------------------------------------------------------------*/
void LITFree		( LITLightPtr *light );

/*---------------------------------------------------------------------------*
  LITInitAttn - sets the attenuation values for a light
 *---------------------------------------------------------------------------*/
void LITInitAttn(
         LITLightPtr   	light,
         f32          	a0,
         f32           	a1,
         f32           	a2,
         f32           	k0,
         f32           	k1,
         f32           	k2 );

/*---------------------------------------------------------------------------*
  LITInittSpot - Sets the spotlight function for a light.
 *---------------------------------------------------------------------------*/
void LITInitSpot(
         LITLightPtr   	light,
         f32           	cutoff,
         GXSpotFn      	spot_func );

/*---------------------------------------------------------------------------*
  LITInitDistAttn - Sets the distance attenuation function for a light.
 *---------------------------------------------------------------------------*/
void LITInitDistAttn(
         LITLightPtr   	light,
         f32           	ref_distance,
         f32           	ref_brightness,
         GXDistAttnFn  	dist_func );

/*---------------------------------------------------------------------------*
  LITInitPos - Sets the local space position of a light.
 *---------------------------------------------------------------------------*/
void LITInitPos 	( LITLightPtr light, f32 x, f32 y, f32 z );

/*---------------------------------------------------------------------------*
  LITInitDir - Sets the local space direction of a light.
 *---------------------------------------------------------------------------*/
void LITInitDir 	( LITLightPtr light, f32 nx, f32 ny, f32 nz );

/*---------------------------------------------------------------------------*
  LITInitColor - Sets the color of a light. 
 *---------------------------------------------------------------------------*/
void LITInitColor   ( LITLightPtr light, GXColor color );

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  LITXForm - Transforms the local position and direction of a light by the 
  light matrix and the view matrix to bring it into view space where lighting 
  is performed.
 *---------------------------------------------------------------------------*/
void LITXForm       ( LITLightPtr light, Mtx view );

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  LITAttach - Attaches a light to either a bone of an Actor, a display 
  object, or a matrix.  When attached, the light will act as a hierarchical 
  child of the parent it is attached to.
 *---------------------------------------------------------------------------*/
void LITAttach	    ( LITLightPtr light, Ptr parent, CPParentType type );

/*---------------------------------------------------------------------------*
  LITDetach - Removes a light from a previously-defined attachment.
 *---------------------------------------------------------------------------*/
void LITDetach	    ( LITLightPtr light );

/*---------------------------------------------------------------------------*
  LITGetControl - returns a pointer to a light's control structure.
 *---------------------------------------------------------------------------*/
CTRLControlPtr	LITGetControl	( LITLightPtr light );

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  LITSetAnimSequence - not implemented.
 *---------------------------------------------------------------------------*/
void LITSetAnimSequence	( LITLightPtr light, ANIMBankPtr animBank, char *seqName, float time );

/*---------------------------------------------------------------------------*
  LITTick - not implemented.
 *---------------------------------------------------------------------------*/
void LITTick    		( LITLightPtr light );

/*---------------------------------------------------------------------------*/
#if 0
}
#endif

#endif  // __LIGHT_H__