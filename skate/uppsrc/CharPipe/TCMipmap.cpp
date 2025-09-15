/*---------------------------------------------------------------------*

Project:  tc library
File:     TCMipmap.cpp

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:


 $Log: /Dolphin/build/charPipeline/tc/src/TCMipmap.cpp $
    
    7     8/10/00 6:02p Mikepc
    
    6     4/03/00 5:39p Mikepc
    change #include TCCreateDDS.h to TCCreateS3.h
    change TCConvertToDDS call to TCConvertToS3
    
    5     3/17/00 1:19p Mikepc
    change tc to use indices numbered from 0.
    
    4     2/14/00 1:03p Mikepc
    removed power of 2 size restriction for single LOD images.
    
    3     1/03/00 3:10p Mikepc
    added comments regarding image size restrictions to
    TCCheckMipMapConversionParams, TCCheckPower2.
    even single LODs must have power2 dimensions due to
    1) OpenGL emulation requirements.
    2) converter's default filter mode  is GX_REPEAT in TCWriteTplFile.
    
    
    2     1/03/00 12:32p Mikepc
    fixed potential divide by 0 bug in TCBoxFilter()
    
    1     12/03/99 3:45p Ryan
    
    2     11/23/99 2:52p Mikepc
    improved mipmap code to use alpha information when mipmapping color.
    
    22    10/08/99 2:45p Mikepc
    update for tplConv portability: altered data structures, replaced
    references to 'read tga code' with ' *fileFn, removed redundant
    functions.  Changed some file conversion paths.
    
    21    10/01/99 1:28p Mikepc
    changed CreateNextMipMapLayer() to remove redundant 'level' param.
    
    20    10/01/99 12:20p Mikepc
    Integrated s3.lib code to generate s3 textures 'on the fly' as direct
    draw surfaces.  Removed .dds file reading code.  Changed CMP texture
    generation 'order of operations' to- rgb layer->s3->CMPR format per
    mipmap LOD.
    
    19    9/27/99 11:32a Mikepc
    line 342: changed for loop condition to allow (minLOD > 0) case when
    creating mipmaps.
    
  
    14    9/16/99 8:47p Mikepc
    updated code for auto-palette generation
    
    13    9/02/99 11:12a Mikepc
    some code re-organization between files.
    added code (verify.cpp) to invoke s3tc.exe from within tc program.
    changed some routines to accommodate the new texture creation path.
    
    12    8/30/99 5:07p Mikepc
    changes to dds/cmp processing code to allow odd sized images and images
    smaller thn 8x8 texel.
    
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

#include <stdio.h>

#include <CharPipe/TCCommon.h>

#include "TCMipmap.h"
#include "TCImageList.h"
#include "TCLayerInternal.h"
#include "TCMem.h"
#include "TCTPLToolbox.h"
#include "TCCreateS3.h"
#include "TCGXDefs.h"

/********************************/
// macro to check an LOD for valid range of mipmap pyramid ( 0 to 10 )
#define CheckRangeLOD( lod )  (((lod) < 0) || ((lod) > 10)) ? (0) : (1)

/*>*******************************(*)*******************************<*/
static void TCCreateNextMipMapLayer( TCImage* srcImage, TCLayer* dstColor, 
									 TCLayer* dstAlpha );

static void TCBoxFilter		( TCLayer* srcC, TCLayer* srcA, TCLayer* dstC, 
							  TCLayer* dstA, u32 dstCol, u32 dstRow, 
							  u32 boxWid, u32 boxHgt );

static void TCCheckMipMapConvParams	( TCImage* thisImage );
static u32  TCCheckPower2			( TCImage* thisImage );
static u32  TCCheckMipMapReduction	( TCImage* thisImage );
static u32  TCCheckMipMapFormats	( TCImage* thisImage );

