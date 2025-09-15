/*****************************************************************
 * Project		: File Library
 *****************************************************************
 * File			: file_int.h
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 28 Dec 1996
 * Last Update	: 28 Dec 1996
 *****************************************************************
 * Description	:
 *****************************************************************
 * 13/06/98		: Initial Coding
 *****************************************************************/

#ifndef THEYER_FILE_INT_H
#define THEYER_FILE_INT_H

#include <type/datatype.h>
#include "file.h"

#define	FILE_MAXOPEN	5

typedef struct {
	Bool	 inuse;
	int		 mode;			// unused
	FILE	*file;
	void	*buffer;
	int		 size;
	int		 i;
} FileEntry;

typedef struct {
	int			mode;		// unused
	int			nopen;
	FileEntry	openfile[FILE_MAXOPEN];
} FileData;

extern FileEntry *fileGetEntry( File file );

#endif
