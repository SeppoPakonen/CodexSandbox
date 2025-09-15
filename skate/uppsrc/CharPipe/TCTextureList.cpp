/*---------------------------------------------------------------------*

Project:  tc library
File:     TCTextureList.cpp

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:

 $Log: /Dolphin/build/charPipeline/tc/src/TCTextureList.cpp $
    
    3     8/10/00 6:03p Mikepc
    
    2     3/17/00 1:19p Mikepc
    change tc to use indices numbered from 0.
    
    1     12/03/99 3:45p Ryan
    
    7     9/16/99 8:47p Mikepc
    updated code for auto-palette generation
    
    6     8/26/99 4:59p Mikepc
    renamed file extensions from .c to .cpp.
    .cpp extension allows addition of namespace protection to remove
    potential name collisions with tool code.  Exceptions are CreateTplFile
    and QuickConvert.  These are extern "C" linked.
    
    5     8/26/99 11:38a Mikepc
    
    4     8/26/99 11:03a Mikepc
    tplCon rewrite for efficient memory usage, batch file processing
    ability.
    
  
 $NoKeywords: $

-----------------------------------------------------------------------*/


#include <CharPipe/TCCommon.h>

#include "TCTextureList.h"
#include "TCMem.h"

/********************************/
// global internal linked list variables
TCTexture*     TxHead    = NULL; 

/*>*******************************(*)*******************************<*/ 
static void TCSwapTexture				( TCTexture* thisTex, TCTexture* thatTex );

/*>*******************************(*)*******************************<*/ 
TCTexture* TCNewTexture ( void )
{
	TCTexture* newTx, *tail;

	newTx = (TCTexture*)TCCalloc( 1, sizeof(TCTexture) );
	
	if( TxHead == NULL )
	{
		TxHead = newTx;
	}
	else
	{
		tail = TxHead;
		while( tail->next )
		{
			tail = tail->next;
		}

		tail->next  = newTx;
		newTx->prev = tail;
		newTx->next = NULL;
	}

	return newTx;
}
	
/*>*******************************(*)*******************************<*/
void TCSetTextureAttributes ( TCTexture* tx, u32 index, u32 image, 
							  u32 palette )
{
    TCAssertMsg( (tx != NULL), "TCSetTextureAttributes: NULL TCTexture ptr.\n" );

	tx->index   = index;
	tx->image   = image;
	tx->palette = palette;
}
	
/*>*******************************(*)*******************************<*/
// sort indices from lowest to highest
// check for 0 index, missing indices, duplicate indices
/*>*******************************(*)*******************************<*/
void TCSortTextureByIndex ( void )
{
	TCTexture* thisTex, *nextTex;


    // empty or single-node list (already sorted)
	if( (TxHead == NULL) || (TxHead->next == NULL) )
	{
		return;
	}

	thisTex = TxHead;
	while( thisTex->next)
	{
		nextTex = thisTex->next;

		if( nextTex->index < thisTex->index )
		{	
			// swap just the data, not the pointers
			TCSwapTexture( thisTex, nextTex );

			thisTex = TxHead;
			continue;
		}

		thisTex = thisTex->next;
	}
}

/*>*******************************(*)*******************************<*/
// swap data members only, not pointers
/*>*******************************(*)*******************************<*/
static void TCSwapTexture( TCTexture* thisTex, TCTexture* thatTex )
{
	TCTexture tmpTx;


	tmpTx.index               = thisTex->index;                 
	tmpTx.image               = thisTex->image;          
	tmpTx.palette             = thisTex->palette;
    tmpTx.tplImageOffset      = thisTex->tplImageOffset;        
    tmpTx.tplPaletteOffset    = thisTex->tplPaletteOffset;       
 
	thisTex->index            = thatTex->index;                 
	thisTex->image            = thatTex->image;          
	thisTex->palette          = thatTex->palette;
    thisTex->tplImageOffset   = thatTex->tplImageOffset;        
    thisTex->tplPaletteOffset = thatTex->tplPaletteOffset; 
	
	thatTex->index            = tmpTx.index;                 
	thatTex->image            = tmpTx.image;          
	thatTex->palette          = tmpTx.palette;
    thatTex->tplImageOffset   = tmpTx.tplImageOffset;        
    thatTex->tplPaletteOffset = tmpTx.tplPaletteOffset; 
}

/*>*******************************(*)*******************************<*/

