/*---------------------------------------------------------------------*

Project:  tc library
File:     TCMem.c

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:

 $Log: /Dolphin/build/charPipeline/tc/src/TCMem.cpp $
    
    2     3/17/00 1:19p Mikepc
    change tc to use indices numbered from 0.
    
    1     12/03/99 3:45p Ryan
    
    7     10/13/99 12:35p Mikepc
    removed unused local var. from mFreeMem.
    
    6     10/08/99 2:45p Mikepc
    update for tplConv portability: altered data structures, replaced
    references to 'read tga code' with ' *fileFn, removed redundant
    functions.  Changed some file conversion paths.
    
    5     9/02/99 11:12a Mikepc
    some code re-organization between files.
    added code (verify.cpp) to invoke s3tc.exe from within tc program.
    changed some routines to accommodate the new texture creation path.
    
    4     8/26/99 4:59p Mikepc
    renamed file extensions from .c to .cpp.
    .cpp extension allows addition of namespace protection to remove
    potential name collisions with tool code.  Exceptions are CreateTplFile
    and QuickConvert.  These are extern "C" linked.
    
    3     8/26/99 11:38a Mikepc
    
    2     8/26/99 11:03a Mikepc
    tplCon rewrite for efficient memory usage, batch file processing
    ability.
    
  
 $NoKeywords: $

-----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

#include <CharPipe/TCCommon.h>

#include "TCMem.h"
#include "TCImageList.h"
#include "TCPaletteList.h"
#include "TCSrcImageList.h"
#include "TCTextureList.h"
#include "TCFileInternal.h"

/********************************/
typedef struct TCMemNode
{
	void*  mem;
	u32    size;
 
	struct TCMemNode* prev;
	struct TCMemNode* next;

}TCMemNode, *TCMemNodePtr;

/********************************/
// debugging- compare app. allocated memory to freed memory
// including memory allocated to 'MemNodes'
u32 TotalAlloc = 0;
u32 TotalFreed = 0;

// allocated memory linked list
TCMemNode* MemList = NULL;

/*>*******************************(*)*******************************<*/
static TCMemNode* TCNewMemNode	( void );
static void       TCFreeMemNode	( TCMemNode** thisNode );

/*>*******************************(*)*******************************<*/
static TCMemNode* TCNewMemNode ( void )
{
	TCMemNode* newM, *thisM;


	// use stdlib calloc
	if( (newM = (TCMemNode*)calloc( 1,sizeof(TCMemNode))) == NULL )
	{
		TCErrorMsg( "TCNewMemNode: couldn't allocate new TCMemNode\n" );
		return NULL;
	}


	if( MemList == NULL )
	{
		MemList    = newM;
		newM->prev = NULL;
		newM->next = NULL;
		return newM;
	}
	else if( MemList->next == NULL )
	{
		MemList->next = newM;
		newM->prev    = MemList;
		newM->next    = NULL;
		return newM;
	}
	else
	{
		thisM = MemList;
		while( thisM->next )
		{
			thisM = thisM->next;
		}

		thisM->next = newM;
		newM->prev  = thisM;
		newM->next  = NULL;
		return newM;
	}

	return newM;
}

