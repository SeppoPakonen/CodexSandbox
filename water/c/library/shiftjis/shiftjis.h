/**********************************************************
 * Copyright (C) 1999, 2000 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:	
 **********************************************************
 * File:	shiftjis.h
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

#ifndef THEYER_SHIFTJIS_H
#define THEYER_SHIFTJIS_H

/*
 * includes
 */

#include <type/datatype.h>
#include <text/text.h>

/*
 * typedefs
 */

typedef unsigned short ShiftJIS;

/*
 * macros
 */

#define SHIFT_JIS_HIGH(jis,n)	((jis[n])>>8)
#define SHIFT_JIS_LOW(jis,n)	((jis[n])<<8>>8)

/*
 * prototypes
 */

extern Bool jisAsciiToShiftJIS( Text str, ShiftJIS *jis, int jsize );
extern Bool jisShiftJISToAscii( ShiftJIS *jis, int jsize, Text str );

#endif //THEYER_SHIFTJIS_H