/*>*******************************(*)*******************************<*/
// create a range of mipmaps from the source color layer
/*>*******************************(*)*******************************<*/
u32 TCWriteTplImageMipMaps ( FILE* fp, TCImage* thisImage )
{
	u32      size, level, numLOD;	
	u8*      tplBuffer, *thisBuffer;
    u32      bytesWritten = 0;
	TCLayer* colorLayer   = NULL;
	TCLayer* alphaLayer   = NULL;
	TCLayer  cmpLayer;   // intermediate layer for CMP textures
	TCImage  imTmp;


    TCAssertMsg( (fp != NULL),        "TCWriteTplImageMipMaps: NULL file pointer\n"  );
    TCAssertMsg( (thisImage != NULL), "TCWriteTplImageMipMaps: NULL image pointer\n" );


    // check for acceptable image sizes, LOD range for mipmaps.
	TCCheckMipMapConvParams( thisImage );

	// create a copy of thisImage.  The copy's layers will be
	// modified to produce each successive LOD
	TCCopyImage( thisImage, &imTmp );
	imTmp.prev = NULL;
	imTmp.next = NULL;

	// create a temporary layer with a buffer large enough to hold the 1st LOD;
    // each successive LODs will overwrite a portion of this buffer
    colorLayer         =   &imTmp.lyColor;
	colorLayer->type   =   thisImage->lyColor.type;	
	colorLayer->width  = ( thisImage->lyColor.width  >> thisImage->minLOD );
	colorLayer->height = ( thisImage->lyColor.height >> thisImage->minLOD );
	colorLayer->data   =   NULL;
	TCSetLayerBuffer( colorLayer );


    // create optional alpha layer
	if( thisImage->alphaSrcImage != TC_UNUSED )
	{
        // check layer dimensions
		if( (thisImage->lyColor.width != thisImage->lyAlpha.width) || (thisImage->lyColor.height != thisImage->lyAlpha.height) )
		{
			TCErrorMsg( "WriteTplImageMipMaps: color layer %d and alpha layer %d have different dimensions\n", thisImage->colorSrcImage, thisImage->alphaSrcImage );
			return 0;
		}

        alphaLayer         =   &imTmp.lyAlpha;
	    alphaLayer->type   =   thisImage->lyAlpha.type;	
	    alphaLayer->width  = ( thisImage->lyAlpha.width  >> thisImage->minLOD );
	    alphaLayer->height = ( thisImage->lyAlpha.height >> thisImage->minLOD );
	    alphaLayer->data   =   NULL;
		TCSetLayerBuffer( alphaLayer );
	}
				
	// allocate and zero a buffer large enough to hold all output LODs end to end
	// 'thisBuffer' points to the current LOD within 'tplBuffer'
	tplBuffer  = (u8*)TCCalloc( 1, thisImage->tplBufferSize );
	thisBuffer = tplBuffer;

	// generate mipmap pyramid sequentially from minLOD to maxLOD
	// use LOD 0 from the source layers regardless of how many mipmaps are present
	
	// convert each mipmap in turn to Dolphin format and pack
	// sequentially in thisImage->tplBuffer

	// swap the original image layers for temporary 'next mipmap' layers each iteration;
	// restore the original at the end.

	numLOD = imTmp.maxLOD - imTmp.minLOD + 1;

	for( level = imTmp.minLOD; level < (imTmp.minLOD + numLOD); level++ )
	{

		// use the same allocated memory, but reset the buffer dimensions for each LOD
		colorLayer->width   = ( thisImage->lyColor.width  >> level );
		colorLayer->height  = ( thisImage->lyColor.height >> level );

		if( alphaLayer )
		{
		    alphaLayer->width  = ( thisImage->lyAlpha.width  >> level ); 
		    alphaLayer->height = ( thisImage->lyAlpha.height >> level );
		}

        // convert from base layer to next LOD
		TCCreateNextMipMapLayer( thisImage, colorLayer, alphaLayer );
	
		// write the LOD to the buffer				
		switch( thisImage->texelFormat )
		{
		case TPL_IMAGE_TEXEL_FMT_I4:		TCWriteTplImage_I4(      colorLayer, thisBuffer             );     break;       	
        case TPL_IMAGE_TEXEL_FMT_I8:		TCWriteTplImage_I8(      colorLayer, thisBuffer             );     break;
        case TPL_IMAGE_TEXEL_FMT_IA4:		TCWriteTplImage_IA4(     colorLayer, alphaLayer, thisBuffer );     break;
        case TPL_IMAGE_TEXEL_FMT_IA8:		TCWriteTplImage_IA8(     colorLayer, alphaLayer, thisBuffer );     break;
        case TPL_IMAGE_TEXEL_FMT_R5G6B5:	TCWriteTplImage_R5G6B5(  colorLayer, thisBuffer             );     break;          
        case TPL_IMAGE_TEXEL_FMT_RGB5A3:	TCWriteTplImage_RGB5A3(  colorLayer, alphaLayer, thisBuffer );     break;
        case TPL_IMAGE_TEXEL_FMT_RGBA8:		TCWriteTplImage_RGBA8(   colorLayer, alphaLayer, thisBuffer );     break;    	 		

		case TPL_IMAGE_TEXEL_FMT_CI4:		TCWriteTplImage_CI4(     colorLayer, thisBuffer );                 break;
		case TPL_IMAGE_TEXEL_FMT_CI8:       TCWriteTplImage_CI8(     colorLayer, thisBuffer );                 break;
		case TPL_IMAGE_TEXEL_FMT_CI14_X2:   TCWriteTplImage_CI14_X2( colorLayer, thisBuffer );                 break;

		// special case- create a cmp layer from the color, alpha layers and use it instead
		case TPL_IMAGE_TEXEL_FMT_CMP:
			TCConvertToS3( colorLayer, alphaLayer, &cmpLayer );
			TCWriteTplImage_CMP( &cmpLayer, thisBuffer );
			TCFree( (void**)(&cmpLayer.data) );
			break;
			
		default:  
			TCErrorMsg( "TCCreateMipMaps: unknown texel format for image %d\n", thisImage->index );
			return 0;
			break;		
		}
					
		// must update the 'current LOD' buffer pointer within the full size 
		// buffer to the start of the next LOD. 
		size       = TCComputeTplImageBufferSize( &imTmp );
		thisBuffer = ( thisBuffer + size );
	
	} // end for

	// free the temporary mipmap layer buffers
	TCFree( (void**)( &colorLayer->data ) );

	if( alphaLayer )
	{       
        TCFree( (void**)( &alphaLayer->data ) );	
	}

    // write the full mipmap block to the .tpl file
	bytesWritten = fwrite( tplBuffer, 1, thisImage->tplBufferSize, fp );

	TCFree( (void**)(&tplBuffer) );
	return bytesWritten;
}

