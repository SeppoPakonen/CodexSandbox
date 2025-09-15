/***************************************************************** 
 * Project		: Game Development
 *****************************************************************
 * File			: tga.h
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 18 Apr 1998
 * Last Update	: 18 Apr 1998
 *****************************************************************
 * Description	: All GUI definitions in here 
 *****************************************************************
 * 28/12/96		: New Version.
 *****************************************************************/

#ifndef TGA_H
#define TGA_H

/*
 * Targa image types
 */

#define	TGA_NO_IMAGE_PRESENT			 0
#define	TGA_COLORMAPPED_UNCOMPRESSED	 1
#define	TGA_TRUECOLOR_UNCOMPRESSED		 2
#define	TGA_BLACKWHITE_UNCOMPRESSED		 3
#define	TGA_COLORMAPPED_RLECOMPRESSED	 9
#define	TGA_TRUECOLOR_RLECOMPRESSED		10	
#define	TGA_BLACKWHITE_RLECOMPRESSED	11

/*
 *
 * Structures
 *
 * byte and word structures are used in the header as they are
 * 8 and 16 bit quantities (our integers are 32 bit!)
 *
 * 16 bit words need to be converted from Intel "big-edian" format
 * into integers so I can make sense of things.
 *
 */

typedef	unsigned char	byte;
typedef	struct	{
	byte	lower,
			upper;
} word;


/*
 * The standard Targa header
 */
typedef struct	{
	byte		id_field_length,
				colormap_type,
				image_type;
	word 		color_map_entry,
				color_map_length;
	byte		color_map_size;
	word		x_origin,
				y_origin,
				width,
				height;
	byte		bits_per_pixel,
				descriptor_bits;
} TgaHeader;


/*
 *
 * The structure created from converted data in the 
 * standard header file
 *
 */
typedef struct	{
	char	title[30],
			id[255];
	int		scale,
			size,
			bits_per_pixel,
			x, 
			y,
			width,
			height;
	byte   *data;
} TgaFile;

/*
 * prototypes 
 */

extern TgaFile * tgaOpen( char *filename, char **errstr );

#endif
