/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     actor.h

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/charPipeline/actor.h $
    
    12    8/14/00 6:22p John
    Added user-defined data to ANM, ACT, and GPL formats.
    
    11    8/04/00 5:12p John
    Added orientationInvMtx and renamed some fields.
    
    10    7/19/00 4:19p Ryan
    update to work with precompiled shaders
    
    9     4/12/00 6:28a Ryan
    Update for color/alpha and texgen shader revision
    
    8     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
    7     3/14/00 1:34p Ryan
    update for display priority move to actor
    
    6     3/08/00 4:12p Ryan
    update to fix csim VAList bug
    
    5     1/25/00 1:21p Ryan
    removed pivot point and change actor format to 01252000
    
    4     1/20/00 4:17p Ryan
    update to add stitching functionality
    
    3     12/15/99 12:05p Ryan
    Added version number and checking
    
    2     12/08/99 12:16p Ryan
    added function descriptions

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  The actor library provides routines for loading and unpacking .act files from
  disk.  Also, it provides methods to display and manipulate entire 
  hierarchies that have been extracted from a .act file.
 *---------------------------------------------------------------------------*/

#ifndef ACTOR_H
#define ACTOR_H

/********************************/
#if 0
extern "C" {
#endif

/********************************/
#include <stdio.h>
#include <stdarg.h>

#include <CharPipe/GeoPalette.h>
#include <CharPipe/control.h>
#include <CharPipe/anim.h>

/********************************/
#define ACT_VERSION             8092000

#define ACT_INHERIT_NONE        0
#define ACT_INHERIT_ALL         1

#define ACT_NO_DISPLAY_OBJECT   0xFFFF

/********************************/
typedef struct  
{
    CTRLControlPtr  orientationCtrl;

    DSBranch        branch;

    u16             geoFileID;
    
    u16             boneID;

    u8              inheritanceFlag;
    u8              drawingPriority;
    u16             pad16;

} ACTBoneLayout, *ACTBoneLayoutPtr;

/********************************/
typedef struct 
{
    u32             versionNumber;

    u16             actorID;
    u16             totalBones;

    DSTree          hierarchy;

    char*           geoPaletteName;

    u16             skinFileID;
    u16             pad16;

    u32             userDataSize;
    Ptr             userData;

} ACTLayout, *ACTLayoutPtr;

/********************************/
typedef struct 
{
    u16             boneID;

    u8              inheritanceFlag;
    u8              drawingPriority;

    DSBranch        branch;

    DODisplayObjPtr dispObj;

    CTRLControl     orientationCtrl;
    CTRLControl     animationCtrl;

    ANIMPipePtr     animPipe;

    MtxPtr          forwardMtx;
    MtxPtr          skinMtx;
    MtxPtr          skinInvTransposeMtx;    // For stitching world space lighting
    MtxPtr          orientationInvMtx;

    DSLink          drawPriorityLink;

} ACTBone, *ACTBonePtr;

/********************************/
typedef struct 
{
    ACTLayoutPtr    layout;

    u16             actorID;
    u16             totalBones;

    DSTree          hierarchy;

    GEOPalettePtr   pal;

    DODisplayObjPtr skinObject;

    ACTBonePtr*     boneArray;

    CTRLControl     worldControl;

    MtxPtr          forwardMtxArray;
    MtxPtr          skinMtxArray;
    MtxPtr          skinInvTransposeMtxArray;
    MtxPtr          orientationInvMtxArray;

    DSList          drawPriorityList;

} ACTActor, *ACTActorPtr;

/*>*******************************(*)*******************************<*/
/*---------------------------------------------------------------------------*
  ACTGet - Loads and unpacks the specified .act file.  This system
  loads files from disk in a cached manner if the display cache has been 
  turned on with a call to the DOInitDisplayCache function.
 *---------------------------------------------------------------------------*/
void    ACTGet                  ( ACTActorPtr *actor, char *name ); 

/*---------------------------------------------------------------------------*
  ACTRelease - If the display cache is initialized, this function will
  decrement the reference count of the specified actor and free its memory 
  if the reference count is 0.  If the display cache is not initialized, the 
  specified actor is simply freed.
 *---------------------------------------------------------------------------*/
void    ACTRelease              ( ACTActorPtr *actor );

/*>*******************************(*)*******************************<*/
/*---------------------------------------------------------------------------*
  ACTRender - Renders the actor to the frame buffer.  If any lights 
  are passed down, the object will be lit as required by them.
 *---------------------------------------------------------------------------*/
void    ACTRender               ( ACTActorPtr actor, Mtx camera, 
                                  u8 numLights, ... );

/*---------------------------------------------------------------------------*
  ACTHide - Traverses the actor's hierarchy and sets each display object's 
  display flag to DO_DISPLAY_OFF so that every display object in the hierarchy 
  will not render.
 *---------------------------------------------------------------------------*/
void    ACTHide                 ( ACTActorPtr actor );

/*---------------------------------------------------------------------------*
  ACTShow - Traverses the actor's hierarchy and sets each display object's 
  display flag to DO_DISPLAY_ON so that every display object in the hierarchy 
  will render.
 *---------------------------------------------------------------------------*/
void    ACTShow                 ( ACTActorPtr actor );

/*---------------------------------------------------------------------------*
  ACTBuildMatrices - Traverses the hierarchy and builds each display object's 
  world matrix based on the information set in the actor.
 *---------------------------------------------------------------------------*/
void    ACTBuildMatrices        ( ACTActorPtr actor );

/*---------------------------------------------------------------------------*
  ACTGetControl - Returns a pointer to the global control for the actor.  The 
  matrix specified by this control is inherited by each display object in the 
  hierarchy regardless of the setting of the actor's inheritance flag.
 *---------------------------------------------------------------------------*/
CTRLControlPtr  ACTGetControl   ( ACTActorPtr actor );

/*---------------------------------------------------------------------------*
  ACTSetInheritance - Sets the inheritance flag of an actor.  A setting of
  ACT_INHERIT_ALL causes each bone's matrix to be concatenated with all the 
  matrices of it's parents and the actor's global matrix.A setting of 
  ACT_INHERIT_NONE causes each bone's matrix to be concatenated only with the 
  actor's global matrix.
 *---------------------------------------------------------------------------*/
void    ACTSetInheritance       ( ACTActorPtr actor, u8 inheritanceFlag );

/*---------------------------------------------------------------------------*
  ACTSetAmbientPercentage - Traverses the hierarchy and sets each display 
  object's ambient percentage to the specified.
 *---------------------------------------------------------------------------*/
void    ACTSetAmbientPercentage ( ACTActorPtr actor, f32 percent );

/*---------------------------------------------------------------------------*
  ACTSort - Traverses the hierarchy and produces a list of bones sorted in
  display priority order.  This function is called when an actor is initially
  unpacked, but it must be called whenever any objects are either added to or
  deleted from an actor.
 *---------------------------------------------------------------------------*/
void    ACTSort                 ( ACTActorPtr actor );

/*---------------------------------------------------------------------------*
  ACTSetEffectsShader - Sets Shader callback functions to be used to properly
  display the specified Actor.
 *---------------------------------------------------------------------------*/
void    ACTSetEffectsShader     ( ACTActorPtr actor, Ptr shaderFunc, Ptr data );

/*---------------------------------------------------------------------------*
  ACTGetUserDataSize - Returns the size in bytes of the user-defined data.
 *---------------------------------------------------------------------------*/
u32     ACTGetUserDataSize      ( ACTActorPtr actor );

/*---------------------------------------------------------------------------*
  ACTGetUserData - Returns a pointer in the layout to the user-defined data.
 *---------------------------------------------------------------------------*/
Ptr     ACTGetUserData          ( ACTActorPtr actor );

/********************************/
#include <CharPipe/actorAnim.h>

/********************************/
#if 0
}
#endif

#endif