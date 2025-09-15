/*---------------------------------------------------------------------------*
  Project: [structures]
  File:    [Tree.h]

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/include/charPipeline/structures/Tree.h $
    
    2     12/08/00 1:12p John
    Changed define from _TREE_H to _TREE_H_ for MSL C++ template
    compatibility.
    
    2     9/29/99 4:39p John
    Changed header to make them all uniform with logs.

  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef _TREE_H_
#define _TREE_H_

#include <Dolphin/types.h>

#if 0
extern "C" {
#endif

typedef struct 
{
	Ptr	Prev;
	Ptr	Next;
	Ptr	Parent;
	Ptr	Children;

} DSBranch, *DSBranchPtr;

typedef struct 
{
	u32 Offset;
	Ptr Root;

} DSTree, *DSTreePtr;

void	DSExtractBranch		( DSTreePtr tree, Ptr obj );
void	DSInitTree			( DSTreePtr tree, Ptr obj, DSBranchPtr branch );
void	DSInsertBranchBelow	( DSTreePtr tree, Ptr cursor, Ptr obj );
void	DSInsertBranchBeside( DSTreePtr tree, Ptr cursor, Ptr obj );
void	DSRemoveBranch		( DSTreePtr tree, Ptr obj );

#if 0
}
#endif

#endif // _TREE_H_
