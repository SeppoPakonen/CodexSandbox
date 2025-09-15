/*****************************************************************
 * Project		: Model Library
 *****************************************************************
 * File			: modparse.c
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 30 May 1998
 * Last Update	: 30 May 1998
 *****************************************************************
 * Description	: Model library file parsing functions
 *****************************************************************/

/*
 * includes
 */

#include <stdio.h>
#include <text/text.h>
#include <parse/parse.h>
#include <memory/memory.h>
#include "modmsg.h"
#include "model.h"
#include "model_int.h"

/*
 * macros
 */

#define MOD_READING_NONE		0x00
#define MOD_READING_POINTS		0x01
#define MOD_READING_GORAUD		0x02
#define MOD_READING_TRIANGLES	0x04
#define MOD_READING_UVEES		0x08
#define MOD_READING_NORMALS		0x10

/*
 * typedefs
 */

typedef struct {
	ModModel	*model;
	Bool		 reading;
	Text		 layer;
} ModParseInfo;

/*
 * prototypes
 */

void modParseJoint( void );
void modParseLayer( void );
void modParseTransparency( void );
void modParseChild( void );
void modParseImage( int i );
void modParsePoints( void );
int  modParseGetAxis( int n );

/*
 * variables
 */

ModParseInfo	pinfo = { 0, 0, 0 };

/* 
 * globals variables...
 */

/*
 * functions
 */


/******************************************************************************
 * Function:
 * modParseReset -- reset values for parsing an input file
 * 
 * Description:
 *
 * Returns:
 * 
 */

void	modParseReset ( 
	ModModel	*model
	)
{
	modSetErrorString( NULL );
	pinfo.model           = model;
	pinfo.reading         = MOD_READING_NONE;
	memFree( pinfo.layer );
}


/******************************************************************************
 * Function:
 * modParse -- parse input file
 * 
 * Description:
 *
 * Returns:
 * 
 */

void	modParse ( 
	void
	)
{
	/* get points, triangle or goraud data */
	if ( pseArgIsInt( 0 ) ) {
		modParsePoints();
		return;
	}

	/* points */
	if ( pseArgValidate( "points", 0 ) ) {
		pinfo.reading = MOD_READING_POINTS;
		return;
	}

	/* normals */
	if ( pseArgValidate( "normals", 0 ) ) {
		pinfo.reading = MOD_READING_NORMALS;
		return;
	}

	/* layer */
	if ( pseArgValidate( "layer", 0 ) ) {
		pinfo.reading = MOD_READING_NONE;
		modParseLayer();
		return;
	}

	/* transparency */
	if ( pseArgValidate( "transparency", 0 ) ) {
		pinfo.reading = MOD_READING_NONE;
		modParseTransparency();
		return;
	}

	/* joint */
	if ( pseArgValidate( "joint", 0 ) ) {
		pinfo.reading = MOD_READING_NONE;
		modParseJoint();
		return;
	}

	/* triangles */
	if ( pseArgValidate( "triangles", 0 ) ) {
		pinfo.reading = MOD_READING_TRIANGLES;
		return;
	}

	/* goraud */
	if ( pseArgValidate( "goraud", 0 ) ) {
		pinfo.reading = MOD_READING_GORAUD;
		return;
	}

	/* uvees */
	if ( pseArgValidate( "uvees", 0 ) ) {
		pinfo.reading = MOD_READING_UVEES;
		return;
	}

	/* image */
	if ( pseArgValidate( "image", 0 ) ) {
		pinfo.reading = MOD_READING_NONE;
		modParseImage( 0 );
		return;
	}

	/* mirrorimage */
	if ( pseArgValidate( "mirrorimage", 0 ) ) {
		pinfo.reading = MOD_READING_NONE;
		modParseImage( 1 );
		return;
	}

	/* child */
	if ( pseArgValidate( "child", 0 ) ) {
		pinfo.reading = MOD_READING_NONE;
		modParseChild();
		return;
	}
}


/******************************************************************************
 * Function:
 * modParseLayer -- get layer info from args
 * 
 * Description:
 *
 * Returns:
 * 
 */

void modParseLayer (
	void
	)
{
	int		r, g, b;
	Bool	singlesided;
	Bool	mirrored;
	int		mirroraxis;

	if ( pseNumArgs() < 6 ) {
		modSetErrorString( MOD_MSG_BAD_LAYER );
		pseAbortParse();
		return;
	}
	
	memFree( pinfo.layer );
	pinfo.layer = txtDupl( pseGetStringValue( 1 ) );
	r = pseGetIntValue( 2 );
	g = pseGetIntValue( 3 );
	b = pseGetIntValue( 4 );
	if ( pseArgValidate( "singlesided", 5 ) ) 
		singlesided = TRUE;
	else
		singlesided = FALSE;

	if ( pseNumArgs() == 8 ) {
		mirrored = pseArgValidate( "mirror", 6 ); 
		if ( mirrored ) mirroraxis = modParseGetAxis( 7 );
	} else {
		mirrored = FALSE;
		mirroraxis = MOD_X_AXIS;
	}

	if ( ! modAddLayer( pinfo.model, pinfo.layer, r, g, b, singlesided, mirrored, mirroraxis ) )
		pseAbortParse();
}


