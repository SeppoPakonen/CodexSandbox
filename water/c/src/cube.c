/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		cube.c
 * Author:		Mark Theyer
 * Created:		04 May 2001
 **********************************************************
 * Description:	First tests
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 04-May-01	Theyer	Initial coding.
 *
 **********************************************************/

/*
 * includes
 */

#include <surfgame.h>
#include <gfx/gfxps2.h>
#include <gfx/ps2perf.h>

/*
 * typedefs
 */

/*
 * macros
 */

/*
 * prototypes
 */

void mem_test ( void );

/*
 * variables
 */

extern SurfData    	surf;
long vu1mem[2048];

extern Ps2Data	ps2;

/*
 * functions
 */


/**********************************************************
 * Function:	surfInitJetSki
 **********************************************************
 * Description: Prepare to play with the jet ski
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void cube_test (
	void
	)
{
	GfxObject	*cube;
	Point_f3d	 eye;
	float		 angle;
	float		 pitch;
	int			 frame;

	/* init */
	eye.x = 0.0f;
	eye.y = 0.0f;
	eye.z = 50.0f;
	eye.w = 1.0f;
	angle = 0.0f;
	pitch = 0.0f;
	frame = 0;

	/* lighting */
	gfxSetAmbient( 0.5f, 0.5f, 0.5f );
	gfxSetFlatLight( 1, 0.0f, 0.0f,  -1.0f, 1.0f, 1.0f, 1.0f );
	//gfxSetFlatLight( 2, 0.0f, 1.0f,  0.0f, 0.6f, 0.6f, 0.6f );
	//gfxSetFlatLight( 3, 0.0f, 0.0f, -1.0f, 0.6f, 0.6f, 0.6f );

#if 1
	/* locate models */
	gfxFilePath( "models/cube" );

    /* create model */
	printf( "loading cube...\n" );
	surfLoadMap( "cube.map", TRUE );
    if ( ! p2mModelToGfxObject( "cube", (GfxObject **)&cube, NULL ) ) {
		printf( p2mModelError() );
		surfExit(SURF_ERROR_JETSKI_LOAD_FAILED);
	}

	printf( "done\n" );
	while(1) {
		frame++;
		padRead();
		if ( padAnyButtonPressed( PAD_BUTTON_START )  != PAD_NONE ) goto done;
		if ( padAnyButtonDown( PAD_BUTTON_UP )    != PAD_NONE ) pitch += 2.0f; 
		if ( padAnyButtonDown( PAD_BUTTON_DOWN )  != PAD_NONE ) pitch -= 2.0f; 
		if ( padAnyButtonDown( PAD_BUTTON_LEFT )  != PAD_NONE ) angle -= 2.0f; 
		if ( padAnyButtonDown( PAD_BUTTON_RIGHT ) != PAD_NONE ) angle += 2.0f;
		//angle += 1.0f;
		if ( angle < 0.0f ) angle += 360.0f;
		if ( angle >= 360.0f ) angle -= 360.0f;
		if ( pitch < 0.0f ) pitch += 360.0f;
		if ( pitch >= 360.0f ) pitch -= 360.0f;
		//pitch = 0.0f;
		//gfxSetCamera(&(cam)->lookat,(cam)->angle,(cam)->pitch,(cam)->focus)
		gfxSetCamera( &eye, 0.0f, 0.0f, 10.0f );
		//gfxResetMatrix( cube );
		gfxRotateObject( cube, GFX_Z_AXIS, angle );
		gfxRotateObject( cube, GFX_X_AXIS, pitch );
		angle = 0.0f;
		pitch = 0.0f;
		//printf( "draw\n" );
		//gfxTranslateObject( cube, 15.0f, -15.0f, 0.0f );
		gfxDrawObject( cube );
		//ps2DumpVu1();
		//exit(1);
		//test_strip2( 1, 0, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 2.0f, 1.0f, 0.5f, 0.0f );
		//gfxTranslateObject( cube, -30.0f, 30.0f, 0.0f );
		//gfxDrawObject( cube );
		//gfxTranslateObject( cube, 15.0f, -15.0f, 0.0f );
		gfxBufSwap();
		//goto done;
	}
#endif

done:

#if 1
	/* copy VU1 memory and save */
	gfxFilePath( "saves" );
	memCopy( 0x1100C000, vu1mem, 16384 );
	gfxFileSave( "vu1mem.dat", (Byte *)vu1mem, 16384 );
#endif
}


/**********************************************************
 * Function:	surfInitJetSki
 **********************************************************
 * Description: Prepare to play with the jet ski
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void ski_test (
	void
	)
{
	struct {
		GfxObject	*ski;
		GfxObject	*pole;
		GfxObject	*bars;
		GfxObject	*jet;
	} jet;
	Point_f3d	 eye;
	float		 angle;
	float		 pitch;
	int			 frame;
	float		 pos;
	float		 up;
	float		 y;
	float		 pole;
	float		 turn;
	Bool		 doturn;
	int			 last;
	float		 interpol;
	float		 step;

	/* init */
	eye.x = 0.0f;
	eye.y = 0.0f;
	eye.z = 500.0f;
	eye.w = 1.0f;
	angle = 0.0f;
	pitch = 0.0f;
	pos   = 0.0f;
	pole  = 0.0f;
	up	  = 0.0f;
	y	  = 0.0f;
	turn  = 0.0f;
	doturn = 0;
	frame = 0;
	last = 0;
	interpol = 0.0f;
	step = 0.05f;

	/* lighting */
	gfxSetAmbient( 0.5f, 0.5f, 0.5f );
	//gfxSetFlatLight( 1, 0.0f, 0.5, -1.0f, 0.7f, 0.7f, 0.7f );
	//gfxSetFlatLight( 2, -1.0f, -1.0, -1.0f, 0.8f, 0.8f, 0.8f );
	//gfxSetFlatLight( 3, 1.0f, 1.0, 1.0f, 0.8f, 0.8f, 0.8f );
	gfxSetBackgroundColor( 0.5f, 0.5f, 0.8f );

#if 1
	/* locate models */
	gfxFilePath( "models/jetskis/s1" );

    /* create model */
	printf( "loading ski...\n" );
	surfLoadMap( "sjet.map", TRUE );
    if ( ! p2mModelToGfxObject( "ski1", (GfxObject **)&jet, NULL ) ) {
		printf( p2mModelError() );
		surfExit(SURF_ERROR_JETSKI_LOAD_FAILED);
	}
	printf( "done\n" );

	/* load animation data */
	//surfLoadPos( jet.ski, "jetski.p2a" );

	while(1) {
		//frame++;
		padRead();
		if ( padAnyButtonPressed( PAD_BUTTON_START )  != PAD_NONE ) break;
		if ( padAnyButtonPressed( PAD_BUTTON_SELECT )  != PAD_NONE ) doturn = !doturn;
		if ( padAnyButtonDown( PAD_BUTTON_C )  != PAD_NONE ) y += 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_A )  != PAD_NONE ) y -= 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_RIGHT_1 )  != PAD_NONE ) up += 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_RIGHT_2 )  != PAD_NONE ) up -= 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_LEFT_1 )  != PAD_NONE ) pole += 1.0f;
		if ( padAnyButtonDown( PAD_BUTTON_LEFT_2 )  != PAD_NONE ) pole -= 1.0f;
		if ( padAnyButtonDown( PAD_BUTTON_B )  != PAD_NONE ) pos -= 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_D )  != PAD_NONE ) pos += 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_UP )    != PAD_NONE ) pitch += 1.0f; 
		if ( padAnyButtonDown( PAD_BUTTON_DOWN )  != PAD_NONE ) pitch -= 1.0f; 
		turn = -(padAnalogX( 0, 0 ) * 2.0f);
		pole = -(padAnalogY( 0, 0 ) * 0.5f);
		if ( padAnyButtonDown( PAD_BUTTON_LEFT )  != PAD_NONE ) angle -= 1.0f; 
		if ( padAnyButtonDown( PAD_BUTTON_RIGHT ) != PAD_NONE ) angle += 1.0f;
		pitch += -(padAnalogY( 0, 1 ) * 2.0f);
		angle += -(padAnalogX( 0, 1 ) * 2.0f);
		//angle += 1.0f;
		if ( angle < 0.0f ) angle += 360.0f;
		if ( angle >= 360.0f ) angle -= 360.0f;
		if ( pitch < 0.0f ) pitch += 360.0f;
		if ( pitch >= 360.0f ) pitch -= 360.0f;
		//pitch = 0.0f;
		gfxSetCamera( &eye, 0.0f, 0.0f, 500.0f );
