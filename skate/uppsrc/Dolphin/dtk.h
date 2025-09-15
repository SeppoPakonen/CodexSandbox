/*---------------------------------------------------------------------------*
  Project:  DVD Audio Track Player
  File:     dtk.h
 
  Copyright 2001 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/dolphin/dtk.h $
    
    4     5/18/01 5:58p Eugene
    Added new state "DTK_STATE_PREPARE" --> this works just like "RUN"
    except that it does NOT assert the AI streaming sample clock. Instead,
    DTK will issue a "PREPARED" event callback to the application; the app
    may then set state to "RUN" to begin streaming at its discretion. This
    is handy for synchronizing dvd streamed audio with in-game cinematics
    and the like.
    
    3     5/04/01 4:40p Billyjack
    new simplified player .. does not use HW queue
    
    2     5/03/01 6:06p Billyjack
    bug fixes... changed DTKFlushTracks API
    
    1     5/02/01 3:40p Eugene
    Header file for DVD track player "demo" library
    
    7     8/23/00 2:05p Billyjack
    added DTKNextTrack() and DTKPrevTrack()
    
    6     8/22/00 11:08a Billyjack
    changed queue scheme and DVD async handling to callbacks
    
    5     8/21/00 3:55p Billyjack
    added DTKSetVolume() and DTKGetVolume()
    
    4     6/23/00 10:30a Billyjack
    Added
    void        DTKSetInterruptFrequency(u32 samples);
    u32         DTKGetInterruptFrequency(void);
    
    3     6/16/00 5:16p Billyjack
    
    1     6/14/00 5:19p Billyjack
    created
  
  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef __DTK_H__
#define __DTK_H__

#if 0
extern "C" {
#endif


/*---------------------------------------------------------------------------*
    Sample rate definition  
 *---------------------------------------------------------------------------*/
#define DTK_SAMPLERATE_48KHZ            AI_SAMPLERATE_48KHZ
#define DTK_SAMPLERATE_32KHZ            AI_SAMPLERATE_32KHZ


/*---------------------------------------------------------------------------*
    Event mask definition  
 *---------------------------------------------------------------------------*/
#define DTK_EVENT_PLAYBACK_STARTED      0x00000001
#define DTK_EVENT_PLAYBACK_STOPPED      0x00000002  
#define DTK_EVENT_PLAYBACK_PAUSED       0x00000004
#define DTK_EVENT_TRACK_QUEUED          0x00000008
#define DTK_EVENT_TRACK_ENDED           0x00000010
#define DTK_EVENT_TRACK_PREPARED        0x00000020


/*---------------------------------------------------------------------------*
    Queue track return message codes  
 *---------------------------------------------------------------------------*/
#define DTK_QUEUE_SUCCESS               0x00000000
#define DTK_QUEUE_CANNOT_OPEN_FILE      0x00000001
#define DTK_QUEUE_TRACK_IN_DVD			0x00000002

 
/*---------------------------------------------------------------------------*
    Repeat modes  
 *---------------------------------------------------------------------------*/
#define DTK_MODE_NOREPEAT               0x00000000
#define DTK_MODE_ALLREPEAT              0x00000001
#define DTK_MODE_REPEAT1                0x00000002    
 
 
/*---------------------------------------------------------------------------*
    State definition  
 *---------------------------------------------------------------------------*/
#define DTK_STATE_STOP                  0x00000000
#define DTK_STATE_RUN                   0x00000001
#define DTK_STATE_PAUSE                 0x00000002
#define DTK_STATE_BUSY					0x00000003
#define DTK_STATE_PREPARE               0x00000004


/*---------------------------------------------------------------------------*
    DTK callback interface
 *---------------------------------------------------------------------------*/
typedef void (*DTKCallback)(u32 eventMask); // for track events
typedef void (*DTKFlushCallback)(void);     // for flushing tracks


/*---------------------------------------------------------------------------*
    DTKTrack definition  
 *---------------------------------------------------------------------------*/
typedef struct DTKTrack
{
	struct DTKTrack*	prev;
    struct DTKTrack*    next;
    char*               fileName;
    u32                 eventMask;
    DTKCallback         callback;
    DVDFileInfo         dvdFileInfo;

} DTKTrack;


/*---------------------------------------------------------------------------*
    Exposed function prototypes
 *---------------------------------------------------------------------------*/
void        DTKInit             (void);
void        DTKShutdown         (void);
u32         DTKQueueTrack       (char* fileName, DTKTrack* track, u32 eventMask, DTKCallback callback);
u32         DTKRemoveTrack      (DTKTrack* track);
void        DTKFlushTracks      (DTKFlushCallback callback);
void        DTKSetSampleRate    (u32 samplerate);       // Obsoleted, retained for backwards compatibility
u32         DTKGetSampleRate    (void);
void        DTKSetInterruptFrequency(u32 samples);
u32         DTKGetInterruptFrequency(void);
void        DTKSetRepeatMode    (u32 repeat);
u32         DTKGetRepeatMode    (void);
void        DTKSetState         (u32 state);
u32         DTKGetState         (void);
void		DTKNextTrack		(void);
void		DTKPrevTrack		(void);
u32         DTKGetPosition      (void);
DTKTrack*   DTKGetCurrentTrack  (void);
void 		DTKSetVolume		(u8 left, u8 right);
u16			DTKGetVolume		(void);

#if 0
}
#endif

#endif // __DTK_H__ 