/******************************************************************************
 * Function:
 * modParseImage -- get image info from args
 * 
 * Description:
 *
 * Returns:
 * 
 */

void modParseImage (
	int		i
	)
{
	int		xaxis, yaxis;
	Text	image;
	int		nargs;

	if ( i ) 
		nargs = 2;
	else
		nargs = 4;
	if ( pseNumArgs() != nargs ) {
		modSetErrorString( MOD_MSG_BAD_IMAGE );
		pseAbortParse();
		return;
	}

	if ( pseArgValidate( "none", 1 ) )
		image = NULL;
	else
		image = pseGetStringValue( 1 );

	if ( i ) {
		xaxis = 0;
		yaxis = 0;
	} else {
		xaxis = modParseGetAxis( 2 );
		yaxis = modParseGetAxis( 3 );
	}

	if ( ! modAddImage( pinfo.model, pinfo.layer, i, image, xaxis, yaxis ) )
		pseAbortParse();
}


/******************************************************************************
 * Function:
 * modParseChild -- get child info from args
 * 
 * Description:
 *
 * Returns:
 * 
 */

void modParseChild (
	void
	)
{
	Text		 model_name;
	Point_f3d	 position;
	Point_f3d	 scale;
	Point_f3d	 angle;
	int			 i;

	if ( pseNumArgs() != 11 ) {
		modSetErrorString( MOD_MSG_BAD_CHILD );
		pseAbortParse();
		return;
	}

	model_name = pseGetStringValue( 1 );
	for ( i=2; i<11; i++ ) {
		if ( pseArgIsString( i ) ) {
			modSetErrorString( MOD_MSG_BAD_CHILD );
			pseAbortParse();
			return;
		}
	}
	position.x = pseGetFloatValue( 2 );
	position.y = pseGetFloatValue( 3 );
	position.z = pseGetFloatValue( 4 );
	scale.x = pseGetFloatValue(  5 );
	scale.y = pseGetFloatValue(  6 );
	scale.z = pseGetFloatValue(  7 );
	angle.x = pseGetFloatValue(  8 );
	angle.y = pseGetFloatValue(  9 );
	angle.z = pseGetFloatValue( 10 );

	if ( ! modAddChild( pinfo.model, model_name, position, scale, angle ) )
		pseAbortParse();
}


/******************************************************************************
 * Function:
 * modParseJoint -- get joint info from args
 * 
 * Description:
 *
 * Returns:
 * 
 */

void modParseJoint (
	void
	)
{
	Text		 joint_name;
	Text		 parent_name;
	int			 x, y, z;
	int			 npts;

	if ( pseNumArgs() != 7 ) {
		modSetErrorString( MOD_MSG_BAD_JOINT );
		pseAbortParse();
		return;
	}

	joint_name = pseGetStringValue( 1 );
	x = pseGetIntValue( 2 );
	y = pseGetIntValue( 3 );
	z = pseGetIntValue( 4 );
	parent_name = pseGetStringValue( 5 );
	npts = pseGetIntValue( 6 );

	if ( ! modAddJoint( pinfo.model, joint_name, x, y, z, parent_name, npts ) )
		pseAbortParse();
}


/******************************************************************************
 * Function:
 * modParseTransparency -- get transparency for this layer
 * 
 * Description:
 *
 * Returns:
 * 
 */

void modParseTransparency (
	void
	)
{
	float		 trans;

	if ( pseNumArgs() != 2 ) {
		modSetErrorString( MOD_MSG_BAD_TRANSPARENCY );
		pseAbortParse();
		return;
	}

	trans = pseGetFloatValue( 1 );
	if ( ! modAddTransparency( pinfo.model, pinfo.layer, trans ) )
		pseAbortParse();
}


/******************************************************************************
 * Function:
 * modParseGetAxis -- get axis from args
 * 
 * Description:
 *
 * Returns:
 * 
 */

int modParseGetAxis (
	int		n
	)
{
	if ( pseArgValidate( "x", n ) ) return( MOD_X_AXIS );
	if ( pseArgValidate( "y", n ) ) return( MOD_Y_AXIS );
	if ( pseArgValidate( "z", n ) ) return( MOD_Z_AXIS );
	if ( pseArgValidate( "-x", n ) ) return( MOD_NEG_X_AXIS );
	if ( pseArgValidate( "-y", n ) ) return( MOD_NEG_Y_AXIS );
	if ( pseArgValidate( "-z", n ) ) return( MOD_NEG_Z_AXIS );
	/* error */
	modSetErrorString( MOD_MSG_BAD_AXIS );
	pseAbortParse();
	return( 0 );
}


