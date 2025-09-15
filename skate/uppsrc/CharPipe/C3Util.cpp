/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3Util.c]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/c3/src/C3Util.c $
    
    13    11/13/00 1:49p John
    Fixed bug so C3ReportStatus or C3ReportError need not print information
    with NULL arguments.
    
    12    6/27/00 6:08p John
    Substituted quantize.h macros.
    
    11    4/06/00 3:27p John
    Added C3CharReplace and removed C3toupper.
    
    10    3/17/00 1:54p John
    Renamed C3InitializeExtraction to C3Initialize and C3CleanExtraction to
    C3Clean.
    
    9     2/29/00 7:26p John
    Added C3InitOptions to C3Initialize.
    
    8     2/04/00 6:08p John
    Untabified code.
    
    7     1/31/00 4:16p John
    Added pruning of unnecessary bones and animations.
    Finished stitching with FIFO matrix cache and stripping.
    
    6     1/20/00 1:15p John
    Added stitching (work in progress).
    Added C3 string table for efficient management of strings.
    
    5     1/04/00 6:58p John
    Added little/big endian output  option
    
    4     12/15/99 2:58p John
    Removed warnings for Visual C++ 6.0
    
    3     12/08/99 7:28p John
    Added C3WriteQuantizeFloat and C3GetQuantTypeSize for animation
    quantization.
    
    2     12/06/99 3:27p John
    Added test code to analyze C3StrCpy usage.  Will add better string
    management routines for memory efficiency.
    
    20    11/18/99 1:18p John
    Moved math stuff to C3Math
    Added function to pad buffer to 32 byte boundary.
    
    19    11/11/99 1:23p John
    Added some Mtx and Quat code temporarily.  (work in progress).
    
    18    10/14/99 4:32p John
    Added function to return todays date.
    
    17    10/01/99 11:17a John
    With C3StatsPool(), no need to clean stats.
    
    16    9/29/99 4:34p John
    Changed header to make them all uniform with logs.
    Moved C3NextUniqueIndex here.
    
    15    9/20/99 7:05p John
    Added C3MakePathForwardSlash.
    
    14    9/17/99 9:34a John
    Added C3Options and cleaned up code.
    
    13    9/14/99 6:53p John
    Added code to init and clean stats.
    Added C3MakePathSuffix.
    
    12    9/03/99 2:05p John
    Swaps groups of 3 bytes in SwapBytes in response
    to adding color and index quantization.

  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#include "C3Private.h"

#define C3_STRING_TABLE_SIZE    64

// Global variables
static DSHashTable* stringTable;
static DSList*      stringTableList;


/*---------------------------------------------------------------------------*
  Name:         C3Initialize

  Description:  Call all the initalization functions for all the modules

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3Initialize( void )
{
#ifdef C3_DEBUG
    C3InitDebug( );
#endif

    C3InitPools();

    C3InitStringTable( );
    C3InitGeometryExtraction( );    // initialize geometry
    C3InitHierarchyExtraction( );   // initialize hierarchy
    C3InitAnimationExtraction( );   // initialize animation
    C3InitConversionOptimization( );// initialize conversion optimization
    C3InitializeTextures( );
//  C3InitCameraExt( ); 
    C3InitStats();
    C3InitOptions();
}


/*---------------------------------------------------------------------------*
  Name:         C3Clean

  Description:  Call all the clean up functions for all the modules

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3Clean( void )
{
//  C3CleanCameraExt();
    C3CleanConversionOptimization( );// clean conversion optimization
    C3CleanAnimationExtraction( );  // clean animation
    C3CleanHierarchyExtraction( );  // clean hierarchy
    C3CleanStringTable( );

    C3ClearPools();

#ifdef C3_DEBUG
    C3CleanDebug( C3_TRUE );
#endif
}


/*---------------------------------------------------------------------------*
  Name:         C3GetTodaysDate

  Description:  Returns todays date in the format of MMDDYYYY as an unsigned
                integer

  Arguments:    NONE
                  
  Returns:      an unsigned integer with the date in the format of MMDDYYYY
/*---------------------------------------------------------------------------*/
u32
C3GetTodaysDate( )
{
    char  buffer[16];
    u32   u32Date;
    time_t ltime;
    struct tm *today;

    time( &ltime );
    today = localtime( &ltime );

    strftime( buffer, 16, "%m%d%Y", today );
    u32Date = atol( buffer );

    return u32Date;
}


