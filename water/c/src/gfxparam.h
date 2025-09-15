/**********************************************************
 * Copyright (C) 1997 Mark Theyer
 * All Rights Reserved
 **********************************************************
 * Project:	GFX library
 **********************************************************
 * File:	gfxparam.h
 * Author:	Mark Theyer
 * Created:	30 Sep 1999
 **********************************************************
 * Description:	PS2 engine custom parameters
 **********************************************************
 * Revision History:
 * 30-Sep-99	Theyer	Initial coding.
 **********************************************************/

#ifndef THEYER_GFXPARAM_H
#define THEYER_GFXPARAM_H

/*
 * includes
 */

/*
 * macros
 */

/* limits for PS2 GFX engine */

/* objects */
#define PS2_MAX_OBJECT				     	  512

/* textures */
#define PS2_MAX_TEXTURES					  256

/* sprites */
#define PS2_MAX_SPRITES						  256

/* internal files */
#define PSX_MAX_INTERNAL_FILE_REGISTER		  256

/* file buffer - big enough for a 512x256 16bit image */
#define PS2_FILEBUFFER_SIZE				 0x200000		// 2Meg

/* drawing code buffer */
#define PS2_DATABUFFER_SIZE			     0x400000		// 4Meg

/* end of customisable limits */

#endif	// THEYER_GFXPARAM_H
