/**********************************************************
 * Copyright (C) 1998 Mark Theyer
 * All Rights Reserved
 **********************************************************
 * Project:	PSX game
 **********************************************************
 * File:	gfxpsx.c
 * Author:	Mark Theyer
 * Created:	01 May 1997
 **********************************************************
 * Description:	PSX Graphics Interface
 **********************************************************
 * Notes:		You cannot use 'psx' as a variable name.
 **********************************************************
 * Functions:
 *	gfxInit()
 *				Initialise the graphics system.
 *	gfxGetVideoMode()
 *				Get the current video mode.
 *	gfxSetResolution()
 *				Set the graphics resolution for display.
 *	gfxSplitScreen()
 *				Set split screen mode.
 *	gfxSelectSplitScreen()
 *				Select split screen buffer.
 *	gfxReset()
 *				Reset the graphics system.
 *	gfxCurrentBuffer()
 *				Get the current display buffer.
 *	gfxAdjustScreen()
 *				Adjust the screen position on the display.
 *	gfxSetBackgroundColor()
 *				Set the background fill color.
 *	gfxClose()
 *				Shutdown the graphics system.
 *	gfxAbort()
 *				Shutdown and halt due to a fatal problem.
 *	gfxUsageDetails()
 *				Return a text string with system usage info'.
 *	gfxSetDoubleSided()
 *				Set double sided flag for new surfaces.
 *	gfxAddPoints()
 *				Add points to the current graphics object.
 *	gfxAddNormals()
 *				Add point normals to the current graphics 
 *				object.
 *	gfxDepthStep()
 *				Set z depth step for subsequently drawn 
 *				objects.
 *	gfxSetColor()
 *				Set the current color for new surfaces.
 *	gfxSetTransparency()
 *				Set the transparency for new surfaces.
 *	gfxSetFog()
 *				Set the fog value for rendering.
 *	gfxResetTick()
 *				Reset the game timer tick.
 *	gfxTick()
 *				Get the game timer tick value.
 *	gfxFrameRate()
 *				Get the current frame display rate per second.
 *	gfxLightingOn()
 *				Turn lighting on or off for new surfaces.
 *	gfxSetFlatLight()
 *				Define a flat directional light.
 *	gfxSetAmbient()
 *				Set the ambient lighting color.
 *	gfxSin()
 *				Get the sine value for a degree angle.
 *	gfxCos()
 *				Get the cosine value for a degree angle.
 *	gfxRandom()
 *				Get a random value.
 *
 *  Private PSX Functions:
 *  ----------------------
 *
 *	psxUpdateTimer()
 *				Update the timer to identify frame rate.
 *
 **********************************************************
 * Revision History:
 * 01-May-97	Theyer	Initial Coding
 * 29-May-97	Theyer	Updated documentation
 * 17-Nov-99	Theyer	Updated documentation
 **********************************************************/

/*
 * includes
 */

#include <type/datatype.h>
#include <gfx/gfx.h>
#include <gfx/gfxpsx.h>
#include <triangle/triangle.h>
#include <text/text.h>
#include <parse/parse.h>
#include <memory/memory.h>

/*
 * macros
 */

/*
 * typedefs
 */

/*
 * prototypes
 */

private void         psxInitLighting( void );
private void	     psxUpdateTimer( void );

/*
 * variables
 */

PsxData ps;

#ifdef PSX_FULL_DEV_KIT
#ifdef PSX_USE_8MB_RAM
unsigned long _ramsize   = 0x00800000; //   8 megabytes
unsigned long _stacksize = 0x00020000; // 128 kilobytes
#else
unsigned long _ramsize   = 0x00200000; //   2 megabytes
unsigned long _stacksize = 0x00020000; // 128 kilobytes
#endif
#endif

/*
 * functions
 */

/**********************************************************
 * Function:	gfxInit
 **********************************************************
 * Description: Initialise the graphics system
 * Inputs:	args  - int arg and data value
 *			nargs - number of args passed
 *
 *			supported arg				arg data value
 *			-----------------------------------------------
 *			GFX_INITARG_SETVIDEOMODE	GFX_VIDEO_MODE_PAL
 *			GFX_INITARG_SETVIDEOMODE	GFX_VIDEO_MODE_NTSC
 *			-----------------------------------------------
 *
 * Notes:	Call padInit() first to initialise controllers
 *			before calling this function.
 * Returns:	TRUE
 **********************************************************/

