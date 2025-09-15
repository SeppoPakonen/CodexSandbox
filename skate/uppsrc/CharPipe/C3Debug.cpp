/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3Debug.c]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/c3/src/C3Debug.c $
    
    6     11/13/00 1:37p John
    Removed newlines from all C3ReportError strings.
    
    5     2/29/00 7:21p John
    Cleaned up header.
    
    4     2/04/00 6:06p John
    Untabified code.
    
    3     1/31/00 4:15p John
    Added pruning of unnecessary bones and animations.
    Finished stitching with FIFO matrix cache and stripping.
    
    2     1/20/00 1:14p John
    Added stitching (work in progress).
    Added C3 string table for efficient management of strings.
    
    11    9/29/99 4:28p John
    Changed header to make them all uniform with logs.
    
    10    9/21/99 6:40p John
    Minor code change.
    
    9     9/17/99 9:34a John
    Added C3Options and cleaned up code.

  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#include "C3Private.h"

#define C3_DEBUG_REPORT    "report.txt"
#define C3_MAX_MEM_BUFFER  150000

#define C3_DBG_FREE(a)  {if(a){free(a);a = NULL;}} 

// Local types
typedef struct C3MemBuffer
{
  u32   next;

  char* allocFile;
  u32   allocLine;
  char* freeFile;
  u32   freeLine;
  Ptr   buffer;
  u32   size;
  u32   address;

} C3MemBuffer, *C3MemBufferPtr;


// Local Functions 
static u32          C3GetMemBufferIndex( void );
static C3MemBuffer* C3GetMemBuffer( void* ptr );
static void         C3DbgStrCpy( char** dest, char* src );
static void         C3StampDebugReport( void );
static void         C3PrintDebugInfo( C3MemBuffer* memBuffer );


// Variables
C3AssertFunc      C3Assert = NULL;
C3DebugPrintFunc  c3DebugPrint = (C3DebugPrintFunc)printf;

C3MemBuffer       c3MemBuffers[ C3_MAX_MEM_BUFFER ];
u32               c3MemBufferIdx  = 0;