#if 1
		gfxRotateObject( jet.ski, GFX_Z_AXIS, angle );
		gfxRotateObject( jet.ski, GFX_X_AXIS, pitch );
		gfxRotateObject( jet.pole, GFX_Y_AXIS, pole );
		gfxRotateObject( jet.bars, GFX_Z_AXIS, (turn) );
#endif
		gfxRotateObject( jet.jet, GFX_Z_AXIS, -(turn/1.5f) );
		turn = 0.0f;
		pole = 0.0f;
		angle = 0.0f;
		pitch = 0.0f;
		//printf( "draw\n" );
#if 1
		gfxTranslateObject( jet.ski, pos, y, up );
		//gfxSetPosition( &jet.ski, 4, last, frame, interpol );
		gfxDrawObject( jet.pole );
		gfxDrawObject( jet.bars );
#endif
		gfxDrawObject( jet.jet );
		gfxDrawObject( jet.ski );
		//break;
		gfxTranslateObject( jet.ski, -(pos), -(y), -(up) );
		gfxBufSwap();
		/* update animation */
		interpol += step;
		if ( interpol > 1.0f ) {
			last = frame;
			frame++;
			if ( frame == 3 )
				frame = 0;
			interpol -= 1.0f;
		}
	}
#endif

#if 1
	/* copy VU1 memory and save */
	gfxFilePath( "saves" );
	memCopy( 0x1100C000, vu1mem, 16384 );
	gfxFileSave( "vu1mem.dat", (Byte *)vu1mem, 16384 );
#endif
}


/**********************************************************
 * Function:	surfInitJetSki
 **********************************************************
 * Description: Prepare to play with the jet ski
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void rider_test (
	void
	)
{
	GfxObject	*slash;
	GfxObject	*board;
	Point_f3d	 eye;
	float		 angle;
	float		 pitch;
	int			 frame;
	float		 pos;
	float		 up;
	float		 y;
	float		 pole;
	float		 turn;
	Bool		 doturn;
	float		 step;
	int			 last;
	float		 interpol;

	/* init */
	eye.x = 0.0f;
	eye.y = 0.0f;
	eye.z = 2200.0f;
	eye.w = 1.0f;
	angle = 0.0f;
	pitch = 0.0f;
	pos   = 0.0f;
	pole  = 0.0f;
	up	  = 0.0f;
	y	  = 0.0f;
	turn  = 0.0f;
	doturn   = 0;
	frame    = 1;
	last     = 0;
	interpol = 0.0f;
	step     = 0.02f;

	/* lighting */
	gfxSetAmbient( 0.5f, 0.5f, 0.5f );
	//gfxSetFlatLight( 1, 0.0f, 0.5, -1.0f, 0.7f, 0.7f, 0.7f );
	//gfxSetFlatLight( 2, -1.0f, -1.0, -1.0f, 0.8f, 0.8f, 0.8f );
	//gfxSetFlatLight( 3, 1.0f, 1.0, 1.0f, 0.8f, 0.8f, 0.8f );

#if 1
	/* locate models */
	gfxFilePath( "models/riders/s9" );

    /* create model */
	printf( "loading slash...\n" );
	surfLoadMap( "s9p1.map", TRUE );
    if ( ! p2mModelToGfxObject( "s9", (GfxObject **)&slash, NULL ) ) {
		printf( p2mModelError() );
		surfExit(SURF_ERROR_JETSKI_LOAD_FAILED);
	}
	printf( "done\n" );

	/* load animation data */
	surfLoadPos( slash, "s8.p2a" );

	while(1) {
		padRead();
		if ( padAnyButtonPressed( PAD_BUTTON_START )  != PAD_NONE ) return;
		if ( padAnyButtonPressed( PAD_BUTTON_SELECT )  != PAD_NONE ) doturn = !doturn;
		if ( padAnyButtonDown( PAD_BUTTON_C )  != PAD_NONE ) y += 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_A )  != PAD_NONE ) y -= 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_RIGHT_1 )  != PAD_NONE ) up += 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_RIGHT_2 )  != PAD_NONE ) up -= 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_LEFT_1 )  != PAD_NONE ) pole += 1.0f;
		if ( padAnyButtonDown( PAD_BUTTON_LEFT_2 )  != PAD_NONE ) pole -= 1.0f;
		if ( padAnyButtonDown( PAD_BUTTON_B )  != PAD_NONE ) pos -= 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_D )  != PAD_NONE ) pos += 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_UP )    != PAD_NONE ) pitch += 1.0f; 
		if ( padAnyButtonDown( PAD_BUTTON_DOWN )  != PAD_NONE ) pitch -= 1.0f; 
		//if ( doturn ) {
		//	if ( padAnyButtonDown( PAD_BUTTON_LEFT )  != PAD_NONE ) turn -= 1.0f; 
		//	if ( padAnyButtonDown( PAD_BUTTON_RIGHT ) != PAD_NONE ) turn += 1.0f;
		//} else {
			if ( padAnyButtonDown( PAD_BUTTON_LEFT )  != PAD_NONE ) angle -= 1.0f; 
			if ( padAnyButtonDown( PAD_BUTTON_RIGHT ) != PAD_NONE ) angle += 1.0f;
		//}
		//angle += 1.0f;
		if ( angle < 0.0f ) angle += 360.0f;
		if ( angle >= 360.0f ) angle -= 360.0f;
		if ( pitch < 0.0f ) pitch += 360.0f;
		if ( pitch >= 360.0f ) pitch -= 360.0f;
		//pitch = 0.0f;
		gfxSetCamera( &eye, 0.0f, 0.0f, 500.0f );
		gfxRotateObject( slash, GFX_Z_AXIS, angle );
		gfxRotateObject( slash, GFX_X_AXIS, pitch );
		turn = 0.0f;
		pole = 0.0f;
		angle = 0.0f;
		pitch = 0.0f;
		//printf( "draw\n" );
		gfxTranslateObject( slash, pos, y, up );
		gfxSetPosition( &slash, 1, last, frame, interpol );
		gfxDrawObject( slash );
		gfxTranslateObject( slash, -(pos), -(y), -(up) );
		gfxBufSwap();
		interpol += step;
		if ( interpol > 1.0f ) {
			last = frame;
			frame++;
			if ( frame == 45 )
				frame = 0;
			interpol -= 1.0f;
		}
	}
#endif

#if 0
	/* copy VU1 memory and save */
	gfxFilePath( "saves" );
	memCopy( 0x1100C000, vu1mem, 16384 );
	gfxFileSave( "vu1mem.dat", (Byte *)vu1mem, 16384 );
#endif
}


/**********************************************************
 * Function:	board_test
 **********************************************************
 * Description: Prepare to play with the jet ski
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void board_test (
	void
	)
{
	GfxObject	*board;
	Point_f3d	 eye;
	float		 angle;
	float		 pitch;
	int			 frame;
	float		 pos;
	float		 up;
	float		 y;
	float		 pole;
	float		 turn;
	Bool		 doturn;
	int			 last;
	float		 interpol;
	float		 step;
	float		 time;

	/* init */
	eye.x = 0.0f;
	eye.y = 0.0f;
	eye.z = 2500.0f;
	eye.w = 1.0f;
	angle = 0.0f;
	pitch = 0.0f;
	pos   = 0.0f;
	pole  = 0.0f;
	up	  = 0.0f;
	y	  = 0.0f;
	turn  = 0.0f;
	doturn = 1;
	frame = 0;
	last = 0;
	interpol = 0.0f;
	step = 0.05f;
	time = 0.0f;

	/* lighting */
	gfxSetAmbient( 0.15f, 0.15f, 0.15f );
	//gfxSetAmbient( 0.15f, 0.15f, 0.15f );
	gfxSetFlatLight( 1, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, 1.0f );
	//gfxSetFlatLight( 2, 0.0f, -1.0f,   0.0f, 1.0f, 0.2f, 0.2f );
	//gfxSetFlatLight( 2, -1.0f, -1.0, -1.0f, 0.8f, 0.8f, 0.8f );
	//gfxSetFlatLight( 3, 1.0f, 1.0, 1.0f, 0.8f, 0.8f, 0.8f );
	gfxSetBackgroundColor( 0.5f, 0.5f, 0.8f );