public Bool gfxInit (
    GfxInitArgs	args[],
    int			nargs
    )
{
    int			 i, j;
	int			*iptr;
    Text		 tptr;
    u_long		*l;
	RECT		 rect;

	/* clear memory */
	//memClear( &ps, sizeof(PsxData) );

    /* init */
    ps.mode = -1;
    ps.xmax = 0;
    ps.ymax = 0;

	/* unlock screen (allows use of temporary buffer) */
	ps.screen_lock = FALSE;

#ifdef PSX_FULL_DEV_KIT	
	/* reset callbacks */
    ResetCallback();
    ResetGraph(0);
	/* set debug mode (0:off, 1:monitor, 2:dump) */
	SetGraphDebug(0);
#else
    /* full reset */
    ResetGraph(0);
#endif

	/* clear VRAM */
	setRECT( &rect, 0, 0, 1023, 511 );
	ClearImage( &rect, 0, 0, 0 );
	DrawSync(0);

    /* handle input args */
    for ( i=0; i<nargs; i++ ) {
    	if ( args[i].arg == GFX_INITARG_SETVIDEOMODE ) {
	    	/* set video mode */
			iptr = (int *)args[i].data;
			if ( *iptr == GFX_VIDEO_MODE_PAL ) {
				/* PAL */
	    		SetVideoMode( MODE_PAL );
				ps.mode = MODE_PAL;
				/* set default resolution */
				ps.xmax = 512;
	    		ps.ymax = 256;
				ps.interlace = FALSE;
			} else if ( *iptr == GFX_VIDEO_MODE_NTSC ) {
				/* NTSC */
          		SetVideoMode( MODE_NTSC );
				ps.mode = MODE_NTSC;
				/* set default resolution */
				ps.xmax = 512;
				ps.ymax = 240;
				ps.interlace = FALSE;
			}
		}
    }

    /* no mode no graphics! */
    if ( ps.mode == -1 ) {
        gfxError( GFX_ERROR_MODE_NOT_SET );
		exit(1);
    }

#if GFX_DEBUG
    printf( "gfx: mode set, x = %d, y = %d\n", ps.xmax, ps.ymax );
#endif
   	
    /* init GS library (and GPU) (Non-interlace, use GsGPU offset) */
    if ( ps.interlace )
    	GsInitGraph( ps.xmax, ps.ymax, GsOFSGPU|GsINTER,
			PSX_DITHER_ON, PSX_COLOR_16BIT );
    else
    	GsInitGraph( ps.xmax, ps.ymax, GsOFSGPU|GsNONINTER,
			PSX_DITHER_ON, PSX_COLOR_16BIT );

	/* define double buffer origins */
    if( ps.ymax < 480 )
    	GsDefDispBuff( 0, 0, 0, ps.ymax );
    else
		GsDefDispBuff( 0, 0, 0, 0 );
 
    /* init 3D part of PSX library */
    GsInit3D();
	GsSetLightMode( 0 );

#ifdef PSX_FULL_DEV_KIT
#ifdef PSX_USE_8MB_RAM
	SetMem(8);
#endif
#endif

    /* set up the ordering table handlers */
	ps.split_screen.on = FALSE;
    ps.screen.full.worldorder[0].length = PSX_OT_LENGTH;
    ps.screen.full.worldorder[1].length = PSX_OT_LENGTH;
    ps.screen.full.worldorder[0].org = ps.screen.full.zsorttable[0];
    ps.screen.full.worldorder[1].org = ps.screen.full.zsorttable[1];

    /* default background color is black */
    ps.bkcolor.red   = 0;
    ps.bkcolor.green = 0;
    ps.bkcolor.blue  = 0;
    ps.bkcolor.alpha = 0;

    /* init draw and display page */
    ps.drawpage = GsGetActiveBuff();
    if ( ps.drawpage == 0 )
		ps.displaypage = 1;
    else
		ps.displaypage = 0;

    /* clear order tables */  
    GsClearOt( 0, 0, &ps.screen.full.worldorder[0] );    
    GsClearOt( 0, 0, &ps.screen.full.worldorder[1] );

    /* init graphics state */
    ps.lightingoff   = FALSE;
    ps.texture_as_is = FALSE;
    ps.doublesided   = TRUE;
    ps.transparency  = FALSE;
	ps.transparency_mode = PSX_TRANSPARENCY_25;
    ps.gradation     = FALSE;
    ps.gouraud       = FALSE;
    ps.textureon     = FALSE;
    ps.textureid     = GFX_NO_TEXTURE;
    ps.id            = 0;
	ps.sid			 = 0;
	//ps.has_background = FALSE;	// not supported
	ps.ntexmaps		 = 0;
	ps.depth_step    = 0;
    
	/* render color is white */
    ps.color.red   = 255;
    ps.color.green = 255;
    ps.color.blue  = 255;

	/* init timer and frame rate data */
	ps.time        = 0;
	ps.fps         = 0;
	ps.nframes	   = 0;
	ps.ndraw	   = 0;
	ps.tick        = 0;

	ps.nextrahandlers = 0;

	/* clear texture entries */
	memClear( ps.textures, (sizeof(PsxTexture) * PSX_MAX_TEXTURES) );

    /* initialise static graphics data storage */
	psxClearModels();

	/* set basic ambient lighting */	
    GsSetAmbient( 2048, 2048, 2048 );

    /* timer callback */
    VSyncCallback( psxUpdateTimer );

	/* this call to swap buffers will avoid a hang (don't ask me why!) */
	gfxBufSwap();

	/* do another to blank screen */
	gfxBufSwap();

	/* reset all graphics */
	gfxReset( GFX_RESET_ALL );

	/* initialise file services */
	gfxFileInit();

	/* init display screen */
	GsDISPENV.screen.x = 0;
	/* PAL mode is supposed to be shifted down by 24 lines... see doc on GsInitGraph() */
	if ( ps.mode == MODE_PAL )
		GsDISPENV.screen.y = 24;
	else
		GsDISPENV.screen.y = 0;
	GsDISPENV.screen.w = 256;
	GsDISPENV.screen.h = 256;

	/* init file register */
	ps.nregfiles = 0;

    /* success */
    return( TRUE );
}


