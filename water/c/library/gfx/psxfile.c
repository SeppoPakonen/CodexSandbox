/**********************************************************
 * Copyright (C) 1998 Mark Theyer
 * All Rights Reserved
 **********************************************************
 * Project:	PlayStation interface
 **********************************************************
 * File:	psxfile.c
 * Author:	Mark Theyer
 * Created:	07 Dec 1999
 **********************************************************
 * Description:	PSX file interface
 **********************************************************
 * Notes:		You cannot use 'psx' as a variable name.
 **********************************************************
 * Functions:
 *	gfxFileInit()
 *				Initialise the file system.
 *	gfxFileClose()
 *				Shutdown the file system.
 *	gfxFileRoot()
 *				Set the root path for finding files.
 *	gfxFilePath()
 *				Set the path relative to root for finding files.
 *	gfxFileLocation()
 *				Set a file location for internal files.
 *	gfxFileLoad()
 *				Load a file into the file buffer.
 *	gfxFileSave()
 *				Save a file.
 *	gfxSaveScreen()
 *				Save the currently rendered display screen.
 *
 **********************************************************
 * Revision History:
 * 07-Dec-99	Theyer	Initial creation from gfxpsx.c
 **********************************************************/

/*
 * includes
 */

#include <type/datatype.h>
#include <gfx/gfx.h>
#include <gfx/gfxpsx.h>
#include <triangle/triangle.h>
#include <text/text.h>
#include <parse/parse.h>
#include <memory/memory.h>
#include <fsnap1_0.h>

/* action replay file server */
#ifdef GFX_USE_ARS
#define AR_DEBUG
#include <ar_lib.h>
#endif

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


extern PsxData ps;


/**********************************************************
 * Function:	gfxFileInit
 **********************************************************
 * Description: Initialise file services
 * Inputs:		code - error code
 * Notes:		nil
 * Returns:		nil
 **********************************************************/

void gfxFileInit (
	void
	)
{
	//int		i;
#ifdef GFX_USE_SN_FILESERVER
	/* initialise remote file services */
	printf( "Using SN File Server...\n" );
	PCinit();
	//if ( i ) {
	//	printf( "\nSN File Server connect failed (%d)\n", i );
	//	exit(1);
	//}
	printf( "\nGFX connected to SN File Server\n" );
#endif

#ifdef GFX_USE_ARS
	/* initialise remote file services */
	printf( "Using AR FileServer...\n" );
	cg_fsinit();
	printf( "GFX connected to ARS\n" );
#endif

#ifdef GFX_USE_CDROM
#ifdef PSX_FULL_DEV_KIT
	while( !CdInit() );
#endif
#endif

	/* init paks */
	ps.pak_loaded = FALSE;
}


/**********************************************************
 * Function:	gfxFileClose
 **********************************************************
 * Description: Close the file system
 * Inputs:		void
 * Notes:		Used for PSX shutdown
 * Returns:		void
 **********************************************************/

public void gfxFileClose (
    void
    )
{
#ifdef GFX_USE_ARS
	/* shutdown remote file services */
	//printf( "ARS shutdown...\n" );
	cg_shutdown();
	printf( "ARS disconnected\n" );
#endif
}


/**********************************************************
 * Function:	gfxFileRoot
 **********************************************************
 * Description: Set the root path for loading data files
 * Inputs:		rootpath - pathname to use for files
 * Notes:	
 * Returns:	
 **********************************************************/

void	gfxFileRoot (
	Text	rootpath
	)
{
	int		len;

	/* save path */
	txtCopy( rootpath, ps.fileroot );

	/* add trailing backslash if not there */
	len = txtLength( ps.fileroot );
	if ( ps.fileroot[len-1] != '\\' ) {
		ps.fileroot[len] = '\\';
		ps.fileroot[len+1] = '\0';
	}
}


/**********************************************************
 * Function:	gfxFilePath
 **********************************************************
 * Description: Set the path for loading subsequent files
 * Inputs:	pathename - pathname to use for files
 * Notes:	
 * Returns:	
 **********************************************************/

void	gfxFilePath (
	Text	pathname
	)
{
	int		len;

	if ( pathname == NULL || pathname[0] == '\0' ) {
		ps.filepath[0] = '\0';
	} else {
		/* save path */
		txtCopy( pathname, ps.filepath );

		/* add trailing backslash if not there */
		len = txtLength( ps.filepath );
		if ( len > 0 ) {
			if ( ps.filepath[(len-1)] != '\\' ) {
				ps.filepath[len] = '\\';
				ps.filepath[(len+1)] = '\0';
			}
		}
	}
}


