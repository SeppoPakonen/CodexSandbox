/*---------------------------------------------------------------------------*
  Project: [structures]
  File:    [List.c]

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/build/charPipeline/structures/src/List.c $
    
    6     9/29/99 4:39p John
    Changed header to make them all uniform with logs.

  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#include <charpipeline\structures\list.h>

/*>*******************************(*)*******************************<*/
/*>-----------------------------------------------------------------<*/
//	Initializes the head and tail of the list to be 0 and sets the 
//	offset of the Link into the obj structure.
/*>-----------------------------------------------------------------<*/
void	DSInitList ( DSListPtr list, Ptr obj, DSLinkPtr link )
{
	list->Head = 0;
	list->Tail = 0;
	list->Offset = ((u32)link - (u32)obj);
}

/*>*******************************(*)*******************************<*/
/*>-----------------------------------------------------------------<*/
//	Inserts an object into the specified list BEFORE the cursor object
/*>-----------------------------------------------------------------<*/
void	DSInsertListObject ( DSListPtr list, Ptr cursor, Ptr obj )
{
	DSLinkPtr link = (DSLinkPtr)((u32)obj + list->Offset);
	DSLinkPtr linkNext;
	DSLinkPtr linkPrev;

	if(list->Head)	// IF THE LIST HAS A HEAD...
	{
		if(!cursor)	// IF THERE WAS NO CURSOR PASSED, INSERT AT THE TAIL.
		{
			linkPrev = (DSLinkPtr)(((u32)list->Tail) + list->Offset);
			linkPrev->Next = obj;
			link->Prev = list->Tail;
			link->Next = 0;
			list->Tail = obj;
			return;
		}

		linkNext = (DSLinkPtr)((u32)cursor + list->Offset);

		if(cursor == list->Head)	// IF THE CURSOR PASSED IS THE HEAD, MAKE obj THE NEW HEAD.
		{
			list->Head = obj;
			link->Next = (Ptr)cursor;
			linkNext->Prev = obj;
			return;
		}
	
		linkPrev = (DSLinkPtr)(((u32)linkNext->Prev) + list->Offset);	// ELSE INSERT obj BEFORE THE 
		link->Next = cursor;										// CURSOR.	
		link->Prev = linkNext->Prev;
		linkNext->Prev = obj;
		linkPrev->Next = obj;
		return;
	}				// ELSE THE LIST IS EMPTY SO MAKE HEAD AND TAIL POINT TO obj.
	
	list->Head = list->Tail = obj;
	link->Next = link->Prev = 0;
}

/*>*******************************(*)*******************************<*/
/*>-----------------------------------------------------------------<*/
//	Removes the specified obj from the list.
/*>-----------------------------------------------------------------<*/
void	DSRemoveListObject ( DSListPtr list, Ptr obj )
{
	DSLinkPtr link = (DSLinkPtr)((u32)obj + list->Offset);

	if( !obj )
		return;

	if(link->Prev)														// IF obj HAS A PREVIOUS
		((DSLinkPtr)((u32)link->Prev + list->Offset))->Next = link->Next;	// PREV->NEXT = OBJ->NEXT
	else																// ELSE
		list->Head = link->Next;										// LIST->HEAD = OBJ->NEXT		

	if(link->Next)														// IF obj HAS A NEXT
		((DSLinkPtr)((u32)link->Next + list->Offset))->Prev = link->Prev;	// NEXT->PREV = OBJ->PREV
	else																// ELSE
		list->Tail = link->Prev;										// LIST->TAIL = OBJ->PREV

	link->Prev = 0;
	link->Next = 0;
}

/*---------------------------------------------------------------------------*
  Name:         DSAttachList

  Description:  Attach a list to another one.

  Arguments:    baseList      List to which a new one will be attached
                attachList    List that will be attached

  Returns:      NONE
 *---------------------------------------------------------------------------*/
void 
DSAttachList( DSListPtr baseList, DSListPtr attachList )
{
	DSLinkPtr   link     = NULL;
	DSLinkPtr   linkPrev = NULL;


    // TBD add assertions
    if( baseList->Offset != attachList->Offset )
        return;

    // if the list to attach is empty, don't do anything
    if( !attachList->Head && !attachList->Tail )
        return;

    link = (DSLinkPtr)((u32)attachList->Head + attachList->Offset);

    // if the list has a head
	if(baseList->Head)	
    {
		linkPrev = (DSLinkPtr)(((u32)baseList->Tail) + baseList->Offset);
		linkPrev->Next = attachList->Head;
		link->Prev = baseList->Tail;
		baseList->Tail = attachList->Tail;
	}				
    else // Else the base list is empty, so make it the same a the attach list
    {
	    baseList->Head = attachList->Head;
	    baseList->Tail = attachList->Tail;
    }
} 

/*---------------------------------------------------------------------------*
  Name:         DSNextListObj

  Description:  Returns the next object after obj in the list

  Arguments:    list		List that contains obj (need the offset)
                obj			Find the object after obj

  Returns:      Returns the next object after obj in the list.  Otherwise NULL
 *---------------------------------------------------------------------------*/
void*
DSNextListObj( DSListPtr list, Ptr obj )
{
	if( !list )
		return NULL;

	// If object is null, return the head of the list
	if( !obj )
	{
		return list->Head;
	}

	return ((DSLinkPtr)((u32)obj + list->Offset))->Next;
}