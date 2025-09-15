/**********************************************************
 * Copyright (C) 1999,2000 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:	PlayStation surf game
 **********************************************************
 * File:	camera.h
 * Author:	Mark Theyer
 * Created:	13 Jul 1999
 **********************************************************
 * Description:	Header file for camera stuff.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 13-Jul-99	Theyer	Initial coding
 *
 **********************************************************/

#ifndef SURF_CAMERA_H
#define SURF_CAMERA_H

/* 
 * includes
 */

#include <compile.h>
#include <type/datatype.h>
//#include <wave.h>

/* 
 * macros 
 */

/* update incremental values for dynamic camera positioning */
#define SURF_CAMERA_ANGLE_INC					  1
#define SURF_CAMERA_PITCH_INC					  1
#define SURF_CAMERA_TRANSITION_STEPS			 30.0f

// camera range of positions
#define SURF_CAMERA_MIN_POS						  1
#define SURF_CAMERA_MAX_POS						 10

// delay frames
#define SURF_CAMERA_DELAY						  5

// camera positions
#define SURF_CAMERA_POS_HIGH_FRONT_RIGHT		  1
#define SURF_CAMERA_POS_LOW_FRONT_RIGHT			  2 
#define SURF_CAMERA_POS_LOW_FRONT_CENTRE		  3 
#define SURF_CAMERA_POS_LOW_FRONT_LEFT			  4 
#define SURF_CAMERA_POS_HIGH_FRONT_LEFT			  5
#define SURF_CAMERA_POS_HIGH_ONWAVE_LEFT		  6
#define SURF_CAMERA_POS_LOW_ONWAVE_LEFT			  7
#define SURF_CAMERA_POS_HIGH_ONWAVE_OVERHEAD	  8
#define SURF_CAMERA_POS_LOW_ONWAVE_RIGHT		  9
#define SURF_CAMERA_POS_HIGH_ONWAVE_RIGHT		 10

// camera angles
#define SURF_CAMERA_ANGLE_HIGH_FRONT_RIGHT		 45.0f
#define SURF_CAMERA_ANGLE_LOW_FRONT_RIGHT		 30.0f
#define SURF_CAMERA_ANGLE_LOW_FRONT_CENTRE		  0.0f
#define SURF_CAMERA_ANGLE_LOW_FRONT_LEFT	    -30.0f
#define SURF_CAMERA_ANGLE_HIGH_FRONT_LEFT		-45.0f
#define SURF_CAMERA_ANGLE_HIGH_ONWAVE_LEFT		-65.0f
#define SURF_CAMERA_ANGLE_LOW_ONWAVE_LEFT		-80.0f
#define SURF_CAMERA_ANGLE_HIGH_ONWAVE_OVERHEAD	  0.0f
#define SURF_CAMERA_ANGLE_LOW_ONWAVE_RIGHT		 80.0f
#define SURF_CAMERA_ANGLE_HIGH_ONWAVE_RIGHT		 65.0f

#if 0
// camera pitch
#define SURF_CAMERA_PITCH_HIGH_FRONT_RIGHT		 65.0f
#define SURF_CAMERA_PITCH_LOW_FRONT_RIGHT		 75.0f
#define SURF_CAMERA_PITCH_LOW_FRONT_CENTRE		 80.0f
#define SURF_CAMERA_PITCH_LOW_FRONT_LEFT		 75.0f
#define SURF_CAMERA_PITCH_HIGH_FRONT_LEFT		 65.0f
#define SURF_CAMERA_PITCH_HIGH_ONWAVE_LEFT		 70.0f
#define SURF_CAMERA_PITCH_LOW_ONWAVE_LEFT		 80.0f
#define SURF_CAMERA_PITCH_HIGH_ONWAVE_OVERHEAD	 45.0f
#define SURF_CAMERA_PITCH_LOW_ONWAVE_RIGHT		 80.0f
#define SURF_CAMERA_PITCH_HIGH_ONWAVE_RIGHT		 70.0f
#else
// camera pitch
#define SURF_CAMERA_PITCH_HIGH_FRONT_RIGHT		 55.0f
#define SURF_CAMERA_PITCH_LOW_FRONT_RIGHT		 65.0f
#define SURF_CAMERA_PITCH_LOW_FRONT_CENTRE		 70.0f
#define SURF_CAMERA_PITCH_LOW_FRONT_LEFT		 65.0f
#define SURF_CAMERA_PITCH_HIGH_FRONT_LEFT		 55.0f
#define SURF_CAMERA_PITCH_HIGH_ONWAVE_LEFT		 60.0f
#define SURF_CAMERA_PITCH_LOW_ONWAVE_LEFT		 70.0f
#define SURF_CAMERA_PITCH_HIGH_ONWAVE_OVERHEAD	 45.0f
#define SURF_CAMERA_PITCH_LOW_ONWAVE_RIGHT		 70.0f
#define SURF_CAMERA_PITCH_HIGH_ONWAVE_RIGHT		 60.0f
#endif

// camera distances (zoom)
#define SURF_CAMERA_DIST_MIN				   5000.0f
#define SURF_CAMERA_DIST_MAX				  10000.0f
#define SURF_CAMERA_ZOOM_MIN				      0.0f
#define SURF_CAMERA_ZOOM_MAX				   3000.0f

// camera distances
#define SURF_CAMERA_DIST_HIGH_FRONT_RIGHT	   8000.0f
#define SURF_CAMERA_DIST_LOW_FRONT_RIGHT	   7500.0f
#define SURF_CAMERA_DIST_LOW_FRONT_CENTRE	   8000.0f
#define SURF_CAMERA_DIST_LOW_FRONT_LEFT		   7500.0f
#define SURF_CAMERA_DIST_HIGH_FRONT_LEFT	   8000.0f
#define SURF_CAMERA_DIST_HIGH_ONWAVE_LEFT	   9000.0f
#define SURF_CAMERA_DIST_LOW_ONWAVE_LEFT	   6000.0f
#define SURF_CAMERA_DIST_HIGH_ONWAVE_OVERHEAD  8000.0f
#define SURF_CAMERA_DIST_LOW_ONWAVE_RIGHT	   6000.0f
#define SURF_CAMERA_DIST_HIGH_ONWAVE_RIGHT	   9000.0f

/*
 * typedefs
 */

typedef struct {
	int					 delay;			// camera change delay
	int					 pos;			// camera position code
	int					 transition;	// number of transition steps
	Bool				 inposition;	// camera in position flag
	Point_f3d			 lookat;		// look at point
	float				 angle;			// camera angle
	float				 pitch;			// camera pitch
	float				 dist;			// distance length
	float				 zoom;			// zoom length
	float				 focus;			// distance plus zoom length
	float				 angle_inc;		// camera angle transition increment
	float				 pitch_inc;		// camera pitch transition increment
	float				 dist_inc;		// distance length transition increment
} SurfCamera;

/*
 * prototypes
 */

extern void	surfInitCamera( SurfCamera *camera );
extern void surfUpdateCamera( SurfCamera *camera, float posz );
extern void surfSetCamera( SurfCamera *camera );
extern void surfNextCamera( SurfCamera *camera, Bool next );

#define surfResetCamera(cam)			surfInitCamera(cam)
#define surfSetCamera(cam)				gfxSetCamera(&(cam)->lookat,(cam)->angle,(cam)->pitch,(cam)->focus)

#endif	// SURF_CAMERA_H