/**********************************************************
 * Function:	gfxFileLocation
 **********************************************************
 * Description: Load a file into the storage buffer
 * Inputs:		filename - name of file to load
 * Notes:	
 * Returns:		buffer location
 **********************************************************/

public void gfxFileLocation (
	Text	 filename,
	Byte	*location
	)
{
	char	 name[64];
	int		 len, i;

#if GFX_VALIDATE
	if ( ps.nregfiles >= PSX_MAX_INTERNAL_FILE_REGISTER ) {
		gfxError( GFX_ERROR_FILE_REGISTER_FULL );
		gfxAbort();
	}
#endif

	/* save path and filename to file register for accurate location */
	txtCopy( ps.filepath, name );
	len = txtLength( ps.filepath );
	txtCopy( filename, &name[len] );
	/* convert case */
	txtToLower( name );

	/* already got it? */
	for ( i=0; i<ps.nregfiles; i++ ) {
		if ( txtMatch( name, ps.fileregister[i].filename ) ) 
			return;
	}

	/* save location file to register */
	txtCopy( name, ps.fileregister[ps.nregfiles].filename );
	ps.fileregister[ps.nregfiles].location = location;
	ps.nregfiles++;
}


/**********************************************************
 * Function:	gfxFileExists
 **********************************************************
 * Description: Check for existence of a file
 * Inputs:		filename - name of file to check
 * Notes:	
 * Returns:		TRUE if file exists, FALSE if not
 **********************************************************/

public Bool gfxFileExists (
	Text	filename
	)
{
	char	fullname[1024];
	long	handle;
	long	nread;
	int		len;
	long	maxbuff;
	int		i;
	CdlFILE	cdfile;

	/* check file register first */	
	txtCopy( ps.filepath, fullname );
	len = txtLength( ps.filepath );
	txtCopy( filename, &fullname[len] );
	/* convert case */
	txtToLower( fullname );
	for ( i=0; i<ps.nregfiles; i++ ) {
		//printf( "%s==%s\n", fullname, ps.fileregister[i].filename );
		if ( txtMatch( fullname, ps.fileregister[i].filename ) ) {
#ifdef PSX_FILEREGISTER_TESTMODE
			goto loop_end;
#else
			//printf( "file %s is internal\n", fullname );
			return( TRUE );
#endif
		}
	}
loop_end:

#ifdef PSX_FILEREGISTER_TESTMODE
	/* complain if the file is not registered */
	if ( i == ps.nregfiles )
		printf( "file not registered: %s\n", fullname );
#endif

	/* init full pathname of file (<root>/<path>/<filename>) */
	txtCopy( ps.fileroot, fullname );
	len = txtLength( ps.fileroot );
	txtCopy( ps.filepath, &fullname[len] );
	len += txtLength( ps.filepath );
	txtCopy( filename, &fullname[len] );

#ifdef GFX_USE_ARS
	handle = cg_fopen( fullname, ARS_READ );
	if ( handle == -1 ) 
		return( FALSE );
	cg_fclose( handle );
	return( TRUE );
#endif

#ifdef GFX_USE_SN_FILESERVER
	handle = PCopen( fullname, 0, 0 );
	if ( handle == -1 ) 
		return( FALSE );
	PCclose( handle );
	return( TRUE );
#endif

#ifdef GFX_USE_CDROM
	/* add ";1" to full pathname */
	len += txtLength( filename );
	txtCopy( ";1", &fullname[len] );
	if ( CdSearchFile( &cdfile, fullname ) == 0 )
		return( FALSE );
	return( TRUE );
#endif

	/* no file support to check this file */
	gfxError( GFX_ERROR_NO_FILE_SUPPORT );
	return( FALSE );
}


/**********************************************************
 * Function:	gfxFileLoad
 **********************************************************
 * Description: Load a file into the storage buffer
 * Inputs:		filename - name of file to load
 * Notes:	
 * Returns:		buffer location
 **********************************************************/

