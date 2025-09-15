/**********************************************************
 * Copyright (C) 1999,2000 TheyerGFX
 * All Rights Reserved
 **********************************************************
 * Project:	Surf game
 **********************************************************
 * File:	sailbrd.h
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

#ifndef SURF_SAILBRD_H
#define SURF_SAILBRD_H

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
#define SURF_SAILBOARD_WEIGHT					55	 

/* roll limit */
#define SURF_SAILBOARD_ROLL_LIMIT				80	
#define SURF_SAILBOARD_SLOWDOWN					60

/* terminal (absolute maximum possible) speeds */
#define SURF_SAILBOARD_TERMINAL_VELOCITY		60

/* speed settings */
#define SURF_SAILBOARD_GLUE_SPEED			 81920 // 20
#define SURF_SAILBOARD_AIRTIME_SPEED		 40960 // 10
#define SURF_SAILBOARD_PLANING_SPEED		 20480 //  5
#define SURF_SAILBOARD_SINK_SPEED			 16384 //  4

/* rates */
#define SURF_SAILBOARD_STRAIGHTENRATE			 2
#define SURF_SAILBOARD_TURNRATE					 2
#define SURF_SAILBOARD_FAST_TURNRATE			 4
#define SURF_SAILBOARD_AIR_TURNRATE				 3
#define SURF_SAILBOARD_FAST_AIR_TURNRATE		 6
#define SURF_SAILBOARD_RICKTA_RATE				 6
#define SURF_SAILBOARD_PITCHRATE				 2

/* sailboard positions */
#define SAILBOARD_POS_WATERSTART_RIGHT			 0
#define SAILBOARD_POS_WATERSTART_LEFT			 1
#define SAILBOARD_POS_PLANING_RIGHT				 2
#define SAILBOARD_POS_PLANING_LEFT				 3
#define SAILBOARD_POS_TABLETOP_RIGHT1			 4
#define SAILBOARD_POS_TABLETOP_RIGHT2			 5
#define SAILBOARD_POS_LUFF_RIGHT				 6
#define SAILBOARD_POS_LUFF_CENTRE				 7
#define SAILBOARD_POS_CRASH_LIP					 8
#define SAILBOARD_POS_R_TURN_RIGHT				 9
#define SAILBOARD_POS_L_TURN_LEFT				10
#define SAILBOARD_POS_L_TURN_RIGHT				11
#define SAILBOARD_POS_R_TURN_LEFT				12
#define SAILBOARD_POS_R_NOSE_UP					13
#define SAILBOARD_POS_L_NOSE_UP					14
#define SAILBOARD_POS_R_EXTREME_NOSE_UP			15
#define SAILBOARD_POS_L_EXTREME_NOSE_UP			16	
#define SAILBOARD_POS_JIBE_RIGHT				17
#define SAILBOARD_POS_JIBE_LEFT					18
#define NSAILBOARDPOSITIONS						19

/*
 * typedefs
 */

typedef struct {
    int					 xangle;
    int					 yangle;
    int					 zangle;
    int					 sailx;
    int					 saily;
    int					 sailz;
    int					 boom;
    int					 tx;
    int					 ty;
    int					 tz;
} SurfSailPosition;

typedef struct {
   	GfxObject			*board;		/* board object     */
	GfxObject			*sail;		/* sail object		*/
	GfxObject			*boom;		/* boom object		*/
	GfxObject			*sail_clone;/* sail clone (so boom does not rescale) */
    GfxMatrix			 mat;		/* board matrix     */
    GfxMatrix			 smat;		/* sail matrix      */
    GfxMatrix			 bmat;		/* boom matrix      */
    int					 pos;		/* position index   */
 	SurfSailPosition	*position;	/* position data pointer */                             	
    SurfSailPosition	 current;	/* current position */
 	SurfSailPosition	*wanted;	/* desired position */                             	
    unsigned long		 state;		/* current state    */                               	
    int					 maxmove;	/* max move per frame */
	SurfVehicle			*veh;		/* reverse pointer  */
	int					 spin;		/* airtime spin angle	*/
	int					 roll;		/* airtime roll value	*/
	int					 pitch;		/* airtime pitch value  */
	struct {						/* following tick values used to detect tricks */
		int				 roll;		/* roll update value this tick	*/
		int				 pitch;		/* pitch update value this tick */
		int				 spin;		/* spin update value this tick  */
	} tick;
	int					 pitchrate;	/* UNUSED */
	int					 turnrate;	/* UNUSED */
} SurfSailBoard;

/*
 * prototypes
 */

/* sailbrd.c */
extern void surfInitSailBoard( SurfSailBoard *sailboard, SurfVehicle *veh, PersonInfo *player );
extern void surfResetSailBoard( SurfSailBoard *sailboard );
extern void surfBuildSailBoard( SurfSailBoard *sailboard, int num, int player );
extern void surfDrawSailBoard( SurfVehicle *veh, SurfSailBoard *sailboard );
extern void surfSailBoardSetPosition( SurfSailBoard *sailboard, int pos );
extern void surfSailBoardUpdate( SurfSailBoard *sailboard, SurfVehicle *veh );
extern Bool surfSailBoardInPosition( SurfSailBoard *sailboard );
extern void surfSailBoardNextPosition( SurfSailBoard *sailboard, int riderpos );
extern void surfSailBoardSetThrust( SurfVehicle *veh, int thrust );
extern int  surfSailBoardPower( SurfVehicle *veh, int wind_dir, int wind_speed, Bool side );

#endif	// SURF_SAILBRD_H

