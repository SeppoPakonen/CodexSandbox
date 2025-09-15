/*---------------------------------------------------------------------*

Project:  tc library
File:     TCSrcImageList.cpp

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:

 $Log: /Dolphin/build/charPipeline/tc/src/TCSrcImageList.cpp $
    
    3     8/10/00 6:03p Mikepc
    
    2     3/17/00 1:19p Mikepc
    change tc to use indices numbered from 0.
    
    1     12/03/99 3:45p Ryan
    
    11    10/08/99 2:45p Mikepc
    update for tplConv portability: altered data structures, replaced
    references to 'read tga code' with ' *fileFn, removed redundant
    functions.  Changed some file conversion paths.
    
    10    10/01/99 12:20p Mikepc
    Integrated s3.lib code to generate s3 textures 'on the fly' as direct
    draw surfaces.  Removed .dds file reading code.  Changed CMP texture
    generation 'order of operations' to- rgb layer->s3->CMPR format per
    mipmap LOD.
    
    9     9/16/99 8:47p Mikepc
    updated code for auto-palette generation
    
    8     9/02/99 11:12a Mikepc
    some code re-organization between files.
    added code (verify.cpp) to invoke s3tc.exe from within tc program.
    changed some routines to accommodate the new texture creation path.
    
    7     8/26/99 4:59p Mikepc
    renamed file extensions from .c to .cpp.
    .cpp extension allows addition of namespace protection to remove
    potential name collisions with tool code.  Exceptions are CreateTplFile
    and QuickConvert.  These are extern "C" linked.
    
    6     8/26/99 11:38a Mikepc
    
    5     8/26/99 11:03a Mikepc
    tplCon rewrite for efficient memory usage, batch file processing
    ability.
    
  
 $NoKeywords: $

-----------------------------------------------------------------------*/

#include <string.h>

#include <CharPipe/TCCommon.h>

#include "TCSrcImageList.h"
#include "TCMem.h"

/********************************/
// internally managed global SrcImage linked list variables
TCSrcImage* SiHead = NULL;

/*>*******************************(*)*******************************<*/
static void TCSwapSrcImage	( TCSrcImage* src, TCSrcImage* dst );

/*>*******************************(*)*******************************<*/
// create a new source image; attach it to the tail of SiHead's list
/*>*******************************(*)*******************************<*/
TCSrcImage* TCNewSrcImage ( void )
{
	TCSrcImage* newSi, *tail;


	newSi = (TCSrcImage*)TCCalloc( 1, sizeof(TCSrcImage));

	if( SiHead == NULL )
	{
		SiHead = newSi;
	}
	else
	{
		tail = SiHead;
		while( tail->next )
		{
			tail = tail->next;
		}

		tail->next  = newSi;
		newSi->prev = tail;
		newSi->next = NULL;
	}

	return newSi;
}
	
/*>*******************************(*)*******************************<*/
// sort indices from lowest to highest
// check for 0 index, missing indices, duplicate indices
/*>*******************************(*)*******************************<*/
void TCSortSrcImageByIndex ( void )
{
	TCSrcImage* thisSi, *nextSi;


	if( (SiHead == NULL) || (SiHead->next == NULL) )
	{
		return;
	}

	thisSi = SiHead;
	while( thisSi->next )
	{
		nextSi = thisSi->next;

		if( nextSi->index < thisSi->index )
		{	
			// swap just the data, not the pointers
			TCSwapSrcImage( thisSi, nextSi );

			thisSi = SiHead;
			continue;
		}

		thisSi = thisSi->next;
	}	
}

/*>*******************************(*)*******************************<*/
static void TCSwapSrcImage ( TCSrcImage* src, TCSrcImage* dst )
{
	TCSrcImage siTmp;


	strcpy( siTmp.fileName, src->fileName );
	siTmp.index = src->index;

	strcpy( src->fileName, dst->fileName );
	src->index  = dst->index;

	strcpy( dst->fileName, siTmp.fileName );
	dst->index  = siTmp.index;
}

/*>*******************************(*)*******************************<*/
TCSrcImage* TCFindSrcImageByIndex ( u32 index )
{
	TCSrcImage* siTmp;


    if( index == TC_UNUSED )
        return NULL;

	siTmp = SiHead;
	while( siTmp )
	{
		if( siTmp->index == index )
		{
			return siTmp;
		}		
		siTmp = siTmp->next;
	}

    TCErrorMsg( "TCFindSrcImageByIndex: source image %d is not part of source image list\n", index );
	return NULL;
}

/*>*******************************(*)*******************************<*/
// given a file name, check for the existence of the file and determine 
// its attributes.
// perform initial error checks
/*>*******************************(*)*******************************<*/
void TCSetSrcImageFromFile ( TCSrcImage* newSi, char* fileName, u32 index )
{

    TCAssertMsg( (newSi     != NULL), "TCSetSrcImageFromFile: NULL TCSrcImage ptr\n" ); 
    TCAssertMsg( (fileName  != NULL), "TCSetSrcImageFromFile: NULL file name\n" ); 
    TCAssertMsg( (*fileName != '\0'), "TCSetSrcImageFromFile: NULL file name\n" ); 


	strcpy( newSi->fileName, fileName );
	newSi->index = index;
}

/*>*******************************(*)*******************************<*/









