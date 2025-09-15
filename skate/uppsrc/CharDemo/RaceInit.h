/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     raceInit.h

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/demos/charPipeline/include/raceInit.h $
    
    1     7/29/00 11:17a Ryan

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#ifndef __RACEINIT_H__
#define __RACEINIT_H__

#if 0
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
void RaceInit ( void );

void InitTextureMap ( void **data, u16 width, u16 height, GXTexObj *texObj, GXTexFmt fmt );

/*---------------------------------------------------------------------------*/
#if 0
}
#endif

#endif