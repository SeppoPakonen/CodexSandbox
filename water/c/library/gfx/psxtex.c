/**********************************************************
 * Copyright (C) 1998 Mark Theyer
 * All Rights Reserved
 **********************************************************
 * Project:		PSX interface
 **********************************************************
 * File:		psxtex.c
 * Author:		Mark Theyer
 * Created:		01 May 1997
 **********************************************************
 * Description:	PSX Graphics Interface
 **********************************************************
 * Notes:		You cannot use 'psx' as a variable name.
 **********************************************************
 * Functions:
 *	gfxLoadMap()
 *				Load a texture map location file.
 *	gfxLoadColorMap()
 *				Load a colormap (RGB color look up table)
 *	gfxLoadTexture()
 *				Load a texture file.
 *	gfxSetTexture()
 *				Set the current texture for new surfaces.
 *	gfxScrollTexture()
 *				Scroll a texture image.
 *	gfxTextureId()
 *				Get the id for a loaded texture.
 *
 *  Private PSX Functions:
 *  ----------------------
 *
 *	psxParseMap()
 *				Read in texture map location file.
 *	psxGetTexturePage()
 *				Get the texture page in the frame buffer
 *				from an x,y location.
 *	psxSetTextureInfo()
 *				Set texture information in a primitive.
 *	psxGetTextureLocation()
 *				Get texture frame buffer location from 
 *				location information loaded via map files.
 *	psxAdjustTexturePoints()
 *				Adjust texture points for loaded texture.
 *
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

/*
 * typedefs
 */

/*
 * prototypes
 */

private u_short	     psxGetTexturePage( int *x, int *y );
private void		 psxParseMap( void *user_data );

/*
 * variables
 */


extern PsxData ps;


/**********************************************************
 * Function:	gfxLoadMap
 **********************************************************
 * Description: Load texture map location info
 * Inputs:		map - map file format data
 * Notes:		Maybe could use parse library in future
 * Returns:		void
 **********************************************************/

public void gfxLoadMap (
	Byte	*map,
	Bool	 clear
    )
{
	int		 i, j, k, m;
	char	 number[32];

	/* replace all maps */
	if ( clear )
		ps.ntexmaps = 0;

	/* parse map file */
	pseParseData( map, "#", 1, psxParseMap, NULL );
}


/**********************************************************
 * Function:	gfxLoadColorMap
 **********************************************************
 * Description: Load a color map
 * Inputs:		data - RGB triplets (range 0-255)
 *				size - number of RGB triplets
 * Notes:	
 * Returns:	
 **********************************************************/

public GfxColorMapId gfxLoadColorMap (
	Text			name,
	unsigned char  *data,
	int				size
	)
{
	GfxTextureId	cmapid;

	/* validate */
	if ( data == NULL ) return( GFX_NO_COLORMAP );

	/* load colormap as texture */
	cmapid = gfxLoadTexture( name, data, GFX_TEXTURE_16BIT, size, 1, GFX_NO_COLORMAP );

	return( (GfxColorMapId)cmapid );
}


/**********************************************************
 * Function:	gfxLoadTexture
 **********************************************************
 * Description: Initialise a texture
 * Inputs:		data   - texture image data
 *				format - GFX_TEXTURE_16BIT (data = RGBA)
 *					   - GFX_TEXTURE_8BIT  (data = CMAP index)
 *					   - GFX_TEXTURE_4BIT  (data = CMAP index)
 *				width  - width  of image
 *				height - height of image
 *				cmapid - color map id for 8 and 4 bit formats
 * Notes:		Loads the texture into the frame buffer
 * Returns:		texture id
 **********************************************************/

