/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     raceCamera.h

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/demos/charPipeline/include/raceCamera.h $
    
    3     8/11/00 10:42a Ryan
    final checkin for spaceworld
    
    2     8/08/00 12:30p Ryan
    update to fix errors/warnings
    
    1     7/29/00 11:17a Ryan
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#ifndef __RACECAMERA_H__
#define __RACECAMERA_H__

#if 0
extern "C" {
#endif

/****************************************************************************/
typedef struct
{
    Vec x, y, z;

    float scale;

} CameraKeyframe;

/*---------------------------------------------------------------------------*/

#define NUM_CAMERA_KEYFRAMES    5

/*---------------------------------------------------------------------------*/
extern Mtx cameraMatrix;

extern Vec CamX;	
extern Vec CamY;
extern Vec CamZ;
extern float CameraLocScale;

extern BOOL VideoModeAA;

extern CameraKeyframe CameraKeyArray[NUM_CAMERA_KEYFRAMES];

/*---------------------------------------------------------------------------*/
void CameraInit             ( void );
void SetMainCamera 		    ( void );
void SetMainCameraTop       ( void );
void SetMainCameraBottom    ( void );
void CopyBottom             ( void );
void CameraAnim             ( void );
void SetUpAA                ( void );
void SetUpIntDF             ( void );

/*---------------------------------------------------------------------------*/
#if 0
}
#endif

#endif