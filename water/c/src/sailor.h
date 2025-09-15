/**********************************************************
 * Copyright (C) 1999,2000 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:	PSX surf game
 **********************************************************
 * File:	sailor.h
 * Author:	Mark Theyer
 * Created:	15 Jul 1999
 **********************************************************
 * Description:	Header file for sailor functions.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game.
 *
 **********************************************************/

#ifndef SURF_SAILOR_H
#define SURF_SAILOR_H

/* 
 * includes
 */

#include <compile.h>
#include <type/datatype.h>
#include <gfx/gfx.h>
#include <vehicle.h>
#include <person.h>

/* 
 * macros 
 */

/* weight (kg) */
#define SURF_SAILOR_WEIGHT				 75		

/* sailor states */
#define SAILOR_STATE_WATERSTART			  1
#define SAILOR_STATE_JIBE				  2
#define SAILOR_STATE_CRASH_LIP			  3
#define SAILOR_STATE_TURN_RIGHT			  4
#define SAILOR_STATE_TURN_LEFT			  5
#define SAILOR_STATE_NOSE_UP			  6
#define SAILOR_STATE_EXTREME_NOSE_UP	  7
#define SAILOR_STATE_RIDING			      8

/* sailor positions */
#define SAILOR_POS_WATERSTART_RIGHT		  0
#define SAILOR_POS_WATERSTART_LEFT		  1
#define SAILOR_POS_PLANING_RIGHT		  2
#define SAILOR_POS_PLANING_LEFT			  3
#define SAILOR_POS_LUFF					  4
#define SAILOR_POS_CRASH_LIP			  5
#define SAILOR_POS_R_TURN_RIGHT           6
#define SAILOR_POS_L_TURN_LEFT            7
#define SAILOR_POS_L_TURN_RIGHT		      8
#define SAILOR_POS_R_TURN_LEFT            9
#define SAILOR_POS_R_NOSE_UP 			 10
#define SAILOR_POS_L_NOSE_UP 			 11
#define SAILOR_POS_R_EXTREME_NOSE_UP	 12
#define SAILOR_POS_L_EXTREME_NOSE_UP	 13
#define SAILOR_POS_JIBE_RIGHT			 14
#define SAILOR_POS_JIBE_LEFT			 15
#define NSAILORPOSITIONS				 16

/*
 * typedefs
 */

/*
 * prototypes
 */

extern void surfInitSailor( PersonInfo *sailor );
extern void surfBuildSailor( PersonInfo *sailor, int num, int skin, int player );
extern void surfDrawSailor( PersonInfo *sailor );
extern int  surfSailorProcessKeyEvent( PersonInfo *sailor, int key, fixed analog );
extern void surfSailorNextPosition( PersonInfo *sailor );
extern int  surfPlaySailBoardingProcess( PersonInfo *sailor );

#endif	// SURF_SAILOR_H

