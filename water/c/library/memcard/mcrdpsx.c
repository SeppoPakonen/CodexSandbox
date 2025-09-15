/**********************************************************
 * Copyright (C) 1999, 2000 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:	PSX Memory card interface
 **********************************************************
 * File:	mcrdpsx.c
 * Author:	Mark Theyer
 * Created:	8 Feb 2000
 **********************************************************
 * Description:	PSX Memory card interface
 **********************************************************
 * Functions:
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 08-Feb-00	Theyer	Initial coding.
 *
 **********************************************************/

/*
 * includes
 */

#include <type/datatype.h>
#include <compile.h>
#include <memory/memory.h>
#include <gfx/gfx.h>

#include <memcard/memcard.h>
#include <shiftjis/shiftjis.h>
#include "mcrdpsx.h"

#ifdef OsTypeIsPsx
#include <libapi.h>
#include <libetc.h>
#include <libmcrd.h>
#endif

/*
 * typedefs
 */

/*
 * macros
 */

#ifdef DISABLE_MEMCARD
#undef PSX_FULL_DEV_KIT
#endif

/*
 * prototypes
 */

#ifndef PSX_FULL_DEV_KIT
private int		test_status = MEMCARD_STATUS_OK;
#define RETURN_STATUS { \
	test_status++; \
	if ( test_status > 10 ) \
		test_status = MEMCARD_STATUS_OK; \
	return( test_status ); \
	}
#endif

private long			memCardGetPortId( int n );
private MemCardStatus	memCardGetStatus( long result );

/*
 * global variables
 */

/*
 * functions
 */          


/**********************************************************
 * Function:	memCardInit
 **********************************************************
 * Description: initialise memory card system
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void memCardInit (
	Bool	use_tap
	)
{
#ifdef PSX_FULL_DEV_KIT
	// init memory card
	ResetGraph(0);
	if ( use_tap )
		MemCardInit(0);	// do NOT use control code in ROM, (should be 0 if using PSX multi-tap or gun)
	else
		MemCardInit(1);	// use control code in ROM

	// start event handling...
	MemCardStart();
#endif
}


/**********************************************************
 * Function:	memCardClose
 **********************************************************
 * Description: shutdown memory card system
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void memCardClose (
	void
	)
{
#ifdef PSX_FULL_DEV_KIT
	// stop event handling...
	MemCardStop();
#endif
}


/**********************************************************
 * Function:	memCardAttached
 **********************************************************
 * Description: is a memory card attached?
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

MemCardStatus memCardAttached (
	int		n
	)
{
#ifdef PSX_FULL_DEV_KIT
	long	cmds;
	long	result;
	long	portid;

	/* init */
	cmds = 0;
	result = 0;

	/* get port id */
	portid = memCardGetPortId( n );
	if ( portid == PSX_MCARD_INVALID_PORT )
		return( MEMCARD_STATUS_ERROR );

	// does card exist?
	MemCardExist( portid );
	
	// wait for async op' to complete...
	MemCardSync( 0, &cmds, &result );

	return( memCardGetStatus( result ) );
#else
	RETURN_STATUS;
#endif
}


/**********************************************************
 * Function:	memCardOk
 **********************************************************
 * Description: is memory card ok?
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

MemCardStatus memCardOk (
	int		n
	)
{
#ifdef PSX_FULL_DEV_KIT
	long	cmds;
	long	result;
	long	portid;

	/* init */
	cmds = 0;
	result = 0;

	/* get port id */
	portid = memCardGetPortId( n );
	if ( portid == PSX_MCARD_INVALID_PORT )
		return( MEMCARD_STATUS_ERROR );

	// does card exist?
	MemCardAccept( portid );
	
	// wait for async op' to complete...
	MemCardSync( 0, &cmds, &result );

	return( memCardGetStatus( result ) );
#else
	RETURN_STATUS;
#endif
}


/**********************************************************
 * Function:	memCardFormat
 **********************************************************
 * Description: format memory card
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

MemCardStatus memCardFormat (
	int		n
	)
{
#ifdef PSX_FULL_DEV_KIT
	int				result;
	long			portid;
	MemCardStatus	status;

	/* get port id */
	portid = memCardGetPortId( n );
	if ( portid == PSX_MCARD_INVALID_PORT )
		return( MEMCARD_STATUS_ERROR );

	// does card exist?
	result = MemCardFormat( portid );

	// (sync op' returned result)
	status = memCardGetStatus( result );

	// bad card indicates comms error 
	if ( status == MEMCARD_STATUS_BAD_CARD )
		status = MEMCARD_STATUS_COMMS_ERROR;

	return( status );
