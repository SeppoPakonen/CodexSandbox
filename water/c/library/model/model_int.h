/*****************************************************************
 * Project		: ModelR Application
 *****************************************************************
 * File			: model_int.h
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 9 May 1998
 * Last Update	: 17 May 1998
 *****************************************************************
 * Description	: Modeller application header
 *****************************************************************/

#ifndef THEYER_MODEL_INT_H
#define THEYER_MODEL_INT_H

/*
 * includes
 */

#include <type/datatype.h>

/* 
 * macros
 */

/*
 * typedefs
 */

typedef struct {
	Text		 name;
	int			 width;
	int			 height;
	void		*idata;
	int			 refcnt;
} ModImage;

/*
 * prototypes
 */

/* modparse.c */
extern void modParse( void );
extern void modParseReset( ModModel *model );

/* modutil.c */
extern void modSetErrorString( Text str );

#endif
