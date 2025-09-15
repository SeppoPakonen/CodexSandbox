/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		surfgame.h
 * Author:		Mark Theyer
 * Created:		13 Jul 1999
 **********************************************************
 * Description:	Master header file
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 13-Jul-99	Theyer	Initial coding
 * 31-May-01	Theyer	Initial coding for PS2 game
 *
 **********************************************************/

#ifndef SURF_SURFGAME_H
#define SURF_SURFGAME_H

/* 
 * includes
 */

#include <stdio.h>

#include <compile.h>
#include <type/datatype.h>
#include <pad/pad.h>
#include <gfx/gfx.h>
#include <text/text.h>
#include <binary/binary.h>
#include <memory/memory.h>
#include <matrix/matrix.h>
#include <ps2/ps2model.h>
#include <ps2/ps2image.h>
#include <sound/sound.h>
#include <memcard/memcard.h>
#include <shiftjis/shiftjis.h>
#include <parse/parse.h>
#include <movie/movie.h>

#include <sounds.h>
#include <vehicle.h>
#include <contour.h>
#include <person.h>
#include <random.h>
#include <surfutil.h>
#include <surferr.h>
#include <surfmsg.h>
#include <font.h>
#include <camera.h>
#include <keyevent.h>
#include <surfer.h>
#include <rider.h>
#include <pilot.h>
#include <sailor.h>
#include <surfbrd.h>
#include <bodybrd.h>
#include <jetski.h>
#include <sailbrd.h>
#include <splash.h>
#include <wave.h>

/* 
 * macros 
 */

// select video mode PAL or NTSC
#define SURF_VIDEO_MODE_NTSC				 1

// build options
#define SURF_TESTMODE						 1
#define SURF_ENABLE_SAVESCREEN				 0
#define SURF_PROTOTYPE_VERSION				 0
#define SURF_SSX_STYLE_CONTROLS				 1

// maximum number of waves in view or drawable
#define SURF_MAX_WAVES						 3

// data file names
#define SURF_PATH_INGAME_PAK				 "ingame.pak"
#define SURF_PATH_FONTS_PAK					 "fonts.pak"
#define SURF_PATH_FRONTEND_PAK				 "frontend.pak"
#define SURF_PATH_RESULTS_PAK				 "results.pak"
#define SURF_PATH_RIDERS_PAK				 "riders.pak"
#define SURF_PATH_BEACHES_PAK				 "beaches.pak"

// location of robot replay files
#if SURF_VIDEO_MODE_NTSC
#define SURF_PATH_ROBOTS					 "robots"
#else
#define SURF_PATH_ROBOTS					 "palbots"
#endif

// California Watersports path names
#define SURF_PATH_MAPS						 "maps"
#define SURF_PATH_IMAGES					 "images"
#define SURF_PATH_IMAGES_FONTS				 "images/fonts"
#define SURF_PATH_IMAGES_INGAME				 "images/ingame"
#define SURF_PATH_IMAGES_FRONTEND			 "images/frontend"
#define SURF_PATH_IMAGES_RESULTS			 "images/results"
#define SURF_PATH_IMAGES_BEACHES			 "images/beaches"
#define SURF_PATH_IMAGES_RIDERS				 "images/riders"
#if SURF_VIDEO_MODE_NTSC
#define SURF_PATH_DEMOS						 "demos/ntsc"
#define SURF_PATH_IMAGES_SCREENS			 "images/ntsc"
#else
#define SURF_PATH_DEMOS						 "demos/pal"
#define SURF_PATH_IMAGES_SCREENS			 "images/screens"
#endif

/* languages */
#define SURF_LANGUAGE_NONE					 0
#define SURF_LANGUAGE_ENGLISH				 1
#define SURF_LANGUAGE_FRENCH				 2
#define SURF_LANGUAGE_GERMAN				 3
#define SURF_NUM_LANGUAGES					 3

/* sport types */
#define SURF_SPORT_NONE						 0
#define SURF_SPORT_SURFBOARD				 1
#define SURF_SPORT_JETSKI					 2
#define SURF_SPORT_WINDSURF					 3
#define SURF_SPORT_BODYBOARD				 4
#define SURF_NUM_SPORTS						 4

/* limits */
#define SURF_MAX_PLAYERS					 4

/* surf controller pads */
#define SURF_NUM_PADS						 8				// number of pads supported
#define SURF_PAD_ATTACHED					 0x1			// attached
#define SURF_PAD_SUPPORTED					 0x2			// supported
#define SURF_PAD_OK							 0x3			// attached and supported
#define SURF_PAD_HAS_ANALOG					 0x4			// has analog stick(s)
#define SURF_PAD_HAS_SHOCK					 0x8			// has shock functionality

