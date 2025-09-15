/*---------------------------------------------------------------------*

Project:  tc library
File:     TCSrcImageList.h

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:

   $Log: /Dolphin/build/charPipeline/tc/include/TCSrcImageList.h $
    
    1     12/03/99 3:44p Ryan
    
    8     9/16/99 8:48p Mikepc
    
    7     9/02/99 11:10a Mikepc
    re-organization of prototypes between files
    
    6     8/26/99 4:56p Mikepc
    added namespace protection to remove potential name collisions with
    tool code.  Exceptions are CreateTplFile and QuickConvert.  These are
    extern "C" linked.
    
    5     8/26/99 11:37a Mikepc
    
    4     8/26/99 11:01a Mikepc
    tplConv rewrite for memory usage efficiency, batch file processing
    ability.
    
  
  $NoKeywords: $

-----------------------------------------------------------------------*/

#ifndef __TCSRCIMAGELIST_H__
#define __TCSRCIMAGELIST_H__    

/********************************/
#include <Dolphin/types.h>

#include "TCScriptFile.h"
                                                           
/********************************/
typedef struct TCSrcImage
{

 	char fileName[NAME_SIZE];    // source file name  	

 	u32   index;                 // source file's index (1 to n) from script file	

	struct TCSrcImage* prev;
	struct TCSrcImage* next;
	
}TCSrcImage, *TCSrcImagePtr;

/********************************/
extern TCSrcImage* SiHead;

/*>*******************************(*)*******************************<*/
TCSrcImage* TCNewSrcImage			( void );
void TCSortSrcImageByIndex			( void );
TCSrcImage* TCFindSrcImageByIndex	( u32 index );

void TCSetSrcImageFromFile			( TCSrcImage* newSi, char* fileName, 
									  u32 index  );                                        

/*>*******************************(*)*******************************<*/
 
#endif  // __SRC_IMAGE_LIST_H__