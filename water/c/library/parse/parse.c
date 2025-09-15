
/*
 * Description:
 * 
 */

/*
 * includes
 */

#include <stdio.h>
#include <stdlib.h>
#include <type/datatype.h>
#include <text/text.h>
#include <memory/memory.h>
//#ifndef PSE_NO_FILE_SUPPORT
#include <file/file.h>	
//#endif
#include "parse.h"
#include "pse_int.h"

/*
 * macros
 */
						 
/*
 * typedefs
 */

/*
 * prototypes
 */

private Bool pseGetArgs( void );
private int	 pseArgType( Text arg );
private Text pseGetWord( int w );
private Bool pseGetLine( void );
    
/*
 * variables
 */

PseInfo		*pse = NULL;

/*
 * functions
 */

/******************************************************************************
 * Function:
 * pseParseFile -- parse an input file
 * 
 * Description:
 *
 * Returns:
 * 
 */

public Bool pseParseFileFunc (
    Text     filename,
	char	*ichars,
	int		 nchars,
	void	*(callback)(),
	void	*user_data
    )
{
	PseInfo  pse_info;

	/* validate */
	if ( filename == NULL || callback == NULL ) 
		return( FALSE );
	if ( ichars == NULL && nchars > 0 ) 
		return( FALSE );
	  
	/* init */
	pse = &pse_info;

	/* open file */   
#ifdef PSE_NO_FILE_SUPPORT
    pse->file = -1; 
#else
	pse->file = fileOpen( filename, "r" );
#endif
    if ( pse->file == -1 ) {
		pse = NULL;
		return( FALSE );
	}

    /* init */
	pse->filename = filename;
	pse->data   = NULL;
	pse->dcnt   = 0;
    pse->max    = PSE_MAXLINE;
	pse->nargs  = 0;
	pse->ichars = ichars;
	pse->nchars = nchars;
	pse->lineno = 0;
	pse->abort  = FALSE;

	while( ! pse->abort && pseGetLine() )
		callback( user_data );
    
#ifndef PSE_NO_FILE_SUPPORT
    fileClose( pse->file );
#endif

	if ( pse->abort ) {
		pse = NULL;
		return( FALSE );
	}

	pse = NULL;

	return( TRUE );
}


/******************************************************************************
 * Function:
 * pseParseData -- parse an internal data stream
 * 
 * Description:
 *
 * Returns:
 * 
 */

public Bool pseParseDataFunc (
    Byte    *data,
	char	*ichars,
	int		 nchars,
	void	*(callback)(),
	void	*user_data
    )
{
	PseInfo  pse_info;

	/* validate */
	if ( data == NULL || callback == NULL ) 
		return( FALSE );
	if ( ichars == NULL && nchars > 0 ) 
		return( FALSE );
	  
	/* init */
	pse = &pse_info;

    /* init */
	pse->filename = "(data)";
	pse->data   = data;
    pse->dcnt   = 0;
    pse->max    = PSE_MAXLINE;
	pse->nargs  = 0;
	pse->ichars = ichars;
	pse->nchars = nchars;
	pse->lineno = 0;
	pse->abort  = FALSE;

	while( ! pse->abort && pseGetLine() )
		callback( user_data );

	if ( pse->abort ) {
		pse = NULL;
		return( FALSE );
	}

	pse = NULL;

	return( TRUE );
}


/******************************************************************************
 * Function:
 * pseGetLine -- get a line from the input form file or data
 * 
 * Description:
 *
 * Returns:
 * 
 */

private Bool pseGetLine (
    void
    )
{
	Bool	gotvalidline;
	int		i; //, j;

	/* clear buffer */
    memClear( pse->line, pse->max );

	gotvalidline = FALSE;
	while( ! gotvalidline ) {
		if ( pse->data != NULL ) {
			/* internal data */
			if ( *pse->data == '\0' ) return( FALSE );
			i = 0;
			while( (*pse->data != '\r') && (*pse->data != '\n') && (*pse->data != '\0') && (i < (pse->max-1)) ) {
				pse->line[i] = *pse->data;
				pse->data++;
				i++;
			}
			/* terminate string */
			pse->line[i] = '\0';
			//printf( "line=%s\n", pse->line );
			/* go to start of next line if necessary (handle DOS \r text files) */
			if ( *pse->data == '\r' ) pse->data++;
			if ( *pse->data == '\n' ) pse->data++;
			if ( i > 0 ) {
				pse->lineno++;
				if ( pseGetArgs() )
					gotvalidline = TRUE;
			}
		} else {			
#ifndef PSE_NO_FILE_SUPPORT
			/* check for end of file */
    		if ( fileEof( pse->file ) ) 
				return( FALSE );
			/* get a line */
			if ( fileGetLine( pse->line, pse->max, pse->file ) ) {
				pse->lineno++;	
				if ( pseGetArgs() )
					gotvalidline = TRUE;
			}
#else
			return( FALSE );
#endif
		}
	}
   
	//printf( "line = %s\n", pse->line );
    return( TRUE );
}


