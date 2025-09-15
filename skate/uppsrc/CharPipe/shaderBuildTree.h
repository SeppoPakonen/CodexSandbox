/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     shaderBuildTree.h

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/shader/include/shaderBuildTree.h $
    
    4     7/19/00 3:59p Ryan
    update for precompiled shaders
    
    3     4/12/00 6:28a Ryan
    Update for color/alpha and texgen shader revision
    
    2     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
    1     2/28/00 11:23a Ryan

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#ifndef __SHADERBUILDTREE_H__
#define __SHADERBUILDTREE_H__

#if 0
extern "C" {
#endif

/*---------------------------------------------------------------------------*/

#include <CharPipe/shader.h>
#include "shaderInternals.h"

/*---------------------------------------------------------------------------*/

BOOL BuildTEVTree ( SHDRShader *rgbShader, SHDRShader *aShader, void **compiledTexGen );

/*---------------------------------------------------------------------------*/
#if 0
}
#endif

#endif