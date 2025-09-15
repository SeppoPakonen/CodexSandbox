/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		font.c
 * Author:		Mark Theyer
 * Created:		12 Oct 1998
 **********************************************************
 * Description:	Font functions
 **********************************************************
 * Functions:
 *	surfInitFont() 
 *				Initialise a font data structure.
 *	surfLoadDebugFont()
 *				Load the debug font set.
 *	surfLoadFontD()
 *				Load game font type D.
 *	surfLoadFontBR()
 *				Load game font type BR (big and red).
 *	surfPrintFont()
 *				Print a font string.
 *	surfResetFont()
 *				Reset font.
 *	surfFontColor()
 *				Set color for font.
 *	surfFontPosition()
 *				Set font position.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 12-Oct-98	Theyer	Initial coding.
 * 03-Aug-99	Theyer	Initial update from existing game.
 * 27-Jun-01	Theyer	PS2 port.
 *
 **********************************************************/

/*
 * includes
 */

#include <gfx/gfx.h>
#include <ps2/ps2image.h>
#include <memory/memory.h>

#include <font.h>
#include <surfutil.h>
#include <waverr.h>

/*
 * typedefs
 */

/*
 * macros
 */

#define SURF_FONT_XMARGIN	0.02f
#define SURF_FONT_YMARGIN	0.02f

#define FONT_ACCENT_CHAR		'\133'			// accent char upper dots
#define FONT_1_OVER_13			0.0769230769f

/*
 * prototypes
 */

/*
 * global variables
 */

static Byte	num1_width[10] = {
		 16, // 0 start char, ASCII=48,0x30
		 16, // 1
		 16, // 2
		 16, // 3
		 16, // 4
		 16, // 5
		 16, // 6
		 16, // 7
		 16, // 8
		 16  // 9 end char, ASCII=57,0x39
	};

static Byte	font1_width[158] = {
		 // pixel scale, pixel size
		 // (width: unused=0, standard=16, (width of 16 = <xspacing> or 1.0))
		 16, 16, // , start char, ASCII code = 44, 0x2C
		 16, 16, // -
		 16, 16, // .
		  0,  0, // / unused
		 16, 16, // 0
		 16, 16, // 1
		 16, 16, // 2
		 16, 16, // 3
		 16, 16, // 4
		 16, 16, // 5=9
		 16, 16, // 6
		 16, 16, // 7
		 16, 16, // 8
		 16, 16, // 9
		  0,  0, // : unused
		  0,  0, // ; unused
		  0,  0, // < unused
		  0,  0, // = unused
		  0,  0, // > unused
		 16, 16, // ? 
		  0,  0, // unused
		 20, 24, // A
		 17, 16, // B=22
		 18, 16, // C
		 17, 16, // D
		 16, 16, // E
		 17, 16, // F
		 19, 24, // G
		 16, 16, // H
		 13, 16, // I=29
		 14, 16, // J
		 17, 16, // K
		 15, 16, // L
		 20, 24, // M
		 16, 16, // N
		 19, 24, // O
		 16, 16, // P=36
		 19, 24, // Q
		 17, 16, // R
		 17, 16, // S
		 16, 16, // T
		 16, 16, // U
		 20, 24, // V
		 26, 24, // W=43
		 16, 16, // X
		 18, 16, // Y
		 17, 16, // Z
		  0,  0, // [ unused
		  0,  0, // \ unused
		  0,  0, // ] unused
		  0,  0, // ^ unused
		  0,  0, // _ unused
		  0,  0, // ` unused
		 15, 16, // a
		 15, 16, // b
		 14, 16, // c
		 15, 16, // d=56
		 14, 16, // e
		 11, 16, // f
		 15, 16, // g
		 14, 16, // h
		  8,  8, // i
		  9,  8, // j
		 13, 16, // k
		  8,  8, // l
		 20, 24, // m=65
		 14, 16, // n
		 16, 16, // o
		 15, 16, // p
		 15, 16, // q
		 10,  8, // r
		 14, 16, // s
		 10,  8, // t
		 14, 16, // u=73
		 14, 16, // v
		 22, 24, // w
		 14, 16, // x
		 16, 16, // y
		 13, 16, // z
	};

