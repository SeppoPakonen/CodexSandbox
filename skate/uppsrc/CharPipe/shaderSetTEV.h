/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     shaderSetTEV.h

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/shader/include/shaderSetTEV.h $
    
    2     7/19/00 3:59p Ryan
    update for precompiled shaders
    
    1     3/06/00 2:32p Ryan

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#ifndef __SHADERSETTEV_H__
#define __SHADERSETTEV_H__

#if 0
extern "C" {
#endif

/*---------------------------------------------------------------------------*/

#include <CharPipe/shader.h>
#include "shaderInternals.h"

/*---------------------------------------------------------------------------*/
void SetTEV	( SHDRCompiled *shader );

/*---------------------------------------------------------------------------*/
#if 0
}
#endif

#endif