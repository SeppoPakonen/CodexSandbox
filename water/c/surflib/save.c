/**********************************************************
 * Copyright (C) 1999,2000 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:	PlayStation surf game
 **********************************************************
 * File:	save.c
 * Author:	Mark Theyer
 * Created:	1 Nov 1999
 **********************************************************
 * Description:	
 **********************************************************
 * Functions:
 *	surfMemCardSave() 
 *				Save data to memory card.
 *	surfMemCardNameConvert()
 *				Convert name to memory card name format
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 01-Nov-99	Theyer	Initial coding.
 *
 **********************************************************/

/*
 * includes
 */

#include <type/datatype.h>
#include <memory/memory.h>
#include <gfx/gfx.h>

/*
 * typedefs
 */

typedef struct {
	Byte	header[128];
	Byte	magic[2];
	Byte	type;
	Byte	nslots;
	char	name[64];
	Byte	pad[28];
	Byte	clut[32];
	Byte	icon_image1[128];
	Byte	data;				// 128 bytes x nslots
} PsxMemCardHeaderType1;

typedef struct {
	Byte	header[128];
	Byte	magic[2];
	Byte	type;
	Byte	nslots;
	char	name[64];
	Byte	pad[28];
	Byte	clut[32];
	Byte	icon_image1[128];
	Byte	icon_image2[128];
	Byte	data;				// 128 bytes x nslots
} PsxMemCardHeaderType2;

typedef struct {
	Byte	header[128];
	Byte	magic[2];
	Byte	type;
	Byte	nslots;
	char	name[64];
	Byte	pad[28];
	Byte	clut[32];
	Byte	icon_image1[128];
	Byte	icon_image2[128];
	Byte	icon_image3[128];
	Byte	data;				// 128 bytes x nslots
} PsxMemCardHeaderType3;

/*
 * macros
 */

#define PSX_MCARD_ICON_IMAGES_1		0x11
#define PSX_MCARD_ICON_IMAGES_2		0x12
#define PSX_MCARD_ICON_IMAGES_3		0x13

/*
 * prototypes
 */

/*
 * global variables
 */

/*
 * functions
 */          

/**********************************************************
 * Function:	surfMemCardSave
 **********************************************************
 * Description: Save data to memory card
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void surfMemCardSave (
	Text	 name,
	Byte	*data,
	int		 size
	)
{
	PsxMemCardHeaderType1	mcard_header;
	int						nslots;

	/* calc number of 128 byte slots required for save */
	nslots = (size / 128);
	if ( size % 128 )
		nslots++;

	/* init header */
	memClear( &mcard_header, sizeof(PsxMemCardHeaderType1) );
	mcard_header.magic[0] = 'S';
	mcard_header.magic[1] = 'C';
	mcard_header.type = PSX_MCARD_ICON_IMAGES_1;
	mcard_header.nslots = nslots;
	surfMemCardNameConvert( name, mcard_header.name );
	/* clut, image to do... */
}


/**********************************************************
 * Function:	surfMemCardNameConvert
 **********************************************************
 * Description: Convert name to memory card name format
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void surfMemCardNameConvert (
	Text	name,
	Byte	mname
	)
{
	mname[0] = 'H';
	mname[1] = 'I';
}

