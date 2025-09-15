/**********************************************************
 * Copyright (C) 1999, 2000 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:	PSX Memory card interface
 **********************************************************
 * File:	mcrdpsx.h
 * Author:	Mark Theyer
 * Created:	8 Feb 2000
 **********************************************************
 * Description:	PSX Memory card interface
 **********************************************************
 * Functions:
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 08-Feb-00	Theyer	Initial coding.
 *
 **********************************************************/

#ifndef THEYER_MCRDPSX_H
#define THEYER_MCRDPSX_H

/*
 * includes
 */

#include <type/datatype.h>

/*
 * typedefs
 */

typedef struct {
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

#define PSX_MCARD_INVALID_PORT		  -1

#define PSX_MCARD_PORT0				0x00
#define PSX_MCARD_PORT1				0x10
#define PSX_MCARD_PORT0_TAP0		0x00
#define PSX_MCARD_PORT0_TAP1		0x01
#define PSX_MCARD_PORT0_TAP2		0x02
#define PSX_MCARD_PORT0_TAP3		0x03
#define PSX_MCARD_PORT1_TAP0		0x10
#define PSX_MCARD_PORT1_TAP1		0x11
#define PSX_MCARD_PORT1_TAP2		0x12
#define PSX_MCARD_PORT1_TAP3		0x13

/*
 * prototypes
 */

#endif //THEYER_MEMCARD_H

