/*****************************************************************
 * Project		: Sound Library
 *****************************************************************
 * File			: sound.h
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 20 Aug 1999
 * Last Update	: 20 Aug 1999
 *****************************************************************
 * Description	:
 *
 *
 *****************************************************************
 * 20/08/99		: Initial coding.
 *****************************************************************/

#ifndef THEYER_SOUND_H
#define THEYER_SOUND_H

#include <type/datatype.h>
#include <text/text.h>

typedef int SndChannel;

#define SOUND_LOAD_FAILED			-1
#define SOUND_MASTER_VOLUME			-2
#define SOUND_FX_VOLUME				-3
#define SOUND_ALL_CHANNELS			-4

extern void sndInit( void );
extern void sndClose( void );
extern int  sndLoad( Text name );
extern void sndMute( Bool on );
extern void sndStop( int channel );
extern void	sndLockChannel( int channel, Bool lock );

/* dev kit build only can use this... */
extern Bool sndFinished( int channel );
/* non dev kit build needs to use this rather than sndStop() */
extern void sndStopPlay( int channel, int file_id, int sound_id );

#if SOUND_FIXEDPOINT_API
/* cd music playback functions */
extern void sndCDPlay( int *tracks, int offset, fixed vol_left, fixed vol_right );
extern void sndCDStop( int *tracks );
extern void sndSetCDVolume( fixed vol_left, fixed vol_right );
extern int  sndGetCDTrack( int *tracks );
/* sound channels */
extern SndChannel sndPlay( int file_id, int sound_id, fixed vol_left, fixed vol_right );
extern void sndSetVolume( SndChannel channel, fixed vol_left, fixed vol_right );
extern void sndSetPitch( int channel, int file_id, int sound_id, fixed	bend );
#endif

#endif
