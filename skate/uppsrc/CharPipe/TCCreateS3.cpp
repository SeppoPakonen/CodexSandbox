/*---------------------------------------------------------------------*

Project:  tc library
File:     TCCreateDDS.cpp

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:

 $Log: /Dolphin/build/charPipeline/tc/src/TCCreateS3.cpp $
    
    1     4/03/00 5:39p Mikepc
    replaces TCCreateDDS.cpp
    contains code to convert TCLayers to compressed S3_TEXTURE
    
    2     2/21/00 1:56p Mikepc
    changed file to use newest (02/21/2000) version of s3tc.lib, S3_intrf.h
    and s3_ddraw.h
    1) changed #include from "s3_intrf.h" to "S3_intrf.h"
    2) added #include <string.h> for 'memset()' call
     ( formerly included in ddraw.h )
    3) removed 1 flag (DDSD_ALPHABITDEPTH) from 'ddsRGB.dwFlags' in
    TCConvertToDDS.  This flag is not defined by s3_ddraw.h
    
    1     12/03/99 3:45p Ryan
    
    17    11/15/99 6:32p Mikepc
    line 97:  changed color weights for s3 compression from 1/3 for each of
    r,g,b to values more representative of intensity.
    
    16    10/12/99 10:17p Howardc
    setup system/local includes correctly
    
    15    10/12/99 8:01p Mikepc
    changed include paths to vcc (tools/options) relative.
    
    14    10/08/99 2:45p Mikepc
    update for tplConv portability: altered data structures, replaced
    references to 'read tga code' with ' *fileFn, removed redundant
    functions.  Changed some file conversion paths.
    
    13    10/01/99 6:17p Mikepc
    
    12    10/01/99 12:20p Mikepc
    Integrated s3.lib code to generate s3 textures 'on the fly' as direct
    draw surfaces.  Removed .dds file reading code.  Changed CMP texture
    generation 'order of operations' to- rgb layer->s3->CMPR format per
    mipmap LOD.
    
    11    9/17/99 4:42p Mikepc
    ConvSi2Dds(): added a 'multiple of 4' dimension check to tga file
    before conversion to dds file.
    
    10    9/16/99 8:47p Mikepc
    updated code for auto-palette generation
    
    9     9/02/99 11:12a Mikepc
    some code re-organization between files.
    added code (verify.cpp) to invoke s3tc.exe from within tc program.
    changed some routines to accommodate the new texture creation path.
    
    8     8/30/99 5:07p Mikepc
    changes to dds/cmp processing code to allow odd sized images and images
    smaller than 8x8 texel.
    
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

#include<string.h>  // for memset()

#include <CharPipe/TCCommon.h>

#include "TCCreateS3.h"
#include "TCMem.h"
#include "TCLayerInternal.h"
#include "S3_intrf.h"


/*>*******************************(*)*******************************<*/
// convert an rgb and alpha layer to a cmp layer using s3.lib.

// note: the only cmp alpha we support is 1 bit color key- alphas
//       should be either 0 or 255 in the alpha layer
/*>*******************************(*)*******************************<*/
void TCConvertToS3( TCLayer* lyColor, TCLayer* lyAlpha, TCLayer* lyCmp )
{

    S3_TEXTURE s3RGB, s3CMP;  // input, output s3 textures
    S3_COLOR*  pClr;
	u32 row, col;
	u16 r, a;
	u8  g, b;
	u32 size;
    s32 ok = 0;

    // threshold alpha
    s32 alphaRef   = 1; 

	// quantization weights for compression.
	// note: these are the default weight from S3TC.doc
	float redWgt   = 0.3086f;
    float greenWgt = 0.6094f;
    float blueWgt  = 0.0820f; 

    // encode with 1-bit alpha
    u32 encodeFlag = S3TC_ENCODE_RGB_ALPHA_COMPARE;


	// zero out structures, then set only the fields we need
    memset( (void*)(&s3RGB), 0, sizeof(S3_TEXTURE) );
	memset( (void*)(&s3CMP), 0, sizeof(S3_TEXTURE) );

    //-------------------------------------------------------

	// combine the color and alpha layers into an rgba s3 texture

	s3RGB.lWidth   = lyColor->width;		
	s3RGB.lHeight  = lyColor->height;		
	s3RGB.lPitch   = lyColor->width * sizeof(S3_COLOR);		

	size           = lyColor->width * lyColor->height * sizeof(S3_COLOR);
	s3RGB.pSurface = (void*)TCCalloc( 1, size );           

	// s3RGB.ColorKey;		// color key is unused

	s3RGB.PixelFormat.nFlags        =  S3_TF_HASALPHA;
	s3RGB.PixelFormat.nARGBBitCount =  32;

    // pixel masks match rgba packing order of pSurface buffer
	// pSurface color is array of u8[4] = {r,g,b,a}. Masks are little-endian.
    s3RGB.PixelFormat.nRedMask      =  0x000000FF;
    s3RGB.PixelFormat.nGreenMask    =  0x0000FF00;
    s3RGB.PixelFormat.nBlueMask     =  0x00FF0000;
    s3RGB.PixelFormat.nAlphaMask    =  0xFF000000;

	// s3RGB.pPalette;		//palette is unused 

    //-------------------------------------------------------

	// combine both layers into the pSurface buffer
	pClr = (S3_COLOR*)( s3RGB.pSurface );

	for( row = 0; row < lyColor->height; row++ )
	{
		for( col = 0; col < lyColor->width; col++ )
		{
			TCGetLayerValue( lyColor, col, row, &r, &g, &b );

			a = 255;  // if no alpha layer, set a to max.
			if( lyAlpha != NULL )
			{
				TCGetLayerValue( lyAlpha, col, row, &a, NULL, NULL );
			}

			// pack as S3_COLORs
            pClr->cRed   = (char)r;
            pClr->cGreen = (char)g;
            pClr->cBlue  = (char)b;
            pClr->cAlpha = (char)a;
            pClr++;
		}
	}
	
    //-------------------------------------------------------

	// create a cmp texture from the rgb texture
	// and a cmp color layer from the cmp texture.
	// S3TCencode() can compress directly to the layer buffer.

    // set color weights
    S3TC_SetColorWeighting( redWgt, greenWgt, blueWgt );

    // set threshold for alpha compare
    S3TC_SetAlphaReference( alphaRef );

	// get the required buffer size.
	// encodeFlag signals 1-bit alpha encoding
    size = S3TC_GetEncodeSize( s3RGB.lWidth, s3RGB.lHeight, encodeFlag );

    // set cmp layer attributes
	lyCmp->type   = LY_IMAGE_COLOR_CMP;
    lyCmp->width  = lyColor->width;             
    lyCmp->height = lyColor->height;                 

	// note: set 'data' explicitly ( not with lySetBuffer )
	//       to use the size returned by S3TCgetEncodeSize
	lyCmp->data = (u8*)TCCalloc( 1, size );

    // compress the texture
    ok = S3TC_Encode( &s3RGB, &s3CMP, lyCmp->data, encodeFlag, NULL, NULL, NULL );

    TCAssertMsg( (ok==0), "error: TCConvertToDDS: s3 encoding failed\n" );

	//---------------------------------------------------------------------------

	// free the input surface memory
	TCFree( (void**)( &s3RGB.pSurface ) );
}

/*>*******************************(*)*******************************<*/
