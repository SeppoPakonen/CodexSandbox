/*---------------------------------------------------------------------*

Project:  tc library
File:     TCLayerInternal.h

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:

   $Log: /Dolphin/build/charPipeline/tc/include/TCLayerInternal.h $
    
    1     12/03/99 3:44p Ryan
    
    9     10/08/99 2:44p Mikepc
    update for tplConv portability: altered data structures, replaced
    references to 'read tga code' with ' *fileFn, removed redundant
    functions.
    
    8     9/16/99 8:48p Mikepc
    
    7     8/26/99 4:56p Mikepc
    added namespace protection to remove potential name collisions with
    tool code.  Exceptions are CreateTplFile and QuickConvert.  These are
    extern "C" linked.
    
    6     8/26/99 11:37a Mikepc
    
    5     8/26/99 11:01a Mikepc
    tplConv rewrite for memory usage efficiency, batch file processing
    ability.
    
  
  $NoKeywords: $

-----------------------------------------------------------------------*/

// API to create lists of layer data
// 1 list for each of image color, image alpha, palette color, palette alpha.

#ifndef __TCLAYERINTERNAL_H__
#define __TCLAYERINTERNAL_H__

/********************************/
#include <CharPipe/TCLayer.h>
#include <CharPipe/TCFile.h>

#include "TCImageList.h"

/********************************/
#define LY_IMAGE_COLOR_CMP	0x0008

/*>*******************************(*)*******************************<*/
void TCMakeImColorLayer		( TCFile* dfPtr, TCLayer* newLy );
void TCMakeImAlphaLayer		( TCFile* dfPtr, TCLayer* newLy );
void TCConvertCI_To_RGB		( TCFile* dfPtr, TCImage* imPtr );

/*>*******************************(*)*******************************<*/

#endif  // __TCLAYERINTERNAL_H__