public Byte *gfxFileLoad (
	Text	filename
	)
{
	char	fullname[1024];
	long	handle;
	long	nread;
	int		len;
	long	maxbuff;
	int		i, j;
	Text	pakname;
	CdlFILE	cdfile;

#if GFX_VALIDATE
	/* validate file name length */
	if ( txtLength(filename) > 12 )
		printf( "file name too long? %s\n", filename );
#endif

	/* check file register first */	
	txtCopy( ps.filepath, fullname );
	len = txtLength( ps.filepath );
	txtCopy( filename, &fullname[len] );
	/* convert case */
	txtToLower( fullname );
	for ( i=0; i<ps.nregfiles; i++ ) {
		//printf( "%s==%s\n", fullname, ps.fileregister[i].filename );
		if ( txtMatch( fullname, ps.fileregister[i].filename ) ) {
#ifdef PSX_FILEREGISTER_TESTMODE
			goto loop_end;
#else
			//printf( "file %s is internal\n", fullname );
			return( ps.fileregister[i].location );
#endif
		}
	}
loop_end:

#ifdef PSX_FILEREGISTER_TESTMODE
	/* complain if the file is not registered */
	if ( i == ps.nregfiles )
		printf( "file not registered: %s\n", fullname );
#endif

#if 1
	/* check package if loaded */
	if ( ps.pak_loaded ) {
		/* validate header magic number (first 4 bytes) */
		if ( txtMatch( "PAK", &ps.filebuffer[0] ) ) {
			/* get number of files in this pak (next 4 bytes) */
			j = *((int *)(&ps.filebuffer[4]));
			if ( j > 0 ) {
				pakname = (char *)(&ps.filebuffer[8]);
				/* check for file we want */
				for ( i=0; i<j; i++ ) {
					if ( txtMatch( filename, pakname ) ) {
						/* got it */
						//printf( "got file from pak: %s\n", filename );
						pakname += 36;
						j = binReadInt( pakname );
						//printf( "offset: %d\n", j );
						//printf( "file: %s\n", (char *)&ps.filebuffer[j] );
						return( &ps.filebuffer[j] );
					}
					pakname += 40;
				}
			}
		} else {
			/* no pak */
			ps.pak_loaded = FALSE;
		}
	}
#endif

	/* init full pathname of file (<root>/<path>/<filename>) */
	txtCopy( ps.fileroot, fullname );
	len = txtLength( ps.fileroot );
	txtCopy( ps.filepath, &fullname[len] );
	len += txtLength( ps.filepath );
	txtCopy( filename, &fullname[len] );

	/* is this a pak file? */
	len = (txtLength( filename ) - 3);
	if ( len > 0 ) {
		if ( txtMatch( "pak", &filename[len] ) )
			ps.pak_loaded = TRUE;
	}

	/* determine the maximum file size we can load */
	maxbuff = (PSX_FILEBUFFER_SIZE + (PSX_GPU_WORK_AREA_SIZE<<1)-1);

#ifdef GFX_USE_ARS
	/* open file via ARS */
	handle = cg_fopen( fullname, ARS_READ );
	if ( handle == -1 ) {
		/* cannot load file, fatal error */
		gfxError( GFX_ERROR_FILE_LOAD_FAILED );
		gfxAbort();
	}
	/* read file */
	nread = cg_fread( handle, (char *)ps.filebuffer, maxbuff );
	/* close file */
	cg_fclose( handle );
	/* handle errors */
	if ( nread == maxbuff ) {
		/* file too big, fatal error */
		gfxError( GFX_ERROR_FILE_TOO_BIG );		
		gfxAbort();
	//} else if ( nread > PSX_FILEBUFFER_SIZE ) {
	//	/* this is OK... */
	//	; // gfxError( GFX_ERROR_FILE_USES_PACKET_BUFFER );
	} else {
		/* success, add terminator to indicate EOF */
		//printf( "loaded file: %s\n", filename );
		ps.filebuffer[nread+1] = '\0';
	}
	/* ok */
	return( ps.filebuffer );
#endif

#ifdef GFX_USE_SN_FILESERVER
	handle = PCopen( fullname, 0, 0 );
	if ( handle == -1 ) {
		printf( "file load failed: %s\n", fullname );
		/* cannot load file, fatal error */
		gfxError( GFX_ERROR_FILE_LOAD_FAILED );
		gfxAbort();
	}
#if 0
	/* get file size */
	i = PClseek( handle, 0, 2 );
	printf( "file size=%d\n", i );
	/* go back to start */
	PClseek( handle, 0, 0 );
	/* read file */
	nread = 0;
	j = 0;
	while( nread < i && j < 4 ) {
		nread += PCread( handle, (char *)&ps.filebuffer[nread], maxbuff );
		printf( "nread=%d\n", nread );
		j++;
	}
#endif
	/* read file */
	nread = PCread( handle, (char *)ps.filebuffer, maxbuff );
	/* close file */
	PCclose( handle );
	/* handle errors */
	if ( nread == maxbuff ) {
		/* file too big, fatal error */
		gfxError( GFX_ERROR_FILE_TOO_BIG );
		gfxAbort();
	//} else if ( nread > PSX_FILEBUFFER_SIZE ) {
	//	/* this is OK... */
	//	; // gfxError( GFX_ERROR_FILE_USES_PACKET_BUFFER );
	} else {
		/* success, add terminator to indicate EOF */
		//printf( "loaded file: %s\n", filename );
		ps.filebuffer[nread+1] = '\0';
	}
	/* ok */
	return( ps.filebuffer );
#endif

#ifdef GFX_USE_CDROM
	/* add ";1" to full pathname */
	len = txtLength( fullname );
	txtCopy( ";1", &fullname[len] );
	/* convert to upper case */
	txtToUpper( fullname );
	/* locate file */
	if ( CdSearchFile( &cdfile, fullname ) == 0 ) {
		/* cannot load file, fatal error */
		//printf( "file load failed: %s\n", fullname );
		gfxError( GFX_ERROR_FILE_LOAD_FAILED );
		gfxAbort();
	}
	nread = CdReadFile( fullname, (u_long *)ps.filebuffer, 0 );
	if ( CdReadSync(0,(u_char *)&i) != 0 ) {
		/* cannot load file, fatal error */
		gfxError( GFX_ERROR_FILE_LOAD_FAILED );
		gfxAbort();
	}
	if ( nread >= maxbuff ) {
		/* file too big, fatal error */
		gfxError( GFX_ERROR_FILE_TOO_BIG );	
		gfxAbort();
	//} else if ( nread > PSX_FILEBUFFER_SIZE ) {
	//	/* this is OK... */
	//	; // gfxError( GFX_ERROR_FILE_USES_PACKET_BUFFER );
	} else {
		/* success, add terminator to indicate EOF */
		ps.filebuffer[nread+1] = '\0';
	}
	/* ok */
	return( ps.filebuffer );
#endif

	/* no file support to load this file */
	//printf( "GFX: No file support (%s)\n", fullname );
	gfxError( GFX_ERROR_NO_FILE_SUPPORT );
	gfxAbort();
	return( NULL );
}


