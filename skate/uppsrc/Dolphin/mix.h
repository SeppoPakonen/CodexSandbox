/*---------------------------------------------------------------------------*
  Project:  Mixer application for AX
  File:     mix.h

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/dolphin/mix.h $
    
    2     5/30/01 3:09p Billyjack
    added definitions for MIXSetSoundMode modes (stereo / mono)
    
    1     5/09/01 1:28p Billyjack
    created
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __MIX_H__
#define __MIX_H__

#if 0
extern "C" {
#endif


/*---------------------------------------------------------------------------*
    mixer sound mode definitions
 *---------------------------------------------------------------------------*/
#define MIX_SOUND_MODE_MONO         OS_SOUND_MODE_MONO
#define MIX_SOUND_MODE_STEREO       OS_SOUND_MODE_STEREO


/*---------------------------------------------------------------------------*
    mixer channel mode definitions
 *---------------------------------------------------------------------------*/
#define MIX_MODE_AUXA_PREFADER      0x00000001
#define MIX_MODE_AUXB_PREFADER      0x00000002
#define MIX_MODE_MUTE               0x00000004


#define MIX_MODE_UPDATE_INPUT       0x10000000    
#define MIX_MODE_UPDATE_INPUT1      0x20000000    
#define MIX_MODE_UPDATE_MIX         0x40000000    
#define MIX_MODE_UPDATE_MIX1        0x80000000    


/*---------------------------------------------------------------------------*
 *---------------------------------------------------------------------------*/
typedef struct MIXChannel
{
    AXVPB   *axvpb;
    
    u32     mode;           

    int     input;
    int     auxA;               // AUX send A
    int     auxB;               // AUX send B
    int     pan;                // 0 - 127 Left - Right
    int     span;               // 0 - 127 Front - Back
    int     fader;              // fader

    int     l;                  // left
    int     r;                  // right
    int     f;                  // front
    int     b;                  // back

    u16     v;
    u16     v1;
    u16     vL;            
    u16     vL1;            
    u16     vR;
    u16     vR1;
    u16     vS;
    u16     vS1;
    u16     vAL;
    u16     vAL1;
    u16     vAR;
    u16     vAR1;
    u16     vAS;
    u16     vAS1;
    u16     vBL;
    u16     vBL1;
    u16     vBR;
    u16     vBR1;
    u16     vBS;
    u16     vBS1;
    

} MIXChannel;


/*---------------------------------------------------------------------------*
    Exposed function prototypes
 *---------------------------------------------------------------------------*/
void	MIXInit				(void);
void	MIXQuit				(void);
void    MIXSetSoundMode     (u32 mode);

void    MIXInitChannel      (
                             AXVPB *p,      // pointer to voice
                             u32 mode,      // initial aux A, B, mute modes
                             int input,     // initial input atten / gain
                             int auxA,      // initial aux A atten / gain
                             int auxB,      // initial aux B atten / gain
                             int pan,       // initial pan 
                             int span,      // initial span
                             int fader      // initial fader atten / gain
                             );    

void    MIXReleaseChannel   (AXVPB *p);

void	MIXResetControls	(AXVPB *p);

void    MIXSetInput         (AXVPB *p, int dB);
void    MIXAdjustInput      (AXVPB *p, int dB);
int     MIXGetInput         (AXVPB *p);

void	MIXAuxAPostFader	(AXVPB *p);
void	MIXAuxAPreFader	    (AXVPB *p);
BOOL	MIXAuxAIsPostFader	(AXVPB *p);
void	MIXSetAuxA			(AXVPB *p, int dB);
void	MIXAdjustAuxA		(AXVPB *p, int dB);
int		MIXGetAuxA			(AXVPB *p);

void	MIXAuxBPostFader	(AXVPB *p);
void	MIXAuxBPreFader	    (AXVPB *p);
BOOL	MIXAuxBIsPostFader	(AXVPB *p);
void	MIXSetAuxB			(AXVPB *p, int dB);
void	MIXAdjustAuxB		(AXVPB *p, int dB);
int		MIXGetAuxB			(AXVPB *p);

void	MIXSetPan			(AXVPB *p, int pan);
void	MIXAdjustPan		(AXVPB *p, int pan);
int		MIXGetPan			(AXVPB *p);

void	MIXSetSPan			(AXVPB *p, int span);
void	MIXAdjustSPan		(AXVPB *p, int span);
int		MIXGetSPan			(AXVPB *p);

void	MIXMute			    (AXVPB *p);
void    MIXUnMute           (AXVPB *p);
BOOL	MIXIsMute			(AXVPB *p);

void	MIXSetFader			(AXVPB *p, int dB);
void	MIXAdjustFader		(AXVPB *p, int dB);
int		MIXGetFader			(AXVPB *p);

void    MIXSetDvdStreamFader(int dB);
int     MIXGetDvdStreamFader(void);

void    MIXUpdateSettings   (void);

#if 0
}
#endif

#endif // __MIX_H__ 
