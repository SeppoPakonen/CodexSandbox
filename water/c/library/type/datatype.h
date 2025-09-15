/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		Library
 **********************************************************
 * File:		datatypes.h
 * Author:		Mark Theyer
 * Created:		28 Dec 1996
 **********************************************************
 * Description:	
 *		This file contains the declarations for various data
 *		types.
 **********************************************************
 * Functions:
 **********************************************************
 * Revision History:
 * 28-Dec-96	Theyer	Initial coding.
 * 21-Apr-01	Theyer	New types for PS2 coding.
 **********************************************************/

#ifndef THEYER_DATATYPE_H
#define THEYER_DATATYPE_H

#include "compile.h"

/* set boolean values */
#define	Bool      	unsigned int
#define TRUE	  	1
#define FALSE		0
#define ON			TRUE
#define OFF			FALSE

/* bits and bytes */
typedef unsigned int   Bit32;
typedef unsigned short Bit16;
typedef unsigned char  Bit8;
typedef unsigned char  Byte;
typedef unsigned short Word;
typedef unsigned int   Dword;

/* bits and bytes */
#ifdef ChipTypeIs64Bits
typedef unsigned long  Bit64;
#endif

#ifdef ChipTypeIs128Bits
typedef unsigned long  Bit64;
typedef unsigned long long Bit128;
#endif

/* function scope indicators */
#ifndef __cplusplus
#define private 	static
#define internal
#define public
#endif

#ifndef OsTypeIsPsx
#ifdef OsTypeIsPs2
#else
#define u_char	unsigned char
#define u_short	unsigned short
#define u_int	unsigned int
#define u_long	unsigned long
#endif
#endif

/* bit testing macros */
#define	bitOn(mask,bit)					((mask&bit)==(bit))
#define	bitOff(mask,bit)				((mask&bit)==(0))

/* fixed type (for PlayStation 4096 = 1.0f) */
#define fixed int
#define FIXED_ONE						(4096)
#define FIXED_ZERO						(0)
#define FIXED_SHIFT4(fxd)				((fixed)((fxd>0)?((fxd<<4)&0x7FFFFFFF):(fxd*16)))
#define FIXED_SHIFT8(fxd)				((fixed)((fxd>0)?((fxd<<8)&0x7FFFFFFF):(fxd*256)))
//#define FIXED_SHIFT8(f)				((fixed)(((f)&(0x80000000))|(((f)<<8)&(0x7FFFFFFF))))
#define FIXED_MULTIPLY(fxa,fxb)			((fixed)((fxa*fxb)/FIXED_ONE))
#define FIXED_NORMALISE(a,b)			((a==b)?FIXED_ONE:((fixed)((a) && ((FIXED_SHIFT8(b))/(a)))?((FIXED_SHIFT8(FIXED_ONE))/((FIXED_SHIFT8(b))/(a))):(0)))
#define FIXED_TO_INT(fxd)				((int)((fxd>0)?((fxd)>>12):(fxd/FIXED_ONE)))
//#define FIXED_TO_INT(fxd)				((int)(((fixed)(fxd))/FIXED_ONE))
/* only convert to or from a float if processing performance in not critical */
#define FIXED_TO_FLOAT(fxd)				((float)((float)fxd/4096.0f))

#define INT_TO_FIXED(i)					((fixed)((i>0)?((i<<12)&0x7FFFFFFF):(i*FIXED_ONE)))
#define FLOAT_TO_FIXED(f)				((fixed)((f)*4096))
#define RGB_TO_FIXED(val)				(FIXED_NORMALISE(val,255))

#define HALF(i)							((i>0)?(i>>1):(i/2))
#define DOUBLE(i)						((i>0)?(i<<1):(i*2))

#ifndef ABS
#define ABS(i)							((i>0)?(i):(-(i)))
#endif

/* PlayStation2 data alignment macros */
#ifdef OsTypeIsPs2
#define ALIGN512						__attribute__((aligned (64)))
#define ALIGN128						__attribute__((aligned (16)))
#define ALIGN64							__attribute__((aligned (8)))
#endif

#ifdef OsTypeIsPs2
/* point types - float */
typedef struct { float x, y, z, w; } Point_f3d;
#else
/* point types - float */
typedef struct { float x, y, z; } Point_f3d;
#endif

typedef struct { float x, y;    } Point_f2d;
/* point types - fixed */
typedef struct { fixed x, y, z; } Point_fx3d;
typedef struct { fixed x, y;    } Point_fx2d;
/* point types - integer */
typedef struct { int   x, y, z; } Point_i3d;
typedef struct { int   x, y;    } Point_i2d;

#endif

