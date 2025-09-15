/*---------------------------------------------------------------------*

Project:  tc library
File:     TCVerify.cpp

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:

 $Log: /Dolphin/build/charPipeline/tc/src/TCVerify.cpp $
    
    2     3/17/00 1:19p Mikepc
    change tc to use indices numbered from 0.
    
    1     12/03/99 3:45p Ryan
    
    3     10/08/99 2:45p Mikepc
    update for tplConv portability: altered data structures, replaced
    references to 'read tga code' with ' *fileFn, removed redundant
    functions.  Changed some file conversion paths.
    
    2     9/16/99 8:47p Mikepc
    updated code for auto-palette generation
    
    1     9/02/99 10:57a Mikepc
    contains code for VerifyLists() ( formerly in convert.cpp ) that checks
    list contents against each other and against real files
   
 $NoKeywords: $

-----------------------------------------------------------------------*/

#include <stdio.h>

#include <CharPipe/TCCommon.h>

#include "TCVerify.h"
#include "TCPaletteList.h"
#include "TCImageList.h"
#include "TCSrcImageList.h"
#include "TCTextureList.h"

/*>*******************************(*)*******************************<*/ 
static void TCSortListsByIndex ( void );

/*>*******************************(*)*******************************<*/ 
// compare SiHead, ImHead, PlHead, TxHead lists to ensure that all index
// references are valid
/*>*******************************(*)*******************************<*/ 
void TCVerifyLists ( void )
{
	TCSrcImage* siPtr = SiHead;
	TCImage*    imPtr = ImHead;
	TCPalette*  plPtr = PlHead;
	TCTexture*  txPtr = TxHead;
	FILE* fp;


	// sort each of SiHead, ImHead, PlHead, TxHead in ascending order
	TCSortListsByIndex();

	// verify that each source image exists 
	// note: extensions will be checked when files are opened for conversion
	while( siPtr )
	{
		if( (fp = fopen( siPtr->fileName, "rb" )) == NULL )
		{
			TCErrorMsg( "TCVerifyLists: unable to open file %s for read\n", siPtr->fileName );
			return;
		}
		fclose( fp );
		siPtr = siPtr->next;
	}

	// verify that each image has a corresponding source image
	while( imPtr )
	{
		// find the source image corresponding to the color index
		// color layer is mandatory
		if( (siPtr = TCFindSrcImageByIndex( imPtr->colorSrcImage )) == NULL )
		{
			TCErrorMsg( "TCVerifyLists: no matching source image for image %d color layer\n", imPtr->index );
			return;
		}
		
		// alpha layer is optional
		if( imPtr->alphaSrcImage != TC_UNUSED )
		{
			if( (siPtr = TCFindSrcImageByIndex( imPtr->alphaSrcImage )) == NULL )
			{
				TCErrorMsg( "TCVerifyLists: no matching source image for image %d alpha layer\n", imPtr->index );
				return;
			}
		}

		imPtr = imPtr->next;

	} // end while( imPtr )


	// verify each palette
	while( plPtr )
	{
		// find the source image corresponding to the palette image index
		if( (siPtr = TCFindSrcImageByIndex( plPtr->srcImage )) == NULL )
		{
			TCErrorMsg( "TCVerifyLists: no matching source image for palette %d srcImage\n", plPtr->index);
			return;
		}	
		plPtr = plPtr->next;

	} // end while( plPtr )


	// verify each texture
	txPtr = TxHead;
	while( txPtr )
	{
		// texture->image is mandatory
		if( (imPtr = TCFindImageByIndex( txPtr->image )) == NULL )
		{
			TCErrorMsg( "TCVerifyLists: no matching image for texture %d\n", txPtr->index);
			return;
		}

		// palette is optional
		if( txPtr->palette != TC_UNUSED )
		{
			if( (plPtr = TCFindPaletteByIndex( txPtr->palette )) == NULL )
			{
				TCErrorMsg( "TCVerifyLists: no matching palette for texture %d\n", txPtr->index);
				return;
			}
		}

		txPtr = txPtr->next;
	} // end while( txPtr )
}

/*>*******************************(*)*******************************<*/
// sort SiHead, ImHead, PlHead, TxHead into ascending order
/*>*******************************(*)*******************************<*/
static void TCSortListsByIndex ( void )
{	
	TCSortSrcImageByIndex();
	TCSortImageByIndex();
	TCSortPaletteByIndex();
	TCSortTextureByIndex();
}

/*>*******************************(*)*******************************<*/
