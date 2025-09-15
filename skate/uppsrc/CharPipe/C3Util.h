/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3Util.h]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/include/charPipeline/c3/C3Util.h $
    
    7     6/27/00 6:02p John
    Moved quantize.h macros here.
    
    6     4/06/00 3:23p John
    Added C3CharReplace and removed C3toupper.
    
    5     3/17/00 1:54p John
    Renamed C3InitializeExtraction to C3Initialize and C3CleanExtraction to
    C3Clean.
    
    4     2/04/00 6:06p John
    Untabified code.
    
    3     1/20/00 1:13p John
    Added stitching (work in progress).
    
    2     12/08/99 7:24p John
    Added animation quantization.
    
    16    11/18/99 1:13p John
    Moved math stuff to C3Math
    
    15    11/11/99 1:11p John
    Added some Mtx and Quat code.  (work in progress).
    
    14    10/14/99 4:32p John
    Added function to return todays date.
    
    13    9/29/99 4:26p John
    Changed header to make them all uniform with logs.

  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef _C3UTIL_H
#define _C3UTIL_H

#if 0
extern "C" {
#endif // __cplusplus

#define C3_SWAP_PTR(a,b)    { void* ptr = NULL; ptr = (void*)(a); (void*)(a) = (void*)(b); (Ptr)(b) = ptr; }
#define C3_PAD32(a)         { u32 m; m = (a) % 4;  (a) = (m)?(a +  4 - m):(a);  } // pad to 32-bits boundary
#define C3_PAD32_BYTES(a)   { u32 m; m = (a) % 32; (a) = (m)?(a + 32 - m):(a);  } // pad to 32-bytes boundary

#define C3_XOR(a,b)         (((a)&&!(b))||(!(a)&&(b)))
#define C3_XNOR(a,b)        ((!(a)&&!(b))||((a)&&(b)))
#define C3_STR( a )         #a

#define C3_QUANT_TYPE(a)    ((a)>>4)
#define C3_QUANT_SHIFT(a)   ((a)&0x0F)
#define C3_MAKE_QUANT(a,b)  (((a)<<4)|((b)&0x0F))

/*****************************************************************************/

typedef u8 C3Bool;

typedef struct
{
    DSLink  link;
    u32     index;

} C3IndexedLink, *C3IndexedLinkPtr;

typedef struct
{
    char*   str;    // C3String* must be able to be cast as char* for string table
    DSLink  link;
    u32     index;

} C3String, *C3StringPtr;

typedef void(*C3ReportStatusFunc) ( char* );

typedef void(*C3ReportErrorFunc) ( char* );

/*****************************************************************************/

void    C3Initialize            ( );
void    C3Clean                 ( );

u32     C3GetTodaysDate         ( );
void    C3CharReplace           ( char* string, char from, char to );
void    C3StrCpy                ( char** dest, char* src );

void    C3MakePath              ( char* path, char* name, char* extension, char** buffer ); 
void    C3MakePathSuffix        ( char* path, char* name, char* extension, char* suffix, char** buffer );
void    C3MakePathForwardSlash  ( char* path );

u32     C3WritePadded32bit      ( void* buffer, u32 size, FILE* outFile );
u32     C3WritePadded32byte     ( void* buffer, u32 size, FILE* outFile );

u8      C3WriteU32              ( u32 num, void* buffer );
u8      C3WriteU16              ( u16 num, void* buffer );
u8      C3WriteFloat            ( float num, void* buffer );
u8      C3WriteQuantizeFloat    ( float num, void* buffer, u8 quantInfo );

u8      C3WriteU32F             ( u32 num, FILE* outFile );
u8      C3WriteU16F             ( u16 num, FILE* outFile );
u8      C3WriteFloatF           ( float num, FILE* outFile );

u8      C3GetQuantTypeSize      ( u8 quantInfo );
 
void    C3SwapBytes             ( void** src, void**  dst, u32 size, u32 compSize );
void*   C3NextUniqueIndex       ( C3IndexedLink* current );

void    C3ReportStatus          ( char* fmt, ... );
void    C3ReportError           ( char* fmt, ... );

void    C3InitStringTable       ( );
void    C3CleanStringTable      ( );
u16     C3HashString            ( void* string );
char*   C3GetStringInTable      ( char* string );
char*   C3AddStringInTable      ( char* string );

#if 0
}
#endif // __cplusplus

#endif // _C3UTIL_H
