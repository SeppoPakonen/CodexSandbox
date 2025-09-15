/*---------------------------------------------------------------------------*
  Project:  AUX effects for AX
  File:     axfx.h

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/dolphin/axfx.h $
    
    3     10/01/01 3:42p Billyjack
    fixed MAX and MIN value defines... these are not used in code so
    there's no functional change
    
    2     6/15/01 6:07p Billyjack
    added AXFXSetHooks()
    
    1     5/09/01 1:28p Billyjack
    created
   
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __AXFX_H__
#define __AXFX_H__

#if 0
extern "C" {
#endif



/*--------------------------------------------------------------------------*
    memory allocation
 *--------------------------------------------------------------------------*/
typedef void*   (*AXFXAlloc)    (u32);   
typedef void    (*AXFXFree)     (void*);

void AXFXSetHooks(AXFXAlloc alloc, AXFXFree free);


/*--------------------------------------------------------------------------*
    buffer update
 *--------------------------------------------------------------------------*/
typedef struct AXFX_BUFFERUPDATE
{

    s32		*left;
	s32		*right;
	s32		*surround;

} AXFX_BUFFERUPDATE;


/*--------------------------------------------------------------------------*
    hi quality reverb
 *--------------------------------------------------------------------------*/
typedef struct AXFX_REVHI_DELAYLINE
{

    s32	inPoint;
	s32	outPoint;
	s32	length;
	f32	*inputs;
	f32	lastOutput;

} AXFX_REVHI_DELAYLINE;

typedef struct AXFX_REVHI_WORK
{

	AXFX_REVHI_DELAYLINE AP[9];
	AXFX_REVHI_DELAYLINE C[9];
	f32	allPassCoeff;
	f32	combCoef[9];
	f32	lpLastout[3];
	f32	level;
	f32	damping;
	s32	preDelayTime;
	f32	crosstalk;
	f32	*preDelayLine[3];
	f32	*preDelayPtr[3];

} AXFX_REVHI_WORK;

typedef struct AXFX_REVERBHI
{

    // do not write to these
    AXFX_REVHI_WORK rv;
    u8 tempDisableFX;
				
    // user params				
    f32	coloration;
    f32	mix;
    f32	time;
    f32	damping;
    f32	preDelay;
    f32	crosstalk;

} AXFX_REVERBHI;


#define	AXFX_REVHI_MIN_TIME         (f32)0.01f		// seconds
#define	AXFX_REVHI_MAX_TIME	        (f32)10.0f		// seconds

#define	AXFX_REVHI_MIN_PREDELAY	    (f32)0.0f		// seconds
#define	AXFX_REVHI_MAX_PREDELAY	    (f32)0.1f		// seconds

#define	AXFX_REVHI_MIN_COLORATION   (f32)0.0f
#define	AXFX_REVHI_MAX_COLORATION   (f32)1.0f

#define	AXFX_REVHI_MIN_DAMPING      (f32)0.0f
#define	AXFX_REVHI_MAX_DAMPING      (f32)1.0f

#define	AXFX_REVHI_MIN_MIX          (f32)0.0f
#define	AXFX_REVHI_MAX_MIX          (f32)1.0f

#define	AXFX_REVHI_MIN_CROSSTALK    (f32)0.0f
#define	AXFX_REVHI_MAX_CROSSTALK    (f32)100.0f


int     AXFXReverbHiInit        (AXFX_REVERBHI *rev);
int     AXFXReverbHiShutdown    (AXFX_REVERBHI *rev);
int     AXFXReverbHiSettings    (AXFX_REVERBHI *rev);
void    AXFXReverbHiCallback    (AXFX_BUFFERUPDATE *bufferUpdate, AXFX_REVERBHI *reverb);


/*--------------------------------------------------------------------------*
    standard reverb
 *--------------------------------------------------------------------------*/
typedef struct AXFX_REVSTD_DELAYLINE
{

    s32 inPoint;
    s32	outPoint;
    s32	length;
    f32	*inputs;
    f32	lastOutput;

} AXFX_REVSTD_DELAYLINE;

typedef struct AXFX_REVSTD_WORK
{

    AXFX_REVSTD_DELAYLINE AP[6];
    AXFX_REVSTD_DELAYLINE C[6];
    f32	allPassCoeff;
	f32	combCoef[6];
	f32	lpLastout[3];
	f32	level;
	f32	damping;
	s32	preDelayTime;
	f32	*preDelayLine[3];
	f32	*preDelayPtr[3];

} AXFX_REVSTD_WORK;

typedef struct AXFX_REVERBSTD
{
    
    // do not write to these
    AXFX_REVSTD_WORK rv;
    u8 tempDisableFX;
				
    // user params				
    f32	coloration;
    f32	mix;
    f32	time;
    f32	damping;
    f32	preDelay;

} AXFX_REVERBSTD;