/*>*******************************(*)*******************************<*/
// free a node from within the list
/*>*******************************(*)*******************************<*/
static void TCFreeMemNode ( TCMemNode** thisNode )
{
	TCMemNode* checkNode = NULL;
	u32        found     =    0;


	if( *thisNode == NULL )
	{
		TCErrorMsg( "TCFreeMemNode: tried to free a NULL TCMemNode\n" );
		return;
	}


	// ensure that *thisNode is part of the main memory list
	// this also guards against an empty list
	checkNode = MemList;
	while( checkNode )
	{
		if( checkNode == *thisNode )
		{
			found = 1;
			break;
		}

		checkNode = checkNode->next;
	}
	
	// couldn't find *thisNode
	TCAssertMsg( (found), "TCFreeMemNode: *thisNode is not part of MemList\n" );


	// free node data first

	// stdlib free
	if( (*thisNode)->mem != NULL )
	{
		free( (*thisNode)->mem );
		(*thisNode)->mem = NULL;

		TotalFreed += (*thisNode)->size;
	}
	(*thisNode)->size = 0;


	//------------------------

	// remove the node from the linked list

	if( *thisNode == MemList )          // head
	{
		if( (*thisNode)->next == NULL ) // single node list
		{
			(*thisNode)->prev = NULL;	
			(*thisNode)->next = NULL;
			MemList           = NULL;
		}
		else                            // multi-node list
		{
            MemList = (*thisNode)->next;

            (*thisNode)->next->prev = NULL;
			(*thisNode)->prev       = NULL;
			(*thisNode)->next       = NULL;
		}
	}
	else
	{
		if( (*thisNode)->next == NULL ) // tail
		{
			(*thisNode)->prev->next = NULL;
            (*thisNode)->next       = NULL;
		    (*thisNode)->prev       = NULL;
		}
		else                           // middle of list
		{
			(*thisNode)->prev->next = (*thisNode)->next;
			(*thisNode)->next->prev = (*thisNode)->prev;
            (*thisNode)->next       = NULL;
		    (*thisNode)->prev       = NULL;
		}

	}

	//-----------------------

	// free thisNode's memory
	free( *thisNode );
    TotalFreed += sizeof(TCMemNode);
	*thisNode   = NULL;

}

/*>*******************************(*)*******************************<*/
void* TCMalloc ( u32 size )
{
	TCMemNode* newM;


	newM = TCNewMemNode();

    if( size == 15 )
    {
        printf("found mem" );
    }

	if( (newM->mem = (void*)malloc(size)) == NULL )
	{
		TCErrorMsg("TCMalloc: couldn't allocate block of size %d\n", size );
		return NULL;
	}

	newM->size = size;

	TotalAlloc += ( size + sizeof(TCMemNode) );

	return newM->mem;
}

/*>*******************************(*)*******************************<*/
void* TCCalloc ( u32 num, u32 size )
{
	TCMemNode* newM;
	u32 total;


	newM  = TCNewMemNode();

	total = num * size;

    if( total == 15 )
    {
        printf("found mem" );
    }

	// use stdlib calloc
	if( (newM->mem = (void*)calloc(num, size)) == 0 )
	{
		TCErrorMsg("TCCalloc: couldn't allocate block of size %d\n", total );
		return 0;
	}

	newM->size = total;

	TotalAlloc += ( total + sizeof(TCMemNode) );

	return newM->mem;
}

/*>*******************************(*)*******************************<*/
void TCFree ( void** vPtr )
{
	TCMemNode* thisNode;


	if( *vPtr == NULL )
	{
		return;
	}

	thisNode = MemList;
	while( thisNode != NULL )
	{
		if( thisNode->mem != NULL )
		{
			if( thisNode->mem == *vPtr )
			{
				TCFreeMemNode( &thisNode );
				*vPtr = NULL;
				return;
			}
		}

		thisNode = thisNode->next;
	}

	return;
}

/*>*******************************(*)*******************************<*/
void TCFreeMem ( void )
{
	TCMemNode* thisNode, *nextNode;


	// free all remaining allocated memory blocks
	thisNode = MemList;
	while( thisNode != NULL )
	{
		nextNode = thisNode->next;

		TCFreeMemNode( &thisNode );

		thisNode = nextNode;
	}
	MemList = NULL;

	// reset all global pointers to NULL
	SiHead = NULL;
	ImHead = NULL;
	PlHead = NULL;
	TxHead = NULL;

	// file cache
	DFSize = 0;
	DF     = NULL;

	// file table
	FileTable = NULL;

	// set the path pointers for txtFile keys to NULL
	*PathName = '\0';   
	 PathPtr  = NULL;


	 // debugging info.
	printf("\n");
	printf( "total alloc: %d\n", TotalAlloc );
	printf( "total freed: %d\n", TotalFreed );
}

/*>*******************************(*)*******************************<*/