#if 0
static Byte	font1_width[90] = {
	//  width, (zero width=unused, height=16, (width of 13 = xspacing or 1.0))
		 16, // " start char, ASCII code = 34, 0x22
		 16, // #
		 16, // $
		 24, // %
		 24, // &
		 16, // \ 
		 16, // (
		 16, // )
		 16, // *
		 16, // +
		 16, // ,
		 16, // -
		 16, // .
		 16, // /
		 16, // 0
		 16, // 1
		 16, // 2
		 16, // 3
		 16, // 4
		 16, // 5
		 16, // 6
		 16, // 7
		 16, // 8
		 16, // 9
		 16, // :
		  0, // ; unused
		  0, // < unused
		  0, // = unused
		  0, // > unused
		 13, // ? 
		  0, // unused
		 13, // A
		 13, // B
		 13, // C
		 13, // D
		 13, // E
		 13, // F
		 13, // G
		 13, // H
		 10, // I
		 13, // J
		 13, // K
		 13, // L
		 20, // M
		 13, // N
		 13, // O
		 13, // P
		 13, // Q
		 13, // R
		 13, // S
		 13, // T
		 13, // U
		 13, // V
		 20, // W
		 13, // X
		 13, // Y
		 13, // Z
	};
#endif

#if 1
static Byte	font2_width[158] = {
		 // pixel scale, pixel size
		 // (width: unused=0, standard=16, (width of 16 = <xspacing> or 1.0))
		  4,  8, // , start char, ASCII code = 44, 0x2C
		  4,  8, // -
		  4,  8, // .
		  0,  0, // / unused
		  8,  8, // 0
		  6,  8, // 1
		  8,  8, // 2
		  8,  8, // 3
		  8,  8, // 4
		  8,  8, // 5
		  8,  8, // 6
		  8,  8, // 7
		  8,  8, // 8
		  8,  8, // 9
		  0,  0, // : unused
		  0,  0, // ; unused
		  0,  0, // < unused
		  0,  0, // = unused
		  0,  0, // > unused
		  8,  8, // ? 
		  0,  0, // unused
		  9, 16, // A
		  8,  8, // B=22
		  8,  8, // C
		  8,  8, // D
		  7,  8, // E
		  7,  8, // F
		  8,  8, // G
		  8,  8, // H
		  6,  8, // I   
		  8,  8, // J
		  8,  8, // K
		  8,  8, // L
		  9, 16, // M
		  8,  8, // N
		  8,  8, // O
		  8,  8, // P
		  8,  8, // Q=37
		  8,  8, // R
		  7,  8, // S
		  8,  8, // T
		  8,  8, // U
		  9, 16, // V
		 12, 16, // W
		  8,  8, // X
		  8,  8, // Y
		  8,  8, // Z
		  0,  0, // [ unused
		  0,  0, // \ unused
		  0,  0, // ] unused
		  0,  0, // ^ unused
		  0,  0, // _ unused
		  0,  0, // ` unused
		  8,  8, // a
		  8,  8, // b
		  8,  8, // c
		  8,  8, // d
		  8,  8, // e=57
		  6,  8, // f
		  8,  8, // g
		  8,  8, // h
		  4,  8, // i
		  6,  8, // j
		  8,  8, // k
		  5,  8, // l
		  9, 16, // m
		  8,  8, // n
		  8,  8, // o
		  8,  8, // p
		  8,  8, // q
		  7,  8, // r
		  8,  8, // s
		  6,  8, // t=72
		  8,  8, // u
		  8,  8, // v
		 10, 16, // w
		  8,  8, // x
		  8,  8, // y
		  7,  8, // z
	};
