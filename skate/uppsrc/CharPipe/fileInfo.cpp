/*>*******************************(*)*******************************<*/
//  Copyright (C) 1999, Nintendo Company, Ltd.

//  These coded instructions, statements, and computer programs contain
//  unpublished  proprietary information of Nintendo Company, Ltd., and
//  are protected by Federal copyright law.  They  may not be disclosed
//  to  third parties or copied or duplicated  in any form, in whole or
//  in part, without the prior written consent of Nintendo Company, Ltd.

/*>*******************************(*)*******************************<*/

#include <CharPipe/Filelib.h>
#include "FullPath.h"

/*>*******************************(*)*******************************<*/
char MACFileName[255];
char FilePath[255];
char FileName[255];
char FileType[255];

/*>*******************************(*)*******************************<*/
static void GetFileName ( void );
static void GetFilePath ( void );

/*>*******************************(*)*******************************<*/
u8 MACGetFile	( void )
{
	short size;
	Handle name;
	u32 i;
	StandardFileReply sfr;

	StandardGetFile(nil, -1, 0, &sfr);  
    	if (!sfr.sfGood) return 0;
	
	GetFullPath(sfr.sfFile.vRefNum,
				sfr.sfFile.parID,
				sfr.sfFile.name,
				&size,
				&name);
	
	for(i = 0; i < size; i++)
	{
		MACFileName[i] = ((char *)(*name))[i];
	}			
	
	MACFileName[size] = 0;
	
	GetFileName();
	GetFilePath();

	return 1;
}

/*>*******************************(*)*******************************<*/
static void GetFileName ( void )
{
	char *cursor, *lastColon, *lastPeriod, *dst, *typeDst = 0;;
	
	cursor = MACFileName;
	lastColon = MACFileName;
	dst = FileName;
	
	while(*cursor)
	{
		if(*cursor == ':')
		{
			lastColon = cursor;
			lastColon++;
		}

		if(*cursor == '.')
		{
			lastPeriod = cursor;
			lastPeriod++;
		}
		cursor ++;
	}

	while(*lastColon)
	{
		*dst = *lastColon;
		dst ++;
		lastColon++;
	}
	*dst = 0;

	dst = FileType;
	while(*lastPeriod)
	{
		*dst = *lastPeriod;
		dst ++;
		lastPeriod++;
	}
	*dst = 0;
}

/*>*******************************(*)*******************************<*/
static void GetFilePath ( void )
{
	char *cursor, *lastColon, *dst;
	
	cursor = MACFileName;
	lastColon = MACFileName;
	dst = FilePath;
	
	while(*cursor)
	{
		if(*cursor == ':')
		{
			while(lastColon != cursor)
			{
				*dst = *lastColon;
				dst++;
				lastColon++;
			}
		}
		cursor++;
	}
	*dst = ':';
	dst++;
	*dst = 0;

	cursor = FilePath;
	while(*cursor)
	{
		if(*cursor == ':')
		{
			*cursor = '/';
		}
		cursor ++;
	}
}