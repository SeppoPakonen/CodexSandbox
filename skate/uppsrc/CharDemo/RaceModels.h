/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     raceModels.h

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/demos/charPipeline/include/raceModels.h $
    
    2     8/11/00 10:42a Ryan
    final checkin for spaceworld
    
    1     7/29/00 11:17a Ryan

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#ifndef __RACEMODELS_H__
#define __RACEMODELS_H__

#if 0
extern "C" {
#endif

/*---------------------------------------------------------------------------*/

extern u8 CubeMapDisplay;

/*---------------------------------------------------------------------------*/
void InitModels             ( void );
void CarDraw                ( void );
void SpecularGeometryDraw   ( Mtx m );
void CarAnim                ( void );
void LightInit              ( void );

/*---------------------------------------------------------------------------*/
#if 0
}
#endif

#endif