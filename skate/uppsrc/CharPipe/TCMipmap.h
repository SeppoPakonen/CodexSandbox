/*---------------------------------------------------------------------*

Project:  tc library
File:     TCMipmap.h

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:

   $Log: /Dolphin/build/charPipeline/tc/include/TCMipmap.h $
    
    1     12/03/99 3:44p Ryan
    
    2     11/23/99 2:51p Mikepc
    changed CreateNextMipMapLyer, BoxFilter prototypes to include alpha
    layer when mipmapping color layer
    
    14    10/08/99 2:44p Mikepc
    update for tplConv portability: altered data structures, replaced
    references to 'read tga code' with ' *fileFn, removed redundant
    functions.
    
    13    10/01/99 1:27p Mikepc
    changed CreateNextMipMapLayer() prototype- 'level' param was redundant.
    
    12    10/01/99 12:16p Mikepc
    changed CreateNextMipmapLayer, BoxFilter prototypes.
    
    11    9/22/99 3:36p Mikepc
    
    10    9/16/99 8:48p Mikepc
    
    9     8/26/99 4:56p Mikepc
    added namespace protection to remove potential name collisions with
    tool code.  Exceptions are CreateTplFile and QuickConvert.  These are
    extern "C" linked.
    
    8     8/26/99 11:37a Mikepc
    
    7     8/26/99 11:01a Mikepc
    tplConv rewrite for memory usage efficiency, batch file processing
    ability.
    
  
  $NoKeywords: $

-----------------------------------------------------------------------*/

#ifndef __TCMIPMAP_H__
#define __TCMIPMAP_H__

/********************************/
#include <CharPipe/TCLayer.h>

#include "TCImageList.h"

/*>*******************************(*)*******************************<*/
u32	 TCWriteTplImageMipMaps	( FILE* fp, TCImage* thisImage );
u32  TCComputeTplMipMapImageBufferSize	( TCImage* thisImage );

/*>*******************************(*)*******************************<*/

#endif // __TCMIPMAP_H__