/*---------------------------------------------------------------------------*
  Name:         C3ReportStatus

  Description:  Call to the report status function pointer to inform the user
                of the status of the conversion.

  Arguments:    fmt - formatted variable length argument ( printf style )
                The total length of the string must not exceed 512 bytes.
                  
  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3ReportStatus( char* fmt, ... )
{
    va_list ap;
    char buffer[512];

    if( !C3GetOptionReportStatusFunc() )
        return;

    va_start( ap, fmt );
    vsprintf( buffer, fmt, ap ); 
    va_end( ap );

    (C3GetOptionReportStatusFunc())( buffer );
}

/*---------------------------------------------------------------------------*
  Name:         C3ReportError

  Description:  Call to the report status function pointer to inform the user
                of an error in the conversion

  Arguments:    fmt - formatted variable length argument ( printf style )
                The total length of the string must not exceed 512 bytes.
                  
  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3ReportError( char* fmt, ... )
{
    va_list ap;
    char buffer[512];

    if( !C3GetOptionReportErrorFunc() )
        return;

    va_start( ap, fmt );
    vsprintf( buffer, fmt, ap ); 
    va_end( ap );

    (C3GetOptionReportErrorFunc())( buffer );
}


/*---------------------------------------------------------------------------*
  Name:         C3CharReplace

  Description:  Replaces all characters in the given string with
                another character

  Arguments:    string - string to replace characters
                from - character to overwrite
                to - character to replace the 'from' character

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3CharReplace( char* string, char from, char to )
{
    u16 length;
    u16 index;

    length = strlen( string );
    for( index = 0; index <= length; index++ )
    {
        if( string[index] == from )
            string[index] = to;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3StrCpy

  Description:  Allocate a buffer in dest and copy the src string in the buffer
                src must be a null terminated string

  Arguments:    dest - destination to write
                src  - source to copy

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3StrCpy( char** dest, char* src )
{
//  FILE *f;
//  static u32 i = 0;

    C3_ASSERT( dest && src );

    *dest = (char*)C3_CALLOC( strlen(src) + 1, sizeof(char));
    C3_ASSERT( *dest );

    // TESTING: Analyze the usage of C3StrCpy
//  f = fopen( "D:\\temp\\strings.txt", "at" );
//  fprintf( f, "%d: %s\n", i++, src );
//  fclose(f);

    strcpy( *dest, src );
}


/*---------------------------------------------------------------------------*
  Name:         C3MakePath

  Description:  Allocates a memory buffer for the complete path, filename and
                    extension.

  Arguments:    path        - path to the file. Can be Null
                name        - name of the file
                extension   - extension of the file
                buffer      - location to write to.

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3MakePath( char* path, char* name, char* extension, char** buffer )
{
    u16 baseLength = 0;


    C3_ASSERT( name && extension && buffer );

    // path + \ + name + extension
    if( path )
        baseLength = strlen( path ) + 1;

    *buffer = (char*)C3_CALLOC(  baseLength + strlen(name) + strlen(extension) + 2, sizeof(char));
    C3_ASSERT( *buffer );
  
    if( path )
    {
        strcpy( *buffer, path );
        strcat( *buffer, "\\" ); 
    }
    strcat( *buffer, name );
    strcat( *buffer, "." );
    strcat( *buffer, extension );
}


/*---------------------------------------------------------------------------*
  Name:         C3MakePathSuffix

  Description:  Allocates a memory buffer for the complete path, filename, 
                suffix, and extension.

  Arguments:    path        - path to the file. Can be Null
                name        - name of the file
                extension   - extension of the file
                suffix      - suffix to append to name
                buffer      - location to write to.

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3MakePathSuffix( char* path, char* name, char* extension, char* suffix, char** buffer )
{
    u16 baseLength = 0;


    C3_ASSERT( name && extension && buffer );

    if( !suffix )
    {
        C3MakePath( path, name, extension, buffer );
        return;
    }

    // path + \ + name + extension
    if( path )
        baseLength = strlen( path ) + 1;

    *buffer = (char*)C3_CALLOC( baseLength + strlen(name) + strlen(suffix) +
                                strlen(extension) + 2, sizeof(char) );
    C3_ASSERT( *buffer );
  
    if( path )
    {
        strcpy( *buffer, path );
        strcat( *buffer, "\\" ); 
    }
    strcat( *buffer, name );
    strcat( *buffer, suffix );
    strcat( *buffer, "." );
    strcat( *buffer, extension );
}


/*---------------------------------------------------------------------------*
  Name:         C3MakePathForwardSlash

  Description:  Replaces the backslashes with forward slashes in filePath.

  Arguments:    filePath - file and path

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3MakePathForwardSlash( char* filePath )
{
    char* cPtr;

    if( !filePath )
        return;

    cPtr = filePath;
    while( (cPtr = strchr(cPtr, '\\')) != NULL )
    {
        *cPtr = '/';
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3WritePadded32bit

  Description:  Writes a buffer to the the file specified padding to a 32 BIT
                boundary.  If buffer is NULL or size is 0, nothing will be 
                written to the file, not even the padding to the 32 BIT boundary.

  Arguments:    buffer  - buffer to write
                size    - size of the buffer
                outFile - file to write to

  Returns:      Actual byte count written to the file
/*---------------------------------------------------------------------------*/
u32
C3WritePadded32bit( void* buffer, u32 size, FILE* outFile )
{
    u32 written      = 0;
    u32 mod          = 0;
    u8  modBuffer[4] = {0, 0, 0, 0};


    if( size > 0 && buffer )
    {
        fwrite( buffer, size, 1, outFile );
        written = size;

        mod = size % 4;
        if( mod )
        {
            written += ( 4 - mod );
            fwrite( (Ptr)modBuffer, 1, 4 - mod, outFile ); 
        }
    }

    return written;
}


