/*****************************************************************
 * Project		: GUI Library
 *****************************************************************
 * File			: memory.h
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 28 Dec 1996
 * Last Update	: 28 Dec 1996
 *****************************************************************
 * Description	:
 *
 *		This file contains the set of data types for linked
 *		list creation and management.
 *
 *****************************************************************
 * 28/12/96		: New Version.
 *****************************************************************/

#ifndef THEYER_FILE_H
#define THEYER_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <type/datatype.h>
#include <text/text.h>

/* file is id */
typedef int		File;

#ifdef __cplusplus
extern "C" {
#endif

extern  Bool	fileInit( void );
extern  File	fileOpen( Text filename, Text mode );
extern  Bool	fileClose( File file );
extern  int		fileRead( File file, void *buffer, int size, int cnt );
extern  int		fileWrite( File file, void *buffer, int size, int cnt );
extern  Bool	fileEof( File file );
extern  Bool	fileError( File file );
extern  char   *fileGetLine( char *str, int size, File file );

#ifdef __cplusplus
}
#endif

#endif