#if 1
	/* locate models */
	gfxFilePath( "models/boards/s20" );

    /* create model */
	printf( "loading board...\n" );
	surfLoadMap( "s20p1.map", TRUE );
    if ( ! p2mModelToGfxObject( "b20", (GfxObject **)&board, NULL ) ) {
		printf( p2mModelError() );
		surfExit(SURF_ERROR_JETSKI_LOAD_FAILED);
	}
	gfxRotateObject( board, GFX_X_AXIS, -75.0f );
	printf( "done\n" );

	/* load animation data */
	surfLoadPos( board, "spin_board.p2a" );
	//gfxAddObjectData( board, );

	while(1) {
		//frame++;
		padRead();
		if ( padAnyButtonPressed( PAD_BUTTON_START )  != PAD_NONE ) break;
		//if ( padAnyButtonPressed( PAD_BUTTON_SELECT )  != PAD_NONE ) doturn = !doturn;
		if ( padAnyButtonDown( PAD_BUTTON_C )  != PAD_NONE ) y += 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_A )  != PAD_NONE ) y -= 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_RIGHT_1 )  != PAD_NONE ) up += 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_RIGHT_2 )  != PAD_NONE ) up -= 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_LEFT_1 )  != PAD_NONE ) pole += 1.0f;
		if ( padAnyButtonDown( PAD_BUTTON_LEFT_2 )  != PAD_NONE ) pole -= 1.0f;
		if ( padAnyButtonDown( PAD_BUTTON_B )  != PAD_NONE ) pos -= 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_D )  != PAD_NONE ) pos += 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_UP )    != PAD_NONE ) pitch += 1.0f; 
		if ( padAnyButtonDown( PAD_BUTTON_DOWN )  != PAD_NONE ) pitch -= 1.0f; 
		if ( doturn ) {
			if ( padAnyButtonDown( PAD_BUTTON_LEFT )  != PAD_NONE ) time -= gfxTick(); 
			if ( padAnyButtonDown( PAD_BUTTON_RIGHT ) != PAD_NONE ) time += gfxTick();
		} else {
			if ( padAnyButtonDown( PAD_BUTTON_LEFT )  != PAD_NONE ) angle -= 1.0f; 
			if ( padAnyButtonDown( PAD_BUTTON_RIGHT ) != PAD_NONE ) angle += 1.0f;
		}
		//angle += 0.5f;
		if ( angle < 0.0f ) angle += 360.0f;
		if ( angle >= 360.0f ) angle -= 360.0f;
		if ( pitch < 0.0f ) pitch += 360.0f;
		if ( pitch >= 360.0f ) pitch -= 360.0f;
		//pitch = 0.0f;
		// time:
		//time += 0.02f;
		if ( time >= 2.0f )
			time -= 2.0f;
		if ( time < 0.0f )
			time += 2.0f;
		printf( "time=%.2f\n", time );
		//
		gfxSetCamera( &eye, 0.0f, 0.0f, 500.0f );
		gfxRotateObject( board, GFX_Z_AXIS, angle );
		gfxRotateObject( board, GFX_X_AXIS, pitch );
		gfxRotateObject( board, GFX_Y_AXIS, 0.5f );
		gfxSetObjectPosition( board, time );
		turn = 0.0f;
		pole = 0.0f;
		angle = 0.0f;
		pitch = 0.0f;
		//printf( "draw\n" );
		gfxTranslateObject( board, pos, y, up );
		gfxDrawObject( board );
		//break;
		gfxTranslateObject( board, -(pos), -(y), -(up) );
		gfxBufSwap();
		/* update animation */
		interpol += step;
		if ( interpol > 1.0f ) {
			last = frame;
			frame++;
			if ( frame == 3 )
				frame = 0;
			interpol -= 1.0f;
		}
	}
#endif

#if 1
	//gfxPs2MemoryDump( "saves", "sprmem.dat", GFX_PS2_SPR_MEM, 0 );
	gfxPs2MemoryDump( "saves", "vu1mem.dat", GFX_PS2_VU1_MEM, 0 );
#endif

#if 0
	/* copy VU1 memory and save */
	gfxFilePath( "saves" );
	memCopy( 0x1100C000, vu1mem, 16384 );
	gfxFileSave( "vu1mem.dat", (Byte *)vu1mem, 16384 );
#endif
}


/**********************************************************
 * Function:	board_test
 **********************************************************
 * Description: Prepare to play with the jet ski
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void summer_test (
	void
	)
{
	GfxObject	*board;
	GfxObject	*summer;
	Point_f3d	 eye;
	float		 angle;
	float		 pitch;
	int			 frame;
	float		 pos;
	float		 up;
	float		 y;
	float		 pole;
	float		 turn;
	Bool		 doturn;
	int			 last;
	float		 interpol;
	float		 step;
	float		 time;
	float		 end_time;
	char		 str[512];
	GfxTextureId	texid;
	GfxSpriteId		logo;

	/* init */
	eye.x = 0.0f;
	eye.y = 0.0f;
	eye.z = 2500.0f;
	eye.w = 1.0f;
	angle = 0.0f;
	pitch = 0.0f;
	pos   = 0.0f;
	pole  = 0.0f;
	up	  = 0.0f;
	y	  = 0.0f;
	turn  = 0.0f;
	doturn = 0;
	frame = 0;
	last = 0;
	interpol = 0.0f;
	step = 0.05f;
	time = 0.0f;

	/* lighting */
	gfxSetAmbient( 0.15f, 0.15f, 0.15f );
	//gfxSetAmbient( 0.15f, 0.15f, 0.15f );
	gfxSetFlatLight( 1, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, 1.0f );
	//gfxSetFlatLight( 2, 0.0f, -1.0f,   0.0f, 1.0f, 0.2f, 0.2f );
	//gfxSetFlatLight( 2, -1.0f, -1.0, -1.0f, 0.8f, 0.8f, 0.8f );
	//gfxSetFlatLight( 3, 1.0f, 1.0, 1.0f, 0.8f, 0.8f, 0.8f );
	gfxSetBackgroundColor( 0.5f, 0.5f, 0.8f );

	/* locate models */
	gfxFilePath( "models/boards/s20" );

    /* create model */
	printf( "loading board...\n" );
	surfLoadMap( "s20p1.map", FALSE );
    if ( ! p2mModelToGfxObject( "b20", (GfxObject **)&board, NULL ) ) {
		printf( p2mModelError() );
		surfExit(SURF_ERROR_JETSKI_LOAD_FAILED);
	}

	printf( "loading Summer...\n" );

	gfxFilePath( "models/riders/s8" );
	surfLoadMap( "s8p1.map", FALSE );
    if ( ! p2mModelToGfxObject( "s8", (GfxObject **)&summer, NULL ) ) {
		printf( p2mModelError() );
		surfExit(SURF_ERROR_JETSKI_LOAD_FAILED);
	}
	/* load animation data */
	surfLoadPos( board,  "board.p2a" );
	surfLoadPos( summer, "rider.p2a" );
	end_time = 9.0f;

	printf( "loading TheyerGFX logo...\n" );
	gfxFilePath( "images/logos" );
	texid = p2iTextureToGfxTexture( "gfxlogo.p2i", FALSE );
	logo  = gfxSpriteFromTexture( texid );

	printf( "done\n" );

	// init font display
	surfResetFont( &surf.font.font1 );
	surfFontDepth( &surf.font.font1, 0.0f );
	//surfFontScale( &surf.font.font1, 0.6f, 0.75f );
	surfFontPosition( &surf.font.font1, 0.025f, 0.05f );
	gfxSetSpritePosition( logo, 0.55f, 0.9f );
	gfxSetSpriteScale( logo, 2.0f, 0.3f );

	while(1) {
		padRead();
		if ( padAnyButtonPressed( PAD_BUTTON_SELECT ) != PAD_NONE ) doturn = !doturn;
		if ( padAnyButtonPressed( PAD_BUTTON_START ) != PAD_NONE ) break;
		if ( padAnyButtonDown( PAD_BUTTON_C )  != PAD_NONE ) y += 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_A )  != PAD_NONE ) y -= 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_B )  != PAD_NONE ) pos -= 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_D )  != PAD_NONE ) pos += 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_UP    )  != PAD_NONE ) y += 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_DOWN  )  != PAD_NONE ) y -= 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_LEFT  )  != PAD_NONE ) pos -= 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_RIGHT )  != PAD_NONE ) pos += 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_RIGHT_1 ) != PAD_NONE ) up += 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_RIGHT_2 ) != PAD_NONE ) up -= 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_LEFT_1 )  != PAD_NONE ) up += 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_LEFT_2 )  != PAD_NONE ) up -= 5.0f;
		if ( padAnalogX( 0, 0 ) >  0.05f ) angle += 2.0f * padAnalogX( 0, 0 );
		if ( padAnalogX( 0, 0 ) < -0.05f ) angle += 2.0f * padAnalogX( 0, 0 ); 
		if ( padAnalogY( 0, 0 ) >  0.05f ) pitch += 2.0f * padAnalogY( 0, 0 ); 
		if ( padAnalogY( 0, 0 ) < -0.05f ) pitch += 2.0f * padAnalogY( 0, 0 ); 
		if ( padAnalogX( 0, 1 ) >  0.05f ) turn =  2.0f * padAnalogX( 0, 1 ); 
		if ( padAnalogX( 0, 1 ) < -0.05f ) turn =  2.0f * padAnalogX( 0, 1 ); 
		if ( padAnalogY( 0, 1 ) >  0.05f ) time += 0.1f * padAnalogY( 0, 1 );
		if ( padAnalogY( 0, 1 ) < -0.05f ) time += 0.1f * padAnalogY( 0, 1 );
		// limit
		if ( angle < 0.0f ) angle += 360.0f;
		if ( angle >= 360.0f ) angle -= 360.0f;
		if ( pitch < 0.0f ) pitch += 360.0f;
		if ( pitch >= 360.0f ) pitch -= 360.0f;
		if ( doturn ) { 
			turn += 0.25f;
			time += gfxTick() * 2.0f;
		}
		//pitch = 0.0f;
		// time:
		//time += 0.02f;
		if ( time >= end_time )
			time -= end_time;
		if ( time < 0.0f )
			time += end_time;
		/* set camera */
		gfxSetCamera( &eye, 0.0f, 0.0f, 500.0f );
		/* summer */
		gfxRotateObject( summer, GFX_Z_AXIS, angle );
		gfxRotateObject( summer, GFX_X_AXIS, pitch );
		gfxRotateObject( summer, GFX_Y_AXIS, turn );
		gfxSetObjectPosition( summer, time );
		gfxTranslateObject( summer, pos, y, up );
		gfxDrawObject( summer );
		gfxTranslateObject( summer, -(pos), -(y), -(up) );
		/* board */
		gfxRotateObject( board, GFX_Z_AXIS, angle );
		gfxRotateObject( board, GFX_X_AXIS, pitch );
		gfxRotateObject( board, GFX_Y_AXIS, turn );
		gfxSetObjectPosition( board, time );
		gfxTranslateObject( board, pos, y, up );
		gfxDrawObject( board );
		gfxTranslateObject( board, -(pos), -(y), -(up) );
		gfxDrawSprite( logo, 0.0f );
		// get frame time
		//printf( "time=%.2f\n", gfxTimerAverage(0) );
		sprintf( str, ",-.0123456789?\n\
ABCDEFGHIJKLMNOP\n\
QRSTUVWXYZ\n\
abcdefghijklmn\n\
opqrstuvwxyz\n\
time=%.2f\n", gfxTimerAverage(0) );
		surfFontPosition( &surf.font.font1, 0.025f, 0.05f );
		surfPrintFont( &surf.font.font1, str );
		gfxBufSwap();
		/* update animation */
		interpol += step;
		if ( interpol > 1.0f ) {
			last = frame;
			frame++;
			if ( frame == 3 )
				frame = 0;
			interpol -= 1.0f;
		}
		turn  = 0.0f;
		pole  = 0.0f;
		angle = 0.0f;
		pitch = 0.0f;
		//goto dump;
	}