#else
	RETURN_STATUS;
#endif
}


/**********************************************************
 * Function:	memCardListFile
 **********************************************************
 * Description: get existing file name from memory card
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

Text memCardListFile (
	int		 n,
	Text	 name,
	int		 nskip,
	int		*size
	)
{
#ifdef PSX_FULL_DEV_KIT
	int				result;
	long			portid;
	long			nfiles;
	struct DIRENTRY	direntry;
	static char		filename[24];

	/* init */
	nfiles = 0;
	*size  = 0;

	/* validate nskip */
	if ( nskip <  0 ) nskip =  0;
	if ( nskip > 14 ) nskip = 14;

	/* get port id */
	portid = memCardGetPortId( n );
	if ( portid == PSX_MCARD_INVALID_PORT )
		return( NULL );

	/* get entry */
	result = MemCardGetDirentry( portid, name, &direntry, &nfiles, (long)nskip, 1 );

	/* any files matching or error? */
	if ( nfiles == 0 || result != McErrNone )
		return( NULL );

	// copy name to static filename buffer and NULL terminate
	memCopy( direntry.name, filename, 20 );
	filename[21] = '\0';
	*size = (int)direntry.size;

	return( filename );
#else
	return( NULL );
#endif
}


/**********************************************************
 * Function:	memCardSaveFile
 **********************************************************
 * Description: save a file to the memory card
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

MemCardStatus memCardSaveFile (
	int		 n,
	Text	 name,
	Byte	*data,
	int		 size
	)
{
#ifdef PSX_FULL_DEV_KIT
	long			cmds;
	long			result;
	MemCardStatus	status;
	long			portid;
	long			nblocks;	// number of 8192 byte blocks required for file
	long			nslots;		// number of 128 bytes size units to write

	/* init */
	result = 0;

	/* init */
	portid = memCardGetPortId( n );
	if ( portid == PSX_MCARD_INVALID_PORT )
		return( MEMCARD_STATUS_ERROR );

	/* get number of blocks */
	nblocks = ((size/8192)+(((size % 8192) > 0)?(1):(0)));

	/* create file first */
	result = MemCardCreateFile( portid, name, nblocks );

	/* get status */
	status = memCardGetStatus( result );
	if ( status != MEMCARD_STATUS_OK && status != MEMCARD_STATUS_FILE_EXISTS ) {
		// bad card indicates comms error 
		if ( status == MEMCARD_STATUS_BAD_CARD )
			status = MEMCARD_STATUS_COMMS_ERROR;
		return( status );
	}

	/* number of 128 byte units to write? */
	nslots = ((size/128)+(((size % 128) > 0)?(1):(0)));

	/* write file */
	result = MemCardWriteFile( portid, name, (unsigned long *)data, 0, (nslots<<7) );
	
	// wait for async op' to complete...
	MemCardSync( 0, &cmds, &result );

	/* get status */
	status = memCardGetStatus( result );

	// bad card indicates comms error 
	if ( status == MEMCARD_STATUS_BAD_CARD )
		status = MEMCARD_STATUS_COMMS_ERROR;
	
	return( status );
#else
	RETURN_STATUS;
#endif
}


/**********************************************************
 * Function:	memCardLoadFile
 **********************************************************
 * Description: load a file from the memory card
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

MemCardStatus memCardLoadFile (
	int		 n,
	Text	 name,
	Byte	*data,
	int		 size
	)
{
#ifdef PSX_FULL_DEV_KIT
	long			cmds;
	long			result;
	MemCardStatus	status;
	long			portid;
	long			nblocks;	// number of 8192 byte blocks required for file
	long			nslots;		// number of 128 bytes size units to write

	/* init */
	result = 0;

	/* init */
	portid = memCardGetPortId( n );
	if ( portid == PSX_MCARD_INVALID_PORT )
		return( MEMCARD_STATUS_ERROR );

	/* number of 128 byte units to write? */
	nslots = ((size/128)+(((size % 128) > 0)?(1):(0)));

	/* write file */
	result = MemCardReadFile( portid, name, (unsigned long *)data, 0, (nslots<<7) );

	// wait for async op' to complete...
	MemCardSync( 0, &cmds, &result );

	/* get status */
	status = memCardGetStatus( result );

	// bad card indicates comms error 
	if ( status == MEMCARD_STATUS_BAD_CARD )
		status = MEMCARD_STATUS_COMMS_ERROR;
	
	return( status );