public GfxTextureId gfxLoadTexture (
	Text			 name,
	void			*data,
	GfxTextureFormat format,
	int	 			 width,
    int	 			 height,
	GfxColorMapId	 cmapid
    )
{
    RECT		 rect;
    int			 x, y;
    int			 size;
	GfxTextureId texid, id;
	int			 psxformat;
	Text		 fullname;

	/* already loaded? */
	texid = gfxTextureId( name );
	if ( texid != GFX_NO_TEXTURE ) {
		//printf( "found image: %d\n", texid );
		//return( texid );
		id = texid - 1;
	} else {
		id = ps.id;
		ps.id++;
	}

    /* room for more textures? */
    if ( ps.id > PSX_MAX_TEXTURES ) {
        gfxError( GFX_ERROR_MAX_TEXTURES_USED );
		gfxAbort();
    }

	/* get frame buffer location for this data */
	psxGetTextureLocation( name, &fullname, &x, &y );

	/* init rectangle */
	rect.x = x;
	rect.y = y;
	switch( format ) {
	case GFX_TEXTURE_16BIT:
		psxformat = PSX_TEXTURE_16BIT;
	    rect.w = width;
	    rect.h = height;
		break;
	case GFX_TEXTURE_8BIT:
		psxformat = PSX_TEXTURE_8BIT;
	    rect.w = width/2;
	    rect.h = height;
		break;
	case GFX_TEXTURE_4BIT:
		psxformat = PSX_TEXTURE_4BIT;
	    rect.w = width/4;
	    rect.h = height;
		break;
	default:
		gfxError( GFX_ERROR_BAD_IMAGE_FORMAT );
		gfxAbort();
	}

	/* load image to the frame buffer */
	if ( data != NULL ) {
	    LoadImage( &rect, (u_long *)data );
		DrawSync(0);
	}

    /* save texture info */
	ps.textures[id].format  = psxformat;
	ps.textures[id].x	    = x;
	ps.textures[id].y	    = y;
    ps.textures[id].width   = width;
    ps.textures[id].height  = height;
    ps.textures[id].texpage = psxGetTexturePage( &x, &y );

	/* adjust offset for format */
	switch( psxformat ) {
	case PSX_TEXTURE_16BIT:
		break;
	case PSX_TEXTURE_8BIT:
		x *= 2;
		break;
	case PSX_TEXTURE_4BIT:
		x *= 4;
		break;
	}

	/* save rest of texture info */
    ps.textures[id].xoffset = x;
    ps.textures[id].yoffset = y;
	ps.textures[id].cmapid  = cmapid;
	ps.textures[id].name    = fullname;

#if GFX_DEBUG
	printf( "gfx texture loaded: %s, format=%d, %X\n", fullname, format, data );
    printf( "texture id %d loaded: x=%d, y=%d, w=%d, h=%d, page=%d, xoff=%d, yoff=%d\n", (int)(ps.id), rect.x, rect.y, width, height, ps.textures[id].texpage, x, y );
#endif

    return( ps.id );
}


/**********************************************************
 * Function:	gfxSetTexture
 **********************************************************
 * Description: Set the current texture to apply when
 *		creating an object
 * Inputs:	id - texture id as passed back from gfxInitTexture()
 * Notes:	-1 means don't use any texture
 * Returns:	void
 **********************************************************/

public void gfxSetTexture( GfxTextureId id )
{
#if 0
	/* temporary hack */
    ps.textureid = GFX_NO_TEXTURE;
    ps.textureon = FALSE;
	return;
#else
    if ( id == GFX_NO_TEXTURE ) {
        ps.textureid = GFX_NO_TEXTURE;
        ps.textureon = FALSE;
    } else {
    	ps.textureid = (id - 1);
        ps.texpage = ps.textures[ps.textureid].texpage;
        ps.textureon = TRUE;
    }
#endif
}


/**********************************************************
 * Function:	gfxScrollTexture
 **********************************************************
 * Description: Scroll the texture referred to by the id
 *				creating an object
 * Inputs:	id - texture id as passed back from gfxLoadTexture()
 *			x  - scroll n pixels in the x direction
 *			y  - scroll n pixels in the y direction
 * Notes:	
 * Returns:	void
 **********************************************************/

public void gfxScrollTexture ( 
	GfxTextureId	id,
	int				y
	)
{
    RECT		   rect;
	PsxTexture	  *tex;
	unsigned char *data;

#if GFX_VALIDATE
	if ( id < 1 || id > ps.id ) {
		gfxError( GFX_ERROR_BAD_TEXTURE_ID );
		gfxAbort();
	}
#endif

	/* init (no validation!) */
	tex = &ps.textures[id-1];

	/* force y scroll value to be in range */
	while ( y > tex->height )
		y -= tex->height;

	/* init rectangle */
	rect.x = tex->x;
	rect.y = tex->y;
	//rect.h = (tex->height - y);
	switch( tex->format ) {
	case PSX_TEXTURE_16BIT:
	    rect.w = tex->width;
		break;
	case PSX_TEXTURE_8BIT:
	    rect.w = tex->width/2;
		break;
	case PSX_TEXTURE_4BIT:
	    rect.w = tex->width/4;
		break;
	}
	rect.h = tex->height;

#if 1
	/* new method? */
	StoreImage( &rect, (u_long*)ps.filebuffer );
    //DrawSync(0);
	data = (unsigned char *)ps.filebuffer;
#else
	/* old internal storage method */
	data = (unsigned char *)tex->data;
#endif

	/* first part of scrolled image */
	rect.h = (tex->height - y);
    LoadImage( &rect, (u_long *)(data + ((int)tex->width * y)) );
    //DrawSync(0);

	/* second part of scrolled image */
	rect.y += (tex->height - y);
	rect.h = y;
	LoadImage( &rect, (u_long *)data );
	//DrawSync(0);
}