/**********************************************************
 * Function:	gfxError
 **********************************************************
 * Description: Print error code which occured
 * Inputs:		code - error code
 * Notes:		nil
 * Returns:		nil
 **********************************************************/

internal void gfxError (
	int		code
	)
{	
	printf( "GFX Error (%d)\n", code );
}


/**********************************************************
 * Function:	gfxGetVideoMode
 **********************************************************
 * Description: Get current video mode
 * Inputs:		nil
 * Notes:		nil
 * Returns:		GFX_VIDEO_MODE_PAL or GFX_VIDEO_MODE_NTSC
 **********************************************************/

public int gfxGetVideoMode (
	void
	)
{	
	if ( ps.mode == MODE_NTSC )
		return( GFX_VIDEO_MODE_NTSC );
	return( GFX_VIDEO_MODE_PAL );
}

	
/**********************************************************
 * Function:	gfxSetResolution
 **********************************************************
 * Description: Set graphics resolution for display
 * Inputs:		x - screen x resolution
 *				y - screen y resolution
 * Notes:		no validation...
 * Returns:		void
 **********************************************************/

public void gfxSetResolution (
	int		x,
	int		y
	)
{	
	Byte	r,g,b,a;

	/* update */
	ps.xmax = x;
	ps.ymax = y;

	/* wait for drawing to finish */
    DrawSync(0);
	VSync(0);

    /* graphics initialisation:Non-interlace, use GsGPU offset */	
    ResetGraph(3);
    //ResetGraph(0);
    if ( ps.interlace )
    	GsInitGraph( ps.xmax, ps.ymax, GsOFSGPU|GsINTER,
			PSX_DITHER_ON, PSX_COLOR_16BIT );
    else
    	GsInitGraph( ps.xmax, ps.ymax, GsOFSGPU|GsNONINTER,
			PSX_DITHER_ON, PSX_COLOR_16BIT );

    /* define double buffer origins */
    if( ps.ymax < 480 )
    	GsDefDispBuff( 0, 0, 0, ps.ymax );
    else
		GsDefDispBuff( 0, 0, 0, 0 );
 
    /* init draw and display page */
    ps.drawpage = GsGetActiveBuff();
    if ( ps.drawpage == 0 )
		ps.displaypage = 1;
    else
		ps.displaypage = 0;

	/* init 3D part of PSX library */
    //GsInit3D();
	GsSetLightMode( 0 );
}


/**********************************************************
 * Function:	gfxSplitScreen
 **********************************************************
 * Description: Set split screen mode
 * Inputs:		split - TRUE for split, FALSE for full screen
 *				mode - split screen style
 *					GFX_SPLIT_SCREEN_VERTICAL
 *					GFX_SPLIT_SCREEN_HORIZONTAL
 *					GFX_SPLIT_SCREEN_QUAD
 * Notes:		no validation...
 * Returns:		void
 **********************************************************/

