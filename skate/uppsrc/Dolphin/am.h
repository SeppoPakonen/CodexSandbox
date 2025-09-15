/*---------------------------------------------------------------------------*
  Project:  ARAM manager for AX Demos
  File:     am.h

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/dolphin/am.h $
    
    2     9/04/01 12:01a Eugene
    First version!
    
    1     8/31/01 2:58p Eugene
    ARAM Manager for audio tasks. Demonstration code. 
    
    1     8/29/01 12:03p Billyjack
    created
    
    2     8/17/01 10:59a Billyjack
    changed AMLoadFile() API
    
    1     8/16/01 12:24p Billyjack
    created

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

typedef void (*AMCallback)(char *path);


#define AM_STACK_ENTRIES         16
#define AM_ZEROBUFFER_BYTES     256

#define AMPushBuffered(p,b,s)           __AMPushBuffered(p,b,s,NULL,FALSE)
#define AMPushBufferedAsync(p,b,s,cb)   __AMPushBuffered(p,b,s,cb,TRUE)


void    AMInit              (u32 aramBase, u32 aramBytes);
u32     AMGetZeroBuffer     (void);
u32     AMGetReadStatus     (void);
u32     AMGetFreeSize       (void);
u32     AMGetStackPointer   (void);

void   *AMLoadFile          (char *path, u32 *length);

void    AMPop               (void);

u32     AMPush              (char *path);
u32     AMPushData          (void *buffer, u32 length);

u32   __AMPushBuffered      (char *path, void *buffer, u32 buffer_size, AMCallback callback, BOOL async_flag);