/**********************************************************
 * Function:	gfxTextureId
 **********************************************************
 * Description: Get a defined texture id
 * Inputs:		name - name of texture
 * Notes:		This is not high performance...
 * Returns:		texture id or GFX_NO_TEXTURE if not loaded
 **********************************************************/

public GfxTextureId gfxTextureId (
    Text	name
    )
{
	int		i, len;
	char	texfile[256];

	/* create name from root path (<path>/<name>) */
	txtCopy( ps.filepath, texfile );
	len = txtLength( ps.filepath );
	txtCopy( name, &texfile[len] );
	/* convert case */
	txtToLower( texfile );

	for ( i=0; i<ps.id; i++ ) {
//#if GFX_DEBUG
//		printf( "match %d: %s, %s, (%s)\n", i, ps.textures[i].name, texfile, name );
//#endif
		if ( txtMatch( ps.textures[i].name, texfile ) )
			return( i+1 );
	}

	return( GFX_NO_TEXTURE );
}


/****************************
 *
 * Private PSX functions... 
 *
 ****************************/


/**********************************************************
 * Function:	psxParseMap
 **********************************************************
 * Description: Parse a texture location map file
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

private void psxParseMap (
    void *user_data
    )
{
	int		i, n;
	Text	name;

	/* validate */
	if ( pseNumArgs() < 3 ) return;

	/* init */
	i = ps.ntexmaps;

	/* validate */
	if ( i >= PSX_MAX_TEXTURES ) {
		gfxError( GFX_ERROR_MAX_MAPS_USED );
		gfxAbort(); 
	}
 
	/* get file name string */
	name = pseGetStringValue(0);
	if ( name == NULL ) return;
	if ( name[0] < 33 ) return;

	/* copy file name */
	n = 0;
	while( name[n] && n<(PSX_MAPNAME_SIZE - 1) ) {
		ps.texmaps[i].name[n] = name[n];
		n++;
	}

	/* add null termination */
	while( n < PSX_MAPNAME_SIZE ) {
		ps.texmaps[i].name[n] = '\0';
		n++;
	}

	/* store x,y location in the frame buffer */
    ps.texmaps[i].x = pseGetIntValue(1);
    ps.texmaps[i].y = pseGetIntValue(2);

#if GFX_DEBUG
	/* debug */
	printf( "map %d = %s, %d, %d (%d)\n", i, ps.texmaps[i].name, ps.texmaps[i].x, ps.texmaps[i].y, ps.texmaps[i].name[0] );		
#endif

	/* increment number of map locations */
	ps.ntexmaps++;
}
 

/**********************************************************
 * Function:	psxGetTexturePage
 **********************************************************
 * Description: Adjust texture offsets for PSX page layout
 * Inputs:	x - x offset
 *		y - y offset
 * Notes:	
 * Returns:	updated x, y offsets and texture page
 **********************************************************/

private u_short psxGetTexturePage (
    int		*x,
    int		*y
    )
{
    u_short	 page;

    /* get psx texture page number */
#if 0
    page = ((*x - 512) / 64) + 8;

    /* adjust x offset */
    *x -= (page - 8) * 64;
#else
	page = (*x / 64);
	*x -= (page * 64);
#endif

    /* adjust for pages at y=256 */
    if ( *y >= 256 ) {
    	page += 16;        
    	*y -= 256;
    }

    return( page );
}


/**********************************************************
 * Function:	psxSetTexture
 **********************************************************
 * Description: Adjust texture offsets for PSX page layout
 * Inputs:	texture - texture data to update
 * Notes:	
 * Returns:	updated texture data
 **********************************************************/

