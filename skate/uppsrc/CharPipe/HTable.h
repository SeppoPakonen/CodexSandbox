/*---------------------------------------------------------------------------*
  Project: [structures]
  File:    [HTable.h]

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/include/charPipeline/structures/HTable.h $
    
    2     12/08/00 1:12p John
    Changed define from _TREE_H to _TREE_H_ for MSL C++ template
    compatibility.
    
    4     9/29/99 4:39p John
    Changed header to make them all uniform with logs.

  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef _HTABLE_H_
#define _HTABLE_H_

#include <Dolphin/types.h>
#include <CharPipe/List.h>

#if 0
extern "C" {
#endif

typedef u16(DSHashFunc)( Ptr );

typedef struct 
{
  DSList*       table;
  u16		    tableSize;
  DSHashFunc*	hash;
} DSHashTable;

void    DSInitHTable        ( DSHashTable* hTable, 
                              u16          size, 
                              DSList*      listArray,
                              DSHashFunc*  hashFunc, 
                              Ptr          obj, 
                              DSLinkPtr    link );
void    DSInsertHTableObj   ( DSHashTable* hTable, Ptr obj );
void    DSHTableToList      ( DSHashTable* hTable, DSList* list );
void*   DSNextHTableObj     ( DSHashTable* hTable, Ptr obj );
s32     DSHTableIndex       ( DSHashTable* hTable, Ptr obj );
void*   DSHTableHead        ( DSHashTable* hTable, s32 index );

#if 0
}
#endif

#endif // _HTABLE_H_
