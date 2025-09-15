/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     fileCache.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/fileCache/src/fileCache.c $
    
    2     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <Dolphin/os.h>

#include <CharPipe/fileCache.h>

/********************************/
//global character pipeline cache variables
DSCache	DODisplayCache;
u8		DOCacheInitialized = 0;

/********************************/
static u8			    AllocCacheNode	( DSCacheNodePtr *cacheNode, char *name );
static DSCacheNodePtr	FindCacheNode	( DSCachePtr cache, char *name, Ptr data );
static void			    FreeCacheNode	( DSCacheNodePtr *cacheNode );

/********************************/
DSCacheNodePtr	DSAddCacheNode ( DSCachePtr cache, char *name, Ptr data, Ptr OSFreeFunc )
{
	DSCacheNodePtr	cacheNode = 0;

	if(!AllocCacheNode(&cacheNode, name)) return 0;

	Strcpy(cacheNode->Name, name);
	cacheNode->Data = data;
	cacheNode->Free = (void(*)(Ptr*))OSFreeFunc;
	cacheNode->ReferenceCount = 0;

	DSInsertListObject((DSListPtr)(&cache->CacheNodeList), 0, (Ptr)cacheNode);

	return cacheNode;
}

/********************************/
static u8	AllocCacheNode ( DSCacheNodePtr *cacheNode, char *name )
{
	if(*cacheNode) FreeCacheNode(cacheNode);

	(*cacheNode) = (DSCacheNodePtr)OSAlloc(sizeof(DSCacheNode));
	if(!(*cacheNode)) return 0;



	(*cacheNode)->Name = (char *)OSAlloc(Strlen(name) + 1);

	if(!(*cacheNode)->Name) return 0;

	return 1;
}

/********************************/
void	DSEmptyCache ( DSCachePtr cache )
{
	DSCacheNodePtr cursor;
	DSCacheNodePtr cacheNode;

	cursor = (DSCacheNodePtr)(cache->CacheNodeList.Head);
	while(cursor)
	{
		cacheNode = cursor;
		cursor = (DSCacheNodePtr)(cursor->Link.Next);	

		DSRemoveListObject(&cache->CacheNodeList, (Ptr)(cacheNode));
		FreeCacheNode(&cacheNode);
	}
}

/********************************/
static DSCacheNodePtr	FindCacheNode ( DSCachePtr cache, char *name, Ptr data )

{
	DSCacheNodePtr cacheNode;

	cacheNode = (DSCacheNodePtr)(cache->CacheNodeList.Head);

	if(data)
	{
		while(cacheNode)
		{
			if(data == cacheNode->Data)		return cacheNode;
			
			cacheNode = (DSCacheNodePtr)(cacheNode->Link.Next);
		}
	}

	else if(name)
	{
		while(cacheNode)
		{
			if(!Strcmp(name, cacheNode->Name))	return cacheNode;

			cacheNode = (DSCacheNodePtr)(cacheNode->Link.Next);
		}
	}

	return 0;
}

/********************************/
Ptr	DSGetCacheObj ( DSCachePtr cache, char *name )
{
	DSCacheNodePtr cacheNode;

	cacheNode = FindCacheNode(cache, name, 0);
	if(cacheNode)
	{
		cacheNode->ReferenceCount++;
		return cacheNode->Data;
	}
	
	return 0;
}

/********************************/
static void	FreeCacheNode ( DSCacheNodePtr *cacheNode )
{
	if(!(*cacheNode))return;
	if((*cacheNode)->Free)
		(*cacheNode)->Free(&(*cacheNode)->Data);


	OSFree((*cacheNode)->Name);
	OSFree(*cacheNode);
	(*cacheNode) = 0;
}

/********************************/
void	DSInitCache ( DSCachePtr cache )
{
	DSCacheNode cacheNode;

	DSInitList(&cache->CacheNodeList, (Ptr)(&cacheNode), &cacheNode.Link);
}

/********************************/
void	DSPurgeCache ( DSCachePtr cache )
{
	DSCacheNodePtr cursor;
	DSCacheNodePtr cacheNode;

	cursor = (DSCacheNodePtr)(cache->CacheNodeList.Head);
	while(cursor)
	{
		cacheNode = cursor;
		cursor = (DSCacheNodePtr)(cursor->Link.Next);		

		if(!cacheNode->ReferenceCount)
		{
			DSRemoveListObject(&cache->CacheNodeList, (Ptr)cacheNode);
			FreeCacheNode(&cacheNode);
		}
	}
}

/********************************/
void	DSReleaseCacheObj ( DSCachePtr cache, Ptr data )
{
	DSCacheNodePtr cacheNode;

	cacheNode = FindCacheNode(cache, 0, data);
	if(!cacheNode) return;

	if(cacheNode->ReferenceCount) cacheNode->ReferenceCount--;
	if((cacheNode->ReferenceCount == 0)&&(cache->PurgeFlag == DS_AUTO_PURGE))
	{
		DSRemoveListObject(&cache->CacheNodeList, (Ptr)(&cacheNode->Link));
		FreeCacheNode(&cacheNode);
	}
}

/********************************/
void	DSSetCachePurgeFlag ( DSCachePtr cache, u8 purgeFlag )
{
	cache->PurgeFlag = purgeFlag;
}

/*>*******************************(*)*******************************<*/
//	GLOBAL DISPLAY CACHE FUNCTIONS
/*>*******************************(*)*******************************<*/
void	CSHInitDisplayCache	( void )
{
	DSInitCache (&DODisplayCache);
	DOCacheInitialized = 1;
}
