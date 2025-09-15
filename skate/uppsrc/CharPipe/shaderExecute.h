/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     shaderExecute.h

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/shader/include/shaderExecute.h $
    
    1     7/19/00 4:03p Ryan

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#ifndef __SHADEREXECUTE_H__
#define __SHADEREXECUTE_H__

#if 0
extern "C" {
#endif



/*---------------------------------------------------------------------------*/

void CheckShaderBindings    ( SHDRCompiled *shader );
void CombineTEVStages  ( SHDRCompiled *shader );

/*---------------------------------------------------------------------------*/
#if 0
}
#endif

#endif  