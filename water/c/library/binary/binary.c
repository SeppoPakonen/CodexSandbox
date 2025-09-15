/**********************************************************
 * Copyright (C) 1998 Mark Theyer
 * All Rights Reserved
 **********************************************************
 * Project:	Binary library
 **********************************************************
 * File:	binary.c
 * Author:	Mark Theyer
 * Created:	05 Dec 1998
 **********************************************************
 * Description:	Cross Platform Binary file format I/O
 **********************************************************
 * Revision History:
 * 05-Dec-98	Theyer	Initial Coding
 **********************************************************/

/*
 * includes
 */

#include <compile.h>
#include <type/datatype.h>
#include <text/text.h>
#include <binary/binary.h>

/*
 * typedefs
 */

/*
 * macros
 */

/*
 * prototypes
 */

/*
 * global variables
 */

/*
 * functions
 */
   
           
/******************************************************************************
 * Function:
 * binReadByte -- output a byte to binary buffer
 * 
 * Description:
 *
 * Returns:
 * 
 */

Byte binReadByte ( 
	Byte	*buffer
	)
{
	return( *buffer );
}

    
/******************************************************************************
 * Function:
 * binWriteByte -- output a byte to binary buffer
 * 
 * Description:
 *
 * Returns:
 * 
 */

int binWriteByte ( 
	Byte	 b,
	Byte	*buffer
	)
{
	*buffer = b;
	return( 1 );
}


/******************************************************************************
 * Function:
 * binWriteString -- output fixed length string to binary buffer
 * 
 * Description:
 *
 * Returns:
 * 
 */

int binWriteString (
	Text	 str,
	int		 size,
	Byte	*buffer
	)
{
	int		i;
	int		len;
	char	c;

	/* output string to output buffer */
	len = txtLength( str );
	for ( i=0; i<size; i++ ) {
		if ( i<len )
			c = str[i];
		else
			c = '\0';
		*(buffer+i) = c;
	}

	return( size );
}

    
/******************************************************************************
 * Function:
 * binReadString -- read in a fixed length string
 * 
 * Description:
 *
 * Returns:
 * 
 */

Text binReadString (
	int		 size,
	Byte	*buffer
	)
{
	int			i;
	static char	str[256];

	i = 0;
	while ( *(buffer+i) != '\0' && i<size && i<255 ) {
		str[i] = *(buffer+i);
		i++;
	}
	str[i] = '\0';

	return( str );
}


/******************************************************************************
 * Function:
 * binReadInt -- read a binary format integer
 * 
 * Description:
 *
 * Returns:
 * 
 */

int binReadInt (
    Byte	*buffer
    )
{
	int		i;
	struct {
		Byte		a,b,c,d;
	} bit32;
	int		*ip;

	ip = (int *)&bit32;
	bit32.a =  *buffer;
	bit32.b = *(buffer+1);
	bit32.c = *(buffer+2);
	bit32.d = *(buffer+3);
	//i = *buffer + (*(buffer+1)<<8) + (*(buffer+2)<<16) + (*(buffer+3)<<24);
	i = *ip;
	//printf( "bin: i = %d\n", i );

	return( i );
}


/******************************************************************************
 * Function:
 * binReadShort -- read a binary format short integer
 * 
 * Description:
 *
 * Returns:
 * 
 */

short binReadShort (
    Byte	*buffer
    )
{
	short	s;
	struct {
		Byte		a,b;
	} bit16;
	short	*sp;

	sp = (short *)&bit16;
	bit16.a =  *buffer;
	bit16.b = *(buffer+1);
	//s = *buffer + (*(buffer+1)<<8);
	s = *sp;
	//printf( "bin: s = %d\n", (int)s );
	
	return( s );
}


/******************************************************************************
 * Function:
 * binReadUShort -- read a binary format short integer
 * 
 * Description:
 *
 * Returns:
 * 
 */

unsigned short binReadUShort (
    Byte	*buffer
    )
{
	unsigned short	 s;
	
	s = (unsigned short)(*buffer + (*(buffer+1)<<8));
	//printf( "bin: us = %d\n", (int)s );

	return( s );
}


/******************************************************************************
 * Function:
 * binReadFloat -- read a binary format float
 * 
 * Description:
 *
 * Returns:
 * 
 */

float binReadFloat (
    Byte	*buffer
    )
{
	float	 f;

	f = ((float)(binReadInt( buffer ))/1000.0f);
	//printf2( "bin: f = %.3f\n", f );

	return( f );
}


/******************************************************************************
 * Function:
 * binReadPs2Float -- output float to binary buffer
 * 
 * Description:
 *
 * Returns:
 * 
 */

float binReadPs2Float (
    Byte	*buffer
	)
{
	float	*f;

	/* precision is 3 decimal places */
	f = (float *)buffer;

	return( *f );
}


/******************************************************************************
 * Function:
 * binWriteShort -- output short integer to binary buffer
 * 
 * Description:
 *
 * Returns:
 * 
 */

int binWriteShort (
	short	 s,
	Byte	*buffer
	)
{
	*buffer     = (Byte)(s<<8>>8);
	*(buffer+1) = (Byte)(s>>8);
	return( 2 );
}


/******************************************************************************
 * Function:
 * binWriteUShort -- output unsigned short integer to binary buffer
 * 
 * Description:
 *
 * Returns:
 * 
 */

int binWriteUShort (
	unsigned short	 s,
	Byte			*buffer
	)
{
	*buffer     = (Byte)(s<<8>>8);
	*(buffer+1) = (Byte)(s>>8);
	return( 2 );
}


/******************************************************************************
 * Function:
 * binWriteInt -- output integer to binary buffer
 * 
 * Description:
 *
 * Returns:
 * 
 */

int binWriteInt (
	int		 i,
	Byte	*buffer
	)
{
	*buffer     = (Byte)(i<<24>>24);
	*(buffer+1) = (Byte)(i<<16>>24);
	*(buffer+2) = (Byte)(i<<8>>24); 
	*(buffer+3) = (Byte)(i>>24);
	return( 4 );
}


/******************************************************************************
 * Function:
 * binWriteFloat -- output float to binary buffer
 * 
 * Description:
 *
 * Returns:
 * 
 */

int binWriteFloat (
	float	 f,
	Byte	*buffer
	)
{
	int		 i;

	/* precision is 3 decimal places */
	i = (int)(f * 1000);

	return( binWriteInt( i, buffer ) );
}

