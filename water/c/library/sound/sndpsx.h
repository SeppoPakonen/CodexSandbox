/*****************************************************************
 * Project		: Sound Library
 *****************************************************************
 * File			: sndpsx.h
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

#ifndef THEYER_SNDPSX_H
#define THEYER_SNDPSX_H

//#include <stdio.h>
//#include <stdlib.h>
#include <type/datatype.h>
#include <text/text.h>

#ifdef PSX_FULL_DEV_KIT
#include <libsnd.h>
#include <libspu.h>
#include <libcd.h>
#else

#include <libps.h>

/*
 * Vag & Vab Structure 
 */ 
typedef struct VabHdr {         /* VAB Bank Headdings */

	long           form;          /* always 'VABp' */
	long           ver;           /* VAB file version number */
	long           id;            /* VAB id */
	unsigned long  fsize;         /* VAB file size */
	unsigned short reserved0;     /* system reserved */
	unsigned short ps;            /* # of the programs in this bank */
	unsigned short ts;            /* # of the tones in this bank */
	unsigned short vs;            /* # of the vags in this bank */
	unsigned char  mvol;          /* master volume for this bank */
	unsigned char  pan;           /* master panning for this bank */
	unsigned char  attr1;         /* bank attributes1 */
	unsigned char  attr2;         /* bank attributes2 */
	unsigned long  reserved1;     /* system reserved */

} VabHdr;			/* 32 byte */


typedef struct ProgAtr {        /* Program Headdings */

	unsigned char tones;          /* # of tones */
	unsigned char mvol;           /* program volume */
	unsigned char prior;          /* program priority */
	unsigned char mode;           /* program mode */
	unsigned char mpan;           /* program pan */
	char          reserved0;      /* system reserved */
	short         attr;           /* program attribute */
	unsigned long reserved1;      /* system reserved */
	unsigned long reserved2;      /* system reserved */

} ProgAtr;			/* 16 byte */


typedef struct VagAtr {         /* VAG Tone Headdings */

	unsigned char  prior;         /* tone priority */
	unsigned char  mode;          /* play mode */
	unsigned char  vol;           /* tone volume*/
	unsigned char  pan;           /* tone panning */
	unsigned char  center;        /* center note */
	unsigned char  shift;         /* center note fine tune */
	unsigned char  min;           /* minimam note limit */
	unsigned char  max;           /* maximam note limit */
	unsigned char  vibW;          /* vibrate depth */
	unsigned char  vibT;          /* vibrate duration */
	unsigned char  porW;          /* portamento depth */
	unsigned char  porT;          /* portamento duration */
	unsigned char  pbmin;         /* under pitch bend max */
	unsigned char  pbmax;         /* upper pitch bend max */
	unsigned char  reserved1;     /* system reserved */
	unsigned char  reserved2;     /* system reserved */
	unsigned short adsr1;         /* adsr1 */
	unsigned short adsr2;         /* adsr2 */
	short          prog;          /* parent program*/
	short          vag;           /* vag reference */
	short          reserved[4];   /* system reserved */

} VagAtr;			/* 32 byte */

#endif

typedef struct {
	Byte			sound_buffer[6144];
	int				loaded;
	unsigned long	lock_mask;
	fixed			cd_volume;
	fixed			sfx_volume;
	fixed			master_volume;
} SndData;

#define VAB_BODY_SIZE(vab)		(2080+(((((VabHdr*)vab)->ps)+1)*512))
#define PROG_NUM(sound_id)		((short)(sound_id>>8))
#define TONE_NUM(sound_id)		((short)(sound_id&0xFF))
#define FIXED_TO_VOLUME(fx)		((short)((fx>0)?(((fx-1)&0xFFF)>>5):(0)))	// range 0-127
#define FIXED_TO_FX_VOLUME(fx)	((short)((FIXED_MULTIPLY(fx,snd.sfx_volume)>0)?(((FIXED_MULTIPLY(fx,snd.sfx_volume)-1)&0xFFF)>>5):(0)))	// range 0-127
#define FIXED_TO_CD_VOLUME(fx)	((short)((fx>0)?(((fx-1)&0xFFF)>>4):(0)))	// range 0-255

#endif
