/*>*******************************(*)*******************************<*/
//  Copyright (C) 1999, Nintendo Company, Ltd.

//  These coded instructions, statements, and computer programs contain
//  unpublished  proprietary information of Nintendo Company, Ltd., and
//  are protected by Federal copyright law.  They  may not be disclosed
//  to  third parties or copied or duplicated  in any form, in whole or
//  in part, without the prior written consent of Nintendo Company, Ltd.

/*>*******************************(*)*******************************<*/

#ifndef FILELIB_H
#define FILELIB_H

/********************************/
#if 0
extern "C" {
#endif

/********************************/

#include <Dolphin/os.h>
#ifdef flagWIN32
	#include <win32/win32.h>
#endif
#ifdef MACOS
#include <StandardFile.h>
#endif

/********************************/
#ifdef MACOS
extern char MACFileName[255];
#endif
extern char FilePath[255];
extern char FileName[255];
extern char FileType[255];

/*>*******************************(*)*******************************<*/
#ifdef MACOS
u8 MACGetFile	( void );
#endif
#ifdef flagWIN32
u8 WINGetFile	(char *);
#endif

/********************************/
#if 0
}
#endif

#endif