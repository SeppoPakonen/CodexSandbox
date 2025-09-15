/*>*******************************(*)*******************************<*/
//  Copyright (C) 1997, Nintendo Company, Ltd.

//  These coded instructions, statements, and computer programs contain
//  unpublished  proprietary information of Nintendo Company, Ltd., and
//  are protected by Federal copyright law.  They  may not be disclosed
//  to  third parties or copied or duplicated  in any form, in whole or
//  in part, without the prior written consent of Nintendo Company, Ltd.

/*>*******************************(*)*******************************<*/

#include <CharPipe/structures.h>

/*>*******************************(*)*******************************<*/
/*>-----------------------------------------------------------------<*/
//	Appends str2 to the end of str1 and places the result in dst.
//	dst should be allocated before this function is called to be 
//	Strlen(str1) + Strlen(str2) + 1.
/*>-----------------------------------------------------------------<*/
u8	Strcat			( char *str1, char *str2, char *dst )
{
	char *srcCursor = str1;
	char *dstCursor = dst;

	if(!dst) return FALSE;
	if(!str1) return FALSE;
	if(!str2) return FALSE;

	while(*srcCursor)
	{
		*dstCursor = *srcCursor;
		dstCursor ++;
		srcCursor ++;
	}

	srcCursor = str2;

	while(*srcCursor)
	{
		*dstCursor = *srcCursor;
		dstCursor ++;
		srcCursor ++;
	}

	*dstCursor = 0;

	return TRUE;
}

/*>*******************************(*)*******************************<*/
/*>-----------------------------------------------------------------<*/
//	Copies a string from src into dst.  dst must already be allocated
//	to be a valid size.
/*>-----------------------------------------------------------------<*/
void Strcpy ( char *dst, char *src )
{
	do
	{
		*dst = *src;
		dst ++;
		src ++;
	}while(*src);
}

/*>*******************************(*)*******************************<*/
/*>-----------------------------------------------------------------<*/
//	Compares 2 strings
//		returns 1 if str1 is less than str2
//		returns -1 if str1 is greater than str2
//		returns 0 if str1 and str2 are equal
/*>-----------------------------------------------------------------<*/
s8	Strcmp	( char *str1, char *str2 )
{
	char *cursor1 = str1;
	char *cursor2 = str2;

	do
	{
		if(*cursor1 < *cursor2) return 1;
		if(*cursor1 > *cursor2) return -1;
		cursor1++;				// INCREMENT CURSOR1
		cursor2++;				// INCREMENT CURSOR2

	}while(*cursor1 && *cursor2);	// WHILE *CURSOR AND CURSOR ARE NOT NULL

	return 0;
}

/*>*******************************(*)*******************************<*/
/*>-----------------------------------------------------------------<*/
//	Returns the length of the specified string.
/*>-----------------------------------------------------------------<*/
u32	Strlen	( char *str )
{
	char *cursor = str;
	u32 counter = 0;

	if(!str) return 0;

	while(*cursor)	// WHILE *CURSOR IS NOT NULL
	{
		cursor++;	// INCREMENT CURSOR
		counter++;	// INCREMENT COUNTER
	}
	return counter;	// RETURN COUNTER
}