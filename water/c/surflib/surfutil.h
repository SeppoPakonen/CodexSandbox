/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		surfutil.h
 * Author:		Mark Theyer
 * Created:		15 Jul 1999
 **********************************************************
 * Description:	Miscellaneous utility functions.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game.
 * 31-May-01	Theyer	Initial coding from existing game.
 *
 **********************************************************/

#ifndef SURF_SURFUTIL_H
#define SURF_SURFUTIL_H

/* 
 * includes
 */

#include <stdio.h>

#include <compile.h>
#include <type/datatype.h>
#include <text/text.h>
#include <gfx/gfx.h>
#include <pad/pad.h>
#include <binary/binary.h>

/* 
 * macros 
 */


/*
 * typedefs
 */


/*
 * prototypes
 */

extern void surfExit( int exitcode );
extern void adjustValue( int *current, int *wanted, int max );
extern void adjustToMax( int *diff, int max );
extern Bool memEqual( int *ptr1, int *ptr2, int size );
extern void surfKeepInRange( float *value, int range );
extern void noprint( char *format, ... );
extern void surfLoadMap( Text mapname, Bool clear );
extern Bool surfWait( int nsecs, Bool interrupt );
extern void surfSetResolution( Bool high );
extern void surfBlankScreen( void );
extern void surfHeightToUnit( int height, char *buff, Bool metres );
extern void surfFloatString( fixed value, char *buff );
extern void surfLoadPos( GfxObject *obj, Text posname );

#endif	// SURF_SURFUTIL_H

