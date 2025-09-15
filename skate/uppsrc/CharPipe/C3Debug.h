/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3Debug.h]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/charPipeline/c3/C3Debug.h $
    
    2     2/04/00 6:06p John
    Untabified code.
    
    6     7/23/99 5:35p Rod
    Fixed Free code to check for null pointer, 
    
    5     6/02/99 11:55a Rod
    Cosmetic changes Ptr to void*

  $NoKeywords: $

 *---------------------------------------------------------------------------*/


#ifndef _C3DEBUG_H
#define _C3DEBUG_H


/*>*******************************(*)*******************************<*/
// Defines

// Types
typedef void(*C3AssertFunc)     ( char*, u32, char* );
typedef int (*C3DebugPrintFunc) ( char*, ... );

#if 0
extern "C" {
#endif // __cplusplus

// Functions
void  C3InitDebug( void );
void  C3CleanDebug( C3Bool verbose );

void* C3Calloc( char* fileName, u32 line, u32 callocCount, u32 callocSize );
void  C3Free( char* fileName, u32 line, void* ptr);
void  C3DebugPrint( char* fmt, ... );
void  C3SetDebugPrintFunc( C3DebugPrintFunc dp );


// Variables
extern C3AssertFunc     C3Assert; // Can be replaced to handle other assert


#if 0
}
#endif // __cplusplus


/*>*******************************(*)*******************************<*/
// Macros

/*>*******************************(*)*******************************<*/

/*---------------------------------------------------------------------------*
  Name:         C3_ASSERT

  Description:  Assertion call the overidden assertion if present otherwise
                call the standard assert

  Arguments:    exp - expression to assert

  Returns:      -
/*---------------------------------------------------------------------------*/
#ifdef C3_DEBUG 
  #define C3_ASSERT( exp ) if(C3Assert){if(!(exp)){C3Assert(__FILE__, __LINE__, #exp);}}else{assert((exp));}
#else
  #define C3_ASSERT( exp ) assert((exp))
#endif


/*---------------------------------------------------------------------------*
  Name:         C3_CALLOC

  Description:  Allocate memory setting a corset if necessary

  Arguments:    callocCount - number of elements to allocate
                callocSize  - size of the elements to allocate  

  Returns:      -
/*---------------------------------------------------------------------------*/
#ifdef C3_DEBUG 
  #define C3_CALLOC(callocCount,callocSize) C3Calloc( __FILE__, __LINE__, callocCount, callocSize ) 
#else
  #define C3_CALLOC(callocCount,callocSize) calloc( callocCount, callocSize )
#endif // C3_DEBUG


/*---------------------------------------------------------------------------*
  Name:         C3_FREE

  Description:  Free the memory, checking corset if necessary

  Arguments:    ptr - pointer to the memory to free

  Returns:      -
/*---------------------------------------------------------------------------*/
#ifdef C3_DEBUG
 #define C3_FREE(ptr){ C3Free( __FILE__, __LINE__, (ptr)); (ptr) = NULL; }
#else
#define C3_FREE(ptr) { if(ptr){free( (ptr) ); (ptr) = NULL;} }
#endif // C3_DEBUG


#endif // _C3DEBUG_H