/*---------------------------------------------------------------------*

Project:  tc library
File:     TCCreateS3.h

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:

   $Log: /Dolphin/build/charPipeline/tc/include/TCCreateS3.h $
    
    1     4/03/00 5:40p Mikepc
    replaces TCCreateDDS.h
    
    1     12/03/99 3:44p Ryan
    
    8     10/01/99 12:13p Mikepc
    Removed old code that called s3tc.exe.  Replaced with a function to
    call the s3.lib functions to make compressed textures.
    
    7     9/16/99 8:48p Mikepc
    
    6     9/02/99 11:10a Mikepc
    re-organization of prototypes between files
    
    5     8/26/99 4:56p Mikepc
    added namespace protection to remove potential name collisions with
    tool code.  Exceptions are CreateTplFile and QuickConvert.  These are
    extern "C" linked.
    
    4     8/26/99 11:37a Mikepc
    
    3     8/26/99 11:01a Mikepc
    tplConv rewrite for memory usage efficiency, batch file processing
    ability.
    
  
  $NoKeywords: $

-----------------------------------------------------------------------*/

#ifndef __TCCREATEDDS_H__
#define __TCCREATEDDS_H__

/********************************/
#include <CharPipe/TCLayer.h>

/*>*******************************(*)*******************************<*/
void TCConvertToS3( TCLayer* lyColor, TCLayer* lyAlpha, TCLayer* lyCmp );

/*>*******************************(*)*******************************<*/

#endif // __TCCREATEDDS_H__