public void gfxSplitScreen (
	Bool	split,
	int		mode
	)
{	
	if ( split ) {
		/* set up the ordering table handlers */
		ps.split_screen.on = TRUE;
		ps.screen.split.worldorder[0].length = (PSX_OT_LENGTH - 2);
		ps.screen.split.worldorder[1].length = (PSX_OT_LENGTH - 2);
		ps.screen.split.worldorder[2].length = (PSX_OT_LENGTH - 2);
		ps.screen.split.worldorder[3].length = (PSX_OT_LENGTH - 2);
		ps.screen.split.worldorder[4].length = (PSX_OT_LENGTH - 2);
		ps.screen.split.worldorder[5].length = (PSX_OT_LENGTH - 2);
		ps.screen.split.worldorder[0].org = ps.screen.split.zsorttable[0];
		ps.screen.split.worldorder[1].org = ps.screen.split.zsorttable[1];
		ps.screen.split.worldorder[2].org = ps.screen.split.zsorttable[2];
		ps.screen.split.worldorder[3].org = ps.screen.split.zsorttable[3];
		ps.screen.split.worldorder[4].org = ps.screen.split.zsorttable[4];
		ps.screen.split.worldorder[5].org = ps.screen.split.zsorttable[5];
		/* init clip rectangle */
		switch( mode ) {
		case GFX_SPLIT_SCREEN_VERTICAL:
			/* vertical split screen */
			ps.split_screen.clip[0].x = 0;
			ps.split_screen.clip[0].y = 0;
			ps.split_screen.clip[0].w = (ps.xmax>>1);
			ps.split_screen.clip[0].h = ps.ymax;
			ps.split_screen.clip[1].x = (ps.xmax>>1);
			ps.split_screen.clip[1].y = 0;
			ps.split_screen.clip[1].w = (ps.xmax>>1);
			ps.split_screen.clip[1].h = ps.ymax;
			ps.split_screen.clip[2].x = 0;
			ps.split_screen.clip[2].y = 0;
			ps.split_screen.clip[2].w = ps.xmax;
			ps.split_screen.clip[2].h = ps.ymax;
			break;
		case GFX_SPLIT_SCREEN_HORIZONTAL:
			ps.split_screen.clip[0].x = 0;
			ps.split_screen.clip[0].y = 0;
			ps.split_screen.clip[0].w = ps.xmax;
			ps.split_screen.clip[0].h = (ps.ymax>>1);
			ps.split_screen.clip[1].x = 0;
			ps.split_screen.clip[1].y = (ps.ymax>>1);
			ps.split_screen.clip[1].w = ps.xmax;
			ps.split_screen.clip[1].h = (ps.ymax>>1);
			ps.split_screen.clip[2].x = 0;
			ps.split_screen.clip[2].y = 0;
			ps.split_screen.clip[2].w = ps.xmax;
			ps.split_screen.clip[2].h = ps.ymax;
			break;
		case GFX_SPLIT_SCREEN_QUAD:
			/* not supported */
			gfxError( GFX_ERROR_QUAD_SPLIT_SCREEN_UNSUPPORTED );
			gfxAbort();
			break;
		}
		/* calculate offsets */
		ps.split_screen.offsetx[0] = (ps.split_screen.clip[0].x + (ps.split_screen.clip[0].w>>1));
		ps.split_screen.offsety[0] = (ps.split_screen.clip[0].y + (ps.split_screen.clip[0].h>>1));
		ps.split_screen.offsetx[1] = (ps.split_screen.clip[1].x + (ps.split_screen.clip[1].w>>1));
		ps.split_screen.offsety[1] = (ps.split_screen.clip[1].y + (ps.split_screen.clip[1].h>>1));
	    /* clear order tables */  
		DrawSync(0);
	    GsClearOt( 0, 0, &ps.screen.split.worldorder[0] );    
	    GsClearOt( 0, 0, &ps.screen.split.worldorder[1] );
	    GsClearOt( 0, 0, &ps.screen.split.worldorder[2] );
	    GsClearOt( 0, 0, &ps.screen.split.worldorder[3] );
	    GsClearOt( 0, 0, &ps.screen.split.worldorder[4] );
	    GsClearOt( 0, 0, &ps.screen.split.worldorder[5] );
	} else {
		/* set up the ordering table handlers */
		ps.split_screen.on = FALSE;
		ps.screen.full.worldorder[0].length = PSX_OT_LENGTH;
		ps.screen.full.worldorder[1].length = PSX_OT_LENGTH;
		ps.screen.full.worldorder[0].org = ps.screen.full.zsorttable[0];
		ps.screen.full.worldorder[1].org = ps.screen.full.zsorttable[1];
	    /* clear order tables */  
		DrawSync(0);
	    GsClearOt( 0, 0, &ps.screen.full.worldorder[0] );    
	    GsClearOt( 0, 0, &ps.screen.full.worldorder[1] );
	}
	/* set active buffer */
	ps.split_screen.active_buffer = 0;
}


/**********************************************************
 * Function:	gfxSelectSplitScreen
 **********************************************************
 * Description: Select active split screen buffer
 * Inputs:		screen - active screen (0 or 1) 
 * Notes:		no validation...
 * Returns:		void
 **********************************************************/

public void gfxSelectSplitScreen (
	int		screen
	)
{
	if ( screen == GFX_SPLIT_SCREEN_FULL )
		ps.split_screen.active_buffer = 4;
	else if ( screen == GFX_SPLIT_SCREEN_TWO )
		ps.split_screen.active_buffer = 2;
	else
		ps.split_screen.active_buffer = 0;
}


/**********************************************************
 * Function:	gfxCurrentSplitScreen
 **********************************************************
 * Description: Return current split screen buffer
 * Inputs:		nil
 * Notes:		No quad screen support
 * Returns:		0 or 1
 **********************************************************/

public int gfxCurrentSplitScreen (
	void
	)
{
	if ( ps.split_screen.on ) {
		if ( ps.split_screen.active_buffer )
			return(1);
	}
	return(0);
}


/**********************************************************
 * Function:	gfxReset
 **********************************************************
 * Description: Reset graphics
 * Inputs:		level - bit mask of values
 * Notes:	
 * Returns:		void
 **********************************************************/

public void gfxReset (
	int		level
	)
{
	/* reset textures */
	if ( bitOn( level, GFX_RESET_TEXTURES ) )
		ps.id  = 0;
	if ( bitOn( level, GFX_RESET_SPRITES ) )
		ps.sid = 0;
	if ( bitOn( level, GFX_RESET_MODELS ) ) 
		psxClearModels();
}


/**********************************************************
 * Function:	gfxCurrentBuffer
 **********************************************************
 * Description: Return the current display buffer
 * Inputs:		void
 * Notes:		Used to save screen...
 * Returns:		0 or 1
 **********************************************************/

public int gfxCurrentBuffer( 
	void 
	)
{
	return( ps.displaypage );
}


/**********************************************************
 * Function:	gfxAdjustScreen
 **********************************************************
 * Description: Adjust screen to fit 
 * Inputs:		x - x relative move
 *				y - y relative move
 *				w - width relative resize
 *				h - height relative resize
 * Notes:	
 * Returns:		void
 **********************************************************/