#define	AXFX_REVSTD_MIN_TIME        (f32)0.01f			// seconds
#define	AXFX_REVSTD_MAX_TIME        (f32)10.0f		    // seconds

#define	AXFX_REVSTD_MIN_PREDELAY    (f32)0.0f			// seconds
#define	AXFX_REVSTD_MAX_PREDELAY    (f32)0.1f			// seconds

#define	AXFX_REVSTD_MIN_COLORATION  (f32)0.0f
#define	AXFX_REVSTD_MAX_COLORATION  (f32)1.0f

#define	AXFX_REVSTD_MIN_DAMPING     (f32)0.0f
#define	AXFX_REVSTD_MAX_DAMPING     (f32)1.0f

#define	AXFX_REVSTD_MIN_MIX         (f32)0.0f
#define	AXFX_REVSTD_MAX_MIX         (f32)1.0f


int     AXFXReverbStdInit       (AXFX_REVERBSTD *rev);
int     AXFXReverbStdShutdown   (AXFX_REVERBSTD *rev);
int     AXFXReverbStdSettings   (AXFX_REVERBSTD *rev);
void    AXFXReverbStdCallback   (AXFX_BUFFERUPDATE *bufferUpdate, AXFX_REVERBSTD *reverb);


/*--------------------------------------------------------------------------*
 *--------------------------------------------------------------------------*/
typedef struct AXFX_DELAY
{
    // do not write to these
    u32	currentSize[3];
    u32	currentPos[3];
    u32	currentFeedback[3];
    u32	currentOutput[3];
    s32	*left;
    s32	*right;
	s32	*sur;
				
    // user params				
    u32	delay[3];       // Delay buffer length in ms per channel
    u32	feedback[3];    // Feedback volume in % per channel
    u32	output[3];		// Output volume in % per channel

} AXFX_DELAY;


#define	AXFX_DELAY_MIN_DELAY    10      // ms
#define	AXFX_DELAY_MAX_DELAY    5000    // ms

#define	AXFX_DELAY_MIN_FEEDBACK 0
#define	AXFX_DELAY_MAX_FEEDBACK 100

#define	AXFX_DELAY_MIN_OUTPUT   0
#define	AXFX_DELAY_MAX_OUTPUT   100


int     AXFXDelayInit           (AXFX_DELAY *delay);
int     AXFXDelayShutdown       (AXFX_DELAY *delay);
int     AXFXDelaySettings       (AXFX_DELAY *delay);
void    AXFXDelayCallback       (AXFX_BUFFERUPDATE *bufferUpdate, AXFX_DELAY *delay);


/*--------------------------------------------------------------------------*
    chours
 *--------------------------------------------------------------------------*/
typedef struct AXFX_CHORUS_SRCINFO
{
    s32	*dest;
    s32	*smpBase;
    s32	*old;
    u32	posLo;
    u32	posHi;
    u32	pitchLo;
    u32	pitchHi;
    u32	trigger;
    u32	target;

} AXFX_CHORUS_SRCINFO;


typedef struct AXFX_CHORUS_WORK
{

    s32	*lastLeft[3];
    s32	*lastRight[3];
    s32	*lastSur[3];
    u8 	currentLast;
    s32	oldLeft[4];
    s32	oldRight[4];
    s32	oldSur[4];
					
    u32	currentPosLo;
    u32	currentPosHi;
					
    s32	pitchOffset;
    u32	pitchOffsetPeriodCount;
    u32	pitchOffsetPeriod;
					
    AXFX_CHORUS_SRCINFO src;

} AXFX_CHORUS_WORK;

typedef struct AXFX_CHORUS
{

    // do not write to these
    AXFX_CHORUS_WORK work;
					
    // user params
    u32	baseDelay;		// Base delay of chorus effect in ms
    u32	variation;		// Variation of base delay in ms
    u32	period;			// Period of variational oscilation in ms

} AXFX_CHORUS;


#define	AXFX_CHORUS_MIN_DELAY       5   // ms
#define	AXFX_CHORUS_MAX_DELAY       15  // ms
//#define	AXFX_CHORUS_MAX_DELAY   (CHORUS_MIN_DELAY+(_SND_CHORUS_NUM_BLOCKS-1)*5)

#define AXFX_CHORUS_MIN_VARIATION   0
#define AXFX_CHORUS_MAX_VARIATION   5

#define	AXFX_CHORUS_MIN_PERIOD      500
#define	AXFX_CHORUS_MAX_PERIOD      10000


int     AXFXChorusInit          (AXFX_CHORUS *chorus);
int     AXFXChorusShutdown      (AXFX_CHORUS *chorus);
int     AXFXChorusSettings      (AXFX_CHORUS *chorus);
void    AXFXChorusCallback      (AXFX_BUFFERUPDATE *bufferUpdate, AXFX_CHORUS *chorus);

#if 0
}
#endif

#endif // __AXFX_H__ 
