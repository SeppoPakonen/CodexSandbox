/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     raceSphereMap.h

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/demos/charPipeline/include/raceSphereMap.h $
    
    2     8/08/00 12:30p Ryan
    update to fix errors/warnings
    
    1     7/29/00 11:18a Ryan

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#ifndef __RACESPHEREMAP_H__
#define __RACESPHEREMAP_H__

#if 0
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
extern GXTexObj SphereMap;

extern GXRenderModeObj *currentRenderMode;

/*---------------------------------------------------------------------------*/
void InitSphereMap      ( void );
void DrawSphereMap      ( void );
void RenderCubeMaps     ( void );
void DisplaySphereMap   ( void );

/*---------------------------------------------------------------------------*/
#if 0
}
#endif

#endif