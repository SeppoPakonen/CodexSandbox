/*>*******************************(*)*******************************<*/
//  Copyright (C) 1997, Nintendo Company, Ltd.

//  These coded instructions, statements, and computer programs contain
//  unpublished  proprietary information of Nintendo Company, Ltd., and
//  are protected by Federal copyright law.  They  may not be disclosed
//  to  third parties or copied or duplicated  in any form, in whole or
//  in part, without the prior written consent of Nintendo Company, Ltd.

/*>*******************************(*)*******************************<*/

#ifndef	STRING_H
#define STRING_H

/********************************/
#if 0
extern "C" {
#endif

/*>*******************************(*)*******************************<*/
u8		Strcat	( char *str1, char *str2, char *dst );
void	Strcpy	( char *dst, char *src );
s8		Strcmp	( char *str1, char *str2 );
u32		Strlen	( char *str );

/********************************/
#if 0
}
#endif

#endif