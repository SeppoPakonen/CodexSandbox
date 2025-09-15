/*---------------------------------------------------------------------------*
  Project: [structures]
  File:    [HTable.c]

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/build/charPipeline/structures/src/HTable.c $
    
    5     9/29/99 4:39p John
    Changed header to make them all uniform with logs.

  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#include <charPipeline\structures\HTable.h>


/*---------------------------------------------------------------------------*
  Name:         DSInitHTable

  Description:  Initialize the hash table and the list that are included in 
                the table.       
                Need to supply an array of lists

  Arguments:    hTable      Hash table to initialize
                size        size of the hash table
                listArray   array of linked lists
                hashFunc    Hash function for the hash table
                obj         Type of object in the table
                link        Link in the object to calculate the offset

  Returns:      NONE
 *---------------------------------------------------------------------------*/
void
DSInitHTable( DSHashTable* hTable, u16 size, DSList* listArray,
			  DSHashFunc* hashFunc, Ptr obj, DSLinkPtr link )
{
    u16 i = 0;

    // Initialize the members of the hash table
    hTable->table     = listArray;
    hTable->tableSize = size;
    hTable->hash      = hashFunc;

    // Initialize the lists
    for( i = 0; i < size; i++ )
    {
        DSInitList ( listArray + i, obj, link );
    }
}


/*---------------------------------------------------------------------------*
  Name:         DSInsertHTableObj

  Description:  Inserts an object in the hash table

  Arguments:    hTable      Hash table to insert the object in
                obj         Object to insert in the hash table

  Returns:      NONE
 *---------------------------------------------------------------------------*/
void
DSInsertHTableObj( DSHashTable* hTable, Ptr obj )
{
	DSList* list = 0;

	// Find in which list the object belongs
	list = hTable->table + hTable->hash( obj );

	// Insert object at the end of the list
	DSInsertListObject( list, 0, obj );
}


/*---------------------------------------------------------------------------*
  Name:         DSHTableToList

  Description:  Converts a hash table to a linked list.
                Starts with list one to list n-1.

  Arguments:    hTable      Hash table to convert
                list        List that will receive the conversion

  Returns:      NONE
 *---------------------------------------------------------------------------*/
void
DSHTableToList( DSHashTable* hTable, DSList* list )
{
    DSLinkPtr   link = 0;
    u16         i    = 0;

    // copy the first list in the new list
    list->Offset = hTable->table[i].Offset;

    for( i = 0; i < hTable->tableSize; i++ )
    {
        DSAttachList( list, hTable->table + i );
    }
} 


/*---------------------------------------------------------------------------*
  Name:         DSNextHTableObj

  Description:  Returns the next object in the hash table.  

  Arguments:    hTable      Hash table
				obj			Find the object after obj

  Returns:      Pointer to the object after obj.  NULL if none.
 *---------------------------------------------------------------------------*/
void*
DSNextHTableObj( DSHashTable* hTable, Ptr obj )
{
	s32	      currentIndex;
	void*     cursor;

	if ( !hTable )
		return NULL;

	// If no object, return the first object in the hash table
	if ( !obj )
	{
		currentIndex = 0;
		cursor = DSNextListObj( hTable->table + currentIndex, NULL );
	}
	else
	{
		// Find the current index of obj
		currentIndex = DSHTableIndex( hTable, obj );
		if (currentIndex == -1)
			return NULL;
		cursor = DSNextListObj( hTable->table + currentIndex, obj );
	}

	// Keep searching through successive indices until we find an object
	// or exhause the hash table
	while ( cursor == NULL && currentIndex < hTable->tableSize - 1 )
	{
		currentIndex++;
		cursor = DSNextListObj( hTable->table + currentIndex, NULL );
	}

	return cursor;
}


/*---------------------------------------------------------------------------*
  Name:         DSHTableIndex

  Description:  Returns the index of object in the hash table.  

  Arguments:    hTable      Hash table
				obj			obj of which to find index in the hash table

  Returns:      Index of obj in hash table.  -1 if hTable of obj is NULL
 *---------------------------------------------------------------------------*/
s32
DSHTableIndex( DSHashTable* hTable, Ptr obj )
{
	if ( !hTable || !obj )
		return -1;

	return hTable->hash( obj );
}


/*---------------------------------------------------------------------------*
  Name:         DSHTableHead

  Description:  Returns the first object in the hash table.  

  Arguments:    hTable      Hash table
				index		index into hash table

  Returns:      Pointer to the first object.  NULL if none or improper index
 *---------------------------------------------------------------------------*/
void*
DSHTableHead( DSHashTable* hTable, s32 index )
{
	if (index < 0 || index >= hTable->tableSize) {
		return NULL;
	}

	return DSNextListObj( hTable->table + index, NULL );
}