public void gfxAdjustScreen (
	int		x,
	int		y,
	int		w,
	int		h
	)
{
	/* update screen location */
	GsDISPENV.screen.x += x;
	GsDISPENV.screen.y += y;

	/* limit to +/- 30 units for adjustment */
	if ( GsDISPENV.screen.x >  30 ) GsDISPENV.screen.x =  30;
	if ( GsDISPENV.screen.x < -30 ) GsDISPENV.screen.x = -30;
	if ( ps.mode == MODE_PAL ) {
		/* PAL mode is supposed to be shifted down by 24 lines... see doc on GsInitGraph() */
		if ( GsDISPENV.screen.y >  54 ) GsDISPENV.screen.y =  54;
		if ( GsDISPENV.screen.y <  -6 ) GsDISPENV.screen.y =  -6;
	} else {
		if ( GsDISPENV.screen.y >  30 ) GsDISPENV.screen.y =  30;
		if ( GsDISPENV.screen.y < -30 ) GsDISPENV.screen.y = -30;
	}

#if 0
	/* disable width, height sizing as not necessary? */
	GsDISPENV.screen.w += w;
	GsDISPENV.screen.h += h;
	printf( "adjust screen rect x=%d, y=%d, w=%d, h=%d\n", GsDISPENV.screen.x, GsDISPENV.screen.y, 
		GsDISPENV.screen.w, GsDISPENV.screen.h );
#endif
}


/**********************************************************
 * Function:	gfxSetBackGroundColor
 **********************************************************
 * Description: Set the background color
 * Inputs:		red, green, blue - fixed color value range 
 *				(0->4096 == 0.0->1.0f == 0..255)
 * Notes:	
 * Returns:		void
 **********************************************************/

public void gfxSetBackgroundColor (
	fixed	red,
    fixed	green,
    fixed	blue
	)
{
    ps.bkcolor.red   = FIXED_TO_COLOR(red);
    ps.bkcolor.green = FIXED_TO_COLOR(green);
    ps.bkcolor.blue  = FIXED_TO_COLOR(blue);
    ps.bkcolor.alpha = 255;
}


/**********************************************************
 * Function:	gfxClose
 **********************************************************
 * Description: Close the graphics system
 * Inputs:		void
 * Notes:		PSX shutdown
 * Returns:		void
 **********************************************************/

public void gfxClose (
    void
    )
{
#if 0	// not supported
	/* free memory we allocated */
	if ( ps.has_background ) {
		free( ps.bkmap.cells );
		free( ps.bkmap.index );
		free( ps.bkwork );
	}
#endif
	/* remove vsync callback */
    VSyncCallback(0);

	/* shutdown file services */
	gfxFileClose();

#ifdef PSX_FULL_DEV_KIT
	StopCallback();
#ifdef USE_LIBPAD_API
	PadStopCom();
#else
	StopPAD();
#endif
#endif
    ResetGraph(3);
}


/**********************************************************
 * Function:	gfxAbort
 **********************************************************
 * Description: Close the graphics system and exit
 * Inputs:		void
 * Notes:		Full abnormal exit
 * Returns:		void
 **********************************************************/

public void gfxAbort (
    void
    )
{
	gfxClose();
	exit(1);
}


/**********************************************************
 * Function:	gfxUsageDetails
 **********************************************************
 * Description: Report statistics on the graphics system
 * Inputs:		void
 * Notes:		
 * Returns:		void
 **********************************************************/

public Text gfxUsageDetails (
    void
    )
{
#if GFX_VALIDATE
	int				len;
	static char		buff[512];
	
#ifndef PSX_FULL_DEV_KIT
    /* report stats - static objects */
    sprintf( buff, "static data\nobjects: %d of %d\n", ps.tmd.nobjects, PSX_MAX_OBJECT );
	len = txtLength(buff);
    sprintf( &buff[len], "primitive buffer: used %d of %d\n", (ps.tmd.primitive_size<<2), PSX_PRIMITIVE_BUFFER_SIZE );
	len = txtLength(buff);
    sprintf( &buff[len], "vertex count: %d of %d, normal count: %d of %d\n", ps.tmd.n_vert, PSX_MAX_VERTEX, ps.tmd.n_normal, PSX_MAX_NORMAL );
#endif

	/* buffer sizes */
	sprintf( &buff[len], "textures: %d of %d\n", ps.id, PSX_MAX_TEXTURES );
	len = txtLength(buff);
	sprintf( &buff[len], "sprites: %d of %d\n", ps.sid, PSX_MAX_SPRITES );
	len = txtLength(buff);
	sprintf( &buff[len], "ifiles: %d of %d, file buffer size=%d\n", ps.nregfiles, PSX_MAX_INTERNAL_FILE_REGISTER, PSX_FILEBUFFER_SIZE );
	len = txtLength(buff);
	sprintf( &buff[len], "extra handlers: %d of %d\n", ps.nextrahandlers, PSX_MAX_EXTRAHANDLERS );
	return( buff );
#else
	return( NULL );
#endif
}


/**********************************************************
 * Function:	gfxSetDoubleSided
 **********************************************************
 * Description: Create double sided polygons
 * Inputs:		set - set or unset double sided option
 * Notes:	
 * Returns:		void
 **********************************************************/

public void gfxSetDoubleSided (
    Bool	set
    )
{
    ps.doublesided = set;
}


/**********************************************************
 * Function:	gfxAddPoints
 **********************************************************
 * Description: Add point data to an object
 * Inputs:		object - object to add points to
 *				pts - points
 *				npts - number of points passed in pts
 * Notes:	
 * Returns:		void
 **********************************************************/