#else
static Byte	font2_width[158] = {
		 // pixel scale, pixel size
		 // (width: unused=0, standard=16, (width of 16 = <xspacing> or 1.0))
		  2,  8, // , start char, ASCII code = 44, 0x2C
		  3,  8, // -
		  2,  8, // .
		  0,  0, // / unused
		  6,  8, // 0
		  4,  8, // 1
		  6,  8, // 2
		  6,  8, // 3
		  6,  8, // 4
		  6,  8, // 5
		  6,  8, // 6
		  6,  8, // 7
		  6,  8, // 8
		  6,  8, // 9
		  0,  0, // : unused
		  0,  0, // ; unused
		  0,  0, // < unused
		  0,  0, // = unused
		  0,  0, // > unused
		  7,  8, // ? 
		  0,  0, // unused
		  9, 16, // A
		  7,  8, // B=22
		  8,  8, // C
		  7,  8, // D
		  6,  8, // E
		  6,  8, // F
		  8,  8, // G
		  7,  8, // H
		  2,  8, // I   
		  6,  8, // J
		  7,  8, // K
		  6,  8, // L
		  9, 16, // M
		  7,  8, // N
		  8,  8, // O
		  7,  8, // P
		  8,  8, // Q=37
		  8,  8, // R
		  7,  8, // S
		  8,  8, // T
		  7,  8, // U
		  9, 16, // V
		 13, 16, // W
		  7,  8, // X
		  8,  8, // Y
		  7,  8, // Z
		  0,  0, // [ unused
		  0,  0, // \ unused
		  0,  0, // ] unused
		  0,  0, // ^ unused
		  0,  0, // _ unused
		  0,  0, // ` unused
		  6,  8, // a
		  6,  8, // b
		  6,  8, // c
		  6,  8, // d
		  6,  8, // e=57
		  5,  8, // f
		  6,  8, // g
		  6,  8, // h
		  2,  8, // i
		  4,  8, // j
		  6,  8, // k
		  2,  8, // l
		 10, 16, // m
		  6,  8, // n
		  6,  8, // o
		  6,  8, // p
		  6,  8, // q
		  5,  8, // r
		  6,  8, // s
		  4,  8, // t=72
		  6,  8, // u
		  7,  8, // v
		 11, 16, // w
		  6,  8, // x
		  7,  8, // y
		  5,  8, // z
	};
#endif

/*
 * functions
 */
          

/**********************************************************
 * Function:	surfInitFont
 **********************************************************
 * Description: Initialise fonts
 * Inputs:		font         - uninitialised font structure
 *				char_buffer  - character buffer to use
 *				xsize_buffer - buffer to save x size values
 *				start_char   - starting ASCII character
 *				end_char     - end ASCII character
 *				xspacing	 - x spacing value
 *				yspacing	 - y spacing value
 * Notes:		nil
 * Returns:		initialised font structure
 **********************************************************/

void surfInitFont (
	SurfFontData *font,
	GfxSpriteId	 *char_buffer,
	float		 *xsize_buffer,
	int			  start_char,
	int			  end_char,
    float		  xspacing,
	float		  yspacing
    )
{
	int		i;
	int		end;

	/* no validation! */
	font->chars      = char_buffer;
	font->xsize		 = xsize_buffer;
	font->start_char = start_char;
	font->end_char   = end_char;
	font->xspacing   = xspacing;
	font->yspacing   = yspacing;
	font->depth      = 0.0f;
	font->xscale	 = 1.0f;
	font->yscale	 = 1.0f;
	font->xshift     = xspacing;
	font->yshift     = yspacing;
	/* init font chars */
	end = end_char - start_char;
	for ( i=0; i<end; i++ ) {
		font->chars[i] = GFX_NO_TEXTURE;
		font->xsize[i] = font->xspacing;
	}
	surfResetFont( font );
}


/**********************************************************
 * Function:	surfLoadNumFont
 **********************************************************
 * Description: Load game font numbers
 * Inputs:		font - initialised font struct' for this set
 * Notes:		nil
 * Returns:		updated font structure with loaded characters
 **********************************************************/

void surfLoadNumFont (
	SurfFontData *font
    )
{
	GfxTextureId	fontid;
	int				i, x, y;

	/* load font image */
	fontid = p2iTextureToGfxTexture( "num1.p2i", FALSE );
	x = 0;
	y = 0;

	if ( fontid == GFX_NO_TEXTURE )
		surfExit( FONT_ERROR_NUM1_LOAD_FAILED );
	
	/* create sprites for each number */
	for ( i=0; i<10; i++ ) {
		/* skip to line 2 */
		if ( i == 8 ) {
			x = 0;
			y += 24;
		}
		/* width of 16 = 1.0f */
		font->chars[i] = gfxSpriteFromSubTexture( fontid, x, y, 16, 24 );
		font->xsize[i] = font->xspacing * ((float)num1_width[i]) * 0.0625f;
		gfxSetSpriteTransparency( font->chars[i], 0.0f );
		x += num1_width[i];
	}
}


/**********************************************************
 * Function:	surfLoadFont1
 **********************************************************
 * Description: Load game font number 1
 * Inputs:		font - initialised font struct' for this set
 * Notes:		nil
 * Returns:		updated font structure with loaded characters
 **********************************************************/

