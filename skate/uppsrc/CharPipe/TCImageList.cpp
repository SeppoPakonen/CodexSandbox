/*---------------------------------------------------------------------*

Project:  tc library
File:     TCImageList.cpp

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:

 $Log: /Dolphin/build/charPipeline/tc/src/TCImageList.cpp $
    
    4     8/10/00 6:02p Mikepc
    removed redundant #include<assert.h>, 
    changed any remaining asserts to TCAssertMsg
    
    3     4/10/00 2:09p Mikepc
    fixed mipmap total # lod bug
    
    2     3/17/00 1:19p Mikepc
    change tc to use indices numbered from 0.
    
    1     12/03/99 3:45p Ryan
    
    15    10/12/99 11:38a Mikepc
    fixed a bug in imNew() - image alpha layer received an invalid pointer
    if imageSrcAlpha was not 0 but file contained no alpha info.
    
    14    10/08/99 2:45p Mikepc
    update for tplConv portability: altered data structures, replaced
    references to 'read tga code' with ' *fileFn, removed redundant
    functions.  Changed some file conversion paths.
    
    13    9/17/99 12:18p Mikepc
    arbitrary palette size has been removed- changed RemapColors() to
    RemapImageColors() and modified code to work only with 8-bit palettes
    
    12    9/16/99 8:47p Mikepc
    updated code for auto-palette generation
    
    11    8/26/99 4:59p Mikepc
    renamed file extensions from .c to .cpp.
    .cpp extension allows addition of namespace protection to remove
    potential name collisions with tool code.  Exceptions are CreateTplFile
    and QuickConvert.  These are extern "C" linked.
    
    10    8/26/99 11:38a Mikepc
    
    9     8/26/99 11:03a Mikepc
    tplCon rewrite for efficient memory usage, batch file processing
    ability.
    
  
 $NoKeywords: $

-----------------------------------------------------------------------*/

#include <CharPipe/TCCommon.h>

#include "TCImageList.h"
#include "TCSrcImageList.h"
#include "TCLayerInternal.h"
#include "TCFileInternal.h"
#include "TCMem.h"
#include "TCGXDefs.h"

/********************************/
// global Image linked list
TCImage* ImHead = NULL;

/*>*******************************(*)*******************************<*/
static void TCSwapImage	( TCImage* src, TCImage* dst );

/*>*******************************(*)*******************************<*/
// set image layer values from a tga file
/*>*******************************(*)*******************************<*/
void TCSetImageValues ( void )
{
	TCSrcImage*      siPtr;
	TCImage*         imPtr;
	TCLayer*         lyPtr;
	TCFilePtr		 dfPtr;


	imPtr = ImHead;
	while( imPtr )
	{
    	// find and decode this image's source file
    	siPtr = TCFindSrcImageByIndex( imPtr->colorSrcImage );
    	dfPtr = TCReadFile( siPtr->fileName );

    	// set color layer attributes
    	lyPtr         = &(imPtr->lyColor);
    	lyPtr->type   = dfPtr->lyColor->type;
    	lyPtr->width  = dfPtr->lyColor->width;
    	lyPtr->height = dfPtr->lyColor->height;
    	lyPtr->data   = NULL;

    	// alpha layer atributes
    	if( imPtr->alphaSrcImage != TC_UNUSED )
    	{
    		// if alpha layer comes from a different source, fetch another file
    		if( imPtr->colorSrcImage != imPtr->alphaSrcImage )
    		{
    			siPtr = TCFindSrcImageByIndex( imPtr->alphaSrcImage );
    			dfPtr = TCReadFile( siPtr->fileName );
    		}

    		// check that alpha layer is present 
    		TCAssertMsg( (dfPtr->lyAlpha != NULL), "ImNew: no alpha layer in file %s for image %d\n", dfPtr->name, imPtr->index );

    		// set alpha layer attributes
    		lyPtr         = &(imPtr->lyAlpha);
    		lyPtr->type   = dfPtr->lyAlpha->type;
    		lyPtr->width  = dfPtr->lyAlpha->width;
    		lyPtr->height = dfPtr->lyAlpha->height;
    		lyPtr->data   = NULL;

    	}

    	imPtr = imPtr->next;

	} // end while ( imPtr );

}