#if 0
dump:
	gfxPs2MemoryDump( "saves", "vu0mem.dat", GFX_PS2_VU0_MEM, 0 );
	//gfxPs2MemoryDump( "saves", "sprmem.dat", GFX_PS2_SPR_MEM, 0 );
	//gfxPs2MemoryDump( "saves", "vu1mem.dat", GFX_PS2_VU1_MEM, 0 );
#endif
}


/**********************************************************
 * Function:	wave_test
 **********************************************************
 * Description: 
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void wave_test (
	void
	)
{
	GfxObject	*wave;
	Point_f3d	 eye;
	float		 angle;
	float		 pitch;
	int			 frame;
	float		 pos;
	float		 up;
	float		 y;
	float		 pole;
	float		 turn;
	Bool		 doturn;
	int			 last;
	float		 interpol;
	float		 step;
	float		 time;
	float		 end_time;

	/* init */
	eye.x = 0.0f;
	eye.y = 0.0f;
	eye.z = 2500.0f;
	eye.w = 1.0f;
	angle = 0.0f;
	pitch = 0.0f;
	pos   = 0.0f;
	pole  = 0.0f;
	up	  = 0.0f;
	y	  = 0.0f;
	turn  = 0.0f;
	doturn = 0;
	frame = 0;
	last = 0;
	interpol = 0.0f;
	step = 0.05f;
	time = 0.0f;

	/* lighting */
	gfxSetAmbient( 0.15f, 0.15f, 0.15f );
	//gfxSetAmbient( 0.15f, 0.15f, 0.15f );
	gfxSetFlatLight( 1, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, 1.0f );
	//gfxSetFlatLight( 2, 0.0f, -1.0f,   0.0f, 1.0f, 0.2f, 0.2f );
	//gfxSetFlatLight( 2, -1.0f, -1.0, -1.0f, 0.8f, 0.8f, 0.8f );
	//gfxSetFlatLight( 3, 1.0f, 1.0, 1.0f, 0.8f, 0.8f, 0.8f );
	gfxSetBackgroundColor( 0.5f, 0.5f, 0.8f );

	/* locate models */
	gfxFilePath( "models/waves" );

    /* create model */
	printf( "loading wave...\n" );
	surfLoadMap( "wave4.map", TRUE );
    if ( ! p2mModelToGfxObject( "fface6a", (GfxObject **)&wave, NULL ) ) {
		printf( p2mModelError() );
		surfExit(SURF_ERROR_JETSKI_LOAD_FAILED);
	}
	/* load animation data */
	surfLoadPos( wave, "wave6a.p2a" );
	end_time = 6.5f;
	printf( "done\n" );

	while(1) {
		//frame++;
		padRead();
		if ( padAnyButtonPressed( PAD_BUTTON_SELECT ) != PAD_NONE ) doturn = !doturn;
		if ( padAnyButtonPressed( PAD_BUTTON_START ) != PAD_NONE ) break;
		if ( padAnyButtonDown( PAD_BUTTON_C )  != PAD_NONE ) y += 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_A )  != PAD_NONE ) y -= 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_B )  != PAD_NONE ) pos -= 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_D )  != PAD_NONE ) pos += 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_UP    )  != PAD_NONE ) y += 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_DOWN  )  != PAD_NONE ) y -= 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_LEFT  )  != PAD_NONE ) pos -= 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_RIGHT )  != PAD_NONE ) pos += 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_RIGHT_1 ) != PAD_NONE ) up += 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_RIGHT_2 ) != PAD_NONE ) up -= 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_LEFT_1 )  != PAD_NONE ) up += 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_LEFT_2 )  != PAD_NONE ) up -= 5.0f;
		if ( padAnalogX( 0, 0 ) >  0.05f ) angle += 2.0f * padAnalogX( 0, 0 );
		if ( padAnalogX( 0, 0 ) < -0.05f ) angle += 2.0f * padAnalogX( 0, 0 ); 
		if ( padAnalogY( 0, 0 ) >  0.05f ) pitch += 2.0f * padAnalogY( 0, 0 ); 
		if ( padAnalogY( 0, 0 ) < -0.05f ) pitch += 2.0f * padAnalogY( 0, 0 ); 
		if ( padAnalogX( 0, 1 ) >  0.05f ) turn =  2.0f * padAnalogX( 0, 1 ); 
		if ( padAnalogX( 0, 1 ) < -0.05f ) turn =  2.0f * padAnalogX( 0, 1 ); 
		if ( padAnalogY( 0, 1 ) >  0.05f ) time += 0.1f * padAnalogY( 0, 1 );
		if ( padAnalogY( 0, 1 ) < -0.05f ) time += 0.1f * padAnalogY( 0, 1 );
		// limit
		if ( angle < 0.0f ) angle += 360.0f;
		if ( angle >= 360.0f ) angle -= 360.0f;
		if ( pitch < 0.0f ) pitch += 360.0f;
		if ( pitch >= 360.0f ) pitch -= 360.0f;
		if ( doturn ) { 
			turn += 0.25f;
			time += gfxTick() * 2.0f;
		}
		//pitch = 0.0f;
		// time:
		//time += 0.02f;
		if ( time >= end_time )
			time -= end_time;
		if ( time < 0.0f )
			time += end_time;
		//printf( "time=%.2f\n", time );
		//
		gfxSetCamera( &eye, 0.0f, 0.0f, 500.0f );
		gfxRotateObject( wave, GFX_Z_AXIS, angle );
		gfxRotateObject( wave, GFX_X_AXIS, pitch );
		gfxRotateObject( wave, GFX_Y_AXIS, turn );
		gfxSetObjectPosition( wave, time );
		gfxTranslateObject( wave, pos, y, up );
		gfxDrawObject( wave );
		gfxTranslateObject( wave, -(pos), -(y), -(up) );
		// get frame time
		printf( "time=%.2f\n", gfxTimerAverage(0) );
		gfxBufSwap();
		/* update animation */
		interpol += step;
		if ( interpol > 1.0f ) {
			last = frame;
			frame++;
			if ( frame == 3 )
				frame = 0;
			interpol -= 1.0f;
		}
		turn  = 0.0f;
		pole  = 0.0f;
		angle = 0.0f;
		pitch = 0.0f;
		//goto dump;
	}

