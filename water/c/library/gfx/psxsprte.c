/**********************************************************
 * Copyright (C) 1998 Mark Theyer
 * All Rights Reserved
 **********************************************************
 * Project:	PSX game
 **********************************************************
 * File:	psxsprte.c
 * Author:	Mark Theyer
 * Created:	07 Dec 1999
 **********************************************************
 * Description:	PSX Graphics Interface
 **********************************************************
 * Notes:		You cannot use 'psx' as a variable name.
 **********************************************************
 * Functions:
 *	gfxSpriteFromTexture()
 *				Create a sprite from a loaded texture.
 *	gfxSpriteFromSubTexture()
 *				Create a sprite from an area in a loaded
 *				texture.
 *	gfxDrawSprite()
 *				Draw a sprite.
 *	gfxSetSpriteScale()
 *				Set the scale for a sprite.
 *	gfxSetSpriteOrigin()
 *				Set the origin for a sprite.
 *	gfxSetSpritePosition()
 *				Set the position of a sprite.
 *	gfxSetSpriteBrightness()
 *				Set the brightness (grey balance) of a sprite.
 *	gfxSetSpriteColor()
 *				Set the color balance for a sprite.
 **********************************************************
 * Revision History:
 * 07-Dec-99	Theyer	Initial creation from gfxpsx.c
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

#define SPRITE_FAST_DRAW_TEST	((sprite->originx == 0) && (sprite->originy == 0) && (sprite->scalex == FIXED_ONE) && (sprite->scaley == FIXED_ONE) && (sprite->angle == 0))

/*
 * typedefs
 */

/*
 * prototypes
 */

/*
 * variables
 */


extern PsxData ps;


/**********************************************************
 * Function:	gfxSpriteFromTexture
 **********************************************************
 * Description: Create a sprite from a loaded texture
 * Inputs:	id - texture id as passed back from gfxLoadTexture()
 * Notes:	
 * Returns:	void
 **********************************************************/

public GfxSpriteId gfxSpriteFromTexture ( 
	GfxTextureId	id
	)
{
    int			   i, x, y;
	PsxTexture	  *tex;
	unsigned char *data;
	PsxSprite	  *sprite;

#if GFX_VALIDATE
	if ( id < 1 || id > ps.id ) {
		gfxError( GFX_ERROR_BAD_TEXTURE_ID );
		gfxAbort();
	}

    /* room for more sprites? */
    if ( (ps.sid+1) >= PSX_MAX_SPRITES ) {
		gfxError( GFX_ERROR_MAX_SPRITES_USED );
		gfxAbort();
    }
#endif

	/* find if we have already got a sprite for this texture */
	tex = &ps.textures[id-1];
	for ( i=0; i<ps.sid; i++ ) {
		if ( ps.spritetexid[i] == id ) {
			sprite = &ps.sprites[i];
			if ( sprite->texpage == tex->texpage &&
				 sprite->xoffset == tex->xoffset &&
				 sprite->yoffset == tex->yoffset &&
				 sprite->width   == tex->width   &&
				 sprite->height  == tex->height ) {
				/* found existing sprite - use it... */
				return( (i+1) );
			}
		}	
	}

	/* init (no validation!) */
	ps.sid++;
	tex = &ps.textures[id-1];
	sprite = &ps.sprites[ps.sid-1];

	/* init sprite */
	sprite->unused			= 0;
	sprite->nobrightness	= FALSE;
	sprite->unused2			= 0;
	sprite->format			= tex->format;
	sprite->unused3			= 0;
	sprite->draw_as_is		= TRUE;
	sprite->transparency	= PSX_TRANSPARENCY_25;
	sprite->transparency_on = FALSE;
	sprite->displayoff      = FALSE;
	sprite->x               = -(ps.xmax/2);
	sprite->y               = -(ps.ymax/2);
	sprite->width           = tex->width;
	sprite->height          = tex->height;
	sprite->texpage			= tex->texpage;
	sprite->xoffset         = tex->xoffset;
	sprite->yoffset			= tex->yoffset;
	if ( tex->cmapid != GFX_NO_COLORMAP ) {
		i = tex->cmapid - 1;
		x = ps.textures[i].x;
		y = ps.textures[i].y;
		//printf("clut x, y=%d, %d\n", x, clut->cly );
		sprite->clutx = x;//(x>>4);
		sprite->cluty = y;
	} else {
		sprite->clutx = 0;
		sprite->cluty = 0;
	}
	sprite->r       = 128;
	sprite->g       = 128;
	sprite->b       = 128;
	sprite->originx = 0;
	sprite->originy = 0;
	sprite->scalex  = ONE;
	sprite->scaley  = ONE;
	sprite->angle   = 0;

	/* save the texture id source for this sprite... */
	ps.spritetexid[ps.sid-1] = id;

	return( ps.sid );
}


