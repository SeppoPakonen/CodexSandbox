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
 *		This file contains the set of data types for linked
 *		list creation and management.
 *
 *****************************************************************
 * 28/12/96		: New Version.
 *****************************************************************/

#ifndef THEYER_LINKED_H
#define THEYER_LINKED_H

typedef struct LnkLinkedListStruct LnkLinkedList;

struct LnkLinkedListStruct {
	void					*data;
	struct LnkLinkedListStruct		*prev;
	struct LnkLinkedListStruct		*next;
};

#ifdef __cplusplus
extern "C" {
#endif

LnkLinkedList  *lnkCreate( void );
void 		lnkDestroy( LnkLinkedList *list );
void		lnkAddFunc( LnkLinkedList *list, void *data );     
void		lnkAddToEndFunc( LnkLinkedList *list, void *data );
#define 	lnkAdd(list,data)		lnkAddFunc(list,(void *)data)
#define 	lnkAddToEnd(list,data)	lnkAddToEndFunc(list,(void *)data)
void		lnkDelete( LnkLinkedList *list );              
LnkLinkedList  *lnkNext( LnkLinkedList *list );

#ifdef __cplusplus
}
#endif

#endif
