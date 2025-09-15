/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3AnmOut.h]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/charPipeline/c3/C3AnmOut.h $
    
    3     2/04/00 6:05p John
    Untabified code.
    
    2     12/08/99 7:22p John
    Added animation quantization.
    
    4     9/29/99 4:23p John
    Changed header to make them all uniform with logs.
    
  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef _C3ANMOUT_H
#define _C3ANMOUT_H

#if 0
extern  "C" {
#endif //__cplusplus

void    C3WriteAnimation( char* path, char* name );
u8      C3WriteControl( C3Control* control, void* array, C3Bool trueControl, u8 quantInfo );

#if 0
}
#endif //__cplusplus

#endif // _C3ANMOUT_H

