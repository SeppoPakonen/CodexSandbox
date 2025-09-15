/*---------------------------------------------------------------------*

Project:  tc library
File:     TCPaletteList.cpp

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:

 $Log: /Dolphin/build/charPipeline/tc/src/TCPaletteList.cpp $
    
    3     8/10/00 6:02p Mikepc
    
    2     3/17/00 1:19p Mikepc
    change tc to use indices numbered from 0.
    
    1     12/03/99 3:45p Ryan
    
    10    10/08/99 2:45p Mikepc
    update for tplConv portability: altered data structures, replaced
    references to 'read tga code' with ' *fileFn, removed redundant
    functions.  Changed some file conversion paths.
    
    9     9/17/99 12:19p Mikepc
    arbitrary palette size has been removed- changed SetPalette() to create
    only an 8-bit palette
    
    8     9/16/99 8:47p Mikepc
    updated code for auto-palette generation
    
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


#include <CharPipe/TCPalTable.h>
#include <CharPipe/TCCommon.h>

#include "TCPaletteList.h"
#include "TCMem.h"
#include "TCFileInternal.h"
#include "TCSrcImageList.h"
#include "TCGXDefs.h"

/********************************/
// global internal linked list variables
TCPalette* PlHead = 0;

/********************************/
// u8 array indices for color values
#define ID_R       0
#define ID_G       1
#define ID_B       2
#define ID_A       3

/*>*******************************(*)*******************************<*/
static void TCSwapPalette	( TCPalette* thisPal, TCPalette* thatPal );

/*>*******************************(*)*******************************<*/
// allocate a PalTable structure; allocate its associated 'rgba' array;
// set PalTable->numEntry
/*>*******************************(*)*******************************<*/
TCPalTable* TCCreatePalTable ( u32 numEntry )
{
	TCPalTable* ptNew;


	ptNew = (TCPalTable*)TCCalloc( 1, sizeof(TCPalTable));

	ptNew->numEntry = numEntry;
	ptNew->rgba     = (u8*)TCCalloc( numEntry, 4 ); // 4B per entry (rgba)

	return ptNew;
}

/*>*******************************(*)*******************************<*/
void TCSetPalTableValue ( TCPalTable* ptPtr, u32 index, u8 r, u8 g, u8 b, u8 a )
{
	u8* rgbaPtr;


	rgbaPtr = (u8*)( (u8*)(ptPtr->rgba) + (index * 4) );

	rgbaPtr[ID_R] = r;
	rgbaPtr[ID_G] = g;
	rgbaPtr[ID_B] = b;
	rgbaPtr[ID_A] = a;
}

/*>*******************************(*)*******************************<*/
void TCGetPalTableValue ( TCPalTable* ptPtr, u32 index, u8* rPtr, 
						  u8* gPtr, u8* bPtr, u8* aPtr )
{
	u8* rgbaPtr;


	rgbaPtr = (u8*)( (u8*)(ptPtr->rgba) + (index * 4) );

	if( rPtr )
		*rPtr = rgbaPtr[ID_R];
	if( gPtr )
	   *gPtr  = rgbaPtr[ID_G];
	if( bPtr )
		*bPtr = rgbaPtr[ID_B];
	if( aPtr )
		*aPtr = rgbaPtr[ID_A];
}

/*>*******************************(*)*******************************<*/
// note: in this version, palettes must have 256 entries 
/*>*******************************(*)*******************************<*/
void TCSetPalettes ( void )
{
	TCPalette*	plPtr;
	TCSrcImage*	siPtr;
	TCFile*		dfPtr;
	u8          r, g, b, a;
	u32         i;


	plPtr = PlHead;
	while( plPtr )
	{
		// find this palette's source image and decode the file
		siPtr = TCFindSrcImageByIndex( plPtr->srcImage ); 
		dfPtr = TCReadFile( siPtr->fileName );

		// make sure a palette is present in the file
	    TCAssertMsg( (dfPtr->palPtr != NULL), "TCSetPalettes: file %s has no palette\n", siPtr->fileName );

		plPtr->palPtr = (TCPalTable*)TCCreatePalTable( dfPtr->palPtr->numEntry );

		// copy out the palette
		for( i=0; i< dfPtr->palPtr->numEntry; i++ )
		{
			TCGetPalTableValue( dfPtr->palPtr, i, &r, &g, &b, &a );
			TCSetPalTableValue( plPtr->palPtr, i,  r,  g,  b,  a );
		}

		plPtr = plPtr->next;

	} // end while( thisPal )

}

/*>*******************************(*)*******************************<*/
TCPalette* TCNewPalette ( void )
{
	TCPalette* newPl, *tail;


	newPl = (TCPalette*)TCCalloc( 1, sizeof(TCPalette) );
	
	if( PlHead == NULL )
	{
		PlHead = newPl;
	}
	else
	{
		tail = PlHead;
		while( tail->next )
		{
			tail = tail->next;
		}

		tail->next  = newPl;
		newPl->prev = tail;
		newPl->next = NULL;
	}

	return newPl;
}
	
