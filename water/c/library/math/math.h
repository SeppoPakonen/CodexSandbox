
#ifndef THEYER_MATH_H
#define THEYER_MATH_H

#include "compile.h"

#include <stdio.h>

#ifdef OsTypeIsWin95
#include <math.h>
#endif

#ifdef OsTypeIsPsx
#include <libps.h>
#endif

#include <type/datatype.h>

#define M_PI						3.1415927
#define DEGREES_TO_RADIANS(angle)	(double)((M_PI*2) * (angle/360.0))
#define RADIANS_TO_DEGREES(angle)	(double)(((angle)/M_PI) * 180.0)


#define	absF(val)	((float)fabs((double)val))

extern int mthFastSqrt( int value );

#endif
