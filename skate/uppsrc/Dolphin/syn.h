/*---------------------------------------------------------------------------*
  Project:  Synth application for AX
  File:     syn.h

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/dolphin/syn.h $
    
    2     8/16/01 12:27p Billyjack
    added zeroBuffer offset to API
    
    1     5/09/01 1:28p Billyjack
    created
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __SYN_H__
#define __SYN_H__

#include "wt.h"

#if 0
extern "C" {
#endif

#define SYN_INPUT_BUFFER_SIZE      256

typedef struct SYNSYNTH
{
    void        *next;                  // synth list

    WTINST      *percussiveInst;        // pointer to instrument[0] in wavetable
    WTINST      *melodicInst;           // pointer to instrument[0] in wavetable
    WTREGION    *region;                // pointer to region[0] in wavetable
    WTART       *art;                   // pointer to articulation[0] in wavetable
    WTSAMPLE    *sample;                // pointer to sample[0] in wavetable
    WTADPCM     *adpcm;                 // pointer to adpcm[0] in wavetable
    u32         aramBaseWord;           // base address (16bit) of samples in ARAM
    u32         aramBaseByte;           // base address (8bit)of samples in ARAM
    u32         aramBaseNibble;         // base address (nibble) of samples in ARAM
    u32         zeroBaseWord;           // base address (16bit) of zero buffer in ARAM
    u32         zeroBaseByte;           // base address (8bit)of zero buffer in ARAM
    u32         zeroBaseNibble;         // base address (nibble) of zero buffer in ARAM
    u32         priorityVoiceAlloc;     // priority for allocating new note
    u32         priorityNoteOn;         // priority for notes that are on
    u32         priorityNoteRelease;    // peiority for nots being released

    WTINST      *inst       [16];       // pointer to instrument per channel

    s32         masterVolume;           // master volume for synth             

    u8          controller  [16][128];  // MIDI controller registers

    u8          rpn         [16];       // weather to enter rpn or nrpn data
    s16         dataEntry   [16];       // data entry value

    s32         pwMaxCents  [16];       // pitch wheel cents at + max
    s32         pwCents     [16];       // current pitch wheel cents
    
    s32         volAttn     [16];       // MIDI channel volume
    s32         expAttn     [16];       // expression volume
    s32         auxAAttn    [16];       // aux A (reverb)
    s32         auxBAttn    [16];       // aux B (chorus)

    u8          input[SYN_INPUT_BUFFER_SIZE][3];
    u8          *inputPosition;
    u32         inputCounter;
    
    u32         notes;                  // notes running

    void        *keyGroup[16][16];      // storage for key group notes
    void        *voice[16][128];         // storage for voices index references         

} SYNSYNTH;


void	SYNInit				(void);
void	SYNQuit				(void);
void    SYNRunAudioFrame    (void);

void    SYNInitSynth        (
                             SYNSYNTH *synth,
                             void *wavetable,
                             u32 aramBase,
                             u32 zeroBase,
                             u32 priorityVoiceAlloc,
                             u32 priorityNoteOn,
                             u32 priorityNoteRelease
                             );

void    SYNQuitSynth        (SYNSYNTH *synth);
void    SYNMidiInput        (SYNSYNTH *synth, u8 *input);
u8      SYNGetMidiController(SYNSYNTH *synth, u8 midiChannel, u8 function);
void    SYNSetMasterVolume  (SYNSYNTH *synth, s32 dB);
s32     SYNGetMasterVolume  (SYNSYNTH *synth);
u32     SYNGetActiveNotes   (SYNSYNTH *synth);


#if 0
}
#endif

#endif // __SYN_H__ 
