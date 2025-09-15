/**********************************************************
 * Copyright (C) 1999 TheyerGFX
 * All Rights Reserved
 **********************************************************
 * Project:	Surf wave engine
 **********************************************************
 * File:	vehicle.h
 * Author:	Mark Theyer
 * Created:	15 Jul 1999
 **********************************************************
 * Description:	Header file for vehicle functions.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game.
 *
 **********************************************************/

#ifndef SURF_VEHICLE_H
#define SURF_VEHICLE_H

/* 
 * includes
 */

#include <compile.h>
#include <type/datatype.h>
#include <memory/memory.h>
#include <gfx/gfx.h>
#include <wave.h>
#include <swell.h>
#include <contour.h>

/* 
 * macros 
 */

/* vehicle types */
#define VEHICLE_TYPE_UNDEFINED				 0
#define VEHICLE_TYPE_BODYBOARD				 1
#define VEHICLE_TYPE_SURFBOARD   			 2
#define VEHICLE_TYPE_JETSKI					 3
#define VEHICLE_TYPE_SAILBOARD				 4
#define VEHICLE_TYPE_PWC					 5

#define	VEHICLE_LANDING_ANGLE_TOLERANCE		15
#define VEHICLE_JETSKI_FLOAT_RATE			 5
#define VEHICLE_FLOAT_RATE					 5
#define VEHICLE_FLATTEN_RATE				20

#define VEHICLE_STATE_AIRTIME				 1
#define VEHICLE_STATE_ONWATER				 2
#define VEHICLE_STATE_SUBMERGED				 3

/*
 * typedefs
 */

typedef struct SurfVehicleStruct SurfVehicle;

/* callbacks */
typedef void *(SurfVehicleUpdateCallback)( void *craft, void *veh, void *rider );
typedef Bool *(SurfVehicleInPositionCallback)( void *craft );
typedef void *(SurfVehicleNextPositionCallback)( void *craft );
typedef void *(SurfVehicleDrawCallback)( void *craft );

typedef struct SurfVehicleStruct {
	int					 type;							/* type of vehicle					 */
	int					 state;							/* airtime, onwater or submerged	 */
	void				*craft;							/* vehicle data pointer				 */
	void				*rider;							/* vehicle data pointer				 */
	GfxObject			*posobj;						/* object for positioning vehicle	 */
    GfxMatrix			 mat;							/* saved matrix						 */
	GfxObject			*object;						/* vehicle graphical object			 */
	// flags
	Bool				 onwave;						/* currently on a wave?				 */
	Bool				 sail_side;						/* side sail is on					 */
	Bool				 stay_with_wave;				/* flag to land back on wave		 */
	Bool				 glue_to_wave;					/* stick to wave face				 */
	Bool				 crash;							/* crash flag						 */
	Bool				 frontface;						/* on frontface or backface of wave	 */
	// static values
	float				 terminal_velocity;				/* absolute maximum speed			 */
	float				 slowdown;						/* slow down rate					 */
	//int				 weight;						/* overall weight of rider and veh'	 */
	// speed values
	float				 thrust;						/* thrust applied to vehicle		 */
	float				 speed;							/* current speed of vehicle			 */
	float				 surface_speed;					/* surface speed of vehicle			 */
	float				 vertical_speed;				/* vertical speed of vehicle		 */
	// position values
	float				 posx;							/* world position					 */
	float				 posy;							/* world position					 */
	float				 posz;							/* world position					 */
	float				 airz;							/* height in air above take off z    */
	float				 direction;						/* range 0-360 degrees				 */
	float				 yaw;							/* yaw angle						 */
	float				 pitch;							/* pitch angle						 */
	float				 roll;							/* roll angle						 */
	float				 spin;							/* spin angle (copy of veh' spin)    */
	// airtime values
	float				 gravity;						/* gravity rate for airtime			 */
	float				 gravity_rate;					/* gravity rate for airtime			 */
	float				 takeoffdir;					/* direction angle when we got air	 */
	float				 takeoffz;						/* z position at which we got air	 */
	float				 takeoffangle;					/* angle at which we got air		 */
	float				 takeoffroll;					/* roll angle at which we got air	 */
	int					 nextz;							/* next z position					 */
	// wave and water values
	int					 water_level;					/* water level at this position		 */
	float			     wave_pitch;					/* pitch of wave at this position	 */
	float				 wave_roll;						/* roll								 */
	float				 rate;							/* on wave 0=parallel,1=perpendiculr */
	float				 speed_adjust;					/* speed adjustment factor on wave   */
	float				 facepos;						/* position on wave face (0.0 - 1.0) */
	//
	//int				 splashfactor;					/* splash factor					 */
	struct {
		int				 state;							/* airtime, onwater or submerged	 */
		Bool			 face;							/* on frontface or backface of wave	 */
		float			 posx;							/* world position					 */
		float			 posy;							/* world position					 */
		float			 posz;							/* world position					 */
		float			 yaw;							/* yaw angle						 */
		float			 pitch;							/* pitch angle						 */
		float			 roll;							/* roll angle						 */
		float			 turn;							/* turn angle						 */
		float			 speedx;						/* last speed value in the x directn */
		float			 speedy;						/* last speed value in the y directn */
		float			 vertical_speed;				/* vertical speed of vehicle		 */
	} last;	
	struct {											/* following tick values used to detect tricks	*/
		float			 roll;							/* roll update value this tick					*/
		float			 pitch;							/* pitch update value this tick					*/
		float			 spin;							/* spin update value this tick					*/
	} tick;
	WavePositionData					 wavepos;		/* wave positional information				*/
	SurfVehicleUpdateCallback			*update;		/* vehicle update callback					*/
	SurfVehicleInPositionCallback		*inposition;	/* vehicle 'in position test' callback		*/
	SurfVehicleNextPositionCallback		*nextposition;	/* next vehicle position callback			*/
	SurfVehicleDrawCallback				*draw;			/* draw vehicle callback					*/
} SurfVehicleStruct;

