/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		alpha.h
 * Author:		Mark Theyer
 * Created:		07 Jul 2001
 **********************************************************
 * Description:	Header file for water transparency
 *				modifier system.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 07-Jul-01	Theyer	Initial coding for PS2
 *
 **********************************************************/

#ifndef SURF_ALPHA_H
#define SURF_ALPHA_H

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

#define WAVE_MAX_ALPHA		  6
#define WAVE_MAX_ALPHA_SAVE 181

// axis type values
#define WAVE_ALPHA_AXIS_X	  1
#define WAVE_ALPHA_AXIS_Y	  2

#define WAVE_ALPHA_MIN		  0
#define WAVE_ALPHA_MAX		 45

/*
 * typedefs
 */

// wave alpha data
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
	int			saved[WAVE_MAX_ALPHA_SAVE];
} WaveAlphaData;

typedef struct {
	int				nalpha;
	WaveAlphaData	data[WAVE_MAX_ALPHA];
} WaveAlpha;

/*
 * prototypes
 */

/* alpha.c */
void	surfInitAlpha( WaveAlpha *alpha );
void	surfUpdateAlpha( WaveAlpha *alpha );
int		surfGetWaveAlpha( WaveAlpha *alpha, float fx, float fy );

#endif	// SURF_ALPHA_H