/**********************************************************
 * Function:	gfxSpriteFromSubTexture
 **********************************************************
 * Description: Create a sprite from a portion of a loaded texture
 * Inputs:	id - texture id as passed back from gfxLoadTexture()
 * Notes:	
 * Returns:	void
 **********************************************************/

public GfxSpriteId gfxSpriteFromSubTexture ( 
	GfxTextureId	id,
	int				x,
	int				y,
	int				width,
	int				height
	)
{
    int			   i;//, x, y;
	PsxTexture	  *tex;
	unsigned char *data;
	PsxSprite	  *sprite;

#if GFX_VALIDATE
	if ( id < 1 || id > ps.id ) {
		gfxError( GFX_ERROR_BAD_TEXTURE_ID );
		gfxAbort();
	}

    /* room for more sprites? */
    ps.sid++;
    if ( ps.sid >= PSX_MAX_SPRITES ) {
        gfxError( GFX_ERROR_MAX_SPRITES_USED );
		gfxAbort();
    }
#endif

	/* init (no validation!) */
	tex = &ps.textures[id-1];
	sprite = &ps.sprites[ps.sid-1];

#if GFX_VALIDATE
	//printf( "x=%d, y=%d, width=%d, height=%d, texw=%d, texh=%d\n", x, y, width, height, tex->width, tex->height );
	if (( (x + width)  <= 0 || (x + width)  > tex->width  ) ||
		( (y + height) <= 0 || (y + height) > tex->height )) {
		gfxError( GFX_ERROR_SUBTEXTURE_NOT_IN_LIMITS );
		gfxAbort();
	}
#endif

	/* init sprite */
	sprite->unused			= 0;
	sprite->nobrightness	= FALSE;
	sprite->unused2			= 0;
	sprite->format			= tex->format;
	sprite->unused3			= 0;
	sprite->draw_as_is		= TRUE;
	sprite->transparency	= PSX_TRANSPARENCY_25;
	sprite->transparency_on = FALSE;
	sprite->displayoff      = FALSE;
	sprite->x               = -(ps.xmax/2);
	sprite->y               = -(ps.ymax/2);
	sprite->width           = width;
	sprite->height          = height;
	sprite->texpage			= tex->texpage;
	sprite->xoffset         = tex->xoffset + x;
	sprite->yoffset			= tex->yoffset + y;
	if ( tex->cmapid != GFX_NO_COLORMAP ) {
		i = tex->cmapid - 1;
		x = ps.textures[i].x;
		y = ps.textures[i].y;
		sprite->clutx = x;//(x>>4);
		sprite->cluty = y;
	} else {
		sprite->clutx = 0;
		sprite->cluty = 0;
	}
	sprite->r       = 128;
	sprite->g       = 128;
	sprite->b       = 128;
	sprite->originx = 0;
	sprite->originy = 0;
	sprite->scalex  = ONE;
	sprite->scaley  = ONE;
	sprite->angle   = 0;

	/* save the texture id source for this sprite... */
	ps.spritetexid[ps.sid-1] = id;

	return( ps.sid );
}