// 'tis a bad practise to put these headers here!
#include <splash.h>

/*
 * prototypes
 */

extern void		surfInitVehicle( SurfVehicle *veh, int type, GfxObject *object, void *craft, void *rider, float terminal_velocity, float slowdown, SurfVehicleUpdateCallback *update, SurfVehicleInPositionCallback *inposition, SurfVehicleNextPositionCallback *nextposition, SurfVehicleDrawCallback *draw );
extern float	surfUpdateVehiclePosition( SurfVehicle *veh, SurfContour *contour, float tick );
extern void		surfUpdateVehicleSpeed( SurfVehicle *veh, float tick );
extern void		surfUpdateVehicleAirPosition( SurfVehicle *veh, WaveSwell *swell, SurfSplashData *splash, float tick );
extern void		surfVehicleNextAirPosition( SurfVehicle *veh, float tick );
extern void		surfUpdateVehicle( SurfVehicle *veh );
extern void		surfVehicleSetState( SurfVehicle *veh, int state, SurfSplashData *splash );
extern void		surfChangeVehicleDirection( SurfVehicle *veh, float angle, float tick );
extern void		surfChangeVehiclePitch( SurfVehicle *veh, float angle, float tick );
extern void		surfChangeVehicleRoll( SurfVehicle *veh, float angle, float limit, float tick );
extern void		surfVehicleNextPosition( SurfVehicle *veh, int riderpos );
extern Bool		surfVehicleInPosition( SurfVehicle *veh );
extern float	surfVehiclePitch( SurfVehicle *veh );
extern float	surfVehicleRoll( SurfVehicle *veh );
extern void		surfVehiclePitchFlatten( SurfVehicle *veh );
extern void		surfVehicleStraighten( SurfVehicle *veh, float rate, float tick );
extern void		surfPushVehicleNose( SurfVehicle *veh );
extern void		surfDrawVehicle( SurfVehicle *veh );
extern void		surfVehicleSpit( SurfVehicle *veh, float angle, float pitch, float speed, SurfSplashData *splash );
extern float	surfNextAirPosition( float speed, float angle );
extern void		surfNextXYPosition( float speed, float tick, float direction, float pitch, float *x, float *y );
extern float	surfVehicleLandingPitch( SurfVehicle *veh );
//extern Bool	surfVehicleOnWave( WaveAttributes *waveattr, WaveTypeData *waveinfo, WaveRenderData *wave, SurfContour *contour, WaveSavedPoints *saved, SurfVehicle *veh, float wave_speed, float tick );
extern Bool		surfVehicleOnWave( WaveAttributes *attr, WaveTypeData *waveinfo, WaveRenderData *wave, SurfContour *contour, WaveSwell *swell, WaveSavedPoints *saved, SurfVehicle *veh, float wave_speed, float tick );

#define surfVehicleObject(veh)						(((SurfVehicle *)(veh))->posobj)
#define surfSetVehicleThrust(veh,power)				(((SurfVehicle *)(veh))->thrust = power)
#define surfSetVehicleSubmerseSpeed(veh,spd)		{((SurfVehicle *)(veh))->subspeed = spd;((SurfVehicle *)(veh))->submerged = TRUE;}
#define surfVehicleThrust(veh)						(((SurfVehicle *)(veh))->thrust)

#endif	// SURF_VEHICLE_H

