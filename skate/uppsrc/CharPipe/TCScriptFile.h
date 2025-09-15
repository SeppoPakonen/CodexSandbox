/*---------------------------------------------------------------------*

Project:  tc library
File:     TCScriptFile.h

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:

   $Log: /Dolphin/build/charPipeline/tc/include/TCScriptFile.h $
    
    1     12/03/99 3:44p Ryan
    
    10    9/16/99 8:48p Mikepc
    
    9     9/02/99 11:10a Mikepc
    re-organization of prototypes between files
    
    8     8/26/99 4:56p Mikepc
    added namespace protection to remove potential name collisions with
    tool code.  Exceptions are CreateTplFile and QuickConvert.  These are
    extern "C" linked.
    
    7     8/26/99 11:37a Mikepc
    
    6     8/26/99 11:01a Mikepc
    tplConv rewrite for memory usage efficiency, batch file processing
    ability.
    
  
  $NoKeywords: $

-----------------------------------------------------------------------*/

#ifndef __TCSCRIPTFILE_H__
#define __TCSCRIPTFILE_H__

/********************************/
#include <Dolphin/types.h>

/********************************/
#define NAME_SIZE	256
#define FMT_SIZE	16

/********************************/
extern char  PathName[];     
extern char* PathPtr;

/*>*******************************(*)*******************************<*/
void TCReadTplTxtFile	( char* fileName );

/*>*******************************(*)*******************************<*/

#endif  // __TCSCRIPTFILE_H__