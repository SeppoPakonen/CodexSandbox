/*****************************************************************
 * Project		: Convert Library
 *****************************************************************
 * File			: convert.c
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 27 Dec 1998
 * Last Update	: 27 Dec 1998
 *****************************************************************
 * Description	:
 *
 *		This file contains the set of data types for linked
 *		list creation and management.
 *
 *****************************************************************
 * 27-Dec-98	: Initial coding.
 *****************************************************************/

#ifndef THEYER_CONVERT_H
#define THEYER_CONVERT_H

#include <type/datatype.h>
#include <text/text.h>

#ifdef __cplusplus
extern "C" {
#endif

Bool cnvIntToString( int i, Text buffer );

#ifdef __cplusplus
}
#endif

#endif