void surfLoadFont1 (
	SurfFontData *font
    )
{
	GfxTextureId	fontid;
	int				i, x, y;
	int				pw, c;
	float			ps;

	/* load font image a */
	fontid = p2iTextureToGfxTexture( "font1a.p2i", FALSE );
	x = 0;
	y = 0;

	/* validate */
	if ( fontid == GFX_NO_TEXTURE )
		surfExit( FONT_ERROR_FONT1_LOAD_FAILED );
	
	/* create sprites for each character */
	for ( i=0; i<158; i+=2 ) {
		/* init */
		ps = (float)font1_width[i];
		pw = (int)font1_width[(i+1)];
		c  = (i>>1);
		/* in loop events */
		switch(c) {
		case 9:  // 5
		case 22: // B
		case 29: // I
		case 36: // P
		case 56: // d
		case 65: // m
		case 73: // u
			/* skip to next line */
			x = 0;
			y += 24;
			break;
		case 43: // W
			/* load font image b */
			fontid = p2iTextureToGfxTexture( "font1b.p2i", FALSE );
			/* validate */
			if ( fontid == GFX_NO_TEXTURE )
				surfExit( FONT_ERROR_FONT1_LOAD_FAILED );
			x = 0;
			y = 0;
			break;
		}
		//printf( "i=%d,c=%d,x=%d,y=%d\n", i, c, (int)x, (int)y );
		/* skip unused characters */
		if ( pw == 0 ) continue;
		/* width of 16 = 1.0f */
		if ( (x + pw) < 127 )
			font->chars[c] = gfxSpriteFromSubTexture( fontid, x, y, (pw + 1), 25 );
		else
			font->chars[c] = gfxSpriteFromSubTexture( fontid, x, y, pw, 25 );
		font->xsize[c] = font->xspacing * ps * 0.0625f;
		gfxSetSpriteTransparency( font->chars[c], 0.0f );
		/* update x */
		x += pw;
	}
}


/**********************************************************
 * Function:	surfLoadFont2
 **********************************************************
 * Description: Load game font number 2
 * Inputs:		font - initialised font struct' for this set
 * Notes:		nil
 * Returns:		updated font structure with loaded characters
 **********************************************************/

void surfLoadFont2 (
	SurfFontData *font
    )
{
	GfxTextureId	fontid;
	int				i, x, y;
	int				pw, c;
	float			ps;

	/* load font image a */
	fontid = p2iTextureToGfxTexture( "font2.p2i", FALSE );
	x = 0;
	y = 0;

	/* validate */
	if ( fontid == GFX_NO_TEXTURE )
		surfExit( FONT_ERROR_FONT2_LOAD_FAILED );
	
	/* create sprites for each character */
	for ( i=0; i<158; i+=2 ) {
		/* init */
		ps = (float)font2_width[i];
		pw = (int)font2_width[(i+1)];
		c  = (i>>1);
		/* in loop events */
		switch(c) {
		case 22: // B
		case 37: // Q
		case 57: // e
		case 72: // t
			/* skip to next line */
			x = 0;
			y += 16;
			break;
		}
		//printf( "i=%d,c=%d,x=%d,y=%d\n", i, c, (int)x, (int)y );
		/* skip unused characters */
		if ( pw == 0 ) continue;
		/* width of 8 = 1.0f */
		if ( (x + pw) < 127 )
			font->chars[c] = gfxSpriteFromSubTexture( fontid, x, y, (pw + 1), 16 );
		else
			font->chars[c] = gfxSpriteFromSubTexture( fontid, x, y, pw, 16 );
		font->xsize[c] = font->xspacing * ps * 0.125f;
		gfxSetSpriteTransparency( font->chars[c], 0.0f );
		/* update x */
		x += pw;
	}
}


/**********************************************************
 * Function:	surfPrintFont
 **********************************************************
 * Description: Print a font
 * Inputs:		font - font struct'
 *				str  - string to print out
 * Notes:		nil
 * Returns:		nil
 **********************************************************/