/*---------------------------------------------------------------------------*
  Name:         C3InitDebug

  Description:  initialize the default values of the globals used in the
                debuging module

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3InitDebug( void )
{
  u32 i = 0;

  c3MemBufferIdx = 0;
  for( i = 0; i < C3_MAX_MEM_BUFFER; i++ )
  {
    c3MemBuffers[i].next      = -1;
    c3MemBuffers[i].allocFile = NULL;
    c3MemBuffers[i].allocLine = 0;
    c3MemBuffers[i].freeFile  = NULL;
    c3MemBuffers[i].freeLine  = 0;
    c3MemBuffers[i].buffer    = NULL;
    c3MemBuffers[i].size      = 0;
    c3MemBuffers[i].address   = 0;
  }
} 


/*---------------------------------------------------------------------------*
  Name:         C3CleanDebug

  Description:  Cleans up the memory allocated for the debug module

  Arguments:    verbose - if true will output a debug report of the session

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3CleanDebug( C3Bool verbose )
{
  u32 i = 0;

  if( verbose )
  {
    // stamp the file
    C3StampDebugReport( );
  }

  for( i = 0; i < c3MemBufferIdx; i++ )
  {
    if( c3MemBuffers[i].next != -1 )
    {
      if( verbose )
      {
        C3PrintDebugInfo( c3MemBuffers + i );
      }
      C3_DBG_FREE( c3MemBuffers[i].allocFile );
      C3_DBG_FREE( c3MemBuffers[i].freeFile );
      C3_DBG_FREE( c3MemBuffers[i].buffer );
    }

    c3MemBuffers[i].next      = -1;
    c3MemBuffers[i].allocFile = NULL;
    c3MemBuffers[i].allocLine = 0;
    c3MemBuffers[i].freeFile  = NULL;
    c3MemBuffers[i].freeLine  = 0;
    c3MemBuffers[i].buffer    = NULL;
    c3MemBuffers[i].size      = 0;
    c3MemBuffers[i].address   = 0;
  }
}


/*---------------------------------------------------------------------------*
  Name:         C3Calloc

  Description:  Allocate memory setting a corset if necessary

  Arguments:    fileName    - file from which it is called
                line        - line from which it is called
                callocCount - number of element to allocate
                callocSize  - size of each element

  Returns:      pointer to the block of memory allocated 
/*---------------------------------------------------------------------------*/
void*
C3Calloc( char* fileName, u32 line, u32 callocCount, u32 callocSize )
{
  C3MemBufferPtr  memBuffer = NULL;

  C3_ASSERT( callocCount > 0 );
  C3_ASSERT( callocSize > 0 );


  memBuffer = c3MemBuffers + (u32)C3GetMemBufferIndex();
  
  memBuffer->allocLine = line;
  C3DbgStrCpy( &memBuffer->allocFile, fileName ); 
  
  memBuffer->size   = ((callocCount) * (callocSize)) + sizeof(u32);
  memBuffer->buffer = calloc( 1, memBuffer->size);
  memBuffer->address = (u32)memBuffer->buffer;

  if( memBuffer->buffer != NULL )
   *((u32*)((u32)memBuffer->buffer + memBuffer->size - 4)) = (u32)memBuffer->buffer;
  
  return memBuffer->buffer;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetMemBufferIndex

  Description:  Retrieves the index of the next free buffer

  Arguments:    NONE

  Returns:      index of the next free buffer
/*---------------------------------------------------------------------------*/
static u32
C3GetMemBufferIndex( void )
{
  C3_ASSERT( c3MemBufferIdx < C3_MAX_MEM_BUFFER );
  c3MemBuffers[ c3MemBufferIdx ].next = c3MemBufferIdx;
  return c3MemBufferIdx++;
}



/*---------------------------------------------------------------------------*
  Name:         C3Free

  Description:  Free the memory, checking corset if necessary

  Arguments:    fileName - file from which it is called
                line     - line at which it is called
                ptr      - ptr to free

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3Free( char* fileName, u32 line, void* ptr)
{
  C3MemBufferPtr  memBuffer = NULL;

  C3_ASSERT( fileName && line );

  if( (ptr) != NULL )
  {                     
    memBuffer = C3GetMemBuffer( ptr );
    if( memBuffer == NULL )
    {
      // return an error, trying to free memory not allocated in
      // module using C3_CALLOC()
      C3ReportError( "Freeing memory not allocated with C3_CALLOC. %s at line %d", fileName, line );
      C3_DBG_FREE( ptr );
      return;
    }
  
    memBuffer->freeLine = line; 
    C3DbgStrCpy( &memBuffer->freeFile, fileName ); 
  
    if( *((u32*)((u32)memBuffer->buffer + memBuffer->size - 4)) != ((u32)memBuffer->buffer))
    {
      u32 corset = 0;
      u32 busted = 1;

      // corset busted, memory allocated at line in file
      C3_ASSERT( corset == busted );
    }
    
    C3_DBG_FREE( memBuffer->buffer );
  }
}


/*---------------------------------------------------------------------------*
  Name:         C3GetMemBuffer

  Description:  Searches the memory buffers for the one containing the specified
                pointer

  Arguments:    ptr - pointer to search for

  Returns:      the debug memory buffer
/*---------------------------------------------------------------------------*/
static C3MemBuffer*
C3GetMemBuffer( void* ptr )
{
  C3MemBuffer*  cursor = c3MemBuffers;
  s32           i      = 0;

  for( i = c3MemBufferIdx; i >= 0; i-- )
  {
    if( c3MemBuffers[i].buffer == ptr )
    {
      return c3MemBuffers + i;
    }
  }

  // error couldn't find the buffer...
  return NULL;
}


/*---------------------------------------------------------------------------*
  Name:         C3DbgStrCpy

  Description:  Allocate a buffer in dest and copy the src string in the 
                buffer.
                src must be a null terminated string

  Arguments:    dest - destination where the memory will be allocated
                src  - source string to copy

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void  
C3DbgStrCpy( char** dest, char* src )
{
  C3_ASSERT( dest );
  C3_ASSERT( src );

  *dest = (char*)calloc( strlen(src) + 1, sizeof(char));
  assert( *dest != NULL );

  strcpy( *dest, src );
}


/*---------------------------------------------------------------------------*
  Name:         C3StampDebugReport

  Description:  Prints the header for the debug report file

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3StampDebugReport( void )
{
  FILE*   file = NULL;
  time_t  tp;


  file = fopen( C3_DEBUG_REPORT, "a+");
  if( !file )
    return;

  tp = time(NULL);

  // write date
  fprintf(file, "\n\n");
  fprintf(file, "******************************************************************************\n");
  fprintf(file, "C3Lib Debug Report\n" );
  fprintf(file, "%s", ctime(&tp) );    
  fprintf(file, "******************************************************************************\n");
  fprintf(file, "------------------------------------------------------------------------------\n");
  // write header
  fprintf(file, "                             ____Allocation____   _____Release______\n");
  fprintf(file, " Index   Address     Size    File          Line   File          Line \n");
  fprintf(file, "------------------------------------------------------------------------------\n");

  fclose( file );

}


/*---------------------------------------------------------------------------*
  Name:         C3PrintDebugInfo

  Description:  Prints the debug information for the specified memBuffer

  Arguments:    memBuffer - memory buffer to print the information for

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3PrintDebugInfo( C3MemBuffer* memBuffer )
{
  FILE* file      = NULL;
  char  lNull[13] = "-->(NULL)<--";
  char* freeFile  = lNull;


  file = fopen( C3_DEBUG_REPORT, "a+");
  if( !file )
    return;

  if( memBuffer->freeFile )
    freeFile = strrchr( memBuffer->freeFile, '\\' ) + 1;

  fprintf(file, " %-6d  %-0#8x  %8d  %-12.12s  %4d   %-12.12s  %4d\n", 
                  memBuffer - c3MemBuffers,
                  memBuffer->address,
                  memBuffer->size - 4,
                  strrchr( memBuffer->allocFile, '\\' ) + 1,
                  memBuffer->allocLine,
                  freeFile,
                  memBuffer->freeLine );

  fclose( file );
}

/*---------------------------------------------------------------------------*
  Name:         C3DebugPrint

  Description:  Print a debug message
                Assumes the global debug print function pointer has been set

  Arguments:    fmt - formatted variable length argument (printf style)
                
  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3DebugPrint( char* fmt, ... )
{
  va_list ap;
  char buffer[256];


  C3_ASSERT( c3DebugPrint );

  
  va_start( ap, fmt );
  vsprintf( buffer, fmt, ap ); 
  va_end( ap );

  c3DebugPrint( buffer );
  
}


/*---------------------------------------------------------------------------*
  Name:         C3SetDebugPrintFunc

  Description:  Set the print function for the debug print

  Arguments:    dp - pointer to the function used to output debug information 

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SetDebugPrintFunc( C3DebugPrintFunc dp )
{ 
  C3_ASSERT( dp );
  c3DebugPrint = dp;
}