/* test modes */
#define SURF_TESTMODE_PLAY					 0
#define SURF_TESTMODE_VIEW					 1

/* play modes */
#define SURF_PLAYMODE_INIT					-1
#define SURF_PLAYMODE_MENU					 0
#define SURF_PLAYMODE_FREERIDE				 1
#define SURF_PLAYMODE_SPLIT_SCREEN			 2
#define SURF_PLAYMODE_BIG_AIR_COMP			 3
#define SURF_PLAYMODE_RIDING_COMP			 4
#define SURF_PLAYMODE_RIDING_CHAMP			 5
#define SURF_PLAYMODE_EXTREME_TRICK			 6
#define SURF_PLAYMODE_ATTRACT				 7

/* minimum (fixed,4096=1.0f) absolute input value from an analog input device */
#define SURF_ANALOG_MIN_INPUT			  0.1f

/* pad macros */
#if SURF_ENABLE_SAVESCREEN
/* test mode can do a screen dump */
#define surfPadRead()			((padRead()==FALSE)?(surfUpdateControl()):(surfTestModeDumpScreen()))
#else
/* jump to control handler if a change in controllers or states */
#define surfPadRead()			((padRead()==FALSE)?(surfUpdateControl()):(0))
#endif
#define surfPadDown(button)		(padButtonDown(surf.control.active,button))
#define surfPadPressed(button)	(padButtonPressed(surf.control.active,button))
#define surfPadPressure(button)	(padButtonPressure(surf.control.active,button))
#define surfPadOK(i)			((surf.control.pad_status[i] & SURF_PAD_OK) == SURF_PAD_OK)
#define surfPadSelect()			(surfPadPressed(PAD_BUTTON_A))
#define surfPadGoBack()			(surfPadPressed(PAD_BUTTON_C))
#define surfPadNext()			(surfPadPressed((PAD_BUTTON_DOWN|PAD_BUTTON_RIGHT)))
#define surfPadPrev()			(surfPadPressed((PAD_BUTTON_UP|PAD_BUTTON_LEFT)))
#define surfPadHigher()			(surfPadPressed((PAD_BUTTON_UP|PAD_BUTTON_RIGHT)))
#define surfPadLower()			(surfPadPressed((PAD_BUTTON_DOWN|PAD_BUTTON_LEFT)))
#define surfPadHigherDown()		(surfPadDown((PAD_BUTTON_UP|PAD_BUTTON_RIGHT)))
#define surfPadLowerDown()		(surfPadDown((PAD_BUTTON_DOWN|PAD_BUTTON_LEFT)))
#define surfPadOptionMenu()		(surfPadDown(PAD_BUTTON_SELECT))
#define surfPadPause()			(surfPadDown(PAD_BUTTON_START))
#define surfPadGoUp()			(surfPadPressed(PAD_BUTTON_UP))
#define surfPadGoDown()			(surfPadPressed(PAD_BUTTON_DOWN))
#define surfPadGoRight()		(surfPadPressed(PAD_BUTTON_RIGHT))
#define surfPadGoLeft()			(surfPadPressed(PAD_BUTTON_LEFT))

// debug DMA/VIF/GIF/XGKICK arg 2 for debug_file() in testing.c
#define DEBUG_TEST_XGKICK		1
#define DEBUG_TEST_DMA_GIF		2

/*
 * typedefs
 */

// test mode management
typedef struct {
    int					 mode;
	SurfCamera			 camera;
} SurfTestInfo;
    
// rider selection
typedef struct {
	Byte				 rider;
	Byte				 board;
	Byte				 robot;
	Byte				 difficulty;
} SurfRiderSelection;

// controller info
typedef struct {
	short				 master;							// master controller pad
	short				 active;							// active controller pad
	int					 pad_status[SURF_NUM_PADS];			// pad status
} SurfControl;

// beach cube background
typedef struct {
	GfxObject			*island[8];
	GfxObject			*object;
	GfxObject			*north;
	GfxObject			*east;//[4];
	GfxObject			*west;//[4];
	Bool				 draw_east;
	Bool				 draw_west;
} SurfBeach;

// seabed bottom objects
typedef struct {
	GfxObject			*object;
} SurfBottom;

// data for font sprites and sizes
typedef struct {
	SurfFontData		 num1;
	GfxSpriteId			 num1_chars[10];	
	float				 num1_sizes[10];
	SurfFontData		 font1;
	GfxSpriteId			 font1_chars[80];	
	float				 font1_sizes[80];
} SurfFont;

// data for in-game graphics
typedef struct {
	GfxSpriteId			 gfxlogo;	
} SurfInGame;

