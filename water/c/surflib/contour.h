/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		contour.h
 * Author:		Mark Theyer
 * Created:		13 Jul 1999
 **********************************************************
 * Description:	Header file for surface contour data for
 *				wave generation system.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 13-Jul-99	Theyer	Initial coding from new wave system
 * 31-May-01	Theyer	Initial coding for PS2
 *
 **********************************************************/

#ifndef SURF_CONTOUR_H
#define SURF_CONTOUR_H

/* 
 * includes
 */

#include <stdio.h>

#include <compile.h>
#include <type/datatype.h>
#include <memory/memory.h>
#include <parse/parse.h>
#include <gfx/gfx.h>

#include <surfutil.h>

/* 
 * macros 
 */

#define SURF_CONTOUR_MAX_X				84
#define SURF_CONTOUR_MAX_Y				64

#define SURF_CONTOUR_STEP_X			  1024		// must be power of 2 to allow masking
#define SURF_CONTOUR_STEP_Y			  1024		// must be power of 2 to allow masking
#define SURF_CONTOUR_DIVIDE_X		    12		// shift value right for fast/shortcut division
#define SURF_CONTOUR_DIVIDE_Y		    12		// shift value right for fast/shortcut division
//#define SURF_CONTOUR_FACTOR_X			 0.0009765625f		// 1/1024
//#define SURF_CONTOUR_FACTOR_Y			 0.0009765625f		// 1/1024
#define SURF_CONTOUR_FACTOR_X			64	// 0.0009765625f for 16 bit fixed value (one=65536)
#define SURF_CONTOUR_FACTOR_Y			64	// 0.0009765625f for 16 bit fixed value (one=65536)

/*
 * typedefs
 */

// surface contour
typedef struct {
	int		size_x;
	int		size_y;
	int		limit_x;
	int		limit_y;
	int		width;
	int		normalise;
	short	zval[SURF_CONTOUR_MAX_Y][SURF_CONTOUR_MAX_X]; // grid of points
} SurfContour;

/*
 * prototypes
 */

/* surface contour */
void	surfInitContour( SurfContour *contour, int num );
float	surfGetWaveHeight( SurfContour *contour, float x, float y );
int		surfGetWaveIndex( SurfContour *contour, float x, float y );

#define surfContourWidth(contour)	((float)(((SurfContour *)(contour))->width))

#endif	// SURF_CONTOUR_H

