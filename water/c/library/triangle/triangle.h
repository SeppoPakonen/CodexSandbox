/*****************************************************************
 * Project		: Triangle Library
 *****************************************************************
 * File			: triangle.h
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 27 Jun 1998
 * Last Update	: 27 Jun 1998
 *****************************************************************
 * Description	: Modeller application header
 *****************************************************************/

#ifndef THEYER_TRIANGLE_H
#define THEYER_TRIANGLE_H

/*
 * includes
 */

#include <type/datatype.h>

/* 
 * macros
 */

#define TRI_X_AXIS				1
#define TRI_Y_AXIS				2
#define TRI_Z_AXIS				3
#define TRI_NEG_X_AXIS			4
#define TRI_NEG_Y_AXIS			5
#define TRI_NEG_Z_AXIS			6

#ifdef TRI_FIXEDPOINT_API
extern fixed	fixed_sin_table[];
extern fixed	fixed_cos_table[];
#define triSinFAST(angle)		((fixed)fixed_sin_table[angle])
#define triCosFAST(angle)		((fixed)fixed_cos_table[angle])
#else
extern float	sintable[];
extern float	costable[];
#define triSinFAST(angle)		*(sintable + angle)
#define triCosFAST(angle)		*(costable + angle)
#endif

/*
 * typedefs
 */

/*
 * prototypes
 */

#ifdef TRI_FIXEDPOINT_API
/* tritex.c */
extern Point_fx2d	triCalcTextureCoord( Point_i3d pt, Point_i3d *limits, char xaxis, char yaxis );
extern Point_fx2d  *triCalcTextureCoords( Point_i3d *pts, Point_i3d *limits, char xaxis, char yaxis );
/* trinorm.c */
extern Point_fx3d	triCalcNormal( Point_i3d *pts );
extern Bool			triClockWise( int x0, int y0, int x1, int y1, int x2, int y2 );
/* tricosin.c */
extern fixed		triCos( int angle );
extern fixed		triSin( int angle );
extern fixed		triACos( fixed value );
#else
/* tritex.c */
extern Point_f2d	triCalcTextureCoord( Point_f3d pt, Point_f3d *limits, char xaxis, char yaxis );
extern Point_f2d   *triCalcTextureCoords( Point_f3d *pts, Point_f3d *limits, char xaxis, char yaxis );
/* trinorm.c */
extern Point_f3d	triCalcNormal( Point_f3d *pts );
extern Bool			triClockWise( float x0, float y0, float x1, float y1, float x2, float y2 );
/* tricosin.c */
extern float		triCos( int angle );
extern float		triSin( int angle );
extern float		triACos( float value );
#endif

#endif
