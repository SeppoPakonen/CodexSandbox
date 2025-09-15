/*---------------------------------------------------------------------------*
  Project: [structures]
  File:    [List.h]

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/include/charPipeline/structures/List.h $
    
    2     12/08/00 1:12p John
    Changed define from _LIST_H to _LIST_H_ for MSL C++ template
    compatibility.
    
    3     9/29/99 4:39p John
    Changed header to make them all uniform with logs.

  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef _LIST_H_
#define _LIST_H_

#include <Dolphin/types.h>

#if 0
extern "C" {
#endif

typedef struct 
{
	Ptr	Prev;
	Ptr	Next; 

} DSLink, *DSLinkPtr;

typedef struct 
{
	u32	Offset;
	Ptr	Head;
	Ptr	Tail;

} DSList, *DSListPtr;

void	DSInitList			( DSListPtr list, Ptr obj, DSLinkPtr link );
void	DSInsertListObject	( DSListPtr list, Ptr cursor, Ptr obj );
void	DSRemoveListObject	( DSListPtr list, Ptr obj );
void    DSAttachList        ( DSListPtr baseList, DSListPtr attachList );
void*   DSNextListObj       ( DSListPtr list, Ptr obj );

#if 0
}
#endif

#endif // _LIST_H_
