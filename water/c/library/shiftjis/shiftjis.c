/**********************************************************
 * Copyright (C) 1999, 2000 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:	
 **********************************************************
 * File:	shiftjis.c
 * Author:	Mark Theyer
 * Created:	9 Feb 2000
 **********************************************************
 * Description:	Shift-JIS 16 bit character code functions
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

#include "shiftjis.h"

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

static unsigned short sjis_ascii_table[3][2] = {
	{0x824f, 0x30},	/* 0-9  */
	{0x8260, 0x41},	/* A-Z  */
	{0x8281, 0x61},	/* a-z  */
};

 
// ASCII code to Shift-JIS code transfer table (kigou)
static unsigned short sjis_ascii_k_table[] = {
	0x8140,		/*   */
	0x8149,		/* ! */
	0x8168,		/* " */
	0x8194,		/* # */
	0x8190,		/* $ */
	0x8193,		/* % */
	0x8195,		/* & */
	0x8166,		/* ' */
	0x8169,		/* ( */
	0x816a,		/* ) */
	0x8196,		/* * */
	0x817b,		/* + */
	0x8143,		/* , */
	0x817c,		/* - */
	0x8144,		/* . */
	0x815e,		/* / */
	0x8146,		/* : */
	0x8147,		/* ; */
	0x8171,		/* < */
	0x8181,		/* = */
	0x8172,		/* > */
	0x8148,		/* ? */
	0x8197,		/* @ */
	0x816d,		/* [ */
	0x818f,		/* \ */
	0x816e,		/* ] */
	0x814f,		/* ^ */
	0x8151,		/* _ */
	0x8165,		/* ` */
	0x816f,		/* { */
	0x8162,		/* | */
	0x8170,		/* } */
	0x8150,		/* ~ */
};

/*
 * functions
 */          

/**********************************************************
 * Function:	jisAsciiToShiftJIS
 **********************************************************
 * Description: Convert ASCII to ShiftJIS
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

Bool jisAsciiToShiftJIS ( 
	Text		 str, 
	ShiftJIS	*jis,
	int			 jsize
	) 
{
	int			 i;
	int			 sjis_code;
	int			 ascii_code;
	Byte		 stmp;
	Byte		 stmp2;
	Byte		*ptr;
				
	/* init */
	ptr = (Byte *)jis;

	/* init (spaces) return ShiftJIS */
	i = 0;
	while ( i<jsize ) {
		*ptr = 0x81; ptr++;
		*ptr = 0x40; ptr++;
		i++;
	}
				
	/* convert ASCII to Shift-JIS */
	ptr = (Byte *)jis;
	i   = 0;
	while( str[i] != '\0' ) {
		/* init */
		stmp2 = 0;
		ascii_code = str[i];
		/* process */
		if ((ascii_code >= 0x20) && (ascii_code <= 0x2f))
			stmp2 = 1;
		else if ((ascii_code >= 0x30) && (ascii_code <= 0x39))
			stmp = 0;
		else if ((ascii_code >= 0x3a) && (ascii_code <= 0x40))
			stmp2 = 11;
		else if ((ascii_code >= 0x41) && (ascii_code <= 0x5a))
			stmp = 1;
		else if ((ascii_code >= 0x5b) && (ascii_code <= 0x60))
			stmp2 = 37;
		else if ((ascii_code >= 0x61) && (ascii_code <= 0x7a))
			stmp = 2;
		else if ((ascii_code >= 0x7b) && (ascii_code <= 0x7e))
			stmp2 = 63;
		else
			return( FALSE );
		/* create code */
		if ( stmp2 )
			sjis_code = sjis_ascii_k_table[ascii_code - 0x20 - (stmp2 - 1)];
		else
			sjis_code = sjis_ascii_table[stmp][0] + ascii_code - sjis_ascii_table[stmp][1];
		/* write ShiftJIS */
		*ptr++ = ((sjis_code & 0xFF00)>>8);
		*ptr++ = (sjis_code & 0xFF);
		/* go to next character */
		i++;
	}

	return( TRUE );
}


#if 0
private int ascii2sjis(unsigned char ascii_code)
{
	int sjis_code = 0;
	unsigned char stmp;
	unsigned char stmp2 = 0;

	if ((ascii_code >= 0x20) && (ascii_code <= 0x2f))
		stmp2 = 1;
	else
	if ((ascii_code >= 0x30) && (ascii_code <= 0x39))
		stmp = 0;
	else
	if ((ascii_code >= 0x3a) && (ascii_code <= 0x40))
		stmp2 = 11;
	else
	if ((ascii_code >= 0x41) && (ascii_code <= 0x5a))
		stmp = 1;
	else
	if ((ascii_code >= 0x5b) && (ascii_code <= 0x60))
		stmp2 = 37;
	else
	if ((ascii_code >= 0x61) && (ascii_code <= 0x7a))
		stmp = 2;
	else
	if ((ascii_code >= 0x7b) && (ascii_code <= 0x7e))
		stmp2 = 63;
	else {
		printf("bad ASCII code 0x%x\n", ascii_code);
		return(0);
	}

	if (stmp2)
		sjis_code = ascii_k_table[ascii_code - 0x20 - (stmp2 - 1)];
	else
		sjis_code = ascii_table[stmp][0] + ascii_code - ascii_table[stmp][1];

	return(sjis_code);
}
#endif
