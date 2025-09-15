/**********************************************************
 * Copyright (C) 1998 Mark Theyer
 * All Rights Reserved
 **********************************************************
 * Project:	TGA library
 **********************************************************
 * File:	tga.c
 * Author:	Mark Theyer
 * Created:	18 Apr 1998
 **********************************************************
 * Description:	Read a TGA file
 **********************************************************
 * Revision History:
 * 18-Apr-98	Theyer	Initial Coding
 * 28-Mar-00	Theyer	Handle extended information in file
 **********************************************************/

/*
 * includes
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include "tga.h" 

#include "compile.h"

/*
 * typedefs
 */

/*
 * macros
 */

/*
 * prototypes
 */

int	intel_convert( word bigedian );

/*
 * global variables
 */

/*
 * functions
 */


/**********************************************************
 * Function:	tgaOpen
 **********************************************************
 * Description: Read in a TGA file
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

TgaFile * tgaOpen ( 
	char 	 *filename,
	char	**errstr 
	)
{
	FILE		*infile;
	TgaHeader	 header;	
	TgaFile		 pic;
	TgaFile		*rpic;
	size_t		 read;
	byte		*buffer,
				*pdata;
	int			 buffer_size,
				 j, x, y,
				 pixel_size,
				 offset;
	int			 scale = 1;

	/* open file */
	infile = fopen( filename, "rb" );
	if ( infile == NULL ) {
    	*errstr = "cannot open file";
		return( NULL );        
    }

	/* read in the standard header */
	read = fread((void *)&header, sizeof(TgaHeader), (size_t)1, infile );
	if ( read != 1 ) {
		*errstr = "header read failed";  	
		fclose( infile );
		return( NULL );
	}

	/* only supports TGA_TRUECOLOR_UNCOMPRESSED image types... */	
	if ( header.image_type != TGA_TRUECOLOR_UNCOMPRESSED ) {
		*errstr = "can only process type 2 images";
		fclose( infile );
		return( NULL );
	}

	/* read id */		
	if ( header.id_field_length != 0 ) {
		fgets( pic.id, header.id_field_length, infile );
		/* printf("ID = %s\n", pic.id ); */
	} else	{
		pic.id[0] = '\0';
	}
	
	/* convert targa header data and initialise new structure */
	pic.x = intel_convert( header.x_origin );
	pic.y = intel_convert( header.y_origin );
	pic.width = intel_convert( header.width );
	pic.height = intel_convert( header.height );
	pic.bits_per_pixel = (int)header.bits_per_pixel;

	/* only process 24 or 32 bit files */
	if ( pic.bits_per_pixel == 32 || pic.bits_per_pixel == 24 )
		{
		pic.size = pic.width * pic.height * 3;
	} else	{
		*errstr = "bad bits per pixel"; 	
		fclose( infile );	
		exit(1);
	}

	/* allocate memory */
	buffer_size = pic.width * pic.height * pic.bits_per_pixel/8;
	buffer = (byte *)malloc((size_t)(buffer_size*sizeof(byte)));
	pic.data = (byte *)malloc((size_t)(pic.size*sizeof(byte)));

	/* read in rgb data from the targa file into the buffer */
	read = fread((void *)buffer, sizeof(byte), 
				(size_t)buffer_size, infile ); 	
	j = ferror( infile );
	j = feof( infile );
	fclose( infile );
#if 0
	if ( ((int)read) != pic.size ) {
#else
	/* (handle extended information in file) */
	if ( ((int)read) < pic.size ) {
#endif
		*errstr = "data read failed";
		free(buffer);
		free(pic.data);
		return( NULL );
	}

	/* convert the buffer data into RGB format */
	pdata = pic.data;
	j = 0;
	pixel_size = pic.bits_per_pixel/8;
	for ( y = (pic.height - 1); y >= 0; y-= scale ) {
	    offset = y * (pic.width*pixel_size);
	    for ( x = 0; x<=((pic.width*pixel_size)-pixel_size);
		  	  x += (scale*pixel_size) ) {	
			*(pdata+j) = buffer[offset+x+2]; j++;
			*(pdata+j) = buffer[offset+x+1]; j++;
			*(pdata+j) = buffer[offset+x]; j++;
	    }
	}
    free(buffer);
	
	/* copy to new memory  */	
	rpic = (TgaFile*)malloc(sizeof(TgaFile));
	*rpic = pic;

	return( rpic );
}


/**********************************************************
 * Function:	intel_convert
 **********************************************************
 * Description: convert 16 bit big-edian intel format 
 *				integer to a 32 bit integer
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

int	intel_convert( word bigedian )
{
	short	*svalue;
	int		 ivalue;

#ifdef OsTypeIsUnix
	return( ((int)(bigedian.upper * 256 + bigedian.lower)) );
#else
	svalue = (short *)&bigedian;
	ivalue = (int)*svalue;
	return( ivalue );
#endif
}

