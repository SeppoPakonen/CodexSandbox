/*****************************************************************
 * Project		: GUI Library
 *****************************************************************
 * File			: linked.c
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 28 Dec 1996
 * Last Update	: 28 Dec 1996
 *****************************************************************
 * Description	:
 *
 *		This file contains the set of data types for linked
 *		list creation and management.
 *
 *****************************************************************
 * 28/12/96		: New Version.
 *****************************************************************/

#ifndef THEYER_BINARY_H
#define THEYER_BINARY_H

#include <type/datatype.h>
#include <text/text.h>

#ifdef __cplusplus
extern "C" {
#endif

Byte  binReadByte( Byte *buffer );
int   binWriteByte( Byte b, Byte *buffer );
Text  binReadString( int size, Byte *buffer );
int   binWriteString( Text str, int size, Byte *buffer );
int	  binReadInt( Byte *buffer );
short binReadShort( Byte *buffer );
unsigned short binReadUShort( Byte *buffer );
float binReadFloat( Byte *buffer );
float binReadPs2Float( Byte *buffer );
int	  binWriteInt( int i, Byte *buffer );
int	  binWriteShort( short s, Byte *buffer );
int	  binWriteUShort( unsigned short s, Byte *buffer );
int   binWriteFloat( float f, Byte *buffer );
int   binWritePs2Float( float f, Byte *buffer );

#ifdef __cplusplus
}
#endif

#endif
