/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     raceShaders.h

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/demos/charPipeline/include/raceShaders.h $
    
    1     7/29/00 11:18a Ryan

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#ifndef __RACESHADERS_H__
#define __RACESHADERS_H__

#if 0
extern "C" {
#endif


/*---------------------------------------------------------------------------*/
extern SHDRCompiled *CarShader_REF_DIFF;
extern SHDRCompiled *CarShader_DIFF;
extern SHDRCompiled *CarShader_GLOSS;
extern SHDRCompiled *CarShader_REF;
extern SHDRCompiled *CarShader;

/*---------------------------------------------------------------------------*/
void InitShaders ( void );

/*---------------------------------------------------------------------------*/
#if 0
}
#endif

#endif