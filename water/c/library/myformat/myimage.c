/*****************************************************************
 * Project		: Myformat Library
 *****************************************************************
 * File			: myimage.c
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 27 Jun 1998
 * Last Update	: 27 Jun 1998
 *****************************************************************
 * Description	: my image format (MIF) library functions
 *****************************************************************/

/*
 * includes
 */

#include <stdio.h>
#include <gfx/gfx.h>
#include <memory/memory.h>
#include <text/text.h>
#include <binary/binary.h>
#include "myformat.h"

/*
 * macros
 */

#define BINARY_ONLY		TRUE

/*
 * typedefs
 */

/*
 * prototypes
 */


/*
 * variables
 */


/* 
 * globals variables...
 */


/*
 * functions
 */


/******************************************************************************
 * Function:
 * mifTextureToGfxTexture -- create a GFX texture from a MIF texture
 * 
 * Description:
 *
 * Returns:
 * 
 */

GfxTextureId mifTextureToGfxTexture ( 
	Text	 filename,
	Bool	 reload
	)
{
	//MifTexture		*tex;
	GfxColorMapId		 cmapid;
	GfxTextureFormat	 format;
	GfxTextureId		 texid;
	Text				 fileid;
	Text				 cmapname;
	char				 cmapnamestr[32];
	char				 imagename[32];
	int					 i, len;
	int					 width, height;
	Byte				*data;
	Byte				*image;

	/* validate */
	if ( filename == NULL || *filename == '\0' ) 
		return( GFX_NO_TEXTURE );

	/* save image name */
	for ( i=0; i<31 && filename[i] != '\0'; i++ )
		imagename[i] = filename[i];
	imagename[i] = '\0';
	/* check if this texture is already loaded */
	if ( ! reload ) {
		//printf( "searching for image: %s...", imagename );
		texid = gfxTextureId( imagename );
		//printf( "done\n" );
		if ( texid != GFX_NO_TEXTURE ) {
			//printf( "image %d name %s is loaded\n", texid, imagename );
			return( texid );
		}
	}

	/* load file */
	//printf( "load image: %s\n", imagename );
	data = gfxFileLoad( imagename );
	//printf( "image loaded: %X\n", data );
	if ( data == NULL ) return( GFX_NO_TEXTURE );
	/* validate file is an MIB image */
	fileid = binReadString( 4, data );
	//printf( "file id = %s\n", fileid );
	if ( txtMatch( fileid, MIB_HEADER ) ) 
		image = (data + 28);
	else if ( txtMatch( fileid, MIF_HEADER ) )
		image = (data + 28);
	else {
		//printf( "this is not an MIB or MIF file...\n" );
		return( GFX_NO_TEXTURE );
	}
	
	/* get colormap name from image and load it first */
	cmapname = binReadString( 16, (data + 12) );
	//printf( "colormap name = %s\n", cmapname );
	cmapid = GFX_NO_TEXTURE;
	if ( cmapname[0] != '\0' ) {
		/* save name into local buffer */
		txtCopy( cmapname, cmapnamestr );
		/* add binary colormap (CMB) file extension */
		len = txtLength( cmapnamestr );
		txtCopy( ".cmb", &cmapnamestr[len] );
		//printf( "load colormap: %s...", cmapnamestr );
		cmapid = mifTextureToGfxTexture( cmapnamestr, FALSE );
		/* check for error */
		if ( cmapid == GFX_NO_TEXTURE ) return( GFX_NO_TEXTURE );
		/* will need to reload image if colormap was loaded */
		data = gfxFileLoad( imagename );
	}

	/* validate file is in buffer */
	fileid = binReadString( 4, (data) );
	//printf( "file id = %s\n", fileid );
	if ( ! txtMatch( fileid, MIB_HEADER ) && ! txtMatch( fileid, MIF_HEADER ) ) {
		//printf( "this is not an MIB file...\n" );
		return( GFX_NO_TEXTURE );
	}
	format = binReadInt( (data + 4) );
	width  = binReadShort( (data + 8) );
	height = binReadShort( (data + 10) );
	//printf( "image format=%d, width=%d, height=%d\n", format, width, height );
	switch( format ) {
	case MIF_TEXTURE_8BIT:
		format = GFX_TEXTURE_8BIT;
		break;
	case MIF_TEXTURE_16BIT:
		format = GFX_TEXTURE_16BIT;
		break;
	default:
		//printf( "unsupported MIF image format\n" );
		return( GFX_NO_TEXTURE );
	}
	texid = gfxLoadTexture( imagename, (void *)image, format, width, height, cmapid );
	//printf( "image  loaded: %s, id = %d, width = %d, height = %d, cmap = %d\n", txtToUpper(imagename), texid, width, height, cmapid );

	return( texid );
}