public void gfxAddPoints (
    GfxObject	*object,
    Point_i3d	 pts[],
	int			 npts
    )
{
  	/* add vertex data */
  	psxAddVertex( object, pts, npts, NULL );
}


/**********************************************************
 * Function:	gfxAddNormals
 **********************************************************
 * Description: Add normal data to an object
 * Inputs:		object - object to add points to
 *				normals - normals 
 *				nnormals - number of normals to add
 * Notes:	
 * Returns:		index number allocated to normal
 **********************************************************/

public int gfxAddNormals (
    GfxObject	*object,
    Point_fx3d	 normals[],
	int			 nnormals
    )
{
	u_short			 n;

  	/* add normal data */
  	psxAddNormal( object, normals, 1, &n );

	if ( nnormals > 1 ) 
	  	psxAddNormal( object, &normals[1], (nnormals - 1), NULL );

	return( (int)n );
}


/**********************************************************
 * Function:	gfxDepthStep
 **********************************************************
 * Description: Set the depth step for z sorting range
 * Inputs:		step - step to use
 * Notes:		PlayStation 1 for Z sort functionality
 * Returns:		void
 **********************************************************/

public void gfxDepthStep (
    int		step
    )
{
#if 1
	ps.depth_step = step;
#else
	if ( step == 1 )
		ps.depth_step = 1;
	else
		ps.depth_step = 0;
#endif
}


/**********************************************************
 * Function:	gfxSetColor
 **********************************************************
 * Description: Set the current render color
 * Inputs:		red, green, blue - fixed color value in
 *				range 0-4096 (== 0->255 == 0.0->1.0)
 * Notes:	
 * Returns:		void
 **********************************************************/

public void gfxSetColor (
    fixed	red,
    fixed	green,
    fixed	blue
    )
{
    ps.color.red   = FIXED_TO_COLOR(red);
    ps.color.green = FIXED_TO_COLOR(green);
    ps.color.blue  = FIXED_TO_COLOR(blue);
	ps.color.alpha = 255;
}


/**********************************************************
 * Function:	gfxSetTransparency
 **********************************************************
 * Description: Set the current transparency factor
 * Inputs:		trans - transparency factor (fixed value)
 *					    range = 0 (none) -> 4096 (full)
 * Notes:	
 * Returns:		void
 **********************************************************/

public void gfxSetTransparency (
    fixed	trans
    )
{
#ifdef PSX_USE_HMD_FORMAT
	ps.transparency = FALSE;
	return;
#else
	if ( trans <= FIXED_ZERO || trans > FIXED_ONE ) {
		ps.transparency = FALSE;
		return;
	} else if ( trans < FIXED_NORMALISE(4,10) )
		ps.transparency_mode = PSX_TRANSPARENCY_25;
	else if ( trans <= FIXED_NORMALISE(65,100) )
		ps.transparency_mode = PSX_TRANSPARENCY_50;
	else if ( trans <= FIXED_NORMALISE(8,10) )
		ps.transparency_mode = PSX_TRANSPARENCY_75;
	else
		ps.transparency_mode = PSX_TRANSPARENCY_90;
	ps.transparency = TRUE;
#endif
}


/**********************************************************
 * Function:	gfxSetFog
 **********************************************************
 * Description: Set a fog visual
 * Inputs:		factor - fog factor (0->4096)
 *				r,g,b - fixed format color values
 * Notes:	
 * Returns:	    void
 **********************************************************/

public void gfxSetFog (
	fixed		factor,
	fixed		r,
	fixed		g,
	fixed		b
	)
{
	GsFOGPARAM	fog;

	if ( factor <= FIXED_ZERO || factor > FIXED_ONE ) {	
		GsSetLightMode( 0 );
	} else {
		fog.dqa = -3000;//-() * 4096/64/ps.dist;
		fog.dqb = 20971520;//1.25 * 4096 * 4096;
		fog.rfc = (Byte)FIXED_TO_INT((r * 255));
		fog.gfc = (Byte)FIXED_TO_INT((g * 255));
		fog.bfc = (Byte)FIXED_TO_INT((b * 255));
		GsSetLightMode( 1 );
		GsSetFogParam( &fog );
	}
}


/**********************************************************
 * Function:	gfxResetTick
 **********************************************************
 * Description: Reset ticker
 * Inputs:		void
 * Notes:	
 * Returns:		void
 **********************************************************/

public void gfxResetTick (
    void
    )
{
	ps.tick = VSync(-1) - ps.time;
	ps.time += ps.tick;
	VSync(0);
}


/**********************************************************
 * Function:	gfxTick
 **********************************************************
 * Description: Return time tick since last call
 * Inputs:		void
 * Notes:	
 * Returns:		void
 **********************************************************/

public fixed gfxTick (
    void
    )
{
    switch( ps.mode ) {
    case MODE_PAL:
		return( (ps.tick * 4915) );	// 50 fps
	case MODE_NTSC:
		return( (ps.tick<<12) );	// 60 fps
	}
	return( 0 );
}


/**********************************************************
 * Function:	gfxFrameRate
 **********************************************************
 * Description: Get the number of frames we are rendering
 *				each second.
 * Inputs:		void
 * Notes:	
 * Returns:		frames per second
 **********************************************************/

