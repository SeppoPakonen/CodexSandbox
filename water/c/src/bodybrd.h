/**********************************************************
 * Copyright (C) 1999,2000 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:	PlayStation surf game
 **********************************************************
 * File:	bodybrd.h
 * Author:	Mark Theyer
 * Created:	06 Sep 1999
 **********************************************************
 * Description:	Header file for bodyboard functions.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 06-Sep-99	Theyer	Initial coding from existing game.
 *
 **********************************************************/

#ifndef SURF_BODYBRD_H
#define SURF_BODYBRD_H

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

#define SURF_BODYBOARD_WEIGHT				  1		// 1kg
#define SURF_BODYBOARD_SLOWDOWN			     60		// 82		// fixed value of 0.01f
#define SURF_BODYBOARD_TERMINAL_VELOCITY	 30		

/* bodyboard positions */
#define SURF_BODYBOARD_POS_LAYINGON			  0
#define SURF_BODYBOARD_POS_TURNRIGHT		  1
#define SURF_BODYBOARD_POS_TURNLEFT			  2
#define SURF_BODYBOARD_POS_PLANING			  3
#define SURF_BODYBOARD_POS_STALL			  4
#define SURF_BODYBOARD_NPOSITIONS			  5

/*
 * typedefs
 */

typedef struct {
    int					 xangle;
    int					 yangle;
    int					 zangle;
    int					 tx;
    int					 ty;
    int					 tz;
} SurfBodyBoardPosition;

typedef struct {
   	GfxObject				*object;	/* board object     */
    GfxMatrix				 mat;		/* saved matrix     */
   	GfxObject				*shadow;	/* board shadow object  */
    int						 pos;		/* position index   */
    SurfBodyBoardPosition	 current;	/* current position */
 	SurfBodyBoardPosition   *wanted;	/* desired position */                             	
    unsigned long			 state;		/* current state    */                               	
    int						 maxmove;	/* max move per frame */
	SurfVehicle				*veh; 		/* reverse pointer  */
	SurfBodyBoardPosition	*position;	/* pointer to position info */
	int						 spin;		/* spin around angle */
	int						 roll;		/* airtime roll value */
	int						 pitch;		/* airtime pitch value */
} SurfBodyBoard;

/*
 * prototypes
 */

extern void surfInitBodyboard( SurfBodyBoard *board, SurfVehicle *veh, PersonInfo *player );
extern void surfResetBodyboard( SurfBodyBoard *board );
extern void surfBuildBodyboard( SurfBodyBoard *board, int num, int player );
extern void surfDrawBodyboard( SurfVehicle *veh, SurfBodyBoard *board );
extern void surfBodyboardSetPosition( SurfBodyBoard *board, int pos );
extern void surfBodyboardUpdate( SurfBodyBoard *board, SurfVehicle *veh );
extern Bool surfBodyboardInPosition( SurfBodyBoard *board );
extern void surfBodyboardNextPosition( SurfBodyBoard *board, int riderpos );

#endif	// SURF_SURFBRD_H