/**********************************************************
 * Function:	gfxDrawSprite
 **********************************************************
 * Description: Draw a sprite
 * Inputs:	id - sprite id as passed back from gfxSpriteFromTexture()
 *			depth - depth in screen
 * Notes:	
 * Returns:	void
 **********************************************************/

public void gfxDrawSprite ( 
	GfxSpriteId		id,
	fixed			depth
	)
{
	PsxSprite	  *sprite;

#if GFX_VALIDATE
	if ( id < 1 || id > ps.sid ) {
		gfxError( GFX_ERROR_BAD_SPRITE_ID );
		gfxAbort();
	}
#endif

	/* init (no validation!) */
	sprite = &ps.sprites[id - 1];

	if ( ps.split_screen.on ) {
		if ( sprite->draw_as_is )
			GsSortFastSprite( (GsSPRITE *)sprite, (GsOT *)&ps.screen.split.worldorder[(ps.drawpage + ps.split_screen.active_buffer)], (u_short)FIXED_TO_INT((PSX_OT_SIZE>>1) * depth) );
		else 
			GsSortSprite( (GsSPRITE *)sprite, (GsOT *)&ps.screen.split.worldorder[(ps.drawpage + ps.split_screen.active_buffer)], (u_short)FIXED_TO_INT((PSX_OT_SIZE>>1) * depth) );
	} else {
		if ( sprite->draw_as_is )
			GsSortFastSprite( (GsSPRITE *)sprite, (GsOT *)&ps.screen.full.worldorder[ps.drawpage], (u_short)FIXED_TO_INT(PSX_OT_SIZE * depth) );
		else 
			GsSortSprite( (GsSPRITE *)sprite, (GsOT *)&ps.screen.full.worldorder[ps.drawpage], (u_short)FIXED_TO_INT(PSX_OT_SIZE * depth) );
	}
}


/**********************************************************
 * Function:	gfxSetSpriteScale
 **********************************************************
 * Description: Set the scale for a sprite
 * Inputs:	id - sprite id as passed back from gfxSpriteFromTexture()
 *			scalex - x position on screen in the range 0 ~ 1.0
 *			scaley - y position on screen in the range 0 ~ 1.0
 * Notes:	
 * Returns:	void
 **********************************************************/

public void gfxSetSpriteScale ( 
	GfxSpriteId		id,
	fixed			scalex,
	fixed			scaley
	)
{
	PsxSprite	  *sprite;

#if GFX_VALIDATE
	if ( id < 1 || id > ps.sid ) {
		gfxError( GFX_ERROR_BAD_SPRITE_ID );
		gfxAbort();
	}
	//if ( scalex < 0 || scaley < 0 ) {
	//	gfxError( GFX_ERROR_BAD_SPRITE_SCALE );
	//	gfxAbort();
	//}
#endif

	/* init (no validation!) */
	sprite = &ps.sprites[id - 1];

	/* update */
	sprite->scalex = (short)scalex;
	sprite->scaley = (short)scaley;

	/* update "as is" flag */
	if ( SPRITE_FAST_DRAW_TEST )
		sprite->draw_as_is = TRUE;
	else
		sprite->draw_as_is = FALSE;
}


/**********************************************************
 * Function:	gfxSetSpriteRotation
 **********************************************************
 * Description: Set the rotation angle for a sprite
 * Inputs:	id - sprite id as passed back from gfxSpriteFromTexture()
 *			angle - angle in degrees
 * Notes:	
 * Returns:	void
 **********************************************************/

public void gfxSetSpriteRotation ( 
	GfxSpriteId		id,
	int				angle
	)
{
	PsxSprite	  *sprite;

#if GFX_VALIDATE
	if ( id < 1 || id > ps.sid ) {
		gfxError( GFX_ERROR_BAD_SPRITE_ID );
		gfxAbort();
	}
	if ( angle < -359 || angle > 359 ) {
		gfxError( GFX_ERROR_BAD_SPRITE_ANGLE );
		gfxAbort();
	}
#endif

	/* init (no validation!) */
	sprite = &ps.sprites[id - 1];

	/* update */
	sprite->angle = (long)INT_TO_FIXED(angle);

	/* update "as is" flag */
	if ( SPRITE_FAST_DRAW_TEST )
		sprite->draw_as_is = TRUE;
	else
		sprite->draw_as_is = FALSE;
}