void surfPrintFont (
	SurfFontData *font,
    Text		  str
    )
{
	int		i;
	float	xlast;
	int		c;
	float	xstart;

	/* init */
	xlast = 0;
	i = 0;
	xstart = font->xpos;
	while( str[i] != '\0' ) {
		c = (int)str[i];
		/* special accent character = move back to previous character position */
		if ( c == FONT_ACCENT_CHAR ) font->xpos -= xlast;
		if ( c == '\n' || font->xpos > 1.0f ) {
			font->ypos += font->yshift;
			if ( (font->ypos + font->yshift) > 1.0f )
				font->ypos = SURF_FONT_YMARGIN;
			font->xpos = xstart;
		} else {
			if ( c >= font->start_char && c <= font->end_char ) {
				c -= font->start_char;
				if ( font->chars[c] != GFX_NO_TEXTURE ) {
					gfxSetSpriteScale( font->chars[c], font->xscale, font->yscale );
					gfxSetSpriteColor( font->chars[c], font->r, font->g, font->b );
					gfxSetSpritePosition( font->chars[c], font->xpos, font->ypos );
					gfxDrawSprite( font->chars[c], font->depth );
				}
				xlast = (font->xsize[c] * font->xscale);
			} else
				xlast = font->xshift;
			font->xpos += xlast;
		}
		i++;
	}
}


/**********************************************************
 * Function:	surfPrintFontJustified
 **********************************************************
 * Description: Print a font centred on the x axis
 * Inputs:		font - font struct'
 *				just - justification mode
 *				x - x position
 *				y - y position
 *				str  - string to print out
 * Notes:		nil
 * Returns:		nil
 **********************************************************/

void surfPrintFontJustified (
	SurfFontData *font,
	int			  just,
	float		  x,
	float		  y,
    Text		  str
    )
{
	int		 i, slen;
	int		 c, start;
	float	 xsizes[FONT_MAX_LINES];
	float	*xsize;
	float	 ysize;
	float	 fi;
	float	 xpos;
	float	 ypos;
	float	 xmax;
	int		 nlines;
	char	 linestr[FONT_MAX_LINES][FONT_MAX_LINE_LEN];

	/* calculate xsize, number of lines and copy to temp buffer if necessary */
	i        = 0;
	nlines   = 0;
	xsize    = &xsizes[0];
	(*xsize) = 0.0f;
	start    = 0;
	xmax     = 0.0f;
	while( str[i] != '\0' ) {
		/* init */
		c = str[i];
		/* new line? */
		if ( c == '\n' ) {
			/* copy line to temp buffer */
			slen = (i - start);
			memCopy( &str[start], linestr[nlines], slen );
			linestr[nlines][slen] = '\0';
			nlines++;
			/* validate */
			if ( nlines == FONT_MAX_LINES )
				surfExit( FONT_ERROR_TOO_MANY_LINES );
			/* update xmax */
			if ( *xsize > xmax )
				xmax = *xsize;
			/* re-init */
			start = (i + 1);
			xsize = &xsizes[nlines];
			(*xsize) = 0.0f;
		} else {
			if ( c != FONT_ACCENT_CHAR ) {	// accent characters don't count...
				if ( c >= font->start_char && c <= font->end_char ) {
					c -= font->start_char;
					(*xsize) += (font->xsize[c] * font->xscale);
				} else
					(*xsize) += font->xshift;
			}
		}
		i++;
	}

	/* update */
	if ( nlines > 0 ) {
		/* copy line to temp buffer */
		slen = (i - start);
		memCopy( &str[start], linestr[nlines], slen );
		linestr[nlines][slen] = '\0';
		nlines++;
		/* validate */
		if ( nlines > FONT_MAX_LINES )
			surfExit( FONT_ERROR_TOO_MANY_LINES );
		/* update xmax */
		if ( *xsize > xmax )
			xmax = *xsize;
	} else {
		/* end of first and only line */
		nlines++;
	}

	if ( nlines == 1 ) {

		/* single line */

		/* vertical position adjustment */
		switch( just & FONT_JUST_VERT_MASK ) {
		case FONT_JUST_VERT_CENTRE:
			ypos = (y - (font->yshift * 0.5f));
			break;
		case FONT_JUST_VERT_BOTTOM:
			ypos = (y - font->yshift);
			break;
		case FONT_JUST_VERT_TOP:
		default:
			ypos = y;
			break;
		}

		/* horizontal position adjustment */
		switch( just & FONT_JUST_HOR_MASK ) {
		case FONT_JUST_HOR_RIGHT:
			xpos = (x - *xsize);
			break;
		case FONT_JUST_HOR_CENTRE:
			xpos = (x - (*xsize * 0.5f));
			break;
		case FONT_JUST_HOR_LEFT:
		default:
			xpos = x;
			break;
		}

		/* locate and print */
		surfFontPosition( font, xpos, ypos );
		surfPrintFont( font, str );

	} else {

		/* multi line support */
		ysize = (font->yshift * nlines);
		for ( i=0; i<nlines; i++ ) {

			/* init */
			fi = (float)i;
			xsize = &xsizes[i];

			/* vertical position adjustment */
			switch( just & FONT_JUST_VERT_MASK ) {
			case FONT_JUST_VERT_CENTRE:
				ypos = (y - (ysize * 0.5f)) + (fi * font->yshift);
				break;
			case FONT_JUST_VERT_BOTTOM:
				ypos = (y - ysize) + (fi * font->yshift);
				break;
			case FONT_JUST_VERT_TOP:
			default:
				ypos = y + (fi * font->yshift);
				break;
			}

			/* horizontal position adjustment */
			switch( just & FONT_JUST_HOR_MASK ) {
			case FONT_JUST_HOR_RIGHT:
				xpos = (x - xmax) + (xmax - *xsize);
				break;
			case FONT_JUST_HOR_CENTRE:
				xpos = (x - (*xsize * 0.5f));
				break;
			case FONT_JUST_HOR_LEFT:
			default:
				xpos = x;
				break;
			}

			/* locate and print */
			surfFontPosition( font, xpos, ypos );
			surfPrintFont( font, linestr[i] );
		}
	}
}


