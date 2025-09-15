/*****************************************************************
 * Project		: Sound Library
 *****************************************************************
 * File			: sndpsx.c
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 20 Aug 1999
 * Last Update	: 20 Aug 1999
 *****************************************************************
 * Description	: sound library functions
 *****************************************************************/

/*
 * includes
 */

#include <compile.h>

#include <text/text.h>
#include <memory/memory.h>
#include <sound/sound.h>

#ifdef OsTypeIsPsx
#include <sound/sndpsx.h>
#else
#define SND_DISABLE_SOUND
#endif

/*
 * macros
 */

/*
 * typedefs
 */

/*
 * prototypes
 */

/*
 * variables
 */

/* 
 * globals variables...
 */

#ifdef OsTypeIsPsx
static SndData	snd;
#endif

/*
 * functions
 */

/******************************************************************************
 * Function:
 * sndInit -- initialise sound library
 * 
 * Description:
 *
 * Returns:
 * 
 */

void sndInit ( 
	void
	)
{
#ifndef SND_DISABLE_SOUND
#ifdef PSX_FULL_DEV_KIT
	//static	u_char	table[4];

#ifdef SND_USE_CD
	if ( ! CdInit() ) {
		printf( "sndInit: CdInit failed\n" );
		exit(1);
	}
#endif

	/* init data */
	memClear( &snd, sizeof(SndData) );
	snd.loaded = -1;
	snd.sfx_volume = FIXED_ONE;

	/* fire up sound system */
	SsInit();
	//SsSetTableSize( table, 0, 0 );
	SsSetTickMode(SS_TICKVSYNC);
	SsStart();

	/* cd input volume */
	SsSetSerialAttr( SS_SERIAL_A, SS_MIX, SS_SON );
	snd.cd_volume  = FIXED_ONE;
	sndSetCDVolume( snd.cd_volume, snd.cd_volume );
#endif
	/* set main volume */
	snd.master_volume = 127;
	SsSetMVol( snd.master_volume, snd.master_volume );
#endif
}


/******************************************************************************
 * Function:
 * sndClose -- close down the sound system
 * 
 * Description:
 *
 * Returns:
 * 
 */

void	sndClose ( 
	void
	)
{
#ifndef SND_DISABLE_SOUND

	/* turn off all channels */
	SsUtAllKeyOff(0);

#ifdef PSX_FULL_DEV_KIT
	/* stop sound callbacks for faster loading... */
	SsEnd();
#endif

	/* close open vab */
	if ( snd.loaded != SOUND_LOAD_FAILED ) {
		SsVabClose( snd.loaded );
		snd.loaded = SOUND_LOAD_FAILED;
	}

#ifdef PSX_FULL_DEV_KIT
	/* shutdown sound */
	SsQuit();
#ifdef PSX_DEMO_DISK
	SpuInit();
#endif
#endif

#endif
}


/******************************************************************************
 * Function:
 * sndLockChannel -- lock a channel
 * 
 * Description:
 *
 * Returns:
 * 
 */

void	sndLockChannel ( 
	int		channel,
	Bool	lock
	)
{
#ifndef SND_DISABLE_SOUND
#ifdef PSX_FULL_DEV_KIT
	/* validate */
	if ( channel < 0 || channel > 23 ) return;

	if ( lock ) {
		/* set lock */
		snd.lock_mask |= (0x1L<<channel);
	} else {
		/* unlock */
		snd.lock_mask &= ~(0x1L<<channel);
	}

	/* set mask */
	SsSetVoiceMask( snd.lock_mask );
#endif
#endif
}


/******************************************************************************
 * Function:
 * sndCDPlay -- CD music playback
 * 
 * Description:
 *
 * Returns:
 * 
 */

void sndCDPlay ( 
	int		*tracks,
	int		 offset,
	fixed	 vol_left, 
	fixed	 vol_right
	)
{
#ifndef SND_DISABLE_SOUND
	//unsigned char	rbuf[8];

	/* set CD volume */
	sndSetCDVolume( vol_left, vol_right );
#ifdef PSX_FULL_DEV_KIT
#ifdef SND_USE_CD
	/* play from CD */
	CdPlay( 2, tracks, offset );
#endif
#endif
#endif
}


/******************************************************************************
 * Function:
 * sndCDStop -- Stop CD music playback
 * 
 * Description:
 *
 * Returns:
 * 
 */

void sndCDStop ( 
	int		*tracks
	)
{
#ifndef SND_DISABLE_SOUND
#ifdef PSX_FULL_DEV_KIT
#ifdef SND_USE_CD
	/* play from CD */
	CdPlay( 0, tracks, 0 );
#endif
#endif
#endif
}


