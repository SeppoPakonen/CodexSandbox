/***************************************************************** 
 * Project		: Game Development
 *****************************************************************
 * File			: bmp.h
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 18 Nov 1999
 *****************************************************************
 * Description	: Windows BMP format file library functions
 *****************************************************************
 * 28/12/96		: Initial coding.
 *****************************************************************/

#ifndef THEYER_BMP_H
#define THEYER_BMP_H

#include <type/datatype.h>

/*
 * defines
 */

/* BMP header */
typedef struct	{
	char		b,m;
	int			file_size;
	int			reserved;
	int			image_offset;
} BmpHeader;

/* BMP Information header */
typedef struct	{
	int				header_size;
	int				width;
	int				height;
	unsigned short	bit_planes;
	unsigned short	bits_per_pixel;
	int				compression;
	int				image_size;
	int				x_pixels_per_metre;
	int				y_pixels_per_metre;
	int				colors_used;
	int				num_important_colors;
} BmpInfoHeader;

/*
 * prototypes 
 */

#endif