public int gfxFrameRate (
	void
	)
{
	return( ps.fps );
}


/**********************************************************
 * Function:	gfxLightingOn
 **********************************************************
 * Description: Set lighting as on or off
 * Inputs:	on - on or off
 * Notes:	
 * Returns:	
 **********************************************************/

public void gfxLightingOn (
	Bool	on
	)
{
	ps.lightingoff = ! on;
}


/**********************************************************
 * Function:	gfxSetFlatLight
 **********************************************************
 * Description: Set a flat light source
 * Inputs:		light - the light to set
 *              x - normalised direction in x of light
 *              y - normalised direction in y of light
 *              z - normalised direction in z of light
 *				red - red color component
 *				green - green color component
 *				blue - blue color component
 * Notes:	
 * Returns:		void
 **********************************************************/

public void gfxSetFlatLight (
    int		light,
	fixed	x,
	fixed	y,
	fixed	z,
	fixed	red,
	fixed	green,
	fixed	blue
    )
{ 
    GsF_LIGHT	psxlight;	

#if GFX_VALIDATE 
	if ( light < 0 || light > 2 ) {
		gfxError( GFX_ERROR_BAD_LIGHT_INDEX );
		gfxAbort();
	}
#endif

	/* init light (no validation!) */
	psxlight.vx = x;
	psxlight.vy = -(z);
	psxlight.vz = y;
	psxlight.r  = (Byte)FIXED_TO_INT((red   * 255));
	psxlight.g  = (Byte)FIXED_TO_INT((green * 255));
	psxlight.b  = (Byte)FIXED_TO_INT((blue  * 255));

    GsSetFlatLight( light, &psxlight );
}

 
/**********************************************************
 * Function:	gfxSetAmbient
 **********************************************************
 * Description: Set the ambient lighting
 * Inputs:		red - red color component
 *				green - green color component
 *				blue - blue color component
 * Notes:	
 * Returns:	
 **********************************************************/

public void gfxSetAmbient (
	fixed	red,
	fixed	green,
	fixed	blue
    )
{
	u_short		r,g,b;

	r = (u_short)red;
	g = (u_short)green;
	b = (u_short)blue;
    GsSetAmbient( r, g, b );
}

 
/**********************************************************
 * Function:	gfxSin
 **********************************************************
 * Description: Return the sine value for an angle
 * Inputs:		angle (degrees)
 * Notes:	
 * Returns:	
 **********************************************************/

public fixed gfxSin (
	int		angle
	)
{
#if GFX_VALIDATE
	if ( angle == 360 ) angle = 0;
	if ( angle < 0 || angle > 359 ) {
		gfxError( GFX_ERROR_BAD_SIN_ANGLE );
		gfxAbort();
	}
#endif
	return( triSin(angle) );
}

 
/**********************************************************
 * Function:	gfxCos
 **********************************************************
 * Description: Return the cosine value for an angle
 * Inputs:		angle (degrees)
 * Notes:	
 * Returns:	
 **********************************************************/

public fixed gfxCos (
	int		angle
	)
{
#if GFX_VALIDATE
	if ( angle == 360 ) angle = 0;
	if ( angle < 0 || angle > 359 ) {
		gfxError( GFX_ERROR_BAD_COS_ANGLE );
		gfxAbort();
	}
#endif
	return( triCos(angle) );
}


/**********************************************************
 * Function:	gfxRandom
 **********************************************************
 * Description: Return a random number between the minumum
 *				and maximum values provided
 * Inputs:		min - minimum number
 *				max - maximum number
 * Notes:	
 * Returns:	
 **********************************************************/

public fixed gfxRandom (
	fixed	min,
	fixed	max
	)
{
	long	r;
	fixed	norm;
	int		range;
	fixed	adjust;
	fixed	result;

#if GFX_VALIDATE
	if ( min > max ) {
		gfxError( GFX_ERROR_BAD_RANDOM_VALUES );
		gfxAbort();
	}
#endif

	/* get random number */
	r = rand();
	/* normalize to 0.0 - 1.0 range */
	norm   = FIXED_NORMALISE(r,RAND_MAX);
	range  = FIXED_TO_INT(max - min);
	adjust = range * norm;
	result = min + adjust;

	return( result );
}


/**********************************************************
 * Function:	gfxApplyQuaternion
 **********************************************************
 * Description: Apply a quaternion rotation to an object
 * Inputs:		obj - gfx object
 *				qat - quaternion pointer				
 * Notes:		
 * Returns:		
 **********************************************************/