/*>*******************************(*)*******************************<*/
TCImage* TCNewImage ( void )
{
	TCImage* newIm, *tail;

	
	newIm = (TCImage*)TCCalloc( 1, sizeof(TCImage) );

	
	if( ImHead == NULL )
	{
		ImHead = newIm;
	}
	else
	{
		tail = ImHead;
		while( tail->next )
		{
			tail = tail->next;
		}

		tail->next  = newIm;
		newIm->prev = tail;
		newIm->next = NULL;
	}

	return newIm;
}

/*>*******************************(*)*******************************<*/                      
void TCSetImageIndex ( TCImage* im, u32 index )
{

    TCAssertMsg( (index != TC_UNUSED), "TCSetImageIndex: invalid image index %d\n", index );
    TCAssertMsg( (im != NULL),         "TCSetImageIndex: NULL image ptr for image %d\n", index );
		
	im->index = index;
}

/*>*******************************(*)*******************************<*/
void TCSetImageLayerAtt ( TCImage* im, u32 colorLayer, u32 alphaLayer )
{

    TCAssertMsg( (im != NULL), "TCSetImageLayerAtt: NULL image ptr\n" );


	// color layer is mandatory; alpha is optional (may be TC_UNUSED)

	// note:  if im->index is not already set, the printed index 
	//        in the error message will be wrong
    TCAssertMsg( (colorLayer != TC_UNUSED), "TCSetImageLayerAtt: invalid color layer %d for image %d\n", colorLayer, im->index );
		
	im->colorSrcImage = colorLayer;
	im->alphaSrcImage = alphaLayer;	
}

/*>*******************************(*)*******************************<*/
void TCSetImageTexelFormat ( TCImage* im, u32 texelFmt )
{		
	
    TCAssertMsg( (im != NULL), "TCSetImageTexelFormat: NULL image ptr\n" );


	switch( texelFmt )
	{
	case TPL_IMAGE_TEXEL_FMT_I4:				// fall through
	case TPL_IMAGE_TEXEL_FMT_I8:
	case TPL_IMAGE_TEXEL_FMT_IA4:			
	case TPL_IMAGE_TEXEL_FMT_IA8:

	case TPL_IMAGE_TEXEL_FMT_CI4:
	case TPL_IMAGE_TEXEL_FMT_CI8:
	case TPL_IMAGE_TEXEL_FMT_CI14_X2:

	case TPL_IMAGE_TEXEL_FMT_R5G6B5:
	case TPL_IMAGE_TEXEL_FMT_RGB5A3:
	case TPL_IMAGE_TEXEL_FMT_RGBA8:

	case TPL_IMAGE_TEXEL_FMT_CMP:

		im->texelFormat = texelFmt;
		break;
		
	default:
		// note: if im->index is not set, this message will print an incorrect value
		TCErrorMsg( "TCSetImageTexelFormat: invalid texel format %d for image %d\n", texelFmt, im->index );
		break;	
	}	
}
	
