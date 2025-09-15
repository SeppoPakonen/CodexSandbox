/**********************************************************
 * Copyright (C) 1997 Sony Computer Entertainment Inc.    
 * Copyright (C) 2000 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:	PSX game
 **********************************************************
 * File:	movie.h
 * Author:	Mark Theyer
 * Created:	20 March 2000
 **********************************************************
 * Description:	FMV Movie player library
 **********************************************************
 * Revision History:
 * 13-Feb-97	Theyer	Vince Diesi (SCEI)
 * 20-Mar-00	Theyer	Conversion to movie library
 **********************************************************/
 
#ifndef THEYER_PSXFMV_H
#define THEYER_PSXFMV_H

/*
 * include
 */

#include <type/datatype.h>
#include <memory/memory.h>
#include <text/text.h>
#include <movie/movie.h>
//#include <ar_lib.h>

#ifdef PSX_FULL_DEV_KIT

/* sony includes */
#include <sys/types.h>
#include <libsn.h>
#include <r3000.h>
#include <asm.h>
#include <kernel.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libcd.h>
#include <libspu.h>
#include <libpress.h>
#include <libsnd.h>

/*
 * macros
 */

// Streaming modes.
#define STR_MODE24				1
#define STR_MODE16				0

// Border modes.
#define STR_BORDERS_OFF			0
#define STR_BORDERS_ON			1

// PlayStream return values.
#define	PLAYSTR_END				1
#define	PLAYSTR_USER			2
#define	PLAYSTR_ERROR			3

// Define for timing and testing functions.
#define FMV_TESTING				0
// Define for extra debug info.
#define FMV_DEBUG				0

// Maximum number of retries.
#define MAX_RETRY				5

// Form1 sector size in bytes.
#define FORM1_SIZE				2048 

// Ring buffer size in sectors.
#define RING_SIZE				64

// Maximum possible VLC buffer size (bytes) = 128K + (Header) = 129K.
#define MAX_VLCBUF_SIZE			132096

// Slice width in pixels.
#define SLICE_WIDTH				16

// Words/Pixel for 24-bit and 16-bit modes.
#define PPW24					3 / 2
#define PPW16					1    

// DCT modes for 24-bit and 16-bit (STP = 1) streaming.
#define DCT_MODE24				3
#define DCT_MODE16				2

#define bound(val, n)			((((val) - 1) / (n) + 1) * (n))
#define bound16PPW(val, ppw)	(bound((val), 16 * ppw))
#define bound16(val)			(bound((val), 16))

// Speed of volume fade.
#define FADE_VOL_SPD			8

// RunLevel header size in bytes.
#define RL_HEADER_SIZE			1024

/*
 * typedefs
 */

typedef struct {
	long				 file_handle;			// ARS file handle
	int					 dbuff;					// double buffer
	u_long				*addr;                      
	StHEADER			*sect;
	StHEADER			 header;
	u_long				*ringbuf;
	int					 screen_y;
	int					 frame_y;
	//u_long			 audio_buffer[2][2400];
} FmvData;

typedef struct {
	volatile short		 is24Bit;				// = 1 when 24-bit streaming.
	volatile u_short	 scrWidth;				// Screen width resolution.
	volatile u_short	 x;						// Str X, Y.
	volatile u_short	 y;	
	volatile u_short	 width;					// Str width and height.
	volatile u_short	 height;
	volatile u_long		 endFrame;          	// End frame. 
	volatile short		 volume;				// ADPCM Volume.
	volatile u_long		 frameCount;			// Frame count.
	RECT				 rect[2];
	RECT				 slice;					// Slice decoded by DecDCTout().
	volatile short		 rewindSwitch;			// = 1 when end of stream.
	volatile short		 frameDone;				// = 1 when decoding and drawing done.
	volatile short		 rectId;
	volatile short		 vlcId;					// VLC buffer id.
	volatile short		 imageId;				// Image buffer id.
	u_long				*vlcBuffer[2];			// VLC run-level buffers.
	u_long				*imageBuffer[2];		// Strip image buffers.
	DISPENV				 disp[2];				// Display envs.
} DecEnv;

typedef struct {
		char			*strName;				// Stream file name. 
		short			 mode;					// 24-Bit or 16-Bit streaming.
		short			 drawBorders;			// != 0 if borders on.
		u_short			 scrWidth;				// Screen res width.
		u_short			 x;						// X,Y position. 
		u_short			 y;
		u_short			 width;					// Stream width and height.
		u_short			 height;
		u_long			 endFrame;				// Last frame No.
		u_long			 vlcBufSize;			// Size of each VLC buffer (including header).
		u_short			 volume;				// Left and Right ADPCM volume.
} StrInfo;

/*
 * globals
 */
 
extern u_long	__heapbase;						// LibSN memory vars.
extern u_long	__heapsize;
extern u_long	_ramsize;
extern u_long	_stacksize;

/*
 * prototypes
 */

short PlayStream( StrInfo *str, FmvCallback callback );

#endif
#endif // THEYER_PSXFMV_H
