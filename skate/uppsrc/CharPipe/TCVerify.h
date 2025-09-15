/*---------------------------------------------------------------------*

Project:  tc library
File:     TCVerify.h

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:

 $Log: /Dolphin/build/charPipeline/tc/include/TCVerify.h $
    
    1     12/03/99 3:44p Ryan
    
    2     10/08/99 2:44p Mikepc
    update for tplConv portability: altered data structures, replaced
    references to 'read tga code' with ' *fileFn, removed redundant
    functions.
    
    1     9/02/99 10:58a Mikepc
    prototype for VerifyLists() ( formerly in convert.cpp ) that checks
    list contents against each other and against real files
    
  
 $NoKeywords: $

-----------------------------------------------------------------------*/

#ifndef __TCVERIFY_H__
#define __TCVERIFY_H__

/********************************/
#include <Dolphin/types.h>

/*>*******************************(*)*******************************<*/
void TCVerifyLists	( void );

/*>*******************************(*)*******************************<*/

#endif  // __TCVERIFY_H__