/*>*******************************(*)*******************************<*/                  
void TCSetPaletteIndex ( TCPalette* pl, u32 index )
{
    TCAssertMsg( (pl != NULL), "TCSetPaletteIndex: NULL TCPalette ptr\n" );
		
	pl->index = index;
}

/*>*******************************(*)*******************************<*/
void TCSetPaletteSrcImage ( TCPalette* pl, u32 srcImage )
{
    TCAssertMsg( (pl != NULL), "TCSetPaletteSrcImage: NULL TCPalette ptr\n" );
 		
	pl->srcImage = srcImage;
}

/*>*******************************(*)*******************************<*/
void TCSetPaletteEntryFormat( TCPalette* pl, u32 entryFmt )
{
    
    TCAssertMsg( (pl != NULL), "TCSetPaletteEntryFormat: NULL TCPalette ptr\n" );
		
	switch(entryFmt)
	{
	case TPL_PALETTE_ENTRY_FMT_R5G6B5:  

		pl->entryFormat = entryFmt;
		break;

    case TPL_PALETTE_ENTRY_FMT_RGB5A3:      
	
		pl->entryFormat = entryFmt;
		break;

	default:
        TCErrorMsg( "TCSetPaletteEntryFormat: palette %d; unknown output format\n", pl->index );	
		break;	
	}
}
	
/*>*******************************(*)*******************************<*/
// sort indices from lowest to highest
/*>*******************************(*)*******************************<*/
void TCSortPaletteByIndex ( void )
{
	TCPalette* thisPal, *nextPal;


	if( (PlHead == NULL) || (PlHead->next == NULL) )
	{
		return;
	}

	thisPal = PlHead;
	while( thisPal->next )
	{
		nextPal = thisPal->next;

		if( nextPal->index < thisPal->index )
		{	
			// swap just the data, not the pointers
			TCSwapPalette( thisPal, nextPal );

			thisPal = PlHead;
			continue;
		}

		thisPal = thisPal->next;
	}
}

/*>*******************************(*)*******************************<*/
static void TCSwapPalette ( TCPalette* thisPal, TCPalette* thatPal )
{
	TCPalette tmpPl;


	tmpPl.index                   = thisPal->index;                   
	tmpPl.srcImage                = thisPal->srcImage;           
	tmpPl.entryFormat             = thisPal->entryFormat;            
	tmpPl.palPtr                  = thisPal->palPtr;
    tmpPl.tplPaletteBankOffset    = thisPal->tplPaletteBankOffset;       
    tmpPl.tplBufferSize           = thisPal->tplBufferSize;   

	thisPal->index                = thatPal->index;                   
	thisPal->srcImage             = thatPal->srcImage;           
    thisPal->entryFormat          = thatPal->entryFormat;             
	thisPal->palPtr               = thatPal->palPtr;
    thisPal->tplPaletteBankOffset = thatPal->tplPaletteBankOffset;       
    thisPal->tplBufferSize        = thatPal->tplBufferSize;   

	thatPal->index                = tmpPl.index;                   
	thatPal->srcImage             = tmpPl.srcImage;           
    thatPal->entryFormat          = tmpPl.entryFormat;            
	thatPal->palPtr               = tmpPl.palPtr;
    thatPal->tplPaletteBankOffset = tmpPl.tplPaletteBankOffset;       
    thatPal->tplBufferSize        = tmpPl.tplBufferSize;   
}

/*>*******************************(*)*******************************<*/
TCPalette* TCFindPaletteByIndex ( u32 index )
{
	TCPalette* plTmp;


    if( index == TC_UNUSED )
        return NULL;

	plTmp = PlHead;
	while( plTmp )
	{
		if( plTmp->index == index )
		{
			return plTmp;
		}		
		plTmp = plTmp->next;
	}

    TCErrorMsg( "TCFindPaletteByIndex: palette %d is not part of palette list\n", index );
	return NULL;
}

/*>*******************************(*)*******************************<*/
// find the position of an image relative to the list head;
// this corresponds to its 'array' index (assume a sorted list)
/*>*******************************(*)*******************************<*/
u32 TCFindPalettePos( TCPalette* pl )
{
    TCPalette* plPtr;
    u32        pos;


    TCAssertMsg( (pl     != NULL), "TCFindPalettePos: NULL palette ptr\n"  );
    TCAssertMsg( (PlHead != NULL), "TCFindPalettePos: NULL palette list\n" );

    pos   = 0;
    plPtr = PlHead;
    while( plPtr )
    {
        if( pl == plPtr )
        {
            return pos;
        }

        pos++;
        plPtr = plPtr->next;
    }

    TCErrorMsg( "TCFindPalettePos: palette %d is not part of palette list\n", pl->index );
    return 0;
}

/*>*******************************(*)*******************************<*/