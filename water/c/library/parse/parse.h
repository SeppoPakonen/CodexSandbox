
#ifndef THEYER_PARSE_H
#define THEYER_PARSE_H

/*
 * Description:
 * 
 */

/*
 * includes
 */

#include <type/datatype.h>
#include <text/text.h>

/*
 * macros
 */

#define PSE_MAXLINE    		1024 //512	//256	

/*
 * typedefs
 */

/*
 * prototypes
 */

extern Bool  pseParseFileFunc( Text filename, char *ichars, int nchars, void *(callback)(), void *user_data );
extern Bool  pseParseDataFunc( Byte *data, char *ichars, int nchars, void *(callback)(), void *user_data );

#define      pseParseFile(file,ichars,nchars,callback,udata)		pseParseFileFunc(file,ichars,nchars,(void*)callback,(void*)udata)
#define      pseParseData(data,ichars,nchars,callback,udata)		pseParseDataFunc(data,ichars,nchars,(void*)callback,(void*)udata)

extern int   pseGetLineNum( void );
extern Text  pseGetFilename( void );
extern void  pseAbortParse( void );
extern int	 pseNumArgs(	void );
extern Bool  pseArgValidate( Text str, int i );
extern Bool  pseArgIsString( int i );
extern Bool  pseArgIsInt( int i );
extern Bool  pseArgIsFloat( int i );
extern int   pseGetIntValue( int i );
extern float pseGetFloatValue( int i );
extern Text  pseGetStringValue( int i );
extern Text  pseGetLineValue( void );

/*
 * variables
 */

#endif