#if 0
dump:
	gfxPs2MemoryDump( "saves", "sprmem.dat", GFX_PS2_SPR_MEM, 0 );
	gfxPs2MemoryDump( "saves", "vu1mem.dat", GFX_PS2_VU1_MEM, 0 );
#endif
}


/**********************************************************
 * Function:	board_test
 **********************************************************
 * Description: Prepare to play with the jet ski
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void beach_test (
	void
	)
{
	GfxObject	*wave[12];
	GfxObject	*side[4];
	GfxObject	*east[16];
	GfxObject	*west[16];
	int			 neast;
	int			 nwest;
	Point_f3d	 ref;
	float		 angle;
	float		 pitch;
	int			 frame;
	float		 pos;
	float		 up;
	float		 y;
	float		 pole;
	float		 turn;
	Bool		 doturn;
	int			 last;
	float		 interpol;
	float		 step;
	float		 time;
	float		 end_time;
	float		 rot;
	float		 dist;
	int			 i;
	Point_f3d	 wtrans[12];
	int			 yscroll;
	GfxTextureId texid;

	/* init */
	yscroll = 0;
	ref.x = 0.0f;
	ref.y = -4250.0f;
	ref.z =  500.0f;
	ref.w = 1.0f;
	angle = 45.0f;
	pitch = 75.0f;
	pos   = 0.0f;
	pole  = 0.0f;
	up	  = 0.0f;
	y	  = 0.0f;
	turn  = 0.0f;
	dist = 1.0f;
	doturn = 0;
	frame = 0;
	last = 0;
	interpol = 0.0f;
	step = 0.05f;
	time = 0.0f;
	rot = 0.0f;

	/* lighting */
	gfxSetAmbient( 0.15f, 0.15f, 0.15f );
	//gfxSetAmbient( 0.15f, 0.15f, 0.15f );
	gfxSetFlatLight( 1, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, 1.0f );
	//gfxSetFlatLight( 2, 0.0f, -1.0f,   0.0f, 1.0f, 0.2f, 0.2f );
	//gfxSetFlatLight( 2, -1.0f, -1.0, -1.0f, 0.8f, 0.8f, 0.8f );
	//gfxSetFlatLight( 3, 1.0f, 1.0, 1.0f, 0.8f, 0.8f, 0.8f );
	gfxSetBackgroundColor( 0.5f, 0.5f, 0.8f );

	/* locate models */
	gfxFilePath( "models/beaches/b4" );

    /* create model */
	printf( "loading beach...\n" );
	surfLoadMap( "beach4.map", TRUE );
    neast = p2mModelToGfxObject( "east", (GfxObject **)&east, NULL );
	if ( neast == 0 ) {
		printf( p2mModelError() );
		surfExit(SURF_ERROR_JETSKI_LOAD_FAILED);
	}
    nwest = p2mModelToGfxObject( "west", (GfxObject **)&west, NULL );
	if ( nwest == 0 ) {
		printf( p2mModelError() );
		surfExit(SURF_ERROR_JETSKI_LOAD_FAILED);
	}

	/* locate models */
	gfxFilePath( "models/waves" );

    /* create model */
	printf( "loading wave...\n" );
	surfLoadMap( "wave4.map", TRUE );
    if ( ! p2mModelToGfxObject( "fface6a", (GfxObject **)&wave[0], &texid ) ) {
		printf( p2mModelError() );
		surfExit(SURF_ERROR_JETSKI_LOAD_FAILED);
	}
    if ( ! p2mModelToGfxObject( "fface6b", (GfxObject **)&wave[1], NULL ) ) {
		printf( p2mModelError() );
		surfExit(SURF_ERROR_JETSKI_LOAD_FAILED);
	}
    if ( ! p2mModelToGfxObject( "side6e", (GfxObject **)&side[0], NULL ) ) {
		printf( p2mModelError() );
		surfExit(SURF_ERROR_JETSKI_LOAD_FAILED);
	}
    if ( ! p2mModelToGfxObject( "side6w", (GfxObject **)&side[1], NULL ) ) {
		printf( p2mModelError() );
		surfExit(SURF_ERROR_JETSKI_LOAD_FAILED);
	}
    if ( ! p2mModelToGfxObject( "side6n", (GfxObject **)&side[2], NULL ) ) {
		printf( p2mModelError() );
		surfExit(SURF_ERROR_JETSKI_LOAD_FAILED);
	}
    if ( ! p2mModelToGfxObject( "side6s", (GfxObject **)&side[3], NULL ) ) {
		printf( p2mModelError() );
		surfExit(SURF_ERROR_JETSKI_LOAD_FAILED);
	}
	for ( i=0; i<5; i++ ) {
		if ( ! p2mModelToGfxObject( "fface6c", (GfxObject **)&wave[2+i], NULL ) ) {
			printf( p2mModelError() );
			surfExit(SURF_ERROR_JETSKI_LOAD_FAILED);
		}
	}
	for ( i=0; i<4; i++ ) {
		if ( ! p2mModelToGfxObject( "fface6d", (GfxObject **)&wave[7+i], NULL ) ) {
			printf( p2mModelError() );
			surfExit(SURF_ERROR_JETSKI_LOAD_FAILED);
		}
	}

	/* load animation data */
	for ( i=0; i<11; i++ )
		surfLoadPos( wave[i], "wave6a.p2a" );
	end_time = 6.5f;
	printf( "done\n" );

	/* init */
	wtrans[0].x = 500.0f;
	wtrans[0].y = -4000.0f;
	wtrans[0].z = 0.0f;
	wtrans[1].x = -500.0f;
	wtrans[1].y = -4000.0f;
	wtrans[1].z = 0.0f;
	wtrans[2].x = 2000.0f;
	wtrans[2].y = -4000.0f;
	wtrans[2].z = 0.0f;
	wtrans[3].x = -2000.0f;
	wtrans[3].y = -4000.0f;
	wtrans[3].z = 0.0f;
	wtrans[4].x = 0.0f;
	wtrans[4].y = 4000.0f;
	wtrans[4].z = 0.0f;
	wtrans[5].x = 2000.0f;
	wtrans[5].y = 4000.0f;
	wtrans[5].z = 0.0f;
	wtrans[6].x = -2000.0f;
	wtrans[6].y = 4000.0f;
	wtrans[6].z = 0.0f;
	wtrans[7].x = 5000.0f;
	wtrans[7].y = -4000.0f;
	wtrans[7].z = 0.0f;
	wtrans[8].x = -5000.0f;
	wtrans[8].y = -4000.0f;
	wtrans[8].z = 0.0f;
	wtrans[9].x = 5000.0f;
	wtrans[9].y = 4000.0f;
	wtrans[9].z = 0.0f;
	wtrans[10].x = -5000.0f;
	wtrans[10].y = 4000.0f;
	wtrans[10].z = 0.0f;

	while(1) {
		//frame++;
		padRead();
		if ( padAnyButtonPressed( PAD_BUTTON_SELECT ) != PAD_NONE ) doturn = !doturn;
		if ( padAnyButtonPressed( PAD_BUTTON_START ) != PAD_NONE ) break;
		if ( padAnyButtonDown( PAD_BUTTON_C )  != PAD_NONE ) y += 50.0f;
		if ( padAnyButtonDown( PAD_BUTTON_A )  != PAD_NONE ) y -= 50.0f;
		if ( padAnyButtonDown( PAD_BUTTON_B )  != PAD_NONE ) pos -= 50.0f;
		if ( padAnyButtonDown( PAD_BUTTON_D )  != PAD_NONE ) pos += 50.0f;
		if ( padAnyButtonDown( PAD_BUTTON_UP    )  != PAD_NONE ) ref.z += 50.0f;
		if ( padAnyButtonDown( PAD_BUTTON_DOWN  )  != PAD_NONE ) ref.z -= 50.0f;
		if ( padAnyButtonDown( PAD_BUTTON_LEFT  )  != PAD_NONE ) rot -= 1.0f;
		if ( padAnyButtonDown( PAD_BUTTON_RIGHT )  != PAD_NONE ) rot += 1.0f;
		if ( padAnyButtonDown( PAD_BUTTON_RIGHT_1 ) != PAD_NONE ) dist += 0.01f;
		if ( padAnyButtonDown( PAD_BUTTON_RIGHT_2 ) != PAD_NONE ) dist -= 0.01f;
		if ( padAnyButtonDown( PAD_BUTTON_LEFT_1 )  != PAD_NONE ) dist += 1.0f;
		if ( padAnyButtonDown( PAD_BUTTON_LEFT_2 )  != PAD_NONE ) dist -= 1.0f;
		if ( padAnalogX( 0, 0 ) >  0.05f ) angle += 2.0f * padAnalogX( 0, 0 );
		if ( padAnalogX( 0, 0 ) < -0.05f ) angle += 2.0f * padAnalogX( 0, 0 ); 
		if ( padAnalogY( 0, 0 ) >  0.05f ) pitch += 2.0f * padAnalogY( 0, 0 ); 
		if ( padAnalogY( 0, 0 ) < -0.05f ) pitch += 2.0f * padAnalogY( 0, 0 ); 
		if ( padAnalogX( 0, 1 ) >  0.05f ) rot +=  2.0f * padAnalogX( 0, 1 ); 
		if ( padAnalogX( 0, 1 ) < -0.05f ) rot +=  2.0f * padAnalogX( 0, 1 ); 
		if ( padAnalogY( 0, 1 ) >  0.05f ) time += 0.1f * padAnalogY( 0, 1 );
		if ( padAnalogY( 0, 1 ) < -0.05f ) time += 0.1f * padAnalogY( 0, 1 );
		// limit
		if ( angle < 0.0f ) angle += 360.0f;
		if ( angle >= 360.0f ) angle -= 360.0f;
		if ( pitch < 0.0f ) pitch += 360.0f;
		if ( pitch >= 360.0f ) pitch -= 360.0f;
		if ( doturn ) { 
			turn += 0.25f;
			time += gfxTick() * 2.0f;
		}
		//pitch = 0.0f;
		// time:
		//time += 0.02f;
		if ( time >= end_time )
			time -= end_time;
		if ( time < 0.0f )
			time += end_time;
		//printf( "time=%.2f\n", time );
		//
		//if ( dist < 0.1f ) dist = 0.1f;
		//printf( "dist=%.2f ", dist );
		gfxSetCamera( &ref, angle, pitch, dist );
		/* draw beach */
		gfxRotateObject( east[0], GFX_Z_AXIS, rot );
		gfxTranslateObject( east[0], pos, y, up );
		for ( i=0; i<neast; i++ )
			gfxDrawObject( east[i] );
		gfxTranslateObject( east[0], -(pos), -(y), -(up) );
		// side
		for ( i=0; i<4; i++ ) {
			gfxTranslateObject( side[i], pos, y, up );
			gfxDrawObject( side[i] );
			gfxTranslateObject( side[i], -(pos), -(y), -(up) );
		}
		/* draw beach */
		gfxRotateObject( west[0], GFX_Z_AXIS, rot );
		gfxTranslateObject( west[0], pos, y, up );
		for ( i=0; i<nwest; i++ )
			gfxDrawObject( west[i] );
		gfxTranslateObject( west[0], -(pos), -(y), -(up) );
		for ( i=0; i<11; i++ ) {
			gfxSetObjectPosition( wave[i], time );
			//gfxTranslateObject( wave[i], pos + wtrans[i].x, y + wtrans[i].y, up );
			gfxTranslateObject( wave[i], pos + wtrans[i].x, y + wtrans[i].y, up );
			gfxDrawObject( wave[i] );
		}
		for ( i=0; i<11; i++ )
			gfxTranslateObject( wave[i], -(pos + wtrans[i].x), -(y + wtrans[i].y), -(up) );
		// get frame time
		printf( "time=%.2f\n", gfxTimerAverage(0) );
		gfxBufSwap();
		/* update scroll */
		yscroll--;
		if ( yscroll < 0 )
			yscroll = 127;
		gfxScrollTexture( texid, yscroll );
		/* update animation */
		interpol += step;
		if ( interpol > 1.0f ) {
			last = frame;
			frame++;
			if ( frame == 3 )
				frame = 0;
			interpol -= 1.0f;
		}
		turn  = 0.0f;
		pole  = 0.0f;
		//angle = 0.0f;
		//pitch = 0.0f;
		rot   = 0.0f;
		//goto dump;
	}

