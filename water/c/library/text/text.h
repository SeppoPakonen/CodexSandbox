/*****************************************************************
 * Project		: GUI library
 *****************************************************************
 * File			: text.h
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 28 Dec 1996
 * Last Update  : 28 Dec 1996
 *****************************************************************
 * Description	:
 *                     
 *		This file contains all the stuff for text.
 *
 *****************************************************************
 * 28/12/96		: New Version
 *****************************************************************/

#ifndef THEYER_TEXT_H
#define THEYER_TEXT_H

/*
 * includes...
 */
#include <stdlib.h>
#include <string.h>

typedef	char*	Text;

#define txtLength(s)					(((s == '\0')||(s == NULL)) ? 0 : strlen(s))
//#define txtCopy(s,d)					(((s == '\0')||(s == NULL)) ? d[0] = '\0' : strcpy(d,s))
#define txtCompare(s1,s2)				strcmp(s1,s2)
#define txtCaseCompare(s1,s2)			stricmp(s1,s2)
#define txtLenCompare(s1,s2,len)		strncmp(s1,s2,len)
#define txtLenCaseCompare(s1,s2,len)	strnicmp(s1,s2,len)

#define textCompare(s1,s2)		strcmp(s1,s2)

#define txtMatch(s1,s2)			(txtCompare(s1,s2)==0)

#ifdef __cplusplus
extern "C" {
#endif

extern void txtCopy( Text source, Text destination );
extern Text txtToUpper( Text str );
extern Text txtToLower( Text str );
extern Text txtSwapChars( Text str, char change_this, char to_that );

#ifndef OsTypeIsPsx
/* text duplicate function not permitted on PSX */
extern Text txtDupl( Text str );
#endif

#ifdef __cplusplus
}
#endif

#endif
