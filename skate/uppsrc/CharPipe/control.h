/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     control.h

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/charPipeline/control.h $
    
    6     11/13/00 1:57p John
    Added CTRLGetType macro.
    
    5     8/04/00 5:13p John
    Added CTRLGetMatrix to get direct access to a matrix without the
    overhead of MTXCopy.
    
    4     7/18/00 7:35p John
    Optimized CTRL library to arithmetically and selectively build
    matrices.
    Changed CTRLSetType to CTRLInit.
    
    3     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
    2     12/08/99 12:16p Ryan
    added function descriptions
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  The control library provides an interface for controlling the translation, 
  scale and orientation of objects using different types of parameters through 
  one common structure.  The control structure can support data specified in 
  Euler angles, quaternions, floating point scale and translation, or a full 
  matrix.  The control structure processes this data and yields a matrix.  This 
  structure is used to control all display objects in the character pipeline.
 *---------------------------------------------------------------------------*/

#ifndef CONTROL_H
#define CONTROL_H

#if 0
extern "C" {
#endif

#include <Dolphin/mtx.h>

/********************************/
// CONSTANTS
#define CTRL_NONE       0x00
#define CTRL_SCALE      0x01
#define CTRL_ROT_EULER  0x02
#define CTRL_ROT_QUAT   0x04
#define CTRL_TRANS      0x08
#define CTRL_MTX        0x10

#define CTRL_MAX_PARAMS 12

/********************************/
// STRUCTURES
typedef struct
{
    Vec        s;
    Quaternion r;       // Also used for euler XYZ parameters
    Vec        t;

}CTRLSRTControl, *CTRLSRTControlPtr;

typedef struct
{
    Mtx        m;

}CTRLMTXControl, *CTRLMTXControlPtr;

typedef struct
{
    u8  type;
    u8  pad8;
    u16 pad16;

    union
    {
        CTRLSRTControl  srt;
        CTRLMTXControl  mtx; 

    } controlParams;

}CTRLControl, *CTRLControlPtr;

/*>*******************************(*)*******************************<*/
// MACROS

/*---------------------------------------------------------------------*

Name:           CTRLInit

Description:    Initializes the control.  Should be called before
                using CTRLSetScale, CTRLSetRotEuler, CTRLSetRotQuat,
                or CTRLSetTranslation for the first time for the 
                given control.

Arguments:      controlPtr - pointer to the control

Return:         none

*---------------------------------------------------------------------*/
#define CTRLInit(controlPtr) ( (controlPtr)->type = CTRL_NONE )

/*---------------------------------------------------------------------*

Name:           CTRLDisable

Description:    Useful to disable a control type so that scale, rotation,
                or translation will not be included when building the 
                control with CTRLBuildMatrix.  The argument controlType
                can be a combination of control types.

Arguments:      controlPtr  - pointer to the control
                controlType - type of control to disable.  

Return:         none

*---------------------------------------------------------------------*/
#define CTRLDisable(controlPtr, controlType) \
                             ( (controlPtr)->type &= ~(controlType) )

/*---------------------------------------------------------------------*

Name:           CTRLEnable

Description:    Useful to REenable a control type that was previously
                set with one of the CTRLSet* functions and then disabled
                with CTRLDisable.  Caution must be taken to make sure 
                that the proper control type is reenabled 
                (1. Matrix 2. Scale, rotation, and/or translation).

Arguments:      controlPtr  - pointer to the control
                controlType - type of control to enable.  

Return:         none

*---------------------------------------------------------------------*/
#define CTRLEnable(controlPtr, controlType) \
                             ( (controlPtr)->type |= (controlType) )

/*---------------------------------------------------------------------*

Name:           CTRLGetMtx

Description:    Directly sets mtxPtr to the matrix inside the control.  
                In  a debug build, asserts to make sure the type is CTRL_MTX.
                Same as CTRLBuildMatrix for types of controls which are
                CTRL_MTX, except this bypasses a MTXCopy.

Arguments:      controlPtr - pointer to the control
                mtxPtr     - MtxPtr to set

Return:         none

*---------------------------------------------------------------------*/
#define CTRLGetMtx(controlPtr, mtxPtr) \
                               ASSERTMSG( (controlPtr)->type == CTRL_MTX, "CTRLGetMtx: Not a matrix type" ); \
                               mtxPtr = (controlPtr)->controlParams.mtx.m

/*---------------------------------------------------------------------*

Name:           CTRLGetType

Description:    Returns the type of the control

Arguments:      controlPtr - pointer to the control

Return:         Control type (combination of the CTRL_* constants)

*---------------------------------------------------------------------*/
#define CTRLGetType(controlPtr) \
                             ( (controlPtr)->type )

/*>*******************************(*)*******************************<*/
// FUNCTION PROTOTYPES

void CTRLSetScale            ( CTRLControlPtr control, float x, float y, float z );
void CTRLSetRotation         ( CTRLControlPtr control, float x, float y, float z );
void CTRLSetQuat             ( CTRLControlPtr control, float x, float y, float z, float w );
void CTRLSetTranslation      ( CTRLControlPtr control, float x, float y, float z );
void CTRLSetMatrix           ( CTRLControlPtr control, Mtx m );

void CTRLGetScale            ( CTRLControlPtr control, float *x, float *y, float *z );
void CTRLGetRotation         ( CTRLControlPtr control, float *x, float *y, float *z );
void CTRLGetQuat             ( CTRLControlPtr control, float *x, float *y, float *z, float *w );
void CTRLGetTranslation      ( CTRLControlPtr control, float *x, float *y, float *z );

void CTRLBuildMatrix         ( CTRLControlPtr control, Mtx m );
void CTRLBuildInverseMatrix  ( CTRLControlPtr control, Mtx m );

#if 0
}
#endif

#endif