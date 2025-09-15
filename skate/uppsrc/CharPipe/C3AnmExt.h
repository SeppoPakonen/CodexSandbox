/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3AnmExt.h]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/include/charPipeline/c3/C3AnmExt.h $
    
    10    11/13/00 1:58p John
    C3BeginAnimation no longer has to be extracted within
    C3BeginHierarchyNode/C3EndHierarchyNode context.
    C3 library now extracts Euler XYZ rotation (including Bezier
    interpolation).
    
    9     8/14/00 6:13p John
    Added user-defined data to ANM, ACT, and GPL formats.
    
    8     7/18/00 7:36p John
    Removed Euler XYZ rotation extraction code (unused and unworking).
    
    7     2/15/00 8:07p John
    Changed version date to omit preceding 0 so that it is not confused as
    an octal number.
    
    6     2/04/00 6:05p John
    Untabified code.
    
    5     1/07/00 11:37a John
    Fixed bug with ANM format
    
    4     12/15/99 1:19p John
    Added ANM version information.
    
    3     12/08/99 7:22p John
    Commented out more Euler angle code.
    Cleaned up the way track transform was passed.
    
    2     11/22/99 6:31p Ryan
    
    8     11/11/99 1:09p John
    New animation format.  Added linear, bezier, slerp, and squad
    extraction.
    
    7     10/26/99 2:44p John
    Adding animation (work in progress).
    
    6     9/29/99 4:23p John
    Changed header to make them all uniform with logs.
    
  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef _C3ANMEXT_H
#define _C3ANMEXT_H


#if 0
extern  "C" {
#endif //__cplusplus

#define C3_ANM_VERSION              8092000   // Must be compatible with Previewer

#define C3_INTERPTYPE_NONE          0x00
#define C3_INTERPTYPE_LINEAR        0x01
#define C3_INTERPTYPE_BEZIER        0x02
#define C3_INTERPTYPE_HERMITE       0x03
#define C3_INTERPTYPE_SQUAD         0x04    
#define C3_INTERPTYPE_SQUADEE       0x05
#define C3_INTERPTYPE_SLERP         0x06

void    C3BeginAnimation            ( char* hierNodeName );
void    C3EndAnimation              ( void );

void    C3BeginTrack                ( char* sequenceName );
void    C3SetStartTime              ( float time );
void    C3SetEndTime                ( float time );
void    C3SetInterpTypeTranslation  ( u8 interpType ); 
void    C3SetInterpTypeScale        ( u8 interpType ); 
void    C3SetInterpTypeRotation     ( u8 interpType );
void    C3EndTrack                  ( void );

void    C3BeginKeyFrame             ( float time );
void    C3SetKeyTranslation         ( float x, float y, float z );
void    C3SetKeyScale               ( float x, float y, float z );
void    C3SetKeyRotationQuat        ( float x, float y, float z, float w );
void    C3SetKeyRotationEuler       ( float x, float y, float z );
void    C3SetKeyMatrix              ( MtxPtr mtx );
void    C3SetKeyInControl           ( u8 controlType, float x, float y, float z, float w );
void    C3SetKeyOutControl          ( u8 controlType, float x, float y, float z, float w );
void    C3SetKeyTCB                 ( u8 controlType, float tension, float continuity, float bias );
void    C3SetKeyEase                ( u8 controlType, float easeIn, float easeOut );
void    C3EndKeyFrame               ( void );
 
void    C3SetANMUserData            ( u32 size, void *data );
void    C3GetANMUserData            ( u32 *size, void **data );

#if 0
}
#endif //__cplusplus

#endif // _C3ANMEXT_H

