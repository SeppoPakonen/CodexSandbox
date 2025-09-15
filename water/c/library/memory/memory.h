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

#ifndef THEYER_MEMORY_H
#define THEYER_MEMORY_H

#include "compile.h"

#ifdef OsTypeIsDos
#include <malloc.h>
#include <mem.h>
#endif 

#ifdef OsTypeIsWin95
#include <memory.h>
#include <malloc.h>	
#endif

#ifdef OsTypeIsUnix
#include <malloc.h>
#endif

#ifdef OsTypeIsPs2
#include <string.h>
#include <malloc.h>
#endif

#ifdef OsTypeIsPsx
#include <malloc.h>
/* these functions are not supported on the PSone! */
#define memAlloc(s)			exit(-1);
#define memReAlloc(ptr,s)	exit(-1);
#define memNew(type)		exit(-1);
#define memFree(p)			exit(-1);
#else
/* ok to use... */
#define memAlloc(s)			malloc(s)
#define memReAlloc(ptr,s)	realloc(ptr,s)
#define memNew(type)		(type *)memAlloc(sizeof(type))
#define memFree(p)			{if (p != NULL) free(p);p=NULL;}
#endif

#define memClear(s,n)		memset((void *)s,0,n)
#define memCopy(s,d,n)		memcpy((void *)d,(void *)s,n)

#endif