/**********************************************************
 * Function:	gfxSetSpriteOrigin
 **********************************************************
 * Description: Set the origin for a sprite
 * Inputs:	id - sprite id as passed back from gfxSpriteFromTexture()
 *			scalex - x position on screen in the range 0 ~ 1.0
 *			scaley - y position on screen in the range 0 ~ 1.0
 * Notes:	
 * Returns:	void
 **********************************************************/

public void gfxSetSpriteOrigin ( 
	GfxSpriteId		id,
	fixed			originx,
	fixed			originy
	)
{
	PsxSprite	  *sprite;

#if GFX_VALIDATE
	if ( id < 1 || id > ps.sid ) {
		gfxError( GFX_ERROR_BAD_SPRITE_ID );
		gfxAbort();
	}
	if (( originx < 0 || originx > 4096 ) ||
		( originy < 0 || originy > 4096 )) {
		gfxError( GFX_ERROR_SPRITE_ORIGIN_OUT_OF_RANGE );
		gfxAbort();
	}
#endif

	/* init (no validation!) */
	sprite = &ps.sprites[id - 1];

	/* update */
	sprite->originx = FIXED_TO_INT( (sprite->width  * originx) );
	sprite->originy = FIXED_TO_INT( (sprite->height * originy) );

	/* update "as is" flag */
	if ( SPRITE_FAST_DRAW_TEST )
		sprite->draw_as_is = TRUE;
	else
		sprite->draw_as_is = FALSE;
}


/**********************************************************
 * Function:	gfxSetSpriteTransparency
 **********************************************************
 * Description: Set the transparency for a sprite
 * Inputs:	id - sprite id as passed back from gfxSpriteFromTexture()
 *			scalex - x position on screen in the range 0 ~ 1.0
 *			scaley - y position on screen in the range 0 ~ 1.0
 * Notes:	
 * Returns:	void
 **********************************************************/

public void gfxSetSpriteTransparency ( 
	GfxSpriteId		id,
    fixed			trans
	)
{
	PsxSprite	  *sprite;

#if GFX_VALIDATE
	if ( id < 1 || id > ps.sid ) {
		gfxError( GFX_ERROR_BAD_SPRITE_ID );
		gfxAbort();
	}
#endif

	/* init (no validation!) */
	sprite = &ps.sprites[id - 1];

	/* update */
	if ( trans <= FIXED_ZERO || trans > FIXED_ONE ) {		
		sprite->transparency	= PSX_TRANSPARENCY_25;
		sprite->transparency_on = FALSE;
		return;
	} else if ( trans < FIXED_NORMALISE(4,10) )
		sprite->transparency = PSX_TRANSPARENCY_25;
	else if ( trans <= FIXED_NORMALISE(65,100) )
		sprite->transparency = PSX_TRANSPARENCY_50;
	else if ( trans <= FIXED_NORMALISE(8,10) )
		sprite->transparency = PSX_TRANSPARENCY_75;
	else
		sprite->transparency = PSX_TRANSPARENCY_90;
	sprite->transparency_on = TRUE;
}


/**********************************************************
 * Function:	gfxSetSpritePosition
 **********************************************************
 * Description: Set the position of a sprite
 * Inputs:	id - sprite id as passed back from gfxSpriteFromTexture()
 *			x - x position on screen in the range 0 ~ 1.0
 *			y - y position on screen in the range 0 ~ 1.0
 * Notes:	
 * Returns:	void
 **********************************************************/