/*>*******************************(*)*******************************<*/
// create a mipmap level from srcLayer
// srcLayer is always LOD 0; 'level' is defined relative to this
// srcAlpha is only used when box-filtering a color layer
/*>*******************************(*)*******************************<*/
static void TCCreateNextMipMapLayer ( TCImage* srcImage, TCLayer* dstColor, TCLayer* dstAlpha )			                       
{
	u32      row,    col;
	u32      boxWid, boxHgt;
	TCLayer* srcColor = NULL;
	TCLayer* srcAlpha = NULL;


    TCAssertMsg( (srcImage != NULL), "TCCreateNextMipMapLayer: NULL TCImage ptr\n" );
    TCAssertMsg( (dstColor != NULL), "TCCreateNextMipMapLayer: NULL TCLayer ptr\n" );


	srcColor = &srcImage->lyColor;

	if( srcImage->alphaSrcImage != TC_UNUSED )
	{
		srcAlpha = &srcImage->lyAlpha;
	}

	// compute the size of the filtering box
	boxWid = srcColor->width  / dstColor->width;
	boxHgt = srcColor->height / dstColor->height;

	// write out the mipmap into the dst layers
	// note that both src and dst alpha may be NULL
	for( row=0; row < dstColor->height; row++)
	{		
		for( col=0; col < dstColor->width; col++ )
		{
			TCBoxFilter( srcColor, srcAlpha, dstColor, dstAlpha, col, row, boxWid, boxHgt );			
		} 				
	} 		
}

