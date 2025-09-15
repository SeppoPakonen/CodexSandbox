/*---------------------------------------------------------------------*

Project:  tc library
File:     TCMem.h

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:

   $Log: /Dolphin/build/charPipeline/tc/include/TCMem.h $
    
    1     12/03/99 3:44p Ryan
    
    5     10/12/99 8:00p Mikepc
    changed include paths to vcc (tools/options) relative.
    
    4     8/26/99 4:56p Mikepc
    added namespace protection to remove potential name collisions with
    tool code.  Exceptions are CreateTplFile and QuickConvert.  These are
    extern "C" linked.
    
    3     8/26/99 11:37a Mikepc
    
    2     8/26/99 11:01a Mikepc
    tplConv rewrite for memory usage efficiency, batch file processing
    ability.
    
  
  $NoKeywords: $

-----------------------------------------------------------------------*/

#ifndef __TCMEM_H__
#define __TCMEM_H__

/********************************/
#include <Dolphin/types.h>

/*>*******************************(*)*******************************<*/
void* TCMalloc	( u32 size );
void* TCCalloc	( u32 num, u32 size );
void  TCFree	( void** vPtr );
void  TCFreeMem	( void );

/*>*******************************(*)*******************************<*/

#endif // __TCMEM_H__