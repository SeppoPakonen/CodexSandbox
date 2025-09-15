/**********************************************************
 * Copyright (C) 2000 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:	PSX game
 **********************************************************
 * File:	movie.h
 * Author:	Mark Theyer
 * Created:	20 March 2000
 **********************************************************
 * Description:	FMV Movie player library
 **********************************************************
 * Revision History:
 * 20-Mar-00	Theyer	Initial Coding
 **********************************************************/

#ifndef THEYER_MOVIE_H
#define THEYER_MOVIE_H

/*
 * include
 */

#include <type/datatype.h>

#ifdef OsTypeIsWin95
#include <gui/gui.h>
#endif

/*
 * macros
 */

/*
 * typedefs
 */

typedef Bool *(FmvCallback)( void );

/*
 * prototypes
 */

extern void fmvPlay( Text filename, Bool pal, int nframes, FmvCallback callback );

#endif