/*>*******************************(*)*******************************<*/
// pre-multiply color by source alpha (if present) when filtering
/*>*******************************(*)*******************************<*/
static void TCBoxFilter ( TCLayer* srcC,   TCLayer* srcA,   TCLayer* dstC,   TCLayer* dstA, 
                          u32      dstCol, u32      dstRow, u32      boxWid, u32      boxHgt )
{
	u32 row, col;
	u32 boxRow = ( dstRow * boxHgt );
    u32 boxCol = ( dstCol * boxWid );
	f64 rAcc   =   0;
	f64 gAcc   =   0;
	f64 bAcc   =   0;
	f64 aAcc   =   0;
    u16 a      = 255;
	u16 r;
	u8  g, b;


    // pre-multiply source texels by alpha
 	for( row = boxRow; row < (boxRow + boxHgt); row++ )
	{		
		for( col = boxCol; col < (boxCol + boxWid); col++ )
		{
			TCGetLayerValue( srcC, col, row, &r, &g, &b );

			if( srcA )
			{
				TCGetLayerValue( srcA, col, row, &a, NULL, NULL );          
			}

            rAcc += (f64)( r * a );
			gAcc += (f64)( g * a );
			bAcc += (f64)( b * a );	
			aAcc += (f64)( a );
		}				
	}

	r = g = b = 0;

	if( aAcc )
	{
		r = (u16)( ( rAcc / aAcc )  + 0.5f );
		g = (u8)(  ( gAcc / aAcc )  + 0.5f );
		b = (u8)(  ( bAcc / aAcc )  + 0.5f );
	}

	TCSetLayerValue( dstC, dstCol, dstRow, r, g, b );

	if( dstA )
	{
		a = (u16)( ( aAcc / (f32)(boxWid * boxHgt) ) + 0.5f );

		TCSetLayerValue( dstA, dstCol, dstRow, a, 0, 0 );
	}
}
	
/*>*******************************(*)*******************************<*/
// compute the required image buffer size for all LODs
/*>*******************************(*)*******************************<*/
u32 TCComputeTplMipMapImageBufferSize ( TCImage* thisImage )
{
	u32 level;
	u32 totalSize = 0;	
	u32 saveWidth, saveHeight;
	

	saveWidth  = (thisImage->lyColor).width;
	saveHeight = (thisImage->lyColor).height;

	totalSize  = 0;
	for( level = thisImage->minLOD; level< ( thisImage->maxLOD + 1 ) ; level++ )
	{
		(thisImage->lyColor).width  = ( saveWidth  >> level );
		(thisImage->lyColor).height = ( saveHeight >> level );		
	
		totalSize += TCComputeTplImageBufferSize( thisImage );		
	} 
		
	(thisImage->lyColor).width  = saveWidth;
	(thisImage->lyColor).height = saveHeight;
	
	return totalSize;
}

/*>*******************************(*)*******************************<*/
// check that the proposed LOD conversion is ok
// wrt image size, LOD range
/*>*******************************(*)*******************************<*/
static void TCCheckMipMapConvParams ( TCImage* thisImage )
{
	u32 check;


	// check LODs for valid range			
	check = CheckRangeLOD( thisImage->minLOD );
	TCAssertMsg( (check),"TCCheckMipMapConvParams: minLOD out of range for image %d\n", thisImage->index );

	check = CheckRangeLOD( thisImage->maxLOD );
	TCAssertMsg( (check),"TCCheckMipMapConvParams: maxLOD out of range for image %d\n", thisImage->index );
	
	check = CheckRangeLOD( thisImage->remapMinLOD );
	TCAssertMsg( (check),"TCCheckMipMapConvParams: remapLOD out of range for image %d\n", thisImage->index );

	// scale down only
	if( thisImage->minLOD > thisImage->maxLOD )
	{
		TCErrorMsg( "TCCheckMipMapConvParams: image %d has (minLOD > maxLOD)\n", thisImage->index );
	}
	
	// cant remap beyond maximum LOD pyramid size
	check = thisImage->remapMinLOD + (thisImage->maxLOD - thisImage->minLOD);
	if( check > 10 )
	{
		TCErrorMsg( "TCCheckMipMapConvParams: image %d remaps LODs beyond max pyramid size\n", thisImage->index );
	}

	// check for power of 2 image dimensions
	check = TCCheckPower2( thisImage );
	TCAssertMsg( (check),"TCCheckMipMapConvParams: image %d is not power of 2 dimensions\n", thisImage->index );
	
	// ensure both width and height of source will support the minification
	check = TCCheckMipMapReduction( thisImage );
	TCAssertMsg( (check),"TCCheckMipMapConvParams: numLOD is greater than image %d minimum dimension\n", thisImage->index );
	
	// check for a valid conversion format if multiple LODs
	check = TCCheckMipMapFormats( thisImage );	
    TCAssertMsg( (check),"TCCheckMipMapConvParams: image %d invalid output format for mipmapping\n", thisImage->index );

}
	