public void gfxApplyQuaternion (
	GfxObject	*object,
	fixed		 x,
	fixed		 y,
	fixed		 z,
	fixed		 w
	) 
{
	MATRIX		m;
#if 0
	float		fx,fy,fz,fw;
	float		xx,xy,xz,xw;
	float		yy,yz,yw;
	float		zz,zw;
	float		ww;

	/* init */
	fx = FIXED_TO_FLOAT(x);
	fy = FIXED_TO_FLOAT(y);
	fz = FIXED_TO_FLOAT(z);
	fw = FIXED_TO_FLOAT(w);

	xx = fx * fx,   xy = fx * fy,   xz = fx * fz,   xw = fx * fw;
    yy = fy * fy,   yz = fy * fz,   yw = fy * fw;
    zz = fz * fz,   zw = fz * fw;
    ww = fw * fw;
 
    m.m[0][0] = FLOAT_TO_FIXED((xx - yy - zz + ww));
    m.m[0][1] = FLOAT_TO_FIXED((2.0f * (xy - zw)));
    m.m[0][2] = FLOAT_TO_FIXED((2.0f * (xz + yw)));
 
    m.m[1][0] = FLOAT_TO_FIXED((2.0f * (xy + zw)));
    m.m[1][1] = FLOAT_TO_FIXED((-xx + yy - zz + ww));
    m.m[1][2] = FLOAT_TO_FIXED((2.0f * (yz - xw)));
 
    m.m[2][0] = FLOAT_TO_FIXED((2.0f * (xz - yw)));
    m.m[2][1] = FLOAT_TO_FIXED((2.0f * (yz + xw)));
    m.m[2][2] = FLOAT_TO_FIXED((-xx - yy + zz + ww));
#else
	fixed		xx,xy,xz,xw;
	fixed		yy,yz,yw;
	fixed		zz,zw;
	fixed		ww;

	/* init */
	xx = FIXED_MULTIPLY( x, x );
	xy = FIXED_MULTIPLY( x, y );
	xz = FIXED_MULTIPLY( x, z );
	xw = FIXED_MULTIPLY( x, w );
	yy = FIXED_MULTIPLY( y, y );
	yz = FIXED_MULTIPLY( y, z );
	yw = FIXED_MULTIPLY( y, w );
	zz = FIXED_MULTIPLY( z, z );
	zw = FIXED_MULTIPLY( z, w );
	ww = FIXED_MULTIPLY( w, w );

	/* create matrix */
	m.m[0][0] = xx - yy - zz + ww;
    m.m[0][1] = FIXED_MULTIPLY( 8192, (xy - zw) );
    m.m[0][2] = FIXED_MULTIPLY( 8192, (xz + yw) );
    m.m[1][0] = FIXED_MULTIPLY( 8192, (xy + zw) );
    m.m[1][1] = -xx + yy - zz + ww;
    m.m[1][2] = FIXED_MULTIPLY( 8192, (yz - xw) );
    m.m[2][0] = FIXED_MULTIPLY( 8192, (xz - yw) );
    m.m[2][1] = FIXED_MULTIPLY( 8192, (yz + xw) );
    m.m[2][2] = -xx - yy + zz + ww;
#endif
	m.t[0] = m.t[1] = m.t[2] = 0;

	//printf( "q=%d,%d,%d,%d\n", x, y, z, w );
	//printf( "qmat x=%d,%d,%d\n", m.m[0][0], m.m[0][1], m.m[0][2] );
	//printf( "qmat y=%d,%d,%d\n", m.m[1][0], m.m[1][1], m.m[1][2] );
	//printf( "qmat z=%d,%d,%d\n", m.m[2][0], m.m[2][1], m.m[2][2] );
	//printf( "qmat t=%d,%d,%d\n", m.t[0], m.t[1], m.t[2] );

	psxApplyMatrix( object, &m );
}


/**********************************************************
 * Function:	gfxMemoryBuffer
 **********************************************************
 * Description: Get a memory buffer to use
 * Inputs:	 
 * Notes:	Test
 * Returns:	
 **********************************************************/

public void *gfxMemoryBuffer (
    int		 type,
	int		*size
    )
{
	void	*ptr;

	/* init */
	ptr = NULL;

	switch( type ) {
	case GFX_PSX_OT_MEMORY:
		/* use ot memory buffer (note not to use first 80 bytes (GsOT area)) */
		ptr = (void *)ps.screen.split.zsorttable;
		*size = sizeof(ps.screen.split.zsorttable);
		break;
	case GFX_PSX_FILE_MEMORY:
		/* use file buffer */
		ptr = (void *)ps.filebuffer;
		*size = sizeof(ps.filebuffer);
		break;
	case GFX_PSX_GPU_MEMORY:
		/* use file buffer */
		ptr = (void *)ps.gpupacket;
		*size = sizeof(ps.gpupacket);
		break;
	case GFX_PSX_FILE_AND_GPU_MEMORY:
		/* use file buffer */
		ptr = (void *)ps.filebuffer;
		*size = (sizeof(ps.filebuffer) + sizeof(ps.gpupacket));
		break;
	}

	return( ptr );
}


/****************************
 *
 * Private PSX functions... 
 *
 ****************************/


/**********************************************************
 * Function:	psxUpdateTimer
 **********************************************************
 * Description: Update the timer via VSyncCallback
 * Inputs:	 
 * Notes:	Test
 * Returns:	
 **********************************************************/

private void psxUpdateTimer (
    void
    )
{
	// update vsync flag
	ps.vsync++;
    switch( ps.mode ) {
    case MODE_PAL:
		//ps.time += 4096;
		ps.nframes++;
		if ( ps.nframes == 60 ) {
			ps.fps     = ps.ndraw;
			ps.nframes = 0;
			ps.ndraw   = 0;
		}
        break;
    case MODE_NTSC:
		//ps.time += 3413;
		ps.nframes++;
		if ( ps.nframes == 50 ) {
			ps.fps     = ps.ndraw;
			ps.nframes = 0;
			ps.ndraw   = 0;
		}
        break;
    }
}