/******************************************************************************
 * Function:
 * modParsePoints -- get points
 * 
 * Description:
 *
 * Returns:
 * 
 */

void modParsePoints (
	void
	)
{
	int		x, y, z;
	Byte	r, g, b;
	int		p[512];
	int		quad;
	int		pt, npts;
	int		n;
	float	u, v;
	float	nx, ny, nz;

	switch( pinfo.reading ) {
	case MOD_READING_POINTS:
		/* point data */
		if ( pseNumArgs() == 4 ) {
			n = pseGetIntValue( 0 );
			x = pseGetIntValue( 1 );
			y = pseGetIntValue( 2 );
			z = pseGetIntValue( 3 );
			if ( ! modAddPoint( pinfo.model, n, x, y, z ) )
				pseAbortParse();
		} else {
			modSetErrorString( MOD_MSG_BAD_POINT );
			pseAbortParse();
		}
		break;
	case MOD_READING_GORAUD:
		/* goraud data */
		if ( pseNumArgs() == 4 ) {
			n = pseGetIntValue( 0 );
			r = (Byte)pseGetIntValue( 1 );
			g = (Byte)pseGetIntValue( 2 );
			b = (Byte)pseGetIntValue( 3 );
			if ( ! modAddGoraud( pinfo.model, n, r, g, b ) )
				pseAbortParse();
		} else {
			modSetErrorString( MOD_MSG_BAD_GOURAUD );
			pseAbortParse();
		}
		break;
	case MOD_READING_TRIANGLES:
		/* triangle data */
		quad = pseGetIntValue( 0 );
		switch( quad ) {
		case 0:	// triangle primitive
			if ( pseNumArgs() < 4 ) {
				modSetErrorString( MOD_MSG_BAD_TRIANGLE );
				pseAbortParse();
			}
			p[0] = pseGetIntValue( 1 );
			p[1] = pseGetIntValue( 2 );
			p[2] = pseGetIntValue( 3 );
			if ( ! modAddTriangle( pinfo.model, pinfo.layer, (Bool)quad, p[0], p[1], p[2], 0 ) )
				pseAbortParse();
			break;
		case 1:	// triangle pair "quad" primitive
			if ( pseNumArgs() < 5 ) {
				modSetErrorString( MOD_MSG_BAD_TRIANGLE );
				pseAbortParse();
			}
			p[0] = pseGetIntValue( 1 );
			p[1] = pseGetIntValue( 2 );
			p[2] = pseGetIntValue( 3 );
			p[3] = pseGetIntValue( 4 );
			if ( ! modAddTriangle( pinfo.model, pinfo.layer, (Bool)quad, p[0], p[1], p[2], p[3] ) )
				pseAbortParse();
			break;
		case 2:	// triangle strip primitive
			if ( pseNumArgs() < 4 ) {
				modSetErrorString( MOD_MSG_BAD_TRIANGLE );
				pseAbortParse();
				break;
			}
			npts = (pseNumArgs() - 1);
			// bad if too many points (512 max but can be extended)
			if ( npts > 512 ) {
				modSetErrorString( MOD_MSG_BAD_TRIANGLE );
				pseAbortParse();
				break;
			}
			for ( pt=0; pt<npts, pt<512; pt++ ) {
				p[pt] = pseGetIntValue( (pt+1) );
			}
			if ( ! modAddTriangleStrip( pinfo.model, pinfo.layer, npts, p ) )
				pseAbortParse();
			break;
		default:
			modSetErrorString( MOD_MSG_BAD_TRIANGLE );
			pseAbortParse();
		}
		break;
	case MOD_READING_UVEES:
		/* texture uvee data */
		if ( pseNumArgs() == 3 ) {
			pt = pseGetIntValue( 0 );
			u  = pseGetFloatValue( 1 );
			v  = pseGetFloatValue( 2 );
			if ( ! modAddUVee( pinfo.model, pinfo.layer, (pt - 1), u, v ) )
				pseAbortParse();
		} else {
			modSetErrorString( MOD_MSG_BAD_UVEE );
			pseAbortParse();
		}
		break;
	case MOD_READING_NORMALS:
		/* normals */
		if ( pseNumArgs() == 4 ) {
			n = pseGetIntValue( 0 );
			nx = pseGetFloatValue( 1 );
			ny = pseGetFloatValue( 2 );
			nz = pseGetFloatValue( 3 );
			if ( ! modAddNormal( pinfo.model, n, nx, ny, nz ) )
				pseAbortParse();
		} else {
			modSetErrorString( MOD_MSG_BAD_NORMAL );
			pseAbortParse();
		}
		break;
	default:
		/* error if we get here */
		modSetErrorString( MOD_MSG_BAD_DATA );
		pseAbortParse();
		break;
	}
}