/*>*******************************(*)*******************************<*/
// check an image size for an in-range power of two
// note: single LODs can have any dimensions
/*>*******************************(*)*******************************<*/
static u32 TCCheckPower2( TCImage* thisImage )
{
	u32 i, size;


	// single/original LOD can be any dimension up to 1024 texels
	if( (thisImage->minLOD == 0) && (thisImage->maxLOD == 0) && (thisImage->remapMinLOD == 0) )
	{
		if( ( thisImage->lyColor.width > 1024 ) || ( thisImage->lyColor.height > 1024 ) )
		{
			return 0;
		}

		return 1;
	}


	// multiple LODs must have hgt. and wid. as power of 2
	for( i=0; i< 2; i++ )
	{
		if( i== 0 )
			size = (thisImage->lyColor).width;
		else
			size = (thisImage->lyColor).height;


		switch( size )
		{	
		case 1024:
		case  512:
		case  256:
		case  128:
		case   64:
		case   32:
		case   16:
		case    8:
		case    4:
		case    2:
		case    1:	
			;  // do nothing- proceed with next pass
			break;
	
		default:
			return 0;
			break;	
	}
		
	} // end for

    return 1;

}

/*>*******************************(*)*******************************<*/
// ensure minification is within the width, height range of source image.
// note: use minimum image dimension as limiting factor
/*>*******************************(*)*******************************<*/
static u32 TCCheckMipMapReduction ( TCImage* thisImage )
{
	u32 scale;
	u32 checkWidth, checkHeight;

	
	// all we need is the total reduction from source image
	// so 'minLOD' isn't required
	scale = thisImage->maxLOD;

	if( scale == 0 )
	{
		return 1;
	}

	checkWidth  = ( (thisImage->lyColor).width  >> scale );
	checkHeight = ( (thisImage->lyColor).height >> scale );

	if( (checkWidth == 0) || (checkHeight == 0) )
	{
		return 0;
	}

    return 1;
}

/*>*******************************(*)*******************************<*/
// multiple LODs are only available for true color formats
/*>*******************************(*)*******************************<*/
static u32 TCCheckMipMapFormats( TCImage* thisImage )
{

	// single LOD- all formats ok;
	if( thisImage->minLOD == thisImage->maxLOD )
	{
		return 1;
	}
		
	// multiple LOD- only true color and compressed formats are ok
	switch( thisImage->texelFormat )
	{
		case TPL_IMAGE_TEXEL_FMT_I4:		// all fall through
        case TPL_IMAGE_TEXEL_FMT_I8:
        case TPL_IMAGE_TEXEL_FMT_IA4:  
        case TPL_IMAGE_TEXEL_FMT_IA8:
	    case TPL_IMAGE_TEXEL_FMT_R5G6B5:    
        case TPL_IMAGE_TEXEL_FMT_RGB5A3:
        case TPL_IMAGE_TEXEL_FMT_RGBA8:
		case TPL_IMAGE_TEXEL_FMT_CMP:
            return 1;
            break;
     
        default:
			return 0;
            break;		
	}
}

/*>*******************************(*)*******************************<*/
















