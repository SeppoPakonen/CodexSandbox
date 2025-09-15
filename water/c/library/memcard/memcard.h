/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * File:		memcard.h
 * Author:		Mark Theyer
 * Created:		8 Feb 2000
 **********************************************************
 * Description:	PSX and PS2 Memory card interface
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 08-Feb-00	Theyer	Initial coding.
 * 22-Apr-01	Theyer	PlayStation2 port.
 * 22-Apr-01	Theyer	Added type MEMCARD_STATUS_BAD_TYPE
 * 22-Apr-01	Theyer	Added function memCardCreateDirectory()
 *
 **********************************************************/

#ifndef THEYER_MEMCARD_H
#define THEYER_MEMCARD_H

/*
 * includes
 */

#include <type/datatype.h>
#include <text/text.h>

/*
 * typedefs
 */

typedef enum {
	MEMCARD_STATUS_OK = 1,
	MEMCARD_STATUS_NO_CARD,
	MEMCARD_STATUS_BAD_CARD,
	MEMCARD_STATUS_NEW_CARD,
	MEMCARD_STATUS_NOT_FORMATTED,
	MEMCARD_STATUS_FILE_NOT_EXIST,
	MEMCARD_STATUS_FILE_EXISTS,
	MEMCARD_STATUS_NOT_ENOUGH_MEMORY,
	MEMCARD_STATUS_COMMS_ERROR,
	MEMCARD_STATUS_ERROR,
	MEMCARD_STATUS_BAD_TYPE
} MemCardStatus;

/*
 * macros
 */

/*
 * prototypes
 */

extern void				memCardInit( Bool use_tap );
extern void				memCardClose( void );
extern MemCardStatus	memCardAttached( int n );
extern MemCardStatus	memCardOk( int n );
extern MemCardStatus	memCardDeleteFile( int n, Text name );
extern MemCardStatus	memCardCreateDirectory( int n, Text name );
extern MemCardStatus	memCardFormat( int n );
extern Text				memCardListFile( int n, Text name, int nskip, int *size );
extern MemCardStatus	memCardSaveFile( int n, Text name, Byte *data, int size );
extern MemCardStatus	memCardLoadFile( int n, Text name, Byte *data, int size );

#endif //THEYER_MEMCARD_H

