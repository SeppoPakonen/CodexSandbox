/*---------------------------------------------------------------------------*
  Project:  Dolphin SP library
  File:     sp.h

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.
 *---------------------------------------------------------------------------*/

#ifndef __SP_H__
#define __SP_H__

#if 0
extern "C" {
#endif


typedef struct
{
    AXPBADPCM       adpcm;
    AXPBADPCMLOOP   adpcmloop;

} SPAdpcmEntry;


typedef struct
{
    u32             type;
    u32             sampleRate;
    u32             loopAddr;
    u32             loopEndAddr;
    u32             endAddr;
    u32             currentAddr;
    SPAdpcmEntry    *adpcm;

} SPSoundEntry;


#define SP_TYPE_ADPCM_ONESHOT   0
#define SP_TYPE_ADPCM_LOOPED    1
#define SP_TYPE_PCM16_ONESHOT   2
#define SP_TYPE_PCM16_LOOPED    3
#define SP_TYPE_PCM8_ONESHOT    4
#define SP_TYPE_PCM8_LOOPED     5


typedef struct
{

    u32             entries;
    SPSoundEntry    sound[1];

} SPSoundTable;



void SPInitSoundTable(SPSoundTable *table, u32 aramBase, u32 zeroBase);
SPSoundEntry * SPGetSoundEntry(SPSoundTable *table, u32 index);
void SPPrepareSound(SPSoundEntry *sound, AXVPB *axvpb, u32 sampleRate);
void SPPrepareEnd(SPSoundEntry *sound, AXVPB *axvpb);


#if 0
}
#endif

#endif // __SP_H__ 