/******************************************************************************
 * Function:
 * sndGetCDTrack -- Get the current CD music playback track
 * 
 * Description:
 *
 * Returns:
 * 
 */

int sndGetCDTrack ( 
	int		*tracks
	)
{
#ifndef SND_DISABLE_SOUND
#ifdef PSX_FULL_DEV_KIT
#ifdef SND_USE_CD
	int track;

	/* get current track from CD */
	track = CdPlay( 3, tracks, 0 );

	return( track );
#else
	return(0);
#endif
#else
	return(0);
#endif
#else
	return(0);
#endif
}


/******************************************************************************
 * Function:
 * sndMute -- mute option for sounds
 * 
 * Description:
 *
 * Returns:
 * 
 */

void	sndMute ( 
	Bool	on
	)
{
#ifndef SND_DISABLE_SOUND
	/* set mute */
	if ( on )
		SsSetMute( SS_MUTE_ON );
	else
		SsSetMute( SS_MUTE_OFF );
#endif
}


/******************************************************************************
 * Function:
 * sndStop -- stop sounds
 * 
 * Description:
 *
 * Returns:
 * 
 */

void	sndStop ( 
	int	channel
	)
{
#ifndef SND_DISABLE_SOUND
	if ( channel == SOUND_ALL_CHANNELS ) {
		/* unlock all channels */
		snd.lock_mask = 0x0;
		SsSetVoiceMask( snd.lock_mask );
		/* turn off all channels */
		SsUtAllKeyOff(0);
	} else {
#ifdef PSX_FULL_DEV_KIT		
		/* unlock this channel */
		snd.lock_mask &= ~(0x1L<<channel);
		SsSetVoiceMask( snd.lock_mask );
		SsUtKeyOffV(channel);
#else
		/* unlock all channels */
		snd.lock_mask = 0x0;
		SsSetVoiceMask( snd.lock_mask );
		SsUtAllKeyOff(0);
#endif
	}
#endif
}


/******************************************************************************
 * Function:
 * sndFinished -- check if a sound channel has completed a sound effect
 * 
 * Description:
 *
 * Returns:		TRUE if no sound is playing on the channel
 * 
 */

Bool	sndFinished ( 
	int	channel
	)
{
#ifdef PSX_FULL_DEV_KIT		
	if ( SpuGetKeyStatus( (0x1L<<channel) ) == SPU_ON )
		return( FALSE );
	return( TRUE );
#else
	return(FALSE);
#endif
}


/******************************************************************************
 * Function:
 * sndStopPlay -- stop a playing sound
 * 
 * Description:
 *
 * Returns: 
 * 
 */

void sndStopPlay (
	int		channel,
	int		file_id,
	int		sound_id
	)
{
#ifndef SND_DISABLE_SOUND

	SsUtKeyOff( channel, file_id, PROG_NUM(sound_id), TONE_NUM(sound_id), 60 );
	/* unlock channel */
	snd.lock_mask &= ~(0x1L<<channel);
	SsSetVoiceMask( snd.lock_mask );
#endif
}


/******************************************************************************
 * Function:
 * sndLoad -- load a sound file
 * 
 * Description:
 *
 * Returns:
 * 
 */

int	sndLoad ( 
	Text name
	)
{
#ifdef SND_DISABLE_SOUND
	return( 1 ); //SOUND_LOAD_FAILED );
#else
	char	 buff[256];
	int		 vab_id;
	Byte	*data;
	Byte	*body;
	int		 size;

	/* turn off all channels */
	SsUtAllKeyOff(0);

#ifdef PSX_FULL_DEV_KIT
	/* stop sound callbacks for faster loading... */
	sndSetCDVolume( 0, 0 );
	SsSetMVol( 0, 0 );
	SsEnd();
#endif

	/* init */
	sprintf( buff, "%s.vab", name );
	data = (Byte *)gfxFileLoad( (Text)buff );
	if ( data == NULL ) return( SOUND_LOAD_FAILED );
	size = VAB_BODY_SIZE(data);
	body = data + size;

	/* copy to sound buffer */
	if ( size > 6144 ) {
		printf( "sound buffer too small (6144): %d\n", size );
		return( SOUND_LOAD_FAILED );
	}
	memCopy( data, snd.sound_buffer, size );

	/* close open vab */
	if ( snd.loaded != SOUND_LOAD_FAILED ) {
		SsVabClose( snd.loaded );
		snd.loaded = SOUND_LOAD_FAILED;
	}

#ifdef PSX_FULL_DEV_KIT
	/* open head for allocation of SPU memory */
	vab_id = SsVabOpenHead( (u_char*)snd.sound_buffer, -1 );
	if ( vab_id < 0 )
		return( SOUND_LOAD_FAILED );

	/* transfer body */	
	if ( SsVabTransBody( (u_char*)body, vab_id ) != vab_id )
		return( SOUND_LOAD_FAILED );

	/* wait for completion */
	SsVabTransCompleted( SS_WAIT_COMPLETED );
#else
	/* return vab_id (0-15), open VAB and transfer to sound buffer */
	vab_id = SsVabTransfer( (u_char*)snd.sound_buffer, (u_char*)body, -1, 1 );
	if ( vab_id < 0 )
		return( SOUND_LOAD_FAILED );
#endif

	snd.loaded = vab_id;
	
#ifdef PSX_FULL_DEV_KIT
	/* restart sound callbacks */
	SsStart();
	SsSetMVol( snd.master_volume, snd.master_volume );
	sndSetCDVolume( snd.cd_volume, snd.cd_volume );
#endif

	return( vab_id );
#endif
}