/******************************************************************************
 * Function:
 * pseGetArgs -- get args from input line
 * 
 * Description:
 *
 * Returns:
 * 
 */

private Bool pseGetArgs (
	void
	)
{
	Text	arg;
	int		i;

	/* init */
	pse->nargs = 0;

	/* validate */
	if ( pse->line[0] == '\0' ) return( FALSE );

	/* check for ignore args */
	for ( i=0; i<pse->nchars; i++ )
		if ( pse->line[0] == pse->ichars[i] ) 
			return( FALSE );

	/* clean up last used */
	//for ( i=0; i<pse->nargs; i++ )
	//	memFree( pse->args[i] );

	/* get args */
	for ( i=0; i<MAXARGS; i++ ) {
		arg = pseGetWord( (i+1) );
		if ( arg == NULL ) break;
		pse->argtype[i] = pseArgType( arg );
		switch( pse->argtype[i] ) {
		case PSE_DATATYPE_INT:
			pse->intval[i] = atoi( arg );
			pse->floatval[i] = (float)pse->intval[i];
			break;
		case PSE_DATATYPE_FLOAT:
#ifdef OsTypeIsPs2
			pse->floatval[i] = (float)atoff( arg );
#else
			pse->floatval[i] = (float)atof( arg );
#endif
			//printf( "arg=[%s],fval[%d]=%.2Lf,atof=%.2Lf,atoff=%.2Lf\n", arg, i, (long double)pse->floatval[i], (long double)atof(arg), (long double)atoff(arg) );			
			pse->intval[i] = (int)pse->floatval[i];
			break;
		}
		//pse->args[i] = arg;
		pse->nargs++;
	}

	if ( pse->nargs == 0 ) 
		return( FALSE );

	return( TRUE );
}

		  
/******************************************************************************
 * Function:
 * pseArgType -- determine argument type
 * 
 * Description:
 *
 * Returns:
 * 
 */

private int	pseArgType (
	Text	arg
	)
{
	Bool		gotdot;
	PseDataType	type;
	int			i;

	/* init */
	gotdot = FALSE;
	type   = PSE_DATATYPE_INT;

	i = 0;
	while ( arg[i] != '\0' ) {
		switch( arg[i] ) {
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
		case '-':
			break;
		case '.':
			if ( gotdot ) 
				return( PSE_DATATYPE_STRING );
			gotdot = TRUE;
			type = PSE_DATATYPE_FLOAT;
			break;
		default:
			return( PSE_DATATYPE_STRING );
		}
		i++;
	}

	/* assume int */
	return( type );
}


/******************************************************************************
 * Function:
 * pseGetWord -- get word <n> from the input line.
 *
 * 
 * Description:
 * 
 * Include remaining string if all = TRUE
 *
 * Returns:
 * 
 */

private Text pseGetWord (
    int   	 n
    )
{
    int		 i = 0;
    int		 j;
    int		 w;
    Text	 result;
	Text	 wrd;
    
	/* init */
	wrd = pse->args[(n - 1)];
	i = 0;
	j = 0;
    for ( w=0; w<n; w++ ) {
	
		/* check for end of line */
		if ( pse->line[i] == '\0' /*|| pse->line[i] == '#'*/ || i > (pse->max - 1) ) {
		    j = 0;
		    break;
		}

        /* skip whitespace */
		while ( pse->line[i] == ' ' || pse->line[i] == '\t' )
	    	i++;

		j = 0;
		if ( pse->line[i] == '"' ) {
			i++;	 
			/* get to end of string or line */
	    	while ( pse->line[i] != '"' && pse->line[i] != '\0' && i < (pse->max - 1) ) {
				wrd[j] = pse->line[i];
				j++;
				i++;
	    	}
		} else {
			/* get word only */
	    	while ( pse->line[i] != ' ' && pse->line[i] != '\t' && 
	    		    pse->line[i] != '\0' && i < (pse->max - 1) ) {
				wrd[j] = pse->line[i];
				j++;
				i++;
	    	}
		}	    
		wrd[j] = '\0';
		i++;
    }

	/* check for no value found */
    if ( j == 0 ) {
		//printf( "(gw %d=NULL)\n", n );
		result = NULL;
    } else {
		//printf( "(gw %d=%s)", n, wrd );
		//if ( (n % 10) == 0 ) printf("\n");
		result = wrd;
    }
    
    return( result );
}


