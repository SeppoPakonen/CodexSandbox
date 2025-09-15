/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     fileCache.h

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/charPipeline/fileCache.h $
    
    3     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
    2     12/08/99 12:16p Ryan
    added function descriptions
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  The fileCache library provides a simple, generic caching mechanism.  While
  this mechanism can be used for anything, it is usually used for file access.
  By using this library, the user can be sure that any given file is loaded
  only once.  Any subsequent requests can simply retrieve the cached copy.
 *---------------------------------------------------------------------------*/

#ifndef FILECACHE_H
#define FILECACHE_H

/********************************/
#if 0

extern "C" {

#endif

/********************************/
#include <CharPipe/structures.h>

/********************************/
#define	DS_AUTO_PURGE	0
#define	DS_NO_PURGE	1

/********************************/
typedef struct
{
	DSLink	Link;
	
	void	(*Free)(Ptr *data);

	char	*Name;

	Ptr	Data;


	u16	ReferenceCount;


}DSCacheNode, *DSCacheNodePtr;

/********************************/
typedef struct
{
	u8	PurgeFlag;

	DSList	CacheNodeList;

}DSCache, *DSCachePtr;

/********************************/
/*---------------------------------------------------------------------------*
  DSAddCacheNode - Adds a data node into the cache.
 *---------------------------------------------------------------------------*/
DSCacheNodePtr	DSAddCacheNode		( DSCachePtr cache, char *name, Ptr data, 
									  Ptr freeFunc );

/*---------------------------------------------------------------------------*
  DSEmptyCache - Removes all cache nodes from the specified cache.
 *---------------------------------------------------------------------------*/
void			DSEmptyCache		( DSCachePtr cache );

/*---------------------------------------------------------------------------*
  DSGetCacheObj - Returns a pointer to the requested item in the cache.  
	Returns 0 if item is not found.
 *---------------------------------------------------------------------------*/
Ptr				DSGetCacheObj		( DSCachePtr cache, char *name );

/*---------------------------------------------------------------------------*
  DSInitCache - Initializes a cache structure.
 *---------------------------------------------------------------------------*/
void			DSInitCache			( DSCachePtr cache );

/*---------------------------------------------------------------------------*
  DSPurgeCache - Purges the entire cache list from memory
 *---------------------------------------------------------------------------*/
void			DSPurgeCache		( DSCachePtr cache );

/*---------------------------------------------------------------------------*
  DSReleaseCacheObj - Decrements the requested item's reference count. If the 
	reference count is 0 and the cache flag is set to DS_AUTO_PURGE, purges 
	the requested item from the cache.
 *---------------------------------------------------------------------------*/
void			DSReleaseCacheObj	( DSCachePtr cache, Ptr data );

/*---------------------------------------------------------------------------*
  DSSetCachePurgeFlag - Sets the cache purge flag.
 *---------------------------------------------------------------------------*/
void			DSSetCachePurgeFlag	( DSCachePtr cache, u8 purgeFlag );

/*>*******************************(*)*******************************<*/
//	GLOBAL DISPLAY CACHE FUNCTIONS
/*>*******************************(*)*******************************<*/
/*---------------------------------------------------------------------------*
  CSHInitDisplayCache - Initializes the display cache to be used by all 
  character pipeline disk access functions.
 *---------------------------------------------------------------------------*/
void	CSHInitDisplayCache	( void );

/********************************/

#if 0
}
#endif

/********************************/
#endif
