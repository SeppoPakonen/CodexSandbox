/*---------------------------------------------------------------------*

Project:  tc library
File:     TCLayer.cpp

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:

 $Log: /Dolphin/build/charPipeline/tc/src/TCLayer.cpp $
    
    3     8/10/00 6:02p Mikepc
    
    2     3/17/00 1:19p Mikepc
    change tc to use indices numbered from 0.
    
    1     12/03/99 3:45p Ryan
    
    15    10/08/99 2:45p Mikepc
    update for tplConv portability: altered data structures, replaced
    references to 'read tga code' with ' *fileFn, removed redundant
    functions.  Changed some file conversion paths.
    
    14    10/01/99 12:20p Mikepc
    Integrated s3.lib code to generate s3 textures 'on the fly' as direct
    draw surfaces.  Removed .dds file reading code.  Changed CMP texture
    generation 'order of operations' to- rgb layer->s3->CMPR format per
    mipmap LOD.
    
    13    9/16/99 8:47p Mikepc
    updated code for auto-palette generation
    
    12    9/02/99 11:12a Mikepc
    some code re-organization between files.
    added code (verify.cpp) to invoke s3tc.exe from within tc program.
    changed some routines to accommodate the new texture creation path.
    
    11    8/30/99 5:07p Mikepc
    changes to dds/cmp processing code to allow odd sized images and images
    smaller than 8x8 texel.
    
    10    8/30/99 11:19a Mikepc
    added a descriptive comment to a necessary but misleading code block in
    MakeLayer() (line 50).  The comment describes the difference between
    the layer type passed to the function and the one that is finally set
    from within the function.
    
    9     8/26/99 4:59p Mikepc
    renamed file extensions from .c to .cpp.
    .cpp extension allows addition of namespace protection to remove
    potential name collisions with tool code.  Exceptions are CreateTplFile
    and QuickConvert.  These are extern "C" linked.
    
    8     8/26/99 11:38a Mikepc
    
    7     8/26/99 11:03a Mikepc
    tplCon rewrite for efficient memory usage, batch file processing
    ability.
    
  
 $NoKeywords: $

-----------------------------------------------------------------------*/

#include <CharPipe/TCPalTable.h>
#include <CharPipe/TCCommon.h>

#include "TCLayerInternal.h"
#include "TCMem.h"
#include "TCGXDefs.h"

/*>*******************************(*)*******************************<*/
static void TCSetLayerType		( TCLayer* ly, u32 type );
static void TCSetLayerDimensions( TCLayer* ly, u32 width, u32 height ); 
static void TCCopyLayer			( TCLayer* src, TCLayer* dst );

/*>*******************************(*)*******************************<*/
TCLayer* TCCreateLayer ( void )
{
	TCLayer* newLayer;

	newLayer = (TCLayer*)TCCalloc( 1, sizeof(TCLayer));

	return newLayer;
}

/*>*******************************(*)*******************************<*/
void TCSetLayerAttributes ( TCLayer* ly, u32 type, u32 width, u32 height )
{
    TCAssertMsg( (ly     != NULL), "TCSetLayerAttributes: NULL layer ptr\n" );
    TCAssertMsg( (width  <= 1024), "TCSetLayerAttributes: layer width exceeds 1024 texels\n" );
    TCAssertMsg( (height <= 1024), "TCSetLayerAttributes: layer width exceeds 1024 texels\n" );

    switch( type )
    {
    case LY_IMAGE_COLOR_RGB24:      // fall through
    case LY_IMAGE_COLOR_CI16:
    case LY_IMAGE_ALPHA_A8:
    case LY_IMAGE_COLOR_CMP:
       
    	TCSetLayerType(      ly, type         );		
    	TCSetLayerDimensions(ly, width, height);
        break;

    default:
        TCErrorMsg( "TCSetLayerAttributes: invalid layer type\n" );
        break;
    }
} 

/*>*******************************(*)*******************************<*/
// note:  format and dimensions must already be set 
/*>*******************************(*)*******************************<*/
u8* TCSetLayerBuffer ( TCLayer* ly )
{	
	u32 size;


    TCAssertMsg( (ly != NULL), "TCSetLayerBuffer: NULL layer ptr\n"          );
    TCAssertMsg( (ly->width ), "TCSetLayerBuffer: layer width is not set\n"  );
    TCAssertMsg( (ly->height), "TCSetLayerBuffer: layer height is not set\n" );

	// free buffer if previously allocated
	TCFree( (void**)(&ly->data) ) ;

	// compute total memory size of base image 
	// and any mipmaps included in this layer
	size = 0;
	switch(ly->type)
	{
	case LY_IMAGE_COLOR_RGB24:
		size = ( ly->width * ly->height * 3 );
        break;
 
	case LY_IMAGE_COLOR_CI16:
		size = ( ly->width * ly->height * 2 );
		break;

    case LY_IMAGE_ALPHA_A8:  
 		size = ( ly->width * ly->height );
        break;

	case LY_IMAGE_COLOR_CMP:
		// 4 bits per texel 
		// 4x4 texel block minimum

		//       ( width      x height      x 4 bpp / 8bits per byte
		size = ( ( ly->width  * ly->height  * 4 )   >> 3 );
		if( size < 8 )
		{
			size = 8;
		}
		break;

	default:                     // in case ly->type wasn't set
		TCErrorMsg( "TCSetLayerBuffer: unknown layer type\n" );
		return 0;
		break;		
	}
	
	ly->data = (u8*)TCCalloc( 1, size );
			
	return ly->data;	
}
	
