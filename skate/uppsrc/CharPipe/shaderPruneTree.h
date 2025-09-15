/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     shaderPruneTree.h

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/shader/include/shaderPruneTree.h $
    
    2     6/07/00 1:09p Ryan
    checkin for shader optimization v1.0
    
    1     2/28/00 11:23a Ryan

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#ifndef __SHADERPRUNETREE_H__
#define __SHADERPRUNETREE_H__

#if 0
extern "C" {
#endif

/*---------------------------------------------------------------------------*/

#include <CharPipe/shader.h>
#include "shaderInternals.h"

/*---------------------------------------------------------------------------*/
extern ShaderTEVStage	CollapsedTEVPool[33];
extern u8               OptimalTreeSize;

/*---------------------------------------------------------------------------*/
void PruneTEVTree	( void );

/*---------------------------------------------------------------------------*/
#if 0
}
#endif

#endif