internal void psxSetTextureInfo (
    PsxTextureInfo  	*texture,
    PsxClutLocation		*clut 
    )
{
	int		i, x, y;
	int		mode;

	/* init */
	i = ps.textureid;

    /* set texture info */
    texture->id                = ps.texpage;
    texture->transparency_mode = ps.transparency_mode;
	texture->texture_mode      = ps.textures[i].format;
    texture->unused            = 0;

	/* init clut info */
	clut->clx = 0;
	clut->cly = 0;
	clut->unused = 0;

    /* define color look up table location */
	if ( ps.textures[i].cmapid != GFX_NO_COLORMAP ) {
		i = (ps.textures[i].cmapid - 1);
		x = ps.textures[i].x;
		y = ps.textures[i].y;
		//printf("clut location = %d, %d\n", x, y );
		/* adjust x and y */
#if 0
		if ( ps.texpage > 15 ) {
			x -= ((ps.texpage - 16) * 64);
			y -= 256;
		} else {
			x -= (ps.texpage * 64);
		}
#endif
		/* set clut info */
		clut->clx = (x>>4);
		clut->cly = y;
		//printf("clut x, y=%d, %d\n", x, clut->cly );
	}
}


/**********************************************************
 * Function:	psxGetTextureLocation
 **********************************************************
 * Description: Get texture location for loading into 
 *				the frame buffer
 * Inputs:		data - 
 * Notes:	
 * Returns:		initialised x and y
 **********************************************************/

internal void psxGetTextureLocation (
	Text		 name,
	Text		*fullname,
    int			*x,
	int			*y
    )
{
	int				i, len;
	char			texfile[256];

#if GFX_VALIDATE
	/* validate */
	if ( ps.ntexmaps == 0 ) {
		gfxError( GFX_ERROR_NO_TEXTURE_LOCATION_INFO );
		gfxAbort();
	}
#endif

	/* create name from root path (<path>/<name>) */
	txtCopy( ps.filepath, texfile );
	len = txtLength( ps.filepath );
	txtCopy( name, &texfile[len] );	
	/* convert case */
	txtToLower( texfile );

	for ( i=0; i<ps.ntexmaps; i++ ) {
		//printf( "info=0x%X,%d,%d, data=0x%X\n", info->data, info->x, info->y, data );
		if ( txtMatch( texfile, ps.texmaps[i].name ) ) {
			*x = ps.texmaps[i].x;
			*y = ps.texmaps[i].y;
			*fullname = ps.texmaps[i].name;
			//printf( "texture location is %d, %d\n", *x, *y );
			return;
		}
	}	

	/* failed */
	printf( "no map info for file: %s\n", texfile );
	gfxError( GFX_ERROR_NO_IMAGE_LOCATION_INFO );
	gfxAbort();
}


/**********************************************************
 * Function:	psxAdjustTexturePoints
 **********************************************************
 * Description: Adjust texture points for PSX layout
 * Inputs:	txy - texture points
 * Notes:	
 * Returns:	pointer to copy of updated points
 **********************************************************/

internal PsxTexPoint * psxAdjustTexturePoints (
    Point_fx2d	 txy[],
    int		 	 npts
    )
{
    int					i;
    static PsxTexPoint  atxy[4];
	int					x, y;

	/* not necessary if textures not on */
	if ( ! ps.textureon ) return( atxy );

#if GFX_VALIDATE
    /* validate */	
    if ( txy == NULL ) {
        gfxError( GFX_ERROR_NULL_TEXTURE_POINTS );
		gfxAbort();
    }
#endif

    for ( i=0; i<npts; i++ ) {
		/* NOTE: 0.0 - 1.0 range must be converted using current texture width & height */
		//printf( "image width = %d, height = %d\n", ps.textures[ps.textureid].width, ps.textures[ps.textureid].height );
		x = (Byte)FIXED_TO_INT( (txy[i].x * (ps.textures[ps.textureid].width  - 1)) );
		y = (Byte)FIXED_TO_INT( (txy[i].y * (ps.textures[ps.textureid].height - 1)) );
		//printf2("texture point (%d) %d = %d, %d = %d\n", i, txy[i].x, x, txy[i].y, y );
        atxy[i].x = x + ps.textures[ps.textureid].xoffset;
		atxy[i].y = y + ps.textures[ps.textureid].yoffset;
		//printf( "image xoffset = %d, yoffset = %d\n", ps.textures[ps.textureid].xoffset, ps.textures[ps.textureid].yoffset );
		//printf2("texture point (%d) %f = %d, %f = %d\n", i, txy[i].x, atxy[i].x, txy[i].y, atxy[i].y );
    }

    return( atxy );
}

