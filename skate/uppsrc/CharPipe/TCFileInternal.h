/*---------------------------------------------------------------------*

Project:  tc library
File:     TCFileInternal.h

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:

   $Log: /Dolphin/build/charPipeline/tc/include/TCFileInternal.h $
    
    1     12/03/99 3:44p Ryan
    
    5     10/08/99 2:44p Mikepc
    update for tplConv portability: altered data structures, replaced
    references to 'read tga code' with ' *fileFn, removed redundant
    functions.
    
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

#ifndef __TCFILEINTERNAL_H__
#define __TCFILEINTERNAL_H__

/********************************/
#include <CharPipe/TCFile.h>

/********************************/
typedef struct TCFileNode
{
	char ext[4];                                              // type
    u32 (*fileFn)( u32 rawSize, u8* rawBits, TCFile* dfPtr ); // pointer to 'read' function	

	struct TCFileNode* prev;
	struct TCFileNode* next;

} TCFileNode, *TCFileNodePtr;

/********************************/
extern u32          DFSize;
extern TCFile*		DF;       // cache for files
extern TCFileNode*	FileTable;
extern u32 (*FileFn)( u32 rawSize, u8* rawBits, TCFile* dfPtr );

/*>*******************************(*)*******************************<*/
TCFile* TCReadFile	( char* fileName );

/*>*******************************(*)*******************************<*/

#endif // __TCFILEINTERNAL_H__