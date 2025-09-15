/*****************************************************************
 * Project		: GUI Library
 *****************************************************************
 * File			: linked.c
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 28 Dec 1996
 * Last Update	: 28 Dec 1996
 *****************************************************************
 * Description	:
 *
 *		This file contains the a set of routines for linked
 *		list creation and management.
 *
 *****************************************************************
 * 28/12/96		: New Version.
 *****************************************************************/

#include <stdio.h>

#include <linked/linked.h>
#include <memory/memory.h>


/*****************************************************************
 * lnkCreate - Create a linked list
 *****************************************************************
 * 
 *****************************************************************/
LnkLinkedList *lnkCreate( void )
{
	LnkLinkedList *list;

	list = (LnkLinkedList *)memAlloc( sizeof(LnkLinkedList) );

	if ( list == NULL ) {
		printf("lnkCreate(): memory allocation error\n");
		return( NULL );
	}

	list->prev = NULL;
	list->next = NULL;
	list->data = NULL;

	return( list );
}

/*****************************************************************
 * lnkDestroy - Destroy a linked list
 *****************************************************************
 * 
 *****************************************************************/
void 	lnkDestroy(
   	LnkLinkedList 	*list
	)
{
	LnkLinkedList	*entry, *next;

	entry = list;
	while( entry != NULL ) {
		next = entry->next;
		if ( entry->data != NULL ) memFree( entry->data );
		memFree( entry );
		entry = next;
	}
}
		
/*****************************************************************
 * lnkAdd - Add an entry to linked list
 *****************************************************************
 * 
 *****************************************************************/
void	lnkAddFunc(
	LnkLinkedList 	*list,
	void		  	*data
	)
{
	LnkLinkedList	*newbit;

	if ( list == NULL ) return;

    newbit = (LnkLinkedList *)memAlloc( sizeof(LnkLinkedList) );

	newbit->prev = list;
	newbit->next = list->next;
    list->next = newbit;
	newbit->data = data;		
}

/*****************************************************************
 * lnkDelete - Delete an entry from a linked list
 *****************************************************************
 * 
 *****************************************************************/
void	lnkDelete(
	LnkLinkedList	*list
	)
{
	LnkLinkedList  	*prev, *next;

	/* check for a valid list */
	if ( list == NULL ) return;

	/* bail if at start of list */    
	if ( list->data == NULL ) return;

	prev = list->prev;
	next = list->next;
	prev->next = next;
	if ( next != NULL ) 
		next->prev = prev;

	/* free memory */
	memFree( list->data );
	memFree( list );
}

/*****************************************************************
 * lnkAddToEnd - Add an entry to the end of a linked list
 *****************************************************************
 * 
 *****************************************************************/
void	lnkAddToEndFunc(
	LnkLinkedList 	*list,
	void		  	*data
	)
{
	LnkLinkedList	*lst;

	lst = list;
	while( lst->next != NULL )
		lst = lst->next;

	lnkAdd( lst, data );
}

/*****************************************************************
 * lnkNext - Return the next entry in a linked list
 *****************************************************************
 * 
 *****************************************************************/
LnkLinkedList	*lnkNext(
	LnkLinkedList	*list
	)
{
	if ( list == NULL ) return( NULL );

	return( list->next );
}