/**********************************************************
 * Function:	gfxFileSave
 **********************************************************
 * Description: Save a file
 * Inputs:		filename - name of file to save
 * Notes:	
 * Returns:		
 **********************************************************/

public void gfxFileSave (
	Text	 filename,
	Byte	*data,
	int		 size
	)
{
	char	fullname[1024];
	long	handle;
	long	nread;
	int		len;
	int		i;

	/* init full pathname of file (<root>/<path>/<filename>) */
	txtCopy( ps.fileroot, fullname );
	len = txtLength( ps.fileroot );
	txtCopy( ps.filepath, &fullname[len] );
	len += txtLength( ps.filepath );
	txtCopy( filename, &fullname[len] );

#ifdef GFX_USE_ARS
	/* open file */
	handle = cg_fopen( fullname, ARS_WRITE );
	/* handle error */
	if ( handle == -1 ) {
		printf( "GFX: failed to open file: %s\n", fullname );
		gfxError( GFX_ERROR_FILE_OPEN_FAILED );
		gfxAbort();
	}
	/* write file */
	cg_fwrite( handle, data, size );
	/* close file */
	cg_fclose( handle );
#endif

#ifdef GFX_USE_SN_FILESERVER
	/* open file */
	handle = PCopen( fullname, (int)1, (int)1 );
	/* file does not exist will cause this... */
	if ( handle == -1 ) {
		/* create the file */
		handle = PCcreat( fullname, 0 );
		/* handle error */
		if ( handle == -1 ) {
			printf( "GFX: failed to open file: %s\n", fullname );
			gfxError( GFX_ERROR_FILE_OPEN_FAILED );
			gfxAbort();
		}
	}
	/* write file */
	PCwrite( handle, data, size );
	/* close file */
	PCclose( handle );
#endif
}


/**********************************************************
 * Function:	gfxSaveScreen
 **********************************************************
 * Description: Save the current screen image
 * Inputs:		name - name of screen file name
 * Notes:		
 * Returns:		
 **********************************************************/

public void gfxSaveScreen (
    Text	name,
	int		buffer
    )
{
	int		y, ysize;

	/* init */
	if ( buffer )
		y = ps.ymax;
	else
		y = 0;

	/* wait for end of drawing (as we may trash the gpu packet area) */
    DrawSync(0);

	/* save screen to file buffer storage area */
	FSnapshotFRAMEBUFFERp( (unsigned long *)ps.filebuffer, 0, y, 512, ps.ymax );
	gfxFileSave( name, ps.filebuffer, FSnapshotBytes(512,ps.ymax) );
}