#else
	RETURN_STATUS;
#endif
}


/**********************************************************
 * Function:	memCardDeleteFile
 **********************************************************
 * Description: is memory card ok?
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

MemCardStatus memCardDeleteFile (
	int		n,
	Text	name
	)
{
#ifdef PSX_FULL_DEV_KIT
	long	cmds;
	long	result;
	long	portid;

	/* init */
	cmds = 0;
	result = 0;

	/* get port id */
	portid = memCardGetPortId( n );
	if ( portid == PSX_MCARD_INVALID_PORT )
		return( MEMCARD_STATUS_ERROR );

	// delete file from card...
	result = MemCardDeleteFile( portid, name );
	
	return( memCardGetStatus( result ) );
#else
	RETURN_STATUS;
#endif
}


/**********************************************************
 * Function:	memCardGetPortId
 **********************************************************
 * Description: return memory card port id
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

private long memCardGetPortId ( 
	int		n 
	)
{
	long	portid;

	switch(n) {
	case 0:
		// card 0 in port a
		portid = PSX_MCARD_PORT0;
		break;
	case 1:
		// card 0 in port b
		portid = PSX_MCARD_PORT1;
		break;
	default:
		portid = PSX_MCARD_INVALID_PORT;
	}

	return( portid );
}


/**********************************************************
 * Function:	memCardGetStatus
 **********************************************************
 * Description: return status for a memory card op'
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

private MemCardStatus memCardGetStatus ( 
	long	result 
	)
{
#ifdef PSX_FULL_DEV_KIT
	switch( result ) {
	case McErrNone:
		// 0x00 McErrNone Connected
		return( MEMCARD_STATUS_OK );
	case McErrCardNotExist:
		// 0x01 McErrCardNotExist Not connected
		return( MEMCARD_STATUS_NO_CARD );
	case McErrCardInvalid:
		// 0x02 McErrCardInvalid Bad card
		return( MEMCARD_STATUS_BAD_CARD );
	case McErrNewCard:
		// 0x03 McErrNewCard New card (card was replaced)
		return( MEMCARD_STATUS_NEW_CARD );
	case McErrNotFormat:
		// 0x04 McErrNotFormat Not formatted
		return( MEMCARD_STATUS_NOT_FORMATTED );
	case McErrFileNotExist:
		// 0x05 McErrFileNotExist File not found
		return( MEMCARD_STATUS_FILE_NOT_EXIST );
	case McErrAlreadyExist:
		// 0x06 McErrAlreadyExist File already exists
		return( MEMCARD_STATUS_FILE_EXISTS );
	case McErrBlockFull:
		// 0x07 McErrBlockFull Not enough available blocks
		return( MEMCARD_STATUS_NOT_ENOUGH_MEMORY );
	case -1:
	default:
		return( MEMCARD_STATUS_ERROR );
	}
	return( MEMCARD_STATUS_ERROR );
#else
	return( MEMCARD_STATUS_NO_CARD );
#endif
}


#if 0
/**********************************************************
 * Function:	surfMemCardSave
 **********************************************************
 * Description: Save data to memory card
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void surfMemCardSave (
	Text	 name,
	Byte	*data,
	int		 size
	)
{
	PsxMemCardHeaderType1	mcard_header;
	int						nslots;

	/* calc number of 128 byte slots required for save */
	nslots = (size / 128);
	if ( size % 128 )
		nslots++;

	/* init header */
	memClear( &mcard_header, sizeof(PsxMemCardHeaderType1) );
	mcard_header.magic[0] = 'S';
	mcard_header.magic[1] = 'C';
	mcard_header.type = PSX_MCARD_ICON_IMAGES_1;
	mcard_header.nslots = nslots;
	surfMemCardNameConvert( name, mcard_header.name );
	/* clut, image to do... */
}


/**********************************************************
 * Function:	surfMemCardNameConvert
 **********************************************************
 * Description: Convert name to memory card name format
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void surfMemCardNameConvert (
	Text	name,
	Byte	mname
	)
{
	mname[0] = 'H';
	mname[1] = 'I';
}
#endif