public void gfxSetSpritePosition ( 
	GfxSpriteId		id,
	fixed			x,
	fixed			y
	)
{
	PsxSprite	  *sprite;

#if GFX_VALIDATE
	if ( id < 1 || id > ps.sid ) {
		gfxError( GFX_ERROR_BAD_SPRITE_ID );
		gfxAbort();
	}
#endif

	/* init (no validation!) */
	sprite = &ps.sprites[id - 1];

	/* update */
	sprite->x = (short)FIXED_TO_INT((ps.xmax * x)) - (ps.xmax>>1);
	sprite->y = (short)FIXED_TO_INT((ps.ymax * y)) - (ps.ymax>>1);
}


/**********************************************************
 * Function:	gfxSetSpriteBrightness
 **********************************************************
 * Description: Set the brightness of a sprite
 * Inputs:	id - sprite id as passed back from gfxSpriteFromTexture()
 *			br - brightness (range 0.0 - 1.0)
 * Notes:	
 * Returns:	void
 **********************************************************/

public void gfxSetSpriteBrightness ( 
	GfxSpriteId		id,
	fixed			br
	)
{
	PsxSprite	  *sprite;

#if GFX_VALIDATE
	if ( id < 1 || id > ps.sid ) {
		gfxError( GFX_ERROR_BAD_SPRITE_ID );
		gfxAbort();
	}
#endif

	/* init (no validation!) */
	sprite = &ps.sprites[id - 1];

	/* update */
	sprite->r = (char)FIXED_TO_INT((br * 128));
	sprite->g = sprite->r;
	sprite->b = sprite->r;
}


/**********************************************************
 * Function:	gfxSetSpriteColor
 **********************************************************
 * Description: Set the brightness of a sprite
 * Inputs:	id - sprite id as passed back from gfxSpriteFromTexture()
 *			r - red brightness value (range 0.0 - 2.0)
 *			g - green brightness value (range 0.0 - 2.0)
 *			b - blue brightness value (range 0.0 - 2.0)
 * Notes:	
 * Returns:	void
 **********************************************************/

public void gfxSetSpriteColor ( 
	GfxSpriteId		id,
	fixed			r,
	fixed			g,
	fixed			b
	)
{
	PsxSprite	  *sprite;

#if GFX_VALIDATE
	if ( id < 1 || id > ps.sid ) {
		gfxError( GFX_ERROR_BAD_SPRITE_ID );
		gfxAbort();
	}
#endif

	/* init (no validation!) */
	sprite = &ps.sprites[id - 1];

	/* update */
	sprite->r = (char)FIXED_TO_INT((r * 128));
	sprite->g = (char)FIXED_TO_INT((g * 128));
	sprite->b = (char)FIXED_TO_INT((b * 128));
}


#if 0
/**********************************************************
 * Function:	gfxSetSpriteWorldPosition
 **********************************************************
 * Description: Set the position of a sprite
 * Inputs:	id - sprite id as passed back from gfxSpriteFromTexture()
 *			wxyz - world position
 * Notes:	
 * Returns:	
 **********************************************************/

public void gfxSetSpriteWorldPosition ( 
	GfxSpriteId		 id,
	GfxObject		*object,
	Point_i3d		 wxyz
	)
{
	PsxSprite	    *sprite;
	int			     x, y;
    PsxObject	    *psxobj;
    PsxObjPosition  *position;

#if GFX_VALIDATE
	if ( id < 1 || id > ps.sid ) {
		gfxError( GFX_ERROR_BAD_SPRITE_ID );
		gfxAbort();
	}
#endif

    /* init */
	if ( object != NULL ) {
		psxobj   = (PsxObject *)object->data;
		position = psxGetObjectPosition( psxobj );
	} else
		position = NULL;

	/* init (no validation!) */
	sprite = &ps.sprites[id - 1];

	/* convert to world */
	psxWorldToScreen( position, wxyz.x, wxyz.y, wxyz.z, &x, &y );
	sprite->x = (short)x;
	sprite->y = (short)y;
}
#endif