// per player/display screen informaton
typedef struct {
	Bool				 robot;					// robot player
	SurfCamera			 camera;				// camera info
	Bool				 crash;					// crash flag
	Bool				 out_the_back;			// out the back flag
	int					 tricknum;				// current trick number (training mode only)
	SurfVehicle			 vehicle;				// surf vehicle
	SurfJetSki			 jetski;				// jet ski surf vehicle
	SurfBoard			 board;					// surfboard surf vehicle
	PersonInfo			 player;				// player
	int					 posx;					// player x position 
	int					 posy;					// player y position
	int					 last_wave_position;	// last wave position
} SurfPlayer;

/* global information */
typedef struct {
	// flags
	int					 playmode;							// mode of play
	// modifiable game options
	int					 nplayers;							// number of players
	Bool				 slomoair;							// slomo air switch
	// software managed state and attribute information
	Bool				 paused;							// paused flag
	Bool				 replay;							// running a replay flag
	Bool				 abort_game;						// abort game flag
	SurfControl			 control;							// controller info
	int					 nwaves;							// number of waves in view
	WaveTypeData		*wave;								// currently viewable (and rideable) wave
	WaveTypeData		*waves[SURF_MAX_WAVES];				// currently viewable waves
	SurfPlayer			*current;							// current player
	int					 active_player;						// currently active player
	float				 tick;								// game clock tick (1.0=60fps, 2.0=30fps, 3.0=20fps,etc)
	// colors
	// fonts
	SurfFont			 font;								// game fonts
    // game data
	SurfPlayer			 player[SURF_MAX_PLAYERS];			// player data
	SurfInGame			 ingame;							// in-game graphics
	SurfBeach			 beach;								// beach background graphics
	SurfBottom			 bottom;							// bottom objects
	// wave data 
	SurfContour			 contour;							// ocean bottom surface contour
	WaveAttributes		 waveattr;							// wave attributes
	//WaveGeneratorData	 wavegen;							// wave generator info
	WaveZone			 zone;								// wave zone (group of waves) info
	WaveSwell			 swell ALIGN128;					// wave swells water level modifier
	WaveAlpha			 alpha;								// wave transparency water modifier
	WaveRenderData		 wave_render_data;					// wave render info
	WaveSavedPoints		 savedpts;							// pre saved wave points
	// debug and test mode
	SurfTestInfo		 test;								// testing data
} SurfData;

/*
 * prototypes
 */

/* main.c */
extern void surfGameMainLoop( void );
extern Bool surfGameFreeRide( void );

/* init.c */
extern void surfInit( void );
extern void surfInitGame( int beachnum );
extern void surfInitPlayer( int player, int rider, int diff, int board, Bool robot );
extern Bool surfDisplayScreen( Text name, Bool fade_in, Bool interrupt );
extern GfxSpriteId surfLoadScreen( Text name );

/* control.c */
extern Bool surfInitControl( void );
extern void surfUpdateControl( void );
extern void surfLostControl( int pad_num, Bool frontend );
extern Text surfControlPadString( int pad_num );
extern int  surfPlayerControlPad( int player_num );

/* events.c */
extern void surfProcessEvents( void );
extern void surfPlayModeProcess( void ); 

/* object.c */
extern void surfDrawObjects( void );

/* update.c */
extern void surfUpdateGame( SurfPlayer *current );
extern void surfStartGame( SurfPlayer *current );

/* testmode.c */
extern void surfInitTestMode( void );
extern void surfTestModePrintData( void );
extern Text surfModeString( int mode );
extern void surfViewModeProcess( void );    
extern void surfTestModeDumpScreen( void );

/* ingame.c */
extern void surfInitInGame( SurfInGame *ingame );
extern void surfDrawInGame( SurfInGame *ingame );

/* beach.c */
extern void surfInitBeach( SurfBeach *beach, int num );
extern void surfDrawBeach( SurfBeach *beach );

/* demo.c */
extern void surfInitDemo( void );

/* testing.c */
extern void test_alpha( int id, Bool sprite, float r, float	g, float b, float x, float	y, float sx, float sy, float trans, float depth );
extern void test_alpha2( int id, Bool sprite, float r, float	g, float b, float x, float	y, float sx, float sy, float trans, float depth );
extern void test_strip( int id, Bool sprite, float r, float	g, float b, float x, float	y, float sx, float sy, float trans, float depth );
extern void test_strip2( int id, Bool sprite, float r, float	g, float b, float x, float	y, float sx, float sy, float trans, float depth );
extern void test_strip3( int id, Bool sprite, float r, float	g, float b, float x, float	y, float sx, float sy, float trans, float depth );
extern void debug_init( void );
extern void debug_file( Text name, int test );

#endif // SURF_SURFGAME_H