#if 0
dump:
	//gfxPs2MemoryDump( "saves", "sprmem.dat", GFX_PS2_SPR_MEM, 0 );
	gfxPs2MemoryDump( "saves", "vu0mem.dat", GFX_PS2_VU0_MEM, 0 );
	//gfxPs2MemoryDump( "saves", "vu1mem.dat", GFX_PS2_VU1_MEM, 0 );
#endif
}


/**********************************************************
 * Function:	wave_test2
 **********************************************************
 * Description: 
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void wave_test2 (
	void
	)
{
	GfxObject	*wave;
	Point_f3d	 ref;
	float		 angle;
	float		 pitch;
	int			 frame;
	float		 pos;
	float		 up;
	float		 y;
	float		 pole;
	float		 turn;
	Bool		 doturn;
	int			 last;
	float		 interpol;
	float		 step;
	float		 time;
	float		 end_time;
	float		 times[8];
	int			 sec;
	float		 adjust;
	int			 yscroll;
	GfxTextureId texid;

	/* init */
	yscroll = 0;
	ref.x = 0.0f;
	ref.y = 0.0f;
	ref.z = 2500.0f;
	ref.w = 1.0f;
	angle = 0.0f;
	pitch = 0.0f;
	pos   = 0.0f;
	pole  = 0.0f;
	up	  = 0.0f;
	y	  = 0.0f;
	turn  = 0.0f;
	doturn = 0;
	frame = 0;
	last = 0;
	interpol = 0.0f;
	step = 0.05f;
	time = 0.0f;
	times[0] = 5.0f;
	times[1] = 4.0f;
	times[2] = 3.0f;
	times[3] = 2.0f;
	times[4] = 1.0f;
	sec = 0;
	adjust = 0.0f;

	/* lighting */
	gfxSetAmbient( 0.15f, 0.15f, 0.15f );
	//gfxSetAmbient( 0.15f, 0.15f, 0.15f );
	gfxSetFlatLight( 1, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, 1.0f );
	//gfxSetFlatLight( 2, 0.0f, -1.0f,   0.0f, 1.0f, 0.2f, 0.2f );
	//gfxSetFlatLight( 2, -1.0f, -1.0, -1.0f, 0.8f, 0.8f, 0.8f );
	//gfxSetFlatLight( 3, 1.0f, 1.0, 1.0f, 0.8f, 0.8f, 0.8f );
	gfxSetBackgroundColor( 0.5f, 0.5f, 0.8f );

	/* locate models */
	gfxFilePath( "models/waves" );

    /* create model */
	printf( "loading wave...\n" );
	surfLoadMap( "wave4.map", TRUE );
    if ( ! p2mModelToGfxObject( "fface6a", (GfxObject **)&wave, &texid ) ) {
		printf( p2mModelError() );
		surfExit(SURF_ERROR_JETSKI_LOAD_FAILED);
	}
	/* load animation data */
	surfLoadPos( wave, "wave6a.p2a" );
	end_time = 13.0f;
	printf( "done\n" );

	while(1) {
		//frame++;
		padRead();
		if ( padAnyButtonPressed( PAD_BUTTON_SELECT ) != PAD_NONE )
			ps2ActionAdjustTime( wave, times, 5 );
		if ( padAnyButtonPressed( PAD_BUTTON_START ) != PAD_NONE ) break;
		if ( padAnyButtonDown( PAD_BUTTON_C )  != PAD_NONE ) y += 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_A )  != PAD_NONE ) y -= 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_B )  != PAD_NONE ) pos -= 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_D )  != PAD_NONE ) pos += 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_UP    )  != PAD_NONE ) adjust += 0.01f;
		if ( padAnyButtonDown( PAD_BUTTON_DOWN  )  != PAD_NONE ) adjust -= 0.01f;
		if ( padAnyButtonDown( PAD_BUTTON_LEFT  )  != PAD_NONE ) pos -= 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_RIGHT )  != PAD_NONE ) pos += 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_RIGHT_1 ) != PAD_NONE ) up += 5.0f;
		if ( padAnyButtonDown( PAD_BUTTON_LEFT_1 )  != PAD_NONE ) up += 5.0f;
		if ( padAnyButtonPressed( PAD_BUTTON_RIGHT_2 ) != PAD_NONE ) sec++;
		if ( padAnyButtonPressed( PAD_BUTTON_LEFT_2 )  != PAD_NONE ) sec--;
		if ( padAnalogX( 0, 0 ) >  0.05f ) angle += 2.0f * padAnalogX( 0, 0 );
		if ( padAnalogX( 0, 0 ) < -0.05f ) angle += 2.0f * padAnalogX( 0, 0 ); 
		if ( padAnalogY( 0, 0 ) >  0.05f ) pitch += 2.0f * padAnalogY( 0, 0 ); 
		if ( padAnalogY( 0, 0 ) < -0.05f ) pitch += 2.0f * padAnalogY( 0, 0 ); 
		if ( padAnalogX( 0, 1 ) >  0.05f ) turn =  2.0f * padAnalogX( 0, 1 ); 
		if ( padAnalogX( 0, 1 ) < -0.05f ) turn =  2.0f * padAnalogX( 0, 1 ); 
		if ( padAnalogY( 0, 1 ) >  0.05f ) time += 0.1f * padAnalogY( 0, 1 );
		if ( padAnalogY( 0, 1 ) < -0.05f ) time += 0.1f * padAnalogY( 0, 1 );
		// limit
		if ( sec > 4 ) sec = 0;
		if ( sec < 0 ) sec = 4;
		times[sec] += adjust;
		if ( adjust != 0.0f )
			ps2ActionAdjustTime( wave, times, 5 );
		if ( angle < 0.0f ) angle += 360.0f;
		if ( angle >= 360.0f ) angle -= 360.0f;
		if ( pitch < 0.0f ) pitch += 360.0f;
		if ( pitch >= 360.0f ) pitch -= 360.0f;
		if ( doturn ) { 
			turn += 0.25f;
			time += gfxTick() * 2.0f;
		}
		//pitch = 0.0f;
		// time:
		//time += 0.02f;
		if ( time >= end_time )
			time -= end_time;
		if ( time < 0.0f )
			time += end_time;
		//printf( "time=%.2f\n", time );
		//
		gfxSetCamera( &ref, 0.0f, 0.0f, 500.0f );
		gfxRotateObject( wave, GFX_Z_AXIS, angle );
		gfxRotateObject( wave, GFX_X_AXIS, pitch );
		gfxRotateObject( wave, GFX_Y_AXIS, turn );
		gfxSetObjectPosition( wave, time );
		gfxTranslateObject( wave, pos, y, up );
		gfxDrawObject( wave );
		gfxTranslateObject( wave, -(pos), -(y), -(up) );
		// get frame time
		//intf( "time=%.2f\n", gfxTimerAverage(0) );
		gfxBufSwap();
		/* update scroll */
		yscroll--;
		if ( yscroll < 0 )
			yscroll = 127;
		gfxScrollTexture( texid, yscroll );
		/* update animation */
		interpol += step;
		if ( interpol > 1.0f ) {
			last = frame;
			frame++;
			if ( frame == 3 )
				frame = 0;
			interpol -= 1.0f;
		}
		turn  = 0.0f;
		pole  = 0.0f;
		angle = 0.0f;
		pitch = 0.0f;
		adjust = 0.0f;
		//goto dump;
	}