/******************************************************************************
 * Function:
 * sndPlay -- play a sound from a loaded sound file
 * 
 * Description:
 *
 * Returns: channel allocated for the sound
 * 
 */

int sndPlay (
	int		file_id,
	int		sound_id,
	fixed	vol_left,
	fixed	vol_right
	)
{
#ifdef SND_DISABLE_SOUND
	return( SOUND_LOAD_FAILED );
#else
	int		channel;
	
	if ( snd.sfx_volume > 0 )
		channel = (int)SsUtKeyOn( file_id, PROG_NUM(sound_id), TONE_NUM(sound_id), 60, 0, FIXED_TO_FX_VOLUME(vol_left), FIXED_TO_FX_VOLUME(vol_right) );
	//printf( "vol: %d, svol: %d, result: %d\n", vol_left, snd.sfx_volume, FIXED_TO_FX_VOLUME(vol_left) );

	return( channel );
#endif
}


/******************************************************************************
 * Function:
 * sndSetPitch -- alter (bend) the pitch for a sound
 * 
 * Description:
 *
 * Returns: channel allocated for the sound
 * 
 */

void sndSetPitch (
	int		channel,
	int		file_id,
	int		sound_id,
	fixed	bend
	)
{
#ifndef SND_DISABLE_SOUND
	SsUtPitchBend( channel, file_id, PROG_NUM(sound_id), FIXED_TO_VOLUME(bend), FIXED_TO_VOLUME(bend) );
#endif
}


/******************************************************************************
 * Function:
 * sndSetVolume -- adjust the volume for a sound
 * 
 * Description:
 *
 * Returns:
 * 
 */

void sndSetVolume (
	int		channel,
	fixed	vol_left,
	fixed	vol_right
	)
{
#ifndef SND_DISABLE_SOUND
	/* set main volume? */
	if ( channel == SOUND_MASTER_VOLUME ) {
		snd.master_volume = FIXED_TO_VOLUME(vol_left);
		SsSetMVol( snd.master_volume, snd.master_volume );
	} else if ( channel == SOUND_FX_VOLUME )
		snd.sfx_volume = vol_left;
	else {
		SsUtSetVVol( channel, FIXED_TO_FX_VOLUME(vol_left), FIXED_TO_FX_VOLUME(vol_right) );
		//printf( "vol: %d, svol: %d, result: %d\n", vol_left, snd.sfx_volume, FIXED_TO_FX_VOLUME(vol_left) );
	}
#endif
}


/******************************************************************************
 * Function:
 * sndSetCDVolume -- adjust the volume for CD music playback
 * 
 * Description:
 *
 * Returns:
 * 
 */

void sndSetCDVolume (
	fixed	vol_left,
	fixed	vol_right
	)
{
#ifndef SND_DISABLE_SOUND
#ifdef PSX_FULL_DEV_KIT
#ifdef SND_USE_CD
	CdlATV	vol;

	vol.val0 = FIXED_TO_CD_VOLUME(vol_left);
	vol.val1 = 0;
	vol.val2 = FIXED_TO_CD_VOLUME(vol_right);
	vol.val3 = 0;
	snd.cd_volume = vol_left;

	CdMix( &vol );
	SsSetSerialVol( SS_SERIAL_A, FIXED_TO_VOLUME(vol_left), FIXED_TO_VOLUME(vol_right) );
#endif
#endif
#endif
}