/*>*******************************(*)*******************************<*/
// set an individual pixel value based on an x,y location
// actual internal data representation depends on the buffer format:
// rgb color is stored as rgb triples -> u8[3] = { R, G, B }
//
// 'ria' is an unsigned short to accomodate 14-bit color index indices
//
// NOTE: assume (x,y) coordinates start at (0,0)
/*>*******************************(*)*******************************<*/
void TCSetLayerValue ( TCLayer* ly, u32 x, u32 y, u16 ria, u8 g, u8 b )
{
    u32 offset;
    u8* basePtr;


    TCAssertMsg( (ly != NULL),       "TCSetLayerValue: NULL layer ptr\n"            );
    TCAssertMsg( (ly->width ),       "TCSetLayerValue: layer width is not set\n"    );
    TCAssertMsg( (ly->data != NULL), "TCSetLayerValue: layer data ptr is not set\n" );
 	
	basePtr  = (u8*)(ly->data);
	
    switch(ly->type)
    {
    case LY_IMAGE_COLOR_RGB24:

    	offset   = ( (y * ly->width) + x ) * 3;

        *( basePtr + offset     )  = (u8)ria;
        *( basePtr + offset + 1 )  = g;  
        *( basePtr + offset + 2 )  = b; 
        break;
		
	case LY_IMAGE_COLOR_CI16:  // this is the only 16-bit format
		
		offset = ((y * ly->width) + x) * 2;  // offset in bytes
		*(u16*)(basePtr + offset) = ria;
		break;

    case LY_IMAGE_ALPHA_A8:

        offset               = (y * ly->width) + x;
        *(basePtr + offset)  = (u8)ria; 
        break;

	case LY_IMAGE_COLOR_CMP:     // cant return a value from compressed texture yet
		TCErrorMsg( "TCsetLayerValue: can't 'set' a texel value from a CMPR layer\n" );
		return;
		break;

    default:                 // in case format was not set
		TCErrorMsg( "TCSetLayerValue: unknown layer type\n" );
        return;
        break;         
    }  // end switch
   
}

/*>*******************************(*)*******************************<*/
// retrieve color information from the given layer depending on its 
// format
/*>*******************************(*)*******************************<*/
void TCGetLayerValue ( TCLayer* ly, u32 x, u32 y, u16* riaPtr, u8* gPtr, u8* bPtr )
{
    u32 offset;
    u8* basePtr;


    TCAssertMsg( (ly != NULL),       "TCGetLayerValue: NULL layer ptr\n"            );
    TCAssertMsg( (ly->width ),       "TCGetLayerValue: layer width is not set\n"    );
    TCAssertMsg( (ly->data != NULL), "TCGetLayerValue: layer data ptr is not set\n" );
    TCAssertMsg( (riaPtr != NULL),   "TCGetLayerValue: NULL riaPtr\n"               );
   	
	basePtr  = (u8*)(ly->data);
	
    switch(ly->type)
    {
    case LY_IMAGE_COLOR_RGB24:

    	offset    = ( (y * ly->width) + x ) * 3;            
        *riaPtr   = (u16)( *( basePtr + offset ) );
        
        if( gPtr != NULL )
        {
        	*gPtr = *( basePtr + offset + 1 );  
       	}
        if( bPtr != NULL )
       	{
        	*bPtr = *( basePtr + offset + 2 ); 
       	}
        break;
		
	case LY_IMAGE_COLOR_CI16:  // this is the only 16-bit format
		
		offset  = ((y * ly->width) + x) * 2;  // offset in bytes
		*riaPtr = *((u16*)(basePtr + offset));
		break;
		
    case LY_IMAGE_ALPHA_A8:

        offset  = (y * ly->width) + x;
        *riaPtr = (u16)(*(basePtr + offset)); 
        break;

	case LY_IMAGE_COLOR_CMP:  // cant set a compressed pixel yet

		TCErrorMsg( "TCSetLayerValue: can't 'set' a CMPR texel value\n" );
		return;
		break;

    default:                 // in case type wasn't set
		TCErrorMsg( "TCSetLayerValue: unknown layer type- can't set value\n" );
        return;
        break;           
    } 
 
}