#if 0
dump:
	gfxPs2MemoryDump( "saves", "sprmem.dat", GFX_PS2_SPR_MEM, 0 );
	gfxPs2MemoryDump( "saves", "vu1mem.dat", GFX_PS2_VU1_MEM, 0 );
#endif
}



/**********************************************************
 * Function:	board_test
 **********************************************************
 * Description: Prepare to play with the jet ski
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void wave_test3 (
	void
	)
{
	GfxObject	*wave[12];
	GfxObject	*side[4];
	GfxObject	*east[16];
	GfxObject	*west[16];
	int			 neast;
	int			 nwest;
	Point_f3d	 ref;
	float		 angle;
	float		 pitch;
	int			 frame;
	float		 pos;
	float		 up;
	float		 y;
	float		 pole;
	float		 turn;
	Bool		 doturn;
	int			 last;
	float		 interpol;
	float		 step;
	float		 time;
	float		 end_time;
	float		 rot;
	float		 dist;
	int			 i;
	Point_f3d	 wtrans[12];
	int			 yscroll;
	GfxTextureId texid;
	float		 times[30];
	float		 wx, wy;
	char		 str[256];
	float		 shift;

	/* init */
	yscroll = 0;
	ref.x = 0.0f;
	ref.y = -4250.0f;
	ref.z =  500.0f;
	ref.w = 1.0f;
	angle = 45.0f;
	pitch = 75.0f;
	pos   = 0.0f;
	pole  = 0.0f;
	up	  = 0.0f;
	y	  = 0.0f;
	turn  = 0.0f;
	dist = 1.0f;
	doturn = 0;
	frame = 0;
	last = 0;
	interpol = 0.0f;
	step = 0.05f;
	time = 0.0f;
	rot = 0.0f;
	for ( i=0; i<30; i++ )
		times[i] = 0.0f;

	surfInitContour( &surf.contour, 2 );	
	wx = 48128.0f;
	wy = 59392.0f;

#define HEIGHT(_offset)		(4.0f - surfGetWaveHeight( &surf.contour, (wx - shift + _offset), wy ))

#if 1
	/* test */
	times[0]  = 4.8f;
	times[1]  = 4.2f;
	times[2]  = 3.6f;
	times[3]  = 3.0f;
	times[4]  = 2.4f;
	times[5]  = 2.0f;
	times[6]  = 1.6f;
	times[7]  = 1.2f;
	times[8]  = 0.8f;
	times[9]  = 0.6f;
	times[10] = 0.4f;
	times[11] = 0.2f;
	times[12] = 0.0f;
	times[13] = 0.2f;
	times[14] = 0.4f;
	times[15] = 0.6f;
	times[16] = 0.8f;
	times[17] = 1.2f;
	times[18] = 1.6f;
	times[19] = 2.0f;
	times[20] = 2.4f;
	times[21] = 3.0f;
	times[22] = 3.6f;
	times[23] = 4.2f;
	times[24] = 4.8f;