/******************************************************************************
 * Function:
 * pseNumArgs -- return the number of args found on this line
 *
 * 
 * Description:
 * 
 *
 * Returns:
 * 
 */

public int	pseNumArgs ( 
	void
	)
{
	if ( pse == NULL ) 
		return(0);
	return( pse->nargs );
}

/******************************************************************************
 * Function:
 * pseArgValidate -- check if this arg matches the passed string
 *
 * 
 * Description:
 * 
 *
 * Returns:
 * 
 */

public Bool pseArgValidate (
	Text	str,
	int		i
	)
{
	if ( pse == NULL ) return( FALSE );
	if ( i >= pse->nargs ) return( FALSE );
	if ( str == NULL ) return( FALSE );
	if ( txtMatch(str,pse->args[i]) )
		return( TRUE );
	return( FALSE );
}

 
/******************************************************************************
 * Function:
 * pseArgIsString -- is this arg a string value?
 *
 * 
 * Description:
 * 
 *
 * Returns:
 * 
 */

public Bool pseArgIsString (
	int		i
	)
{
	if ( pse == NULL ) return( FALSE );
	if ( i >= pse->nargs ) return( FALSE );
	if ( pse->argtype[i] == PSE_DATATYPE_STRING )
		return( TRUE );
	return( FALSE );
}


/******************************************************************************
 * Function:
 * pseArgIsInt -- is this arg an integer value?
 *
 * 
 * Description:
 * 
 *
 * Returns:
 * 
 */

public Bool pseArgIsInt (
	int		i
	)
{
	if ( pse == NULL ) return( FALSE );
	if ( i >= pse->nargs ) return( FALSE ); 
	if ( pse->argtype[i] == PSE_DATATYPE_INT )
		return( TRUE );
	return( FALSE );
}


/******************************************************************************
 * Function:
 * pseArgIsFloat -- is this arg an integer value?
 *
 * 
 * Description:
 * 
 *
 * Returns:
 * 
 */

public Bool pseArgIsFloat (
	int		i
	)
{
	if ( pse == NULL ) return( FALSE );
	if ( i >= pse->nargs ) return( FALSE ); 
	if ( pse->argtype[i] == PSE_DATATYPE_FLOAT )
		return( TRUE );
	return( FALSE );
}


/******************************************************************************
 * Function:
 * pseGetIntValue -- get the integer value for this arg
 *
 * 
 * Description:
 * 
 *
 * Returns:
 * 
 */

public int pseGetIntValue (
	int		i	
	)
{ 
	if ( pse == NULL ) return( 0 );
	if ( i >= pse->nargs > i || ! pseArgIsInt(i) ) return( 0 );
	return( pse->intval[i] );
}


/******************************************************************************
 * Function:
 * pseGetFloatValue -- get the integer value for this arg
 *
 * 
 * Description:
 * 
 *
 * Returns:
 * 
 */

public float pseGetFloatValue (
	int		i	
	)
{ 
	if ( pse == NULL ) return( 0.0f );
	if ( i >= pse->nargs > i || (!pseArgIsFloat(i) && !pseArgIsInt(i)) ) return( 0.0f );
	return( pse->floatval[i] );
}


/******************************************************************************
 * Function:
 * pseGetStringValue -- get the string value for this arg
 *
 * 
 * Description:
 * 
 *
 * Returns:
 * 
 */

public Text	pseGetStringValue ( 
	int		i
	)
{
	if ( pse == NULL ) return( NULL );
	if ( i >= pse->nargs ) return( NULL );
	return( pse->args[i] );
}


/******************************************************************************
 * Function:
 * pseGetLineValue -- get the full line string value
 *
 * 
 * Description:
 * 
 *
 * Returns:
 * 
 */

public Text	pseGetLineValue ( 
	void
	)
{
	if ( pse == NULL ) return( NULL );
	return( pse->line );
}
 
 
/******************************************************************************
 * Function:
 * pseGetLineNum -- get the current parse line number
 *
 * 
 * Description:
 * 
 *
 * Returns:
 * 
 */

public int pseGetLineNum ( 
	void
	)
{
	if ( pse == NULL ) return(0);
	return( pse->lineno );
}

  
/******************************************************************************
 * Function:
 * pseGetFilename -- get the current parsed file name
 *
 * 
 * Description:
 * 
 *
 * Returns:
 * 
 */

public Text pseGetFilename ( 
	void
	)
{
	if ( pse == NULL ) return( NULL );
	/* may be NULL */
	return( pse->filename );
}


/******************************************************************************
 * Function:
 * pseAbortParse -- abort parsing
 *
 * 
 * Description:
 * 
 *
 * Returns:
 * 
 */

public void pseAbortParse (
	void
	)
{
	if ( pse )
		pse->abort = TRUE;
}

