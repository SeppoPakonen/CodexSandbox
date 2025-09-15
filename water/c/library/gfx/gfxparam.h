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
 * Description:	PSX engine custom parameters
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

/* limits - custom for each project */

/* textures */
#define PSX_MAX_TEXTURES					  256
/* sprites */
#define PSX_MAX_SPRITES						  256
/* internal file register */
#define PSX_MAX_INTERNAL_FILE_REGISTER		  128
/* static (full life) objects */
#define PSX_MAX_OBJECT		     			  200
#define PSX_PRIMITIVE_BUFFER_SIZE		    40000
#define PSX_MAX_VERTEX		  				 9000
#define PSX_MAX_NORMAL		   				 9000	// 4000
/* file buffer - big enough for a 512x256 16bit image */
#define PSX_FILEBUFFER_SIZE				   264000
/* additional object handlers for rendering the same model <n> times each frame */
#define PSX_MAX_EXTRAHANDLERS		    	 1000
/* GPU work area packet space size */
#define PSX_GPU_WORK_AREA_SIZE			   131072
/* size of ordering table: 1 << PSX_OT_LENGTH */ 
#define PSX_OT_LENGTH						   14

/* end of customisable limits */

#endif	// THEYER_GFXPARAM_H
