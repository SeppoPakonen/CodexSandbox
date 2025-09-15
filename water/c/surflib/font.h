/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		font.h
 * Author:		Mark Theyer
 * Created:		13 Jul 1999
 **********************************************************
 * Description:	Header file for surf font related stuff.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 13-Jul-99	Theyer	Initial coding from new wave system
 * 27-Jun-01	Theyer	PS2 port.
 *
 **********************************************************/

#ifndef SURF_FONT_H
#define SURF_FONT_H

/* 
 * includes
 */

#include <type/datatype.h>
#include <gfx/gfx.h>

/* 
 * macros 
 */

#define FONT_MAX_LINES				  6			// max lines in a justified string
#define FONT_MAX_LINE_LEN			128			// max line string length

#define FONT_JUST_VERT_MASK			0x0F
#define FONT_JUST_VERT_CENTRE		0x01
#define FONT_JUST_VERT_BOTTOM		0x02
#define FONT_JUST_VERT_TOP			0x04

#define FONT_JUST_HOR_MASK			0xF0
#define FONT_JUST_HOR_RIGHT			0x10
#define FONT_JUST_HOR_CENTRE		0x20
#define FONT_JUST_HOR_LEFT			0x40

/*
 * typedefs
 */

typedef struct {
	GfxSpriteId			*chars;
	float				*xsize;
	int					 start_char;
	int					 end_char;
	float				 xspacing;
	float				 yspacing;
	float				 xpos;
	float				 ypos;
	float				 r,g,b;
	float				 depth;
	float				 xscale;
	float				 yscale;
	float				 xshift;	// (xspacing * xscale)
	float				 yshift;	// (yspacing * yscale)
} SurfFontData;

/*
 * prototypes
 */

/* font.c */
extern void  surfInitFont( SurfFontData *font, GfxSpriteId *char_buffer, float *xsize_buffer, int start_char, int end_char, float xspacing, float yspacing );
extern void  surfPrintFont( SurfFontData *font, Text str );
//extern void  surfPrintFontCentred( SurfFontData *font, float x, float y, Text str );
extern void  surfPrintFontJustified( SurfFontData *font, int just, float x, float y, Text str );
//extern float surfFontLength( SurfFontData *font, Text str );
extern void  surfResetFont( SurfFontData *font );
extern void  surfFontColor( SurfFontData *font, float r, float g, float b );
extern void  surfFontScale( SurfFontData *font, float x, float y );
extern void  surfFontPosition( SurfFontData *font, float x, float y );

#define surfFontDepth(font,zvalue)	(((SurfFontData *)font)->depth=zvalue)

extern void surfLoadNumFont( SurfFontData *font );
extern void surfLoadFont1( SurfFontData *font );
extern void surfLoadFont2( SurfFontData *font );

#endif	// SURF_FONT_H