/*>*******************************(*)*******************************<*/
void TCMakeImColorLayer ( TCFile* dfPtr, TCLayer* newLy )
{
	u32 row, col;
	u16 r;
	u8  g,   b;


    TCAssertMsg( (dfPtr != NULL), "TCMakeImColorLayer: NULL TCFile ptr\n" );
    TCAssertMsg( (newLy != NULL), "TCMakeImColorLayer: NULL layer ptr\n"  );
 
	// copy the layer attributes, but set an independent buffer
	newLy->type   = dfPtr->lyColor->type; 
	newLy->width  = dfPtr->lyColor->width; 
	newLy->height = dfPtr->lyColor->height;
	newLy->data   = NULL;

	TCSetLayerBuffer( newLy );

	// copy over the data
	for( row = 0; row< newLy->height; row++ )
	{
		for( col = 0; col < newLy->width; col++ )
		{
			TCGetLayerValue(dfPtr->lyColor, col, row, &r, &g, &b);
			TCSetLayerValue(newLy, col, row, r,   g,  b);
		}
	}
}

/*>*******************************(*)*******************************<*/
void TCMakeImAlphaLayer( TCFile* dfPtr, TCLayer* newLy )
{
	u32 row, col;
	u16 a;


    TCAssertMsg( (dfPtr != NULL), "TCMakeImAlphaLayer: NULL TCFile ptr\n" );
    TCAssertMsg( (newLy != NULL), "TCMakeImAlphaLayer: NULL layer ptr\n"  );

	// copy the layer attributes, but set an independent buffer
	newLy->type   = dfPtr->lyAlpha->type; 
	newLy->width  = dfPtr->lyAlpha->width; 
	newLy->height = dfPtr->lyAlpha->height;
	newLy->data   = NULL;

	TCSetLayerBuffer( newLy );

	// copy over the data
	for( row = 0; row< newLy->height; row++ )
	{
		for( col = 0; col < newLy->width; col++ )
		{
			TCGetLayerValue(dfPtr->lyAlpha, col, row, &a, NULL, NULL);
			TCSetLayerValue(newLy, col, row,  a, NULL, NULL);
		}
	}
}

/*>*******************************(*)*******************************<*/
// convert an image CI layer to an rgb24 layer
// leave the image alpha layer as is.
/*>*******************************(*)*******************************<*/
void TCConvertCI_To_RGB ( TCFile* dfPtr, TCImage* imPtr )
{
	TCLayer  lyColor;
    u16      index;
	u32      row, col;
	u8       r, g, b;


    TCAssertMsg( (dfPtr->palPtr != NULL), "TCConvertCI_To_RGB: TCFile %s has no palette\n", dfPtr->name ); 

    if( imPtr->lyColor.type != LY_IMAGE_COLOR_CI16 )
    {
        TCErrorMsg( "TCConvertCI_To_RGB: image %d color layer type is not color-index\n", imPtr->index ); 
    }

	TCSetLayerAttributes( &lyColor, LY_IMAGE_COLOR_RGB24, imPtr->lyColor.width, imPtr->lyColor.height ); 
	lyColor.data = NULL;
	TCSetLayerBuffer( &lyColor );

    // perform color-table lookups
	for( row = 0; row < imPtr->lyColor.height; row++ )
	{
		for( col = 0; col < imPtr->lyColor.width; col++ )
		{
			// fetch the index, perform the color look-up, convert to rgb
			TCGetLayerValue( &imPtr->lyColor, col, row, &index, NULL, NULL );

            // ignore 'a' value; leave image alpha layer as is.
			TCGetPalTableValue( dfPtr->palPtr, index, &r, &g, &b, NULL );

			TCSetLayerValue( &lyColor, col, row, (u16)r, g, b );
		}
	}

	// free the original CI buffer, copy over the new color layer 
	TCFree( (void**)( &imPtr->lyColor.data ) );
	TCCopyLayer( &lyColor, &imPtr->lyColor );

}

/*>*******************************(*)*******************************<*/
static void TCSetLayerType( TCLayer* ly, u32 type )
{

    TCAssertMsg( (ly != NULL), "TCSetLayerType, Null TCLayer ptr\n" );
		
	switch( type )
	{
	case LY_IMAGE_COLOR_RGB24:
	case LY_IMAGE_COLOR_CI16:       
	case LY_IMAGE_ALPHA_A8:
	case LY_IMAGE_COLOR_CMP:
	
		ly->type = type;
		break;
	default:
		TCErrorMsg( "TCSetLayerType: unknown layer type\n" );
		return;
		break;
	}
}
	
/*>*******************************(*)*******************************<*/	
static void TCSetLayerDimensions( TCLayer* ly, u32 width, u32 height )
{
	
    TCAssertMsg( (ly != NULL), "TCSetLayerDimensions: NULL TCLayer ptr\n" );

    if( (width == 0)  || (width > 1024)  )
        TCErrorMsg( "TCSetLayerDimensions: layer width is out of range\n" );

    if( (height == 0) || (height > 1024) )
        TCErrorMsg( "TCSetLayerDimensions: layer height is out of range\n" );

	ly->width  = width;
	ly->height = height;	
}

/*>*******************************(*)*******************************<*/
// copy the contents of a layer including the data pointer
/*>*******************************(*)*******************************<*/
static void TCCopyLayer( TCLayer* src, TCLayer* dst )
{
	if( (src == NULL) || (dst == NULL ) )
	{
		return;
	}

	dst->type       = src->type;       
    dst->width      = src->width;         
    dst->height     = src->height;          
    dst->data       = src->data; 
}

/*>*******************************(*)*******************************<*/