/*---------------------------------------------------------------------------*
  Name:         C3WritePadded32byte

  Description:  Writes a buffer to the the file specified padding to a 32 BYTE
                boundary.  If buffer is NULL or size is 0, then NOPS will be
                written to the file until the file is 32 BYTE padded (file size
                should be a multiple of 32 bytes).

  Arguments:    buffer  - buffer to write
                size    - size of the buffer
                outFile - file to write to

  Returns:      Actual byte count written to the file
/*---------------------------------------------------------------------------*/
u32
C3WritePadded32byte( void* buffer, u32 size, FILE* outFile )
{
    u32 written      = 0;
    u32 mod          = 0;
    u8  modBuffer[32];


    if( size > 0 && buffer )
    {
        fwrite( buffer, size, 1, outFile );
        written = size;
    }

    mod = ftell( outFile ) % 32;
    if( mod )
    {
        memset( modBuffer, 0, 32 );
        written += ( 32 - mod );
        fwrite( (Ptr)modBuffer, 1, 32 - mod, outFile ); 
    }

    return written;
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteU32

  Description:  Write a u32 number with the proper endian

  Arguments:    num     - number to write
                buffer  - location to write to

  Returns:      NONE
/*---------------------------------------------------------------------------*/
u8
C3WriteU32( u32 num, void* buffer )
{
    if( C3GetOptionOutputEndian() == C3_OUTPUT_BIG_ENDIAN )
    {
        ((u8*)buffer)[0] = ((u8*)&num)[3];
        ((u8*)buffer)[1] = ((u8*)&num)[2];
        ((u8*)buffer)[2] = ((u8*)&num)[1];
        ((u8*)buffer)[3] = ((u8*)&num)[0];
    }
    else
        *((u32*)buffer) = num;

    return sizeof(u32);
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteU16

  Description:  Write a u16 number with the proper endian

  Arguments:    num     - number to write
                buffer  - location to write to

  Returns:      number of bytes written
/*---------------------------------------------------------------------------*/
u8
C3WriteU16( u16 num, void* buffer )
{
    if( C3GetOptionOutputEndian() == C3_OUTPUT_BIG_ENDIAN )
    {
        ((u8*)buffer)[0] = ((u8*)&num)[1];
        ((u8*)buffer)[1] = ((u8*)&num)[0];
    }
    else
        *((u16*)buffer) = num;

    return sizeof(u16);
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteFloat

  Description:  Write a float number with the proper endian

  Arguments:    num     - number to write
                buffer  - location to write to

  Returns:      number of bytes written
/*---------------------------------------------------------------------------*/
u8
C3WriteFloat( float num, void* buffer )
{
    if( C3GetOptionOutputEndian() == C3_OUTPUT_BIG_ENDIAN )
    {
        ((u8*)buffer)[0] = ((u8*)&num)[3];
        ((u8*)buffer)[1] = ((u8*)&num)[2];
        ((u8*)buffer)[2] = ((u8*)&num)[1];
        ((u8*)buffer)[3] = ((u8*)&num)[0];
    }
    else
        *((float*)buffer) = num;

    return sizeof(float);
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteQuantizeFloat

  Description:  Quantize the float number and write with the proper endian

  Arguments:    num       - number to write
                buffer    - location to write to
                quantInfo - used to quantize float

  Returns:      number of bytes written
/*---------------------------------------------------------------------------*/
u8
C3WriteQuantizeFloat( float num, void* buffer, u8 quantInfo )
{
    u8 temp[4];

    switch( C3_QUANT_TYPE(quantInfo) )
    {
    case GX_F32:
        return C3WriteFloat( num, buffer );
    case GX_S16:
    case GX_U16:
        C3QuantizeFloat( temp, quantInfo, num );
        return C3WriteU16( *((u16*)temp), buffer );
    case GX_S8:
    case GX_U8:
        C3QuantizeFloat( temp, quantInfo, num );
        *((u8*)buffer) = temp[0];
        return sizeof(u8);
    }

    return 0;
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteU32F

  Description:  Writes a unsigned 32 bits number in the specified file.
                Uses C3WriteU32 to convert the number to the proper endian.

  Arguments:    num     - number to write
                outFile - file to write to

  Returns:      NONE
/*---------------------------------------------------------------------------*/
u8
C3WriteU32F( u32 num, FILE* outFile )
{
    u32 temp;
 
    C3WriteU32( num, &temp );
    fwrite( (void*)&temp, sizeof(u32), 1, outFile );

    return sizeof(u32);
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteU16F

  Description:  Writes a unsigned 16 bits number in the specified file.
                Uses C3WriteU16 to convert the number to the proper endian.

  Arguments:    num     - number to write
                outFile - file to write to

  Returns:      NONE
/*---------------------------------------------------------------------------*/
u8
C3WriteU16F( u16 num, FILE* outFile )
{
    u16 temp;
 
    C3WriteU16( num, &temp );
    fwrite( (void*)&temp, sizeof(u16), 1, outFile );

    return sizeof(u16);
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteFloatF

  Description:  Writes a float number in the specified file.
                Uses C3WriteFloat to convert the number to the proper endian.

  Arguments:    num     - number to write
                outFile - file to write to

  Returns:      NONE
/*---------------------------------------------------------------------------*/
u8
C3WriteFloatF( float num, FILE* outFile )
{
    float temp;
 
    C3WriteFloat( num, &temp );
    fwrite( (void*)&temp, sizeof(float), 1, outFile );

    return sizeof(float);
}


/*---------------------------------------------------------------------------*
  Name:         C3GetQuantTypeSize

  Description:  Returns the size in bytes of the quantization type

  Arguments:    quantInfo - quantization type

  Returns:      size in bytes
/*---------------------------------------------------------------------------*/
u8
C3GetQuantTypeSize( u8 quantInfo )
{
    switch( C3_QUANT_TYPE(quantInfo) )
    {
    case GX_F32:
        return sizeof(float);
    case GX_S16:
    case GX_U16:
        return sizeof(u16);
    case GX_S8:
    case GX_U8:
        return sizeof(u8);
    default:
        C3_ASSERT( C3_FALSE );
    }

    return 0;
}


/*---------------------------------------------------------------------------*
  Name:         C3SwapBytes

  Description:  Creates a new array of the size specified and copies the 
                array, swapping the bytes for elements with 2 or 4 bytes
                Then swap the arrays

  Arguments:    src      - src array
                dst      - destination array
                size     - size of the array in bytes
                compSize - size of the component in the array

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SwapBytes( void** src, void** dst, u32 size, u32 compSize )
{
    u8*     buff   = NULL;
    u8*     tmp    = NULL;
    u8*     cursor = NULL; 
    u32     i       = 0;    

    buff = tmp = (u8*)C3_CALLOC( size, sizeof(u8));
    C3_ASSERT( tmp );

    cursor = (u8*)(*src);
    for( i = 0; i < size; i += compSize )
    {
        switch( compSize )
        {
            case 2:
                tmp[0] = cursor[1];
                tmp[1] = cursor[0];
                break;

            case 3:
                tmp[0] = cursor[0];
                tmp[1] = cursor[1];
                tmp[2] = cursor[2];
                break;
            
            case 4:
                tmp[0] = cursor[3]; 
                tmp[1] = cursor[2]; 
                tmp[2] = cursor[1]; 
                tmp[3] = cursor[0]; 
                break;

            default:
                C3_ASSERT( C3_FALSE );
                break;
        }
        tmp    = tmp + compSize;
        cursor = cursor + compSize;
    }

    *dst = *src;
    *src = (void*)buff;
}


/*---------------------------------------------------------------------------*
  Name:         C3NextUniqueIndex

  Description:  In an indexed list, finds the next unique position after
                current based upon the index.  Not necessary, but assumes
                that the indices in the list are sorted.

  Arguments:    current - points to current indexed link
                  
  Returns:      Next unique index in list
/*---------------------------------------------------------------------------*/
void*
C3NextUniqueIndex( C3IndexedLink* current )
{
    u32   prev;
    void* cursor;

    // Move to the next unique element after current
    prev = current->index;
    cursor = current->link.Next;
    while( cursor && prev == ((C3IndexedLink*)cursor)->index )
    {
        cursor = ((C3IndexedLink*)cursor)->link.Next;
    }

    return cursor;
}


/*---------------------------------------------------------------------------*
  Name:         C3InitStringTable

  Description:  Initializes the string table.

  Arguments:    NONE
                  
  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3InitStringTable( )
{
    C3String string;

    stringTable = C3_CALLOC( sizeof(DSHashTable), 1 );
    stringTableList = C3_CALLOC( sizeof(DSList), C3_STRING_TABLE_SIZE );
    C3_ASSERT( stringTableList );

    DSInitHTable( stringTable, C3_STRING_TABLE_SIZE, stringTableList,
                  C3HashString, (void*)&string, &string.link );
}


/*---------------------------------------------------------------------------*
  Name:         C3CleanStringTable

  Description:  Cleans the string table.

  Arguments:    NONE
                  
  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3CleanStringTable( )
{
    C3String *current;
    s32  index;
//  FILE *outFile = fopen( "D:\\temp\\stringTable.txt", "wt" );
//  s32  temp;

    // Free each string
    for( index = 0; index < C3_STRING_TABLE_SIZE; index++ )
    {
        current = (C3String*)stringTableList[index].Head;
        while( current )
        {
//          temp = DSHTableIndex( stringTable, (void*)current );
//          fprintf( outFile, "Index: %d Hash value: %d String: %s\n", index, temp, current->str );

            C3_FREE( current->str );
            current->str = NULL;
            current = (C3String*)current->link.Next;
        }   
    }

    /*
    // Works just as fine, but in case strings get modified in string table
    // (which should never ever happen), the bottom will be less likely to free everything.
    current = (C3String*)DSNextHTableObj( stringTable, NULL );
    while( current )
    {
        index = DSHTableIndex( stringTable, (void*)current );
        fprintf( outFile, "%d: %s\n", index, current->str );

        next = (C3String*)DSNextHTableObj( stringTable, (void*)current );
        C3_FREE( current->str );
        current->str = NULL;
        current = next;
    }
    */

//  fclose( outFile );

    C3_FREE( stringTableList );
    C3_FREE( stringTable );
}


/*---------------------------------------------------------------------------*
  Name:         C3HashString

  Description:  Hash function for the string table.

  Arguments:    string - string to hash (C3String* or char**)

  Returns:      index for the hash table 
/*---------------------------------------------------------------------------*/
u16 
C3HashString( void* string )
{
    u32 checksum = 0;
    u32 i;
    u32 length;
    char* fileName = NULL;

    if ( !string )
        return 0;

    fileName = *((char**)string);
    if ( !fileName )
        return 0;

    length = strlen( fileName );
    for ( i = 0; i < length; i++ )
    {
        checksum += (u32)fileName[i];
    }

    return (u16)(checksum % C3_STRING_TABLE_SIZE);
}


/*---------------------------------------------------------------------------*
  Name:         C3GetStringInTable

  Description:  Returns a string in the table, or NULL if it is not in the table
                
  Arguments:    string - string to find
                  
  Returns:      pointer to the string in the table
/*---------------------------------------------------------------------------*/
char*
C3GetStringInTable( char* string )
{
    s32 index; 
    C3String* current; 

    C3_ASSERT( string );

    index = DSHTableIndex( stringTable, (void*)&string );
    current = DSHTableHead( stringTable, index );

    while( current )
    {
        if( !strcmp( current->str, string ) )
            return current->str;

        current = (C3String*)current->link.Next;
    }

    return NULL;
}


/*---------------------------------------------------------------------------*
  Name:         C3AddStringInTable

  Description:  If string does not exist in the table, adds it to the string
                table.  Returns the pointer to the string in the table.
                
  Arguments:    string - string to add/find
                  
  Returns:      pointer to string in the table
/*---------------------------------------------------------------------------*/
char*
C3AddStringInTable( char* string )
{
    char* foundString;
    C3String* newString;

    // See if string is already in the table
    foundString = C3GetStringInTable( string );
    if( foundString )
    {
        return foundString;
    }

    // Otherwise, add to the string table
    newString = C3PoolString();
    C3StrCpy( &newString->str, string );

    DSInsertHTableObj( stringTable, (void*)newString );

    return newString->str;
}
