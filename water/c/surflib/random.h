/**********************************************************
 * Copyright (C) 1999 TheyerGFX
 * All Rights Reserved
 **********************************************************
 * Project:	Surf wave engine
 **********************************************************
 * File:	random.h
 * Author:	Mark Theyer
 * Created:	13 Jul 1999
 **********************************************************
 * Description:	Header file for surf random related stuff.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 13-Jul-99	Theyer	Initial coding from new wave system
 *
 **********************************************************/

#ifndef SURF_RANDOM_H
#define SURF_RANDOM_H

/* 
 * includes
 */

#include <type/datatype.h>
#include <gfx/gfx.h>

/* 
 * macros 
 */

/* random number store */
#define SURF_RANDOM_MAX_SET					20
#define SURF_RANDOM_SET_N1_2				 1
#define SURF_RANDOM_SET_N15_15				 2
#define SURF_RANDOM_SET_N25_75				 3
#define SURF_RANDOM_SET_N164_164			 4

/*
 * typedefs
 */

typedef struct {
	int					 n;									// selector index
	short				 set_n1_2[SURF_RANDOM_MAX_SET];		// random numbers in the range (-1 to 2)
	short				 set_n15_15[SURF_RANDOM_MAX_SET];	// random numbers in the range (-15 to 15)
	short				 set_n25_75[SURF_RANDOM_MAX_SET];	// random numbers in the range (-25 to 75)
	short				 set_n164_164[SURF_RANDOM_MAX_SET];	// random numbers in the range (-164 to 164)
} SurfRandom;

/*
 * prototypes
 */

/* random.c */
extern void surfInitRandom( SurfRandom *random, Bool init );
extern int  surfGetRandom( int type );

#endif	// SURF_RANDOM_H

