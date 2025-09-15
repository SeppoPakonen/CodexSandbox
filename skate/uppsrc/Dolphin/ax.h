/*---------------------------------------------------------------------------*
  Project:  Dolphin AX library
  File:     AX.h

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.
 *---------------------------------------------------------------------------*/

#ifndef __AX_H__
#define __AX_H__

#if 0
extern "C" {
#endif

/*--------------------------------------------------------------------------*
    profiling
 *--------------------------------------------------------------------------*/
typedef struct _AXPROFILE
{

    OSTime  axFrameStart;
    OSTime  auxProcessingStart;
    OSTime  auxProcessingEnd;
    OSTime  userCallbackStart;
    OSTime  userCallbackEnd;
    OSTime  axFrameEnd;
    u32     axNumVoices;

} AXPROFILE;



/*--------------------------------------------------------------------------*
    mixing
 *--------------------------------------------------------------------------*/
typedef struct _AXPBMIX
{
    //  mixing values in .15, 0x8000 = ca. 1.0  

    u16     vL;                 
    u16     vDeltaL;
    u16     vR;
    u16     vDeltaR;
    
    u16     vAuxAL;
    u16     vDeltaAuxAL;
    u16     vAuxAR;
    u16     vDeltaAuxAR;
    
    u16     vAuxBL;
    u16     vDeltaAuxBL;
    u16     vAuxBR;
    u16     vDeltaAuxBR;
    
    u16     vAuxBS;
    u16     vDeltaAuxBS;
    u16     vS;
    u16     vDeltaS;
    u16     vAuxAS;
    u16     vDeltaAuxAS;

} AXPBMIX;


#define AXPBMIX_VL_OFF            AXPBMIX_OFF
#define AXPBMIX_VDELTAL_OFF       (AXPBMIX_OFF+1)
#define AXPBMIX_VR_OFF            (AXPBMIX_OFF+2)
#define AXPBMIX_VDELTAR_OFF       (AXPBMIX_OFF+3)
#define AXPBMIX_VAUXAL_OFF        (AXPBMIX_OFF+4)
#define AXPBMIX_VDELTAAUXAL_OFF   (AXPBMIX_OFF+5)
#define AXPBMIX_VAUXAR_OFF        (AXPBMIX_OFF+6)
#define AXPBMIX_VDELTAAUXAR_OFF   (AXPBMIX_OFF+7)
#define AXPBMIX_VAUXBL_OFF        (AXPBMIX_OFF+8)
#define AXPBMIX_VDELTAAUXBL_OFF   (AXPBMIX_OFF+9)
#define AXPBMIX_VAUXBR_OFF        (AXPBMIX_OFF+10)
#define AXPBMIX_VDELTAAUXBR_OFF   (AXPBMIX_OFF+11)
#define AXPBMIX_VAUXBS_OFF        (AXPBMIX_OFF+12)
#define AXPBMIX_VDELTAAUXBS_OFF   (AXPBMIX_OFF+13)
#define AXPBMIX_VS_OFF            (AXPBMIX_OFF+14)
#define AXPBMIX_VDELTAS_OFF       (AXPBMIX_OFF+15)
#define AXPBMIX_VAUXAS_OFF          (AXPBMIX_OFF+16)
#define AXPBMIX_VDELTAAUXAS_OFF     (AXPBMIX_OFF+17)
#define AXPBMIX_SIZE                18

/*--------------------------------------------------------------------------*
    initial time delay
 *--------------------------------------------------------------------------*/
typedef struct _AXPBITD
{

    u16     flag;               //  on or off for this voice
    u16     bufferHi;           //  MRAM buffer
    u16     bufferLo;           //  
    u16     shiftL;             //  phase shift samples left (current)
    u16     shiftR;             //  phase shift samples right (current)
    u16     targetShiftL;       //  phase shift samples left (target)
    u16     targetShiftR;       //  phase shift samples right (target)

} AXPBITD;

//  flag
#define AX_PB_ITD_OFF      0x0000
#define AX_PB_ITD_ON       0x0001


#define AXPBITD_FLAG_OFF          AXPBITD_OFF
#define AXPBITD_BUFFERHI_OFF      (AXPBITD_OFF+1)
#define AXPBITD_BUFFERLO_OFF      (AXPBITD_OFF+2)
#define AXPBITD_SHIFTL_OFF        (AXPBITD_OFF+3)
#define AXPBITD_SHIFTR_OFF        (AXPBITD_OFF+4)
#define AXPBITD_TARGETSHIFTL_OFF  (AXPBITD_OFF+5)
#define AXPBITD_TARGETSHIFTR_OFF  (AXPBITD_OFF+6)
#define AXPBITD_SIZE              7

/*--------------------------------------------------------------------------*
    update / patch information
 *--------------------------------------------------------------------------*/
typedef struct _AXPBUPDATE
{

    u16     updNum[5];          // number of updates per 1ms step
    u16     dataHi;             // location of update data in MRAM
    u16     dataLo;

} AXPBUPDATE;


#define AXPBUPDATE_UPDNUM     AXPBUPDATE_OFF
#define AXPBUPDATE_DATAHI     (AXPBUPDATE_OFF+5)
#define AXPBUPDATE_DATALO     (AXPBUPDATE_OFF+6)
#define AXPBUPDATE_SIZE       7

/*--------------------------------------------------------------------------*
    depop data (last amplitudes mixed into buffers)
 *--------------------------------------------------------------------------*/
typedef struct _AXPBDPOP
{
    s16     aL;
    s16     aAuxAL;
    s16     aAuxBL;
    
    s16     aR;
    s16     aAuxAR;
    s16     aAuxBR;
    
    s16     aS;
    s16     aAuxAS;
    s16     aAuxBS;

} AXPBDPOP;


#define AXPBDPOP_AL_OFF       AXPBDPOP_OFF
#define AXPBDPOP_AAUXAL_OFF   (AXPBDPOP_OFF+1)
#define AXPBDPOP_AAUXBL_OFF   (AXPBDPOP_OFF+2)
#define AXPBDPOP_AR_OFF       (AXPBDPOP_OFF+3)
#define AXPBDPOP_AAUXAR_OFF   (AXPBDPOP_OFF+4)
#define AXPBDPOP_AAUXBR_OFF   (AXPBDPOP_OFF+5)
#define AXPBDPOP_AS_OFF       (AXPBDPOP_OFF+6)
#define AXPBDPOP_AAUXAS_OFF   (AXPBDPOP_OFF+7)
#define AXPBDPOP_AAUXBS_OFF   (AXPBDPOP_OFF+8)
#define AXPBDPOP_SIZE         9
 
/*--------------------------------------------------------------------------*
    volume envelope
 *--------------------------------------------------------------------------*/
typedef struct _AXPBVE
{

    u16     currentVolume;              // .15 volume at start of frame
    s16     currentDelta;               // signed per sample delta delta

} AXPBVE;

// ramping is deactivated if currentDelta == 0


#define AXPBVE_CURRENTVOLUME_OFF  AXPBVE_OFF
#define AXPBVE_CURRENTDELTA_OFF   (AXPBVE_OFF+1)
#define AXPBVE_SIZE               2

/*--------------------------------------------------------------------------*
    FIR filter info (currently unused)
 *--------------------------------------------------------------------------*/

typedef struct _AXPBFIR
{

    u16     numCoefs;       // reserved, keep zero
    u16     coefsHi;        // reserved, keep zero
    u16     coefsLo;        // reserved, keep zero
    
} AXPBFIR;


#define AXPBFIR_NUMCOEFS      AXPBFIR_OFF
#define AXPBFIR_COEFSHI       (AXPBFIR_OFF+1)
#define AXPBFIR_COEFSLO       (AXPBFIR_OFF+2)
#define AXPBFIR_SIZE          3


/*--------------------------------------------------------------------------*
    buffer addressing
 *--------------------------------------------------------------------------*/
typedef struct _AXPBADDR
{                                       // all values are mesured in samples:
    u16     loopFlag;                   // 0 = one-shot, 1=looping  
    u16     format;                     // sample format used (see below)
    u16     loopAddressHi;              // Start of loop (this will point to a shared "zero" buffer if one-shot mode is active)
    u16     loopAddressLo;
    u16     endAddressHi;               // End of sample (and loop)
    u16     endAddressLo;
    u16     currentAddressHi;           // Current playback position
    u16     currentAddressLo;

} AXPBADDR;


#define AXPBADDR_LOOP_OFF     0           // States for loopFlag field
#define AXPBADDR_LOOP_ON      1


#define AXPBADDR_LOOPFLAG_OFF         AXPBADDR_OFF
#define AXPBADDR_FORMAT_OFF           (AXPBADDR_OFF+1)
#define AXPBADDR_LOOPADDRESSHI_OFF    (AXPBADDR_OFF+2)
#define AXPBADDR_LOOPADDRESSLO_OFF    (AXPBADDR_OFF+3)
#define AXPBADDR_ENDADDRESSHI_OFF     (AXPBADDR_OFF+4)
#define AXPBADDR_ENDADDRESSLO_OFF     (AXPBADDR_OFF+5)
#define AXPBADDR_CURRENTADDRESSHI_OFF (AXPBADDR_OFF+6)
#define AXPBADDR_CURRENTADDRESSLO_OFF (AXPBADDR_OFF+7)
#define AXPBADDR_SIZE                 8

/*--------------------------------------------------------------------------*
    ADPCM decoder state
 *--------------------------------------------------------------------------*/
typedef struct _AXPBADPCM
{

    u16     a[8][2];            //  coef table a1[0],a2[0],a1[1],a2[1]....

    u16     gain;               //  gain to be applied (0 for ADPCM, 0x0800 for PCM8/16)
    
    u16     pred_scale;         //  predictor / scale combination (nibbles, as in hardware)
    u16     yn1;                //  y[n - 1]
    u16     yn2;                //  y[n - 2]
    
} AXPBADPCM;


#define AXPBADPCM_A1                  AXPBADPCM_OFF
#define AXPBADPCM_A2                  (AXPBADPCM_OFF+8)
#define AXPBADPCM_GAIN                (AXPBADPCM_OFF+16
#define AXPBADPCM_PRED_SCALE          (AXPBADPCM_OFF+17)
#define AXPBADPCM_YN1                 (AXPBADPCM_OFF+18)
#define AXPBADPCM_YN2                 (AXPBADPCM_OFF+19)
#define AXPBADPCM_SIZE                20

/*--------------------------------------------------------------------------*
    sample rate converter state
 *--------------------------------------------------------------------------*/
typedef struct _AXPBSRC
{
    
    u16     ratioHi;            //  sampling ratio, integer
    u16     ratioLo;            //  sampling ratio, fraction
    
    u16     currentAddressFrac; //  current fractional sample position
    
    u16     last_samples[4];    //  last 4 input samples

} AXPBSRC;



#define AXPBSRC_RATIOHI_OFF               AXPBSRC_OFF
#define AXPBSRC_RATIOLO_OFF               (AXPBSRC_OFF+1)
#define AXPBSRC_CURRENTADDRESSFRAC_OFF    (AXPBSRC_OFF+2)
#define AXPBSRC_LAST_SAMPLES_OFF          (AXPBSRC_OFF+3)       // 4 words
#define AXPBSRC_SIZE                      7
    
/*--------------------------------------------------------------------------*
    ADPCM loop parameters
 *--------------------------------------------------------------------------*/

typedef struct _AXPBADPCMLOOP
{
    u16     loop_pred_scale;    //  predictor / scale combination (nibbles, as in hardware)
    u16     loop_yn1;           //  y[n - 1]
    u16     loop_yn2;           //  y[n - 2]

} AXPBADPCMLOOP;
    
#define AXPBADPCMLOOP_PRED_SCALE      AXPBADPCMLOOP_OFF
#define AXPBADPCMLOOP_YN1             (AXPBADPCMLOOP_OFF+1)
#define AXPBADPCMLOOP_YN2             (AXPBADPCMLOOP_OFF+2)
#define AXPBADPCMLOOP_SIZE            3

/*--------------------------------------------------------------------------*
    voice parameter block
 *--------------------------------------------------------------------------*/
typedef struct _AXPB
{
    u16             nextHi;     // pointer to next parameter buffer (MRAM)
    u16             nextLo;
                    
    u16             currHi;     // pointer to this parameter buffer (MRAM)
    u16             currLo;
                    
    u16             srcSelect;  // Select type of SRC (none,4-tap,linear)
    u16             coefSelect; // Coef. to be used with 4-tap SRC
    u16             mixerCtrl;  // Mixer control bits
                    
    u16             state;      // current state (see below)
    u16             type;       // type of voice (stream)
                    
    AXPBMIX         mix;    
    AXPBITD         itd;
    AXPBUPDATE      update;
    AXPBDPOP        dpop;
    AXPBVE          ve;
    AXPBFIR         fir;
    AXPBADDR        addr;
    AXPBADPCM       adpcm;
    AXPBSRC         src;
    AXPBADPCMLOOP   adpcmLoop;

    u16             pad[3];     // 32 byte alignment

} AXPB;

//  state
#define AX_PB_STATE_STOP        0x0000
#define AX_PB_STATE_RUN         0x0001

//  type
#define AX_PB_TYPE_NORMAL       0x0000
#define AX_PB_TYPE_STREAM       0x0001  // no loop context programming for ADPCM

//  format
#define AX_PB_FORMAT_PCM16      0x000A  // signed 16 bit PCM mono
#define AX_PB_FORMAT_PCM8       0x0019  // signed 8 bit PCM mono
#define AX_PB_FORMAT_ADPCM      0x0000  // ADPCM encoded (both standard & extended)

//  src select
#define AX_PB_SRCSEL_POLYPHASE  0x0000  // N64 type polyphase filter (4-tap)
#define AX_PB_SRCSEL_LINEAR     0x0001  // Linear interpolator
#define AX_PB_SRCSEL_NONE       0x0002  // No SRC (1:1)

//  coef select
#define AX_PB_COEFSEL_8KHZ      0x0000  // 8KHz low pass response
#define AX_PB_COEFSEL_12KHZ     0x0001  // 12.8KHz N64 type response
#define AX_PB_COEFSEL_16KHZ     0x0002  // 16KHz response

//  mixer ctrl
#define AX_PB_MIXCTRL_RAMPING   0x0008  // Ramping is active
#define AX_PB_MIXCTRL_CONSTANT  0x0000  // Ramping is inactive

#define AX_PB_MIXCTRL_SURROUND  0x0004  // Surround is active
#define AX_PB_MIXCTRL_STEREO    0x0000  // Surround is inactive

#define AX_PB_MIXCTRL_MAIN      0x0000  // Main bus active
#define AX_PB_MIXCTRL_AUXA      0x0001  // AuxA active (additive to above)
#define AX_PB_MIXCTRL_AUXB      0x0002  // AuxB active (additive to above)



#define AX_PB_NEXTHI_OFF        0
#define AX_PB_NEXTLO_OFF        1
#define AX_PB_CURRHI_OFF        2
#define AX_PB_CURRLO_OFF        3

#define AX_PB_SRCSELECT_OFF     4
#define AX_PB_COEFSELECT_OFF    5
#define AX_PB_MIXERCTRL_OFF     6

#define AX_PB_STATE_OFF         7
#define AX_PB_TYPE_OFF          8

#define AXPBMIX_OFF             9

#define AXPBITD_OFF           (AXPBMIX_OFF      + AXPBMIX_SIZE)
#define AXPBUPDATE_OFF        (AXPBITD_OFF      + AXPBITD_SIZE)
#define AXPBDPOP_OFF          (AXPBUPDATE_OFF   + AXPBUPDATE_SIZE)
#define AXPBVE_OFF            (AXPBDPOP_OFF     + AXPBDPOP_SIZE)
#define AXPBFIR_OFF           (AXPBVE_OFF       + AXPBVE_SIZE)
#define AXPBADDR_OFF          (AXPBFIR_OFF      + AXPBFIR_SIZE)
#define AXPBADPCM_OFF         (AXPBADDR_OFF     + AXPBADDR_SIZE)
#define AXPBSRC_OFF           (AXPBADPCM_OFF    + AXPBADPCM_SIZE)
#define AXPBADPCMLOOP_OFF     (AXPBSRC_OFF      + AXPBSRC_SIZE)

#define AX_PB_SIZE            (AXPBADPCMLOOP_OFF+AXPBADPCMLOOP_SIZE)

#define AX_DSP_PATCHDATA_SIZE  128

/*--------------------------------------------------------------------------*
    studio parameter block
 *--------------------------------------------------------------------------*/

typedef struct _AXSPB
{
    u16 dpopLHi;
    u16 dpopLLo;
    s16 dpopLDelta;
    u16 dpopRHi;
    u16 dpopRLo;
    s16 dpopRDelta;
    u16 dpopSHi;
    u16 dpopSLo;
    s16 dpopSDelta;
    
    u16 dpopALHi;
    u16 dpopALLo;
    s16 dpopALDelta;
    u16 dpopARHi;
    u16 dpopARLo;
    s16 dpopARDelta;
    u16 dpopASHi;
    u16 dpopASLo;
    s16 dpopASDelta;
    
    u16 dpopBLHi;
    u16 dpopBLLo;
    s16 dpopBLDelta;
    u16 dpopBRHi;
    u16 dpopBRLo;
    s16 dpopBRDelta;
    u16 dpopBSHi;
    u16 dpopBSLo;
    s16 dpopBSDelta;
    
} AXSPB;


/*---------------------------------------------------------------------------*
 *---------------------------------------------------------------------------*/
#define AX_DSP_CYCLES_PBSYNC        2000
#define AX_DSP_CYCLES               (OS_BUS_CLOCK / 400)

/*---------------------------------------------------------------------------*
 *---------------------------------------------------------------------------*/
#define AX_MAX_VOICES               64

#define AX_MS_PER_FRAME             5

#define AX_IN_SAMPLES_PER_MS        32
#define AX_IN_SAMPLES_PER_SEC       (AX_IN_SAMPLES_PER_MS * 1000)
#define AX_IN_SAMPLES_PER_FRAME     (AX_IN_SAMPLES_PER_MS * AX_MS_PER_FRAME)

/*---------------------------------------------------------------------------*
 *---------------------------------------------------------------------------*/
#define AX_MODE_STEREO              0
#define AX_MODE_SURROUND            1

/*---------------------------------------------------------------------------*
 *---------------------------------------------------------------------------*/
#define AX_PRIORITY_STACKS          32
#define AX_PRIORITY_NODROP          (AX_PRIORITY_STACKS - 1)
#define AX_PRIORITY_LOWEST          1
#define AX_PRIORITY_FREE            0

/*---------------------------------------------------------------------------*
 *---------------------------------------------------------------------------*/
#define AX_SRC_TYPE_NONE            0
#define AX_SRC_TYPE_LINEAR          1
#define AX_SRC_TYPE_4TAP_8K         2
#define AX_SRC_TYPE_4TAP_12K        3
#define AX_SRC_TYPE_4TAP_16K        4

/*---------------------------------------------------------------------------*
 *---------------------------------------------------------------------------*/
#define AX_ADDR_ONESHOT             0
#define AX_ADDR_LOOP                1

/*---------------------------------------------------------------------------*
 *---------------------------------------------------------------------------*/
#define AX_SYNC_NONEWPARAMS         0x00000000
#define AX_SYNC_USER_SRCSELECT      0x00000001
#define AX_SYNC_USER_MIXCTRL        0x00000002
#define AX_SYNC_USER_STATE          0x00000004
#define AX_SYNC_USER_TYPE           0x00000008
#define AX_SYNC_USER_MIX            0x00000010
#define AX_SYNC_USER_ITD            0x00000020
#define AX_SYNC_USER_ITDTARGET      0x00000040
#define AX_SYNC_USER_UPDATE         0x00000080
#define AX_SYNC_USER_DPOP           0x00000100
#define AX_SYNC_USER_VE             0x00000200
#define AX_SYNC_USER_VEDELTA        0x00000400
#define AX_SYNC_USER_FIR            0x00000800
#define AX_SYNC_USER_ADDR           0x00001000
#define AX_SYNC_USER_LOOP           0x00002000
#define AX_SYNC_USER_LOOPADDR       0x00004000
#define AX_SYNC_USER_ENDADDR        0x00008000
#define AX_SYNC_USER_CURRADDR       0x00010000
#define AX_SYNC_USER_ADPCM          0x00020000
#define AX_SYNC_USER_SRC            0x00040000
#define AX_SYNC_USER_SRCRATIO       0x00080000
#define AX_SYNC_USER_ADPCMLOOP      0x00100000
#define AX_SYNC_USER_ALLPARAMS      0x80000000


/*---------------------------------------------------------------------------*
    callback interface
 *---------------------------------------------------------------------------*/
typedef void    (*AXUserCallback)   (void);   
typedef void    (*AXAuxCallback)    (void *data, void *context);
typedef void    (*AXVoiceCallback)  (void *p);


/*---------------------------------------------------------------------------*
 *---------------------------------------------------------------------------*/
typedef struct _AXVPB
{
    void            *next;          // used in priority stacks
    void            *prev;          // used in priority stacks
    void            *next1;         // used in callback stack

    // these ares are used in voice allocation
    u32             priority;       // index to stack
    AXVoiceCallback callback;       // user callback for specified
    u32             userContext;    // user assigned context for callback

    // vars & flags for updating and sync PBs
    u32             index;          // index of VPB in array
    u32             sync;           // bit mask for each PB item to sync
    u32             depop;          // should depop voice
    u32             updateMS;       // update current ms
    u32             updateCounter;  // counter for n updates 
    u32             updateTotal;    // bounds checking for update block
    u16             *updateWrite;   // write pointer for PB updates
    u16             updateData[128];// data for PB updates
    void            *itdBuffer;     // pointer to itd buffer
    AXPB            pb;             // write params to this PB

} AXVPB;

typedef struct _AXPBU
{
    
    u16 data[128];

} AXPBU;

typedef struct _AXPBITDBUFFER
{
    
    s16 data[32];

} AXPBITDBUFFER;

/*---------------------------------------------------------------------------*
 *---------------------------------------------------------------------------*/
void    AXInit                      (void);
void    AXQuit                      (void);

void    AXRegisterCallback          (AXUserCallback callback);
void    AXSetMode                   (u32 mode);
u32     AXGetMode                   (void);

void    AXSetMaxDspCycles           (u32 cycles);
u32     AXGetMaxDspCycles           (void);
u32     AXGetDspCycles              (void);

void    AXRegisterAuxACallback      (AXAuxCallback callback, void *context);
void    AXRegisterAuxBCallback      (AXAuxCallback callback, void *context);

AXVPB*  AXAcquireVoice              (
                                     u32                priority,
                                     AXVoiceCallback    callback,
                                     u32                userContext
                                     );

void    AXFreeVoice                 (AXVPB *p);
void    AXSetVoicePriority          (AXVPB *p, u32 priority);

void    AXSetVoiceSrcType           (AXVPB *p, u32 type);
void    AXSetVoiceState             (AXVPB *p, u16 state);
void    AXSetVoiceType              (AXVPB *p, u16 type);
void    AXSetVoiceMix               (AXVPB *p, AXPBMIX *mix);
void    AXSetVoiceItdOn             (AXVPB *p);
void    AXSetVoiceItdTarget         (AXVPB *p, u16 lShift, u16 rShift);
void    AXSetVoiceUpdateIncrement   (AXVPB *p);
void    AXSetVoiceUpdateWrite       (AXVPB *p, u16 param, u16 data);
void    AXSetVoiceDpop              (AXVPB *p, AXPBDPOP *dpop);
void    AXSetVoiceVe                (AXVPB *p, AXPBVE *ve);
void    AXSetVoiceVeDelta           (AXVPB *p, s16 delta);
void    AXSetVoiceFir               (AXVPB *p, AXPBFIR *fir);
void    AXSetVoiceAddr              (AXVPB *p, AXPBADDR *addr);
void    AXSetVoiceLoop              (AXVPB *p, u16 loop);
void    AXSetVoiceLoopAddr          (AXVPB *p, u32 address);
void    AXSetVoiceEndAddr           (AXVPB *p, u32 address);
void    AXSetVoiceCurrentAddr       (AXVPB *p, u32 address);
void    AXSetVoiceAdpcm             (AXVPB *p, AXPBADPCM *adpcm);
void    AXSetVoiceSrc               (AXVPB *p, AXPBSRC *src);
void    AXSetVoiceSrcRatio          (AXVPB *p, f32 ratio);
void    AXSetVoiceAdpcmLoop         (AXVPB *p, AXPBADPCMLOOP *adpcmloop);

void    AXInitProfile               (AXPROFILE *profile, u32 maxProfiles);
u32     AXGetProfile                (void);

void    AXSetStepMode               (u32);

#if 0
}
#endif

#endif // __AX_H__ 
