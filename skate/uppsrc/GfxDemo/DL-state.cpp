/*---------------------------------------------------------------------------*
  Project:  Dolphin
  File:     DL-state.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/demos/gxdemo/src/DisplayList/DL-state.c $
    
    1     3/06/00 12:08p Alligator
    move from gx/tests and rename
    
    2     2/07/00 6:23p Carl
    Enforced single-frame mode.  Also fixed copyright.
    
    1     2/07/00 6:05p Carl
    Display list test that contains state info.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <demo.h>

// This test is _not_ an example of good programming :-)

void ctmain(void);

// This test only works in single-frame mode
#ifndef __SINGLEFRAME
#define __SINGLEFRAME 0
#endif

#define main(x) ctmain(x)
#define DEMOInit(x) /*nothing*/
#define DEMOBeforeRender(x) /*nothing*/
#define DEMODoneRender(x) /*nothing*/
#undef OSHalt
#define OSHalt(x) /*nothing*/
#include "../src/Texture/tex-2-tex.c"
#undef OSHalt
#define OSHalt(msg)             OSPanic(__FILE__, __LINE__, msg)
#undef DEMODoneRender
#undef DEMOBeforeRender
#undef DEMOInit
#undef main

#define DL_MAX_SIZE     131072

/*---------------------------------------------------------------------------*
   Application main loop
 *---------------------------------------------------------------------------*/
void main ( void )
{
    u8* dlPtr;
    u32 dlSize;

    DEMOInit(NULL);
           
    dlPtr = OSAlloc(DL_MAX_SIZE);
    ASSERTMSG(dlPtr != NULL, "error allocating dlPtr");
    DCInvalidateRange( (void *) dlPtr, DL_MAX_SIZE);

    GXBeginDisplayList( (void *) dlPtr, (u32) DL_MAX_SIZE);
    ctmain();
    dlSize = GXEndDisplayList();

    DEMOBeforeRender();
    GXCallDisplayList(dlPtr, dlSize);
    DEMODoneRender();
}