#endif

	/* lighting */
	gfxSetAmbient( 0.15f, 0.15f, 0.15f );
	//gfxSetAmbient( 0.15f, 0.15f, 0.15f );
	//gfxSetFlatLight( 1, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, 1.0f );
	gfxSetFlatLight( 2, 0.0f, -1.0f,   0.0f, 1.0f, 1.0f, 1.0f );
	//gfxSetFlatLight( 2, -1.0f, -1.0, -1.0f, 0.8f, 0.8f, 0.8f );
	//gfxSetFlatLight( 3, 1.0f, 1.0, 1.0f, 0.8f, 0.8f, 0.8f );
	gfxSetBackgroundColor( 0.5f, 0.5f, 0.8f );

	/* locate models */
	gfxFilePath( "models/waves" );

    /* create model */
	printf( "loading wave...\n" );
	surfLoadMap( "wave4.map", TRUE );
	p2mModelToGfxObject( "fface7d", (GfxObject **)&wave[0], NULL );
	p2mModelToGfxObject( "fface7c", (GfxObject **)&wave[1], NULL );
	p2mModelToGfxObject( "fface7b", (GfxObject **)&wave[2], &texid );
	p2mModelToGfxObject( "fface7a", (GfxObject **)&wave[3], NULL );
	p2mModelToGfxObject( "fface7c", (GfxObject **)&wave[4], NULL );
	p2mModelToGfxObject( "fface7d", (GfxObject **)&wave[5], NULL );

	/* load animation data */
	for ( i=0; i<6; i++ )
		surfLoadPos( wave[i], "wave8.p2a" );
	end_time = 4.0f;
	printf( "done\n" );

	/* init */
	wtrans[3].x = 500.0f;
	wtrans[3].y = -4000.0f;
	wtrans[3].z = 0.0f;
	wtrans[2].x = -500.0f;
	wtrans[2].y = -4000.0f;
	wtrans[2].z = 0.0f;
	wtrans[4].x = 2000.0f;
	wtrans[4].y = -4000.0f;
	wtrans[4].z = 0.0f;
	wtrans[1].x = -2000.0f;
	wtrans[1].y = -4000.0f;
	wtrans[1].z = 0.0f;
	wtrans[5].x = 5000.0f;
	wtrans[5].y = -4000.0f;
	wtrans[5].z = 0.0f;
	wtrans[0].x = -5000.0f;
	wtrans[0].y = -4000.0f;
	wtrans[0].z = 0.0f;

	// init font display
	surfResetFont( &surf.font.font1 );
	surfFontDepth( &surf.font.font1, 0.0f );

	while(1) {
		//frame++;
		padRead();
		if ( padAnyButtonPressed( PAD_BUTTON_SELECT ) != PAD_NONE ) doturn = !doturn;
		if ( padAnyButtonPressed( PAD_BUTTON_START ) != PAD_NONE ) break;
		if ( padAnyButtonDown( PAD_BUTTON_C )  != PAD_NONE ) y += 50.0f;
		if ( padAnyButtonDown( PAD_BUTTON_A )  != PAD_NONE ) y -= 50.0f;
		if ( padAnyButtonDown( PAD_BUTTON_B )  != PAD_NONE ) pos -= 50.0f;
		if ( padAnyButtonDown( PAD_BUTTON_D )  != PAD_NONE ) pos += 50.0f;
		if ( padAnyButtonDown( PAD_BUTTON_UP    )  != PAD_NONE ) wy += 25.0f;
		if ( padAnyButtonDown( PAD_BUTTON_DOWN  )  != PAD_NONE ) wy -= 25.0f;
		if ( padAnyButtonDown( PAD_BUTTON_LEFT  )  != PAD_NONE ) wx -= 25.0f;
		if ( padAnyButtonDown( PAD_BUTTON_RIGHT )  != PAD_NONE ) wx += 25.0f;
		if ( padAnyButtonDown( PAD_BUTTON_RIGHT_1 ) != PAD_NONE ) dist += 0.01f;
		if ( padAnyButtonDown( PAD_BUTTON_RIGHT_2 ) != PAD_NONE ) dist -= 0.01f;
		if ( padAnyButtonDown( PAD_BUTTON_LEFT_1 )  != PAD_NONE ) ref.z += 50.0f;
		if ( padAnyButtonDown( PAD_BUTTON_LEFT_2 )  != PAD_NONE ) ref.z -= 50.0f;
		if ( padAnalogX( 0, 0 ) >  0.05f ) angle += 2.0f * padAnalogX( 0, 0 );
		if ( padAnalogX( 0, 0 ) < -0.05f ) angle += 2.0f * padAnalogX( 0, 0 ); 
		if ( padAnalogY( 0, 0 ) >  0.05f ) pitch += 2.0f * padAnalogY( 0, 0 ); 
		if ( padAnalogY( 0, 0 ) < -0.05f ) pitch += 2.0f * padAnalogY( 0, 0 ); 
		if ( padAnalogX( 0, 1 ) >  0.05f ) rot +=  2.0f * padAnalogX( 0, 1 ); 
		if ( padAnalogX( 0, 1 ) < -0.05f ) rot +=  2.0f * padAnalogX( 0, 1 ); 
		if ( padAnalogY( 0, 1 ) >  0.05f ) time += 0.1f * padAnalogY( 0, 1 );
		if ( padAnalogY( 0, 1 ) < -0.05f ) time += 0.1f * padAnalogY( 0, 1 );
		// limit
		if ( angle < 0.0f ) angle += 360.0f;
		if ( angle >= 360.0f ) angle -= 360.0f;
		if ( pitch < 0.0f ) pitch += 360.0f;
		if ( pitch >= 360.0f ) pitch -= 360.0f;
		if ( doturn ) { 
			turn += 0.25f;
			time += gfxTick() * 2.0f;
		}
		//pitch = 0.0f;
		// time:
		//time += 0.02f;
		time = 3.999f;
		if ( time >= end_time )
			time -= end_time;
		if ( time < 0.0f )
			time += end_time;
		//printf( "time=%.2f\n", time );
		//
		//if ( dist < 0.1f ) dist = 0.1f;
		//printf( "dist=%.2f ", dist );

		/* test */
		shift = (float)((int)wx % 2000);
		times[0]  = HEIGHT(-7000.0f);
		times[1]  = HEIGHT(-6000.0f);
		times[2]  = HEIGHT(-5000.0f);
		times[3]  = HEIGHT(-4000.0f);
		times[4]  = HEIGHT(-3000.0f);
		times[5]  = HEIGHT(-2500.0f);
		times[6]  = HEIGHT(-2000.0f);
		times[7]  = HEIGHT(-1500.0f);
		times[8]  = HEIGHT(-1000.0f);
		times[9]  = HEIGHT(-750.0f);
		times[10] = HEIGHT(-500.0f);
		times[11] = HEIGHT(-250.0f);
		times[12] = HEIGHT(0.0f);
		times[13] = HEIGHT(250.0f);
		times[14] = HEIGHT(500.0f);
		times[15] = HEIGHT(750.0f);
		times[16] = HEIGHT(1000.0f);
		times[17] = HEIGHT(1500.0f);
		times[18] = HEIGHT(2000.0f);
		times[19] = HEIGHT(2500.0f);
		times[20] = HEIGHT(3000.0f);
		times[21] = HEIGHT(4000.0f);
		times[22] = HEIGHT(5000.0f);
		times[23] = HEIGHT(6000.0f);
		times[24] = HEIGHT(7000.0f);

		gfxSetCamera( &ref, angle, pitch, dist );
		for ( i=0; i<6; i++ ) {
			ps2ActionAdjustTime( wave[i], &times[(i*4)], 5 );
			gfxSetObjectPosition( wave[i], time );
			//gfxTranslateObject( wave[i], pos + wtrans[i].x, y + wtrans[i].y, up );
			gfxTranslateObject( wave[i], pos + wtrans[i].x - shift, y + wtrans[i].y, up );
			gfxDrawObject( wave[i] );
		}
		for ( i=0; i<6; i++ )
			gfxTranslateObject( wave[i], -(pos + wtrans[i].x - shift), -(y + wtrans[i].y), -(up) );

		sprintf( str, "x=%.1f, y=%.1f, height=%.2f, time=%.2f\n", wx, wy, HEIGHT(0.0f), gfxTimerAverage(0) );
		surfFontPosition( &surf.font.font1, 0.025f, 0.05f );
		surfPrintFont( &surf.font.font1, str );
		sprintf( str, "shift=%.1f", shift );
		surfFontPosition( &surf.font.font1, 0.025f, 0.1f );
		surfPrintFont( &surf.font.font1, str );

		gfxBufSwap();
		/* update scroll */
		yscroll--;
		if ( yscroll < 0 )
			yscroll = 127;
		gfxScrollTexture( texid, yscroll );
		/* update animation */
		interpol += step;
		if ( interpol > 1.0f ) {
			last = frame;
			frame++;
			if ( frame == 3 )
				frame = 0;
			interpol -= 1.0f;
		}
		turn  = 0.0f;
		pole  = 0.0f;
		//angle = 0.0f;
		//pitch = 0.0f;
		rot   = 0.0f;
		//goto dump;
	}
}