#if 0
/**********************************************************
 * Function:	surfFontLength
 **********************************************************
 * Description: Return the length of a font string
 * Inputs:		font - font struct'
 *				str  - string to get length for
 * Notes:		nil
 * Returns:		length (in float format(1.0f=4096float))
 **********************************************************/

float surfFontLength (
	SurfFontData *font,
    Text		  str
    )
{
	int		i;
	int		c;
	float	xsize;

	/* calculate length */
	xsize = 0.0f;
	i = 0;
	while( str[i] != '\0' ) {
		c = str[i];
		if ( c >= font->start_char && c <= font->end_char ) {
			c -= font->start_char;
			xsize += (font->xsize[c] * font->xscale);
		} else
			xsize += font->xshift;
		i++;
	}

	return( xsize );
}
#endif


/**********************************************************
 * Function:	surfResetFont
 **********************************************************
 * Description: Reset font for new frame
 * Inputs:		font - font data
 * Notes:		nil
 * Returns:		updated font structure
 **********************************************************/

void surfResetFont (
    SurfFontData	*font
    )
{
	surfFontScale( font, 1.0f, 1.0f );
	surfFontColor( font, 1.0f, 1.0f, 1.0f );
	surfFontPosition( font, SURF_FONT_XMARGIN, SURF_FONT_YMARGIN );
}


/**********************************************************
 * Function:	surfFontColor
 **********************************************************
 * Description: Set color for font
 * Inputs:		font - font data
 *				r    - red color
 *				g    - green color
 *				b    - blue color
 * Notes:		nil
 * Returns:		update font structure
 **********************************************************/

void surfFontColor (
    SurfFontData	*font,
    float			 r,
	float			 g,
	float			 b
    )
{
	font->r = r;
	font->g = g;
	font->b = b;
}


/**********************************************************
 * Function:	surfFontPosition
 **********************************************************
 * Description: Set font position for new frame
 * Inputs:		font - font data
 *				x    - x font position
 *				y    - y font position
 * Notes:		range 0..1.0f
 * Returns:		update font structure
 **********************************************************/

void surfFontPosition (
    SurfFontData	*font,
	float			 x,
	float			 y
    )
{
	font->xpos = x;
	font->ypos = y;
}


/**********************************************************
 * Function:	surfFontScale
 **********************************************************
 * Description: Set font scale
 * Inputs:		font - font data
 *				x    - x scale factor
 *				y    - y scale factor
 * Notes:		range 0..1.0f
 * Returns:		update font structure
 **********************************************************/

void surfFontScale (
    SurfFontData	*font,
	float			 x,
	float			 y
    )
{
	font->xscale = x;
	font->yscale = y;
	font->xshift = font->xspacing * font->xscale;
	font->yshift = font->yspacing * font->yscale;
}