/*>*******************************(*)*******************************<*/ 
// set mipmap LODs 
// note:  range check is based on fn params only; whether actual image
//        data can support this range is not checked until mipmap creation.
/*>*******************************(*)*******************************<*/
void TCSetImageMipMap ( TCImage* im, u32 minLOD, u32 maxLOD, u32 baseLOD )
{
    u32 numLOD;

    
    TCAssertMsg( (im != NULL), "TCSetImageMipMap: NULL image ptr\n" );
	
	// check LOD ranges 
    if( (minLOD > 10) || (maxLOD > 10) )
    {
        TCErrorMsg( "TCSetImageMipMap: invalid LOD range for image %d\n", im->index );
    }

    if( minLOD > maxLOD )
    {
        TCErrorMsg( "TCSetImageMipMap: invalid LOD range for image %d\n", im->index );
    }
 
    numLOD = maxLOD - minLOD + 1;
    if( baseLOD + numLOD > 11 )
    {
        TCErrorMsg( "TCSetImageMipMap: invalid LOD range for image %d\n", im->index );
    }

    // set values
    im->minLOD      = minLOD;
	im->maxLOD      = maxLOD;
	im->remapMinLOD = baseLOD;	
}
	

/*>*******************************(*)*******************************<*/
// sort image list in ascending order by index
/*>*******************************(*)*******************************<*/
void TCSortImageByIndex ( void )
{
	TCImage* thisIm, *nextIm;


    // empty or single-node list (already sorted)
	if( (ImHead == NULL) || (ImHead->next == NULL) )
	{
		return;
	}

	thisIm = ImHead;
	while( thisIm->next )
	{
		nextIm = thisIm->next;

		if( nextIm->index < thisIm->index )
		{	
			// swap just the data, not the pointers
			TCSwapImage( thisIm, nextIm );

			thisIm = ImHead;
			continue;
		}

		thisIm = thisIm->next;
	}
}

/*>*******************************(*)*******************************<*/
// swap just the data members, not the linked list pointers
/*>*******************************(*)*******************************<*/
static void TCSwapImage( TCImage* src, TCImage* dst )
{
	TCImage  imTmp;
	TCImage* saveSrcPrev = src->prev;
	TCImage* saveSrcNext = src->next;
	TCImage* saveDstPrev = dst->prev;
	TCImage* saveDstNext = dst->next;


    // src->tmp
    TCCopyImage ( src, &imTmp );

    // dst->src
    TCCopyImage( dst, src );
	src->prev = saveSrcPrev;
	src->next = saveSrcNext;

    // src(tmp)->dst
    TCCopyImage( &imTmp, dst );
	dst->prev = saveDstPrev;
	dst->next = saveDstNext;

}

/*>*******************************(*)*******************************<*/
// copy an entire image including the Layer members and linked list 
// pointers
/*>*******************************(*)*******************************<*/
void TCCopyImage ( TCImage* src, TCImage* dst )
{
	u32 i;
    u8* sPtr = (u8*)src;
    u8* dPtr = (u8*)dst;


	for( i=0; i < sizeof(TCImage); i++ )
	{
		*dPtr++ = *sPtr++;
	}
}

/*>*******************************(*)*******************************<*/
TCImage* TCFindImageByIndex ( u32 index )
{
	TCImage* imTmp;


    if( index == TC_UNUSED )
        return NULL;

	imTmp = ImHead;
	while( imTmp )
	{
		if( imTmp->index == index )
		{
			return imTmp;
		}		
		imTmp = imTmp->next;
	}

    TCErrorMsg( "TCFindImageByIndex: image %d is not part of image list\n", index );
	return NULL;
}

/*>*******************************(*)*******************************<*/
// find the position of an image relative to the list head;
// this corresponds to its 'array' index (assume a sorted list)
/*>*******************************(*)*******************************<*/
u32 TCFindImagePos( TCImage* im )
{
    TCImage* imPtr;
    u32      pos;


    TCAssertMsg( (im != NULL),     "TCFindImagePos: NULL image ptr\n"  );
    TCAssertMsg( (ImHead != NULL), "TCFindImagePos: NULL image list\n" );

    pos   = 0;
    imPtr = ImHead;
    while( imPtr )
    {
        if( im == imPtr )
        {
            return pos;
        }

        pos++;
        imPtr = imPtr->next;
    }

    TCErrorMsg( "TCFindImagePos: image %d is not part of image list\n", im->index );
    return 0;
}

/*>*******************************(*)*******************************<*/