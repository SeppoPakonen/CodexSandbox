/*---------------------------------------------------------------------------*
  Project: OS - Fast F32 cast using gekko
  File:    OSFastCast.h

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/dolphin/os/OSFastCast.h $
    
    7     6/11/01 7:52p Tian
    Integrated SN changes
    
    6     4/17/01 5:29p Tian
    Changed all inlines to static inline
    
    5     7/25/00 7:43p Tian
    Updated to avoid GQR1, so we do not collide with Metrowerks' proposed
    Gekko ABI.
    
    4     3/28/00 12:05p Tian
    Fixed typo that was accidentally checked in.  It was Paul's fault.
    
    3     3/27/00 4:28p Tian
    Ifdefed out for win32
    
    2     3/27/00 2:25p Tian
    Fixed bug in non-gekko code
    
    1     3/27/00 2:00p Tian
    Initial checkin.
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __OSFASTCAST_H__
#define __OSFASTCAST_H__

#ifndef _WIN32


#if 0
extern "C" {
#endif
#ifdef GEKKO

// GQR formats we use for fast cast
#define OS_GQR_U8  0x0004 // GQR 1
#define OS_GQR_U16 0x0005 // GQR 2
#define OS_GQR_S8  0x0006 // GQR 3
#define OS_GQR_S16 0x0007 // GQR 4

// The GQRs that we use.  Note that in the future, the compiler will
// reserve GQRs 0 and 1, so we avoid using GQR1.

#define OS_FASTCAST_U8  2
#define OS_FASTCAST_U16 3
#define OS_FASTCAST_S8  4
#define OS_FASTCAST_S16 5

// This initializes the fast casting facility.
// GQRs 1-4 are set to load/store u8, u16, s8, s16 respectively
static inline void OSInitFastCast ( void )
{
#ifdef __SN__
	      asm volatile ("
		li      3, 0x0004
        oris    3, 3, 0x0004
        mtspr   GQR2, 3

        li      3, 0x0005
        oris    3, 3, 0x0005
        mtspr   GQR3, 3

        li      3, 0x0006
        oris    3, 3, 0x0006
        mtspr   GQR4, 3

        li      3, 0x0007
        oris    3, 3, 0x0007
        mtspr   GQR5, 3
    " : : : "r3" );
#else
    asm
    {
        li      r3, OS_GQR_U8
        oris    r3, r3, OS_GQR_U8
        mtspr   GQR2, r3

        li      r3, OS_GQR_U16
        oris    r3, r3, OS_GQR_U16
        mtspr   GQR3, r3

        li      r3, OS_GQR_S8
        oris    r3, r3, OS_GQR_S8
        mtspr   GQR4, r3

        li      r3, OS_GQR_S16
        oris    r3, r3, OS_GQR_S16
        mtspr   GQR5, r3
    }
#endif
}
    

/*---------------------------------------------------------------------------*
  int to float
 *---------------------------------------------------------------------------*/

#ifdef __SN__

#define OSu8tof32(in,out)	asm volatile ("psq_l   %0, 0(%1), 1, 2	" : "=f" (*(out)) : "b" (in) )
#define OSu16tof32(in,out)	asm volatile ("psq_l   %0, 0(%1), 1, 3	" : "=f" (*(out)) : "b" (in) )
#define OSs8tof32(in,out)	asm volatile ("psq_l   %0, 0(%1), 1, 4	" : "=f" (*(out)) : "b" (in) )
#define OSs16tof32(in,out)	asm volatile ("psq_l   %0, 0(%1), 1, 5	" : "=f" (*(out)) : "b" (in) )

#else

static inline void OSu8tof32(u8* in, f32* out)
{
    asm 
    {
        psq_l   fp1, 0(in), 1, OS_FASTCAST_U8
        stfs    fp1, 0(out)
    }
}

static inline void OSu16tof32(u16* in, f32* out)
{
    asm 
    {
        psq_l   fp1, 0(in), 1, OS_FASTCAST_U16
        stfs    fp1, 0(out)
    }
}

static inline void OSs8tof32(s8* in, f32* out)
{
    asm 
    {
        psq_l   fp1, 0(in), 1, OS_FASTCAST_S8
        stfs    fp1, 0(out)
    }
}

static inline void OSs16tof32(s16* in, f32* out)
{
    asm 
    {
        psq_l   fp1, 0(in), 1, OS_FASTCAST_S16
        stfs    fp1, 0(out)
    }
}

#endif
/*---------------------------------------------------------------------------*
  float to int

  Note that due to a compiler bug, we need to use addresses for the FP
  value.  Theoretically, we can simply use psq_st to store out values, but
  the compiler does not recognize the psq_st as touching static values, and may
  compile out static values.
 *---------------------------------------------------------------------------*/
#ifdef __SN__

#define OSf32tou8(in,out)	asm volatile ("psq_st   %1, 0(%0), 1, 2	" :: "b" (out) , "f" (*(in)) : "memory") 
#define OSf32tou16(in,out)	asm volatile ("psq_st   %1, 0(%0), 1, 3	" :: "b" (out) , "f" (*(in)) : "memory") 
#define OSf32tos8(in,out)	asm volatile ("psq_st   %1, 0(%0), 1, 4	" :: "b" (out) , "f" (*(in)) : "memory")
#define OSf32tos16(in,out)	asm volatile ("psq_st   %1, 0(%0), 1, 5	" :: "b" (out) , "f" (*(in)) : "memory")

#else

static inline void OSf32tou8(f32* in, u8* out)
{
    asm     
    {
        lfs      fp1, 0(in)
        psq_st   fp1, 0(out), 1, OS_FASTCAST_U8
    }
}

static inline void OSf32tou16(f32* in, u16* out)
{
    asm 
    {
        lfs      fp1, 0(in)
        psq_st   fp1, 0(out), 1, OS_FASTCAST_U16
    }
}

static inline void OSf32tos8(f32* in, s8* out)
{
    asm 
    {
        lfs      fp1, 0(in)
        psq_st   fp1, 0(out), 1, OS_FASTCAST_S8
    }
}

static inline void OSf32tos16(f32* in, s16* out)
{
    asm 
    {
        lfs      fp1, 0(in)
        psq_st   fp1, 0(out), 1, OS_FASTCAST_S16
    }
}
#endif 
    
#else
/*---------------------------------------------------------------------------*
  Non-Gekko code
 *---------------------------------------------------------------------------*/
static inline void OSInitFastCast ( void )
{
    while(0)
    {
    }
}


/*---------------------------------------------------------------------------*
  int to float
 *---------------------------------------------------------------------------*/
static inline void OSs16tof32(s16* in, f32* out)
{
    *out = (f32) *in;
}

static inline void OSs8tof32(s8* in, f32* out)
{
    *out = (f32) *in;
}


static inline void OSu16tof32(u16* in, f32* out)
{
    *out = (f32) *in;
}


static inline void OSu8tof32(u8* in, f32* out)
{
    *out = (f32) *in;
}

/*---------------------------------------------------------------------------*
  float to int
 *---------------------------------------------------------------------------*/

static inline void OSf32tou8(f32* in, u8* out)
{
    *out = (u8)*in;
}

static inline void OSf32tou16(f32* in, u16* out)
{
    *out = (u16)*in;
}

static inline void OSf32tos8(f32* in, s8* out)
{
    *out = (s8)*in;
}

static inline void OSf32tos16(f32* in, s16* out)
{
    *out = (s16)*in;
}


#endif // GEKKO

#if 0
}
#endif
#endif

#endif // _WIN32
