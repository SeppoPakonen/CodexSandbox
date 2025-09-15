/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		swell.h
 * Author:		Mark Theyer
 * Created:		07 Jul 2001
 **********************************************************
 * Description:	Header file for swell data for water
 *				swell generation system.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 07-Jul-01	Theyer	Initial coding for PS2
 *
 **********************************************************/

#ifndef SURF_SWELL_H
#define SURF_SWELL_H

/* 
 * includes
 */

#include <stdio.h>

#include <compile.h>
#include <type/datatype.h>
#include <surfutil.h>

/* 
 * macros 
 */

#define WAVE_MAX_SWELLS			  4
#define WAVE_MAX_SWELL_SAVE		  181

// axis type values
#define WAVE_SWELL_AXIS_X		  1
#define WAVE_SWELL_AXIS_Y		  2

/*
 * typedefs
 */

// swell data
typedef struct {
	int			axis;
	int			position;
	int			length;
	int			mask;
	int			normalise;
	float		peak;
	float		trough;
	float		height;
	int			speed;
	//float		saved[WAVE_MAX_SWELL_SAVE];
} WaveSwellData;

typedef struct {
	WaveSwellData	data[WAVE_MAX_SWELLS];
	int				nswells;
} WaveSwell;

/*
 * prototypes
 */

/* swell.c */
extern void	 surfInitSwell( WaveSwell *swell );
extern void	 surfUpdateSwell( WaveSwell *swell );
extern float surfGetSwellHeight( WaveSwell *swell, float fx, float fy );
extern void  surfGetSwellHeights( WaveSwell *swell, float x, float y, Point_f3d *pts, int npts, float *highest_z, float *highest_y );

#endif	// SURF_SWELL_H

