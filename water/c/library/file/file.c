/*****************************************************************
 * Project		: File Library
 *****************************************************************
 * File			: file.c
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 9 May 1998
 * Last Update	: 13 June 1998
 *****************************************************************
 * Description	: file library functions
 *****************************************************************/

/*
 * includes
 */

#include <stdio.h>
#include <text/text.h>
#include <memory/memory.h>
#include "file.h"
#include "file_int.h"

/*
 * macros
 */

/*
 * typedefs
 */

/*
 * prototypes
 */

/*
 * variables
 */


/* 
 * globals variables...
 */

FileData		filedata;
static	Bool	file_initialised = FALSE;

/*
 * functions
 */


/******************************************************************************
 * Function:
 * fileInit -- initialise file library
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool	fileInit( 
	void
	)
{
	int		i;

	/* init */
	if ( ! file_initialised ) {
		filedata.nopen = 0;
		for ( i=0; i<FILE_MAXOPEN; i++ )
			memClear( &filedata.openfile[i], sizeof(FileEntry) );
		file_initialised = TRUE;
	}

	return( TRUE );
}


/******************************************************************************
 * Function:
 * fileGetEntry -- get an entry for an open file
 * 
 * Description:
 *
 * Returns:
 * 
 */

FileEntry	*fileGetEntry( 
	File	file
	)
{
	if ( file >= 0 && file < FILE_MAXOPEN ) 
		return( &filedata.openfile[file] );

	return( NULL );
}


/******************************************************************************
 * Function:
 * fileOpen -- open a file
 * 
 * Description:
 *
 * Returns:
 * 
 */

File fileOpen(
	Text	filename,
	Text	mode
	)
{
	FILE	   *xfile;
	FileEntry   fentry;
	int		    i;

	/* validate */
	if ( filename == NULL || mode == NULL ) return( -1 );	

	/* library initialised? */
	if ( ! file_initialised ) 
		fileInit();

	/* don't go over limit */
	if ( filedata.nopen == FILE_MAXOPEN ) return( -1 );

	xfile = fopen( filename, mode );
	if ( xfile == NULL ) return( -1 );
	/* init entry */
	memClear( &fentry, sizeof(FileEntry) );
	fentry.file = xfile;

	/* save open file info */

	/* get free entry in open file list */
	i = 0;
	while( filedata.openfile[i].inuse && i < FILE_MAXOPEN )
		i++;

	/* this error should not occur */
	if ( i >= FILE_MAXOPEN ) exit(1);

	/* save info' */
	filedata.openfile[i] = fentry;
	filedata.openfile[i].mode  = filedata.mode;
	filedata.openfile[i].inuse = TRUE;
	filedata.nopen++;

	return( i );
}


/******************************************************************************
 * Function:
 * fileRead -- read data from a file
 * 
 * Description:
 *
 * Returns:
 * 
 */

int fileRead (
	File	 file,
	void	*buf,
	int		 size,
	int		 cnt 
	)
{
	FileEntry	*fentry;
	int			 nread;

	/* get file entry */
	fentry = fileGetEntry( file );
	if ( fentry == NULL ) return( 0 );

	nread = fread( buf, (size_t)size, (size_t)cnt, fentry->file );

	return( nread );
}


/******************************************************************************
 * Function:
 * fileWrite -- write to a file
 * 
 * Description:
 *
 * Returns:
 * 
 */

int fileWrite (
	File	 file,
	void	*buf,
	int		 size,
	int		 cnt 
	)
{
	FileEntry	*fentry;
	int			 nwrite;

	/* get file entry */
	fentry = fileGetEntry( file );
	if ( fentry == NULL ) return( 0 );

	nwrite = fwrite( buf, (size_t)size, (size_t)cnt, fentry->file );

	return( nwrite );
}


/******************************************************************************
 * Function:
 * fileEof -- check for end of file
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool fileEof (
	File	file
	)
{
	Bool		 ateof;
	FileEntry	*fentry;
	
	/* get file entry */
	fentry = fileGetEntry( file );
	if ( fentry == NULL ) return( 0 );

	ateof = feof( fentry->file );

	return( ateof );
}


/******************************************************************************
 * Function:
 * fileError -- check for error when accessing a file
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool fileError (
	File	file
	)
{
	Bool		 iserr;
	FileEntry	*fentry;
	
	/* get file entry */
	fentry = fileGetEntry( file );
	if ( fentry == NULL ) return( 0 );

	iserr = ferror( fentry->file );

	return( iserr );
}


/******************************************************************************
 * Function:
 * fileClose -- close an open file
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool fileClose (
	File	file
	)
{
	FileEntry	*fentry;
	
	/* get file entry */
	fentry = fileGetEntry( file );
	if ( fentry == NULL ) return( 0 );

	fclose( fentry->file );
	memClear( &filedata.openfile[file], sizeof(FileEntry) );
	filedata.nopen--;

	return( TRUE );
}


/******************************************************************************
 * Function:
 * fileGetLine -- get a line of text from and ASCII text file
 * 
 * Description:
 *
 * Returns:
 * 
 */

char * fileGetLine (
    char    *str,       /* point to string to return chars */
    int      size,		/* size of string */
    File     file		/* point to File object */
    )
{
    char		*rstr;              /* Result string            */
    int			 l;                 /* Length of string         */
	FileEntry	*fentry;
	
	/* get file entry */
	fentry = fileGetEntry( file );
	if ( fentry == NULL ) return( NULL );

	rstr = fgets ( str, size, fentry->file );
	/* replace newline with terminating character */
	if ( rstr != NULL ) {
		l = txtLength( rstr );
		if ( *(rstr+l-1) == '\n' )
		    *(rstr+l-1) = '\0';
	}

    return( rstr );
}

