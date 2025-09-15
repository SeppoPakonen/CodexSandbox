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
 * 13-Feb-97	Diesi	(SCEI) original software
 * 20-Mar-00	Theyer	Conversion to movie library
 * 20-Mar-00	Theyer	Documented code, reformatted
 * 20-Mar-00	Theyer	function StrGetNext added
 * 20-Mar-00	Theyer	function fmvPlay added
 **********************************************************/

/*
 * include
 */

#include "psxfmv.h"
#include <gfx/gfx.h>

/*
 * macros
 */

/*
 * typedefs
 */

/*
 * globals
 */
 
#if FMV_TESTING
static int		t1, t2;							// Timers.
static DRAWENV	draw[2];						// DrawEnvs for FntPrint.
static short	firstFrame   = 1,				// Frame skipping check.
				numSkipped   = 0,
				prevFrameNum = 0;			
static u_short	maxRunLevel  = 0;
#endif

#ifdef PSX_FULL_DEV_KIT

#define FMV_SOUND_ON

static DecEnv	 dec;							// Streaming Env.
static u_long	 oldHeapbase;  					// Heapbase before PlayStream.
static SndVolume oldVolume;						// Old ADPCM volume.
static FmvData	 fmv;							// FMV data

/*
 * prototypes
 */

static void		 StrDecDCToutCB(void);
static void		 StrInit(StrInfo *str);
static void		 StrEnd(void);
static void		 StrClearVRAM(void);
static short	 StrKickCD(CdlLOC *loc);
static u_long	*StrGetNextFrame(void);
static void		 StrVLCFrame(u_long *frame);
static void		 StrDrawDecodeSync(void);
static short	 StrFadeOutVol(short *fadeVol);
static long		 GetDCToutSize(void);
static u_short	 ScalePPW(u_short n);
static u_short	 GetDCT_MODE(void);
static Bool		 StrGetNext( long timer );
//static u_long	 AllocMem( u_long size );

#endif

#if FMV_TESTING
static void PrintVLCBufSize(void); 
#endif

//static u_long AllocMem(u_long size);

/* 
 * functions
 */


/**********************************************************
 * Function:	fmvPlay
 **********************************************************
 * Description: Play an FMV
 * Inputs:		filename - name of file to play
 *				callback - callback function
 * Notes:		nil
 * Returns:		nil
 **********************************************************/

public void fmvPlay (
	Text		filename,
	Bool		pal,
	int			nframes,
	FmvCallback	callback
	)
{
#ifdef PSX_FULL_DEV_KIT
	StrInfo		str;

	/* init */
	memClear( &fmv, sizeof(FmvData) );

	/* init str data */
	str.strName		= filename;
	str.mode		= STR_MODE24;
	str.drawBorders = 0;
	str.scrWidth	= 320;
	str.x			= 0;
	str.y			= 0;
	str.width		= 320;
	if ( pal ) {
		str.height	 = 256;
		fmv.screen_y = 24;
		fmv.frame_y  = 256;
	} else {
		str.height	 = 240;
		fmv.screen_y = 0;
		fmv.frame_y  = 240;
	}
	str.endFrame	= nframes; //0xFFFFFFFF;
	str.vlcBufSize	= MAX_VLCBUF_SIZE;
	str.volume		= 127;

#ifdef FMV_SOUND_ON
	// Clear last 100K of SPU RAM (i.e. maximum reverb workarea). 
	SpuInit();
	SpuSetTransStartAddr(421887);
	SpuWrite0(1024 * 100);

	/* init sound */
	SsInit();

	SsUtReverbOff();
	SsUtSetReverbType(0);
	SsUtSetReverbDepth(0, 0);

	SsSetTickMode(SS_TICKVSYNC);
	SsSetMVol(127, 127);

	SsSetSerialAttr(SS_SERIAL_A, SS_MIX, SS_SON);
	SsSetSerialVol(SS_SERIAL_A, 127, 127);

	SsUtSetReverbType(SS_REV_TYPE_STUDIO_B);
	SsUtReverbOn();
	VSync(75);						//Delay for a while =;-D
	SsUtSetReverbDepth(40, 40);
#endif

	/* sync */
	DrawSync(0);
	VSync(0);

	/* clear video memory */
	StrClearVRAM();

	/* sync */
	DrawSync(0);
	VSync(0);

	/* play FMV */
	PlayStream( &str, (FmvCallback *)callback );

	/* sync */
	DrawSync(0);
	VSync(0);

	/* clear video memory */
	StrClearVRAM();

	/* sync */
	DrawSync(0);
	VSync(0);
#endif
}


#ifdef PSX_FULL_DEV_KIT
/**********************************************************
 * Function:	PlayStream
 **********************************************************
 * Description: Play an FMV
 * Inputs:		str - stream info
 *				keyHandler - Read controller function 
 *							 (to return non-zero if exit)
 * Notes:		nil
 * Returns:		PLAYSTR_ERROR	= Error.
 *				PLAYSTR_END		= End of stream exit.
 *				PLAYSTR_USER	= User exit.
 **********************************************************/

public short PlayStream ( 
	StrInfo		*str, 
	FmvCallback	 keyHandler
	) 
{
	CdlFILE		 fp;
	u_long		*frame;
	short		 frameRetry = 0;
	short		 strQuit = 0;
	short		 fadeVol = 0;

	/* find file */
	if (!CdSearchFile(&fp, str->strName)) {
#if FMV_DEBUG
		printf("ERROR: MOVIE -- Could not find stream file '%s'.\n", str->strName);
#endif
		return PLAYSTR_ERROR;
	}

	/* init */
	StrInit(str);

	/* start CD */
	if (!StrKickCD(&fp.pos)) {;
#if FMV_DEBUG
		printf("ERROR: MOVIE -- Couldn't start the CD!!\n");
#endif
		StrEnd();
		return PLAYSTR_ERROR;
	}

	/* get first frame */
	while (!(frame = StrGetNextFrame())) {
		if (++frameRetry == MAX_RETRY) {
#if FMV_DEBUG
			printf("ERROR: MOVIE -- Couldn't get first frame!!\n");
#endif
			StrEnd();
			return PLAYSTR_ERROR;
		}
	}

	/* decode huffman compressed frame to RLE */
	frameRetry = 0;
	StrVLCFrame(frame);

	while( !dec.rewindSwitch && !strQuit ) {

		/* decode RLE, output to image buffer */
		DecDCTin( dec.vlcBuffer[dec.vlcId], GetDCT_MODE() );
		DecDCTout( dec.imageBuffer[dec.imageId], GetDCToutSize() );

		/* get next frame */
		if (!(frame = StrGetNextFrame())) {
			if (++frameRetry == MAX_RETRY)
				strQuit = PLAYSTR_ERROR;		
		} else {
			frameRetry = 0;
		}

#if FMV_TESTING
		/* time huffman decode */
		t2 = VSync(1);
		StrVLCFrame(frame);
		t2 = VSync(1) - t2;
#else 
		/* huffman decode */
		StrVLCFrame(frame);

		// Check for exit. 
		if (!fadeVol && (*keyHandler)())
			fadeVol = dec.volume;

		// Fade the volume down to zero. Then trigger exit.
		if (fadeVol)
			strQuit = StrFadeOutVol(&fadeVol);
#endif

		/* wait for decode to finish */
		StrDrawDecodeSync();

#if FMV_TESTING
		/* print timing information */
		DrawSync(0);
		printf( "\nVLC Time = %d\n", t2 );
		printf( "Total Time = %d\n", (VSync(1) - t1) );
#endif

		/* sync... */
		DrawSync(0);
		VSync(0);

#if FMV_TESTING
		/* start timer */
		t1 = VSync(1);
#endif

		PutDispEnv(&dec.disp[dec.rectId]);

#if FMV_TESTING
		// Put DrawEnv for FntPrint.
		PutDrawEnv(&draw[dec.rectId]);
#endif
	}

	/* clean up */
	StrEnd();

	// Check for end of stream exit.
	if (dec.rewindSwitch)
		strQuit =  PLAYSTR_END;

#if FMV_DEBUG
	// check for error
	if (strQuit == PLAYSTR_ERROR) 
		printf("ERROR: MOVIE -- Couldn't get frame from ring buffer!!\n");
#endif

#if FMV_TESTING
	// If stream played completely print the maximum VLC buffer size.
	if (strQuit == PLAYSTR_END)
		PrintVLCBufSize(); 
#endif

	return strQuit;
}


/**********************************************************
 * Function:	StrDecDCToutCB
 **********************************************************
 * Description: Load decoded image chunk into VRAM.
 * Inputs:		nil
 * Notes:		Callback is called when image decode is 
 *				finished.
 * Returns:		nil
 **********************************************************/

static void StrDecDCToutCB (
	void
	) 
{
	// In 24bit mode call StCdInterrupt() manually.
	if (dec.is24Bit) {
		extern StCdIntrFlag;
		if (StCdIntrFlag) {
			StCdInterrupt();
			StCdIntrFlag = 0;
		}
	}

	/* load image chunk into VRAM */
	LoadImage( &dec.slice, dec.imageBuffer[dec.imageId] );	
	dec.imageId ^= 1;
	dec.slice.x += dec.slice.w;

	DrawSync(0);

	/* get next slice to decode or frame is done */
	if (dec.slice.x < (dec.rect[dec.rectId].x + dec.rect[dec.rectId].w) )
		DecDCTout( dec.imageBuffer[dec.imageId], GetDCToutSize() );
	else {
		DecDCTReset(1);
		dec.frameDone = 1;
	}
}


/**********************************************************
 * Function:	StrInit
 **********************************************************
 * Description: Initialise stream data, the MDEC and 
 *				allocate the buffers.
 * Inputs:		str - stream data
 * Notes:		nil
 * Returns:		initialised data structure
 **********************************************************/

static void StrInit (
	StrInfo *str
	) 
{
	short		 x, y, w, h;
	short		 scrWidth;
	u_long		*ringBuf;
	u_long		 vlcBufSize,
				 imageBufSize;	
	int			 size;

	oldHeapbase = __heapbase;

	x = str->x;
	y = str->y;	
	w = str->width;
	h = str->height;
	scrWidth = str->scrWidth;

#if FMV_TESTING
	// Init frame skipping checks.
	firstFrame = 1;
	numSkipped = 0;
	prevFrameNum = 0;			

	// Init DrawEnvs for FntPrint.
	SetDefDrawEnv(&draw[0], 0,   0,           scrWidth, fmv.frame_y );
	SetDefDrawEnv(&draw[1], 0,   fmv.frame_y, scrWidth, fmv.frame_y );
	setRGB0(&draw[0], 0, 0, 0);
	setRGB0(&draw[1], 0, 0, 0);
	draw[0].isbg = draw[1].isbg = 1;

	// init run level
	maxRunLevel = 0;
#endif

	dec.is24Bit = str->mode; 
	dec.scrWidth = scrWidth;
	dec.x = x;
	dec.y = y;
	dec.width = w;
	dec.height = h;
	dec.endFrame = str->endFrame;
	dec.volume = str->volume;
	dec.frameCount = 0;
	dec.rewindSwitch = 0;
	dec.frameDone = 0;
	dec.rectId = 0;
	dec.vlcId = 0;
	dec.imageId = 0;
	
	// Init the display environments.
	SetDefDispEnv(&dec.disp[0], 0,   fmv.frame_y, scrWidth, fmv.frame_y );
	SetDefDispEnv(&dec.disp[1], 0,   0,           scrWidth, fmv.frame_y );
	setRECT(&dec.disp[0].screen, 0, fmv.screen_y, 0, fmv.frame_y );	
	setRECT(&dec.disp[1].screen, 0, fmv.screen_y, 0, fmv.frame_y );	
	dec.disp[0].isrgb24 = dec.disp[1].isrgb24 = (str->mode == STR_MODE24);

	if (str->drawBorders || dec.is24Bit)			// Clear VRAM.
		StrClearVRAM();

	// Set correct display mode before we begin drawing.
	VSync(0);
	PutDispEnv(&dec.disp[dec.rectId]);

	setRECT(&dec.rect[0], ScalePPW(x), y, ScalePPW(w), h);  
	setRECT(&dec.rect[1], ScalePPW(x), y + fmv.frame_y, ScalePPW(w), h);
	dec.slice = dec.rect[dec.rectId];
	dec.slice.w = ScalePPW(SLICE_WIDTH);

	// Alloc VLC and image buffers (amount allocated depends on frame size,
	// streaming mode and maximum runlevel size).
#if FMV_TESTING
	vlcBufSize = MAX_VLCBUF_SIZE;
#else
	if (str->vlcBufSize)
		vlcBufSize = str->vlcBufSize;
	else
		vlcBufSize = MAX_VLCBUF_SIZE;
#endif

#if 0
	dec.vlcBuffer[0] = (u_long *) AllocMem(vlcBufSize);
	dec.vlcBuffer[1] = (u_long *) AllocMem(vlcBufSize);
#else
	dec.vlcBuffer[0] = (u_long *)gfxMemoryBuffer( GFX_PSX_GPU_MEMORY, &size );
	vlcBufSize = size/2;
	dec.vlcBuffer[1] = (u_long *)(dec.vlcBuffer[0] + (vlcBufSize/sizeof(u_long)));
#endif

	if (dec.is24Bit)
		imageBufSize = SLICE_WIDTH * 3 * h;
	else
		imageBufSize = SLICE_WIDTH * 2 * h;

#if 0
	dec.imageBuffer[0] = (u_long *) AllocMem(imageBufSize);
	dec.imageBuffer[1] = (u_long *) AllocMem(imageBufSize); 
#else
	dec.imageBuffer[0] = (u_long *)gfxMemoryBuffer( GFX_PSX_FILE_MEMORY, &size );
	if ( size < (imageBufSize * 2)) {
		printf( "insufficient image memory\n" );
		exit(1);
	}
	dec.imageBuffer[1] = (u_long *)(dec.imageBuffer[0] + (imageBufSize/sizeof(u_long)));
#endif

	// Init the MDEC.
	DecDCTReset(0);
	DecDCTvlcSize(vlcBufSize);
	DecDCToutCallback((void (*)()) StrDecDCToutCB);

	ringBuf = (u_long *)gfxMemoryBuffer( GFX_PSX_OT_MEMORY, &size );
	StSetRing(ringBuf, (size/2336) );
	fmv.ringbuf = ringBuf;

	// Start no matter the frame number!
	StSetStream(dec.is24Bit, 0, 0xffffffff, 0, 0);

#ifdef FMV_SOUND_ON
	// Store old ADPCM volume and set new volume.
	SsGetSerialVol(SS_SERIAL_A, &oldVolume);
	SsSetSerialVol(SS_SERIAL_A, dec.volume, dec.volume);
#endif
}


/**********************************************************
 * Function:	StrEnd
 **********************************************************
 * Description: Clean up.
 * Inputs:		str - stream data
 * Notes:		nil
 * Returns:		nil
 **********************************************************/

static void StrEnd (
	void
	) 
{
	/* clear ring buffer and pause CD */
	StUnSetRing();
	CdControlB(CdlPause, 0, 0);

	/* clear decode callback */
	DecDCToutCallback(0);

	/* re-init heapbase global */
	__heapbase = oldHeapbase;
		
	// Clear VRAM to remove any 24bit graphics.
	if (dec.is24Bit)
		StrClearVRAM();

#ifdef FMV_SOUND_ON
	// Restore previous ADPCM volume. 
	SsSetSerialVol(SS_SERIAL_A, oldVolume.left, oldVolume.right);
#endif

#if FMV_DEBUG
	printf("FrameCount %d\n", dec.frameCount);
#endif
}


/**********************************************************
 * Function:	StrClearVRAM
 **********************************************************
 * Description: Clear frame buffer memory
 * Inputs:		nil
 * Notes:		Useful before and after streaming in 24bit 
 *				mode to ensure 24bit data is not displayed 
 *				in 16bit mode (and vice versa).
 * Returns:		nil
 **********************************************************/

static void StrClearVRAM (
	void
	) 
{
	RECT	clrRect;

#if 1
	setRECT(&clrRect, 0, 0, ScalePPW(dec.scrWidth), fmv.frame_y );
	ClearImage(&clrRect, 0, 0, 0);
	clrRect.y = fmv.frame_y;
	ClearImage(&clrRect, 0, 0, 0);
#else
	setRECT( &clrRect, 0, 0, 512, 512 );
	ClearImage( &clrRect, 0, 0, 0 );
#endif
}


/**********************************************************
 * Function:	StrKickCD
 **********************************************************
 * Description: Start the CD
 * Inputs:		loc - location. 
 * Notes:		
 * Returns:		0 on error, 1 otherwise.
 **********************************************************/

static short StrKickCD (
	CdlLOC *loc
	) 
{
	short	seekRetry = 0,
			readRetry = 0;	

	do {
		while (!CdControl(CdlSeekL, (u_char *)loc, 0)) {
			if (++seekRetry == MAX_RETRY)
				return 0;
		}

		seekRetry = 0;

		if (readRetry++ == MAX_RETRY)
			return 0;

	} while (!CdRead2(CdlModeStream|CdlModeSpeed|CdlModeRT));

	return 1;
}


/**********************************************************
 * Function:	StrGetNextFrame
 **********************************************************
 * Description: Get the next frame from the ring buffer.
 * Inputs:		nil
 * Notes:		Also checks for the end of the stream. If 
 *				the end has been reached rewindSwitch is 
 *				set. If frame is not obtained from the ring 
 *				timeout and return NULL.
 * Returns:		Address of next frame or NULL on error.
 **********************************************************/

static u_long *StrGetNextFrame ( 
	void
	) 
{
	//long		 timer = WAIT_TIME;
	u_long		*addr;
	StHEADER	*sector;    

	// get next sector
	if ( ! StrGetNext( WAIT_TIME ) )
		return NULL;

	// init
	addr   = fmv.addr;
	sector = fmv.sect;

	// Update internal frame count.
	dec.frameCount++;				

#if FMV_TESTING
	// Check for frame skipping.
	if (firstFrame) {
		prevFrameNum = sector->frameCount;
		firstFrame = 0;

	} else if ((sector->frameCount - prevFrameNum) != 1) {
		numSkipped++;	
	}
		
	prevFrameNum = sector->frameCount;
	printf("Skipped =  %d\n", numSkipped);
#endif

#if FMV_DEBUG
	if (dec.frameCount == 1)
		printf("MOVIE: w = %d, h = %d, sectors = %d\n", 
				sector->width, sector->height, sector->nSectors);

	printf("Frame No. = %d\n", dec.frameCount);
#endif
	
	// End of stream checks.
	if (sector->frameCount > dec.endFrame) {
#if FMV_DEBUG	
		printf("ERROR: MOVIE -- Found sector frame count over > end frame (%d > %d)\n",
				sector->frameCount, dec.endFrame);
#endif
		dec.rewindSwitch = 1;
		return addr;
	}
	
	if (sector->frameCount < dec.frameCount) {
#if FMV_DEBUG	
		printf("ERROR: MOVIE -- Found sector frame count < internal frame count. (%d < %d)\n",
			 sector->frameCount, dec.endFrame);
#endif
		dec.rewindSwitch = 1;
		return addr;
	}

	if (sector->width != dec.width || sector->height != dec.height) {
#if FMV_DEBUG	
		printf("ERROR: MOVIE -- Found bad sized frame (%d x %d) is not (%d x %d).\n",
				sector->width, sector->height, dec.width, dec.height);
#endif
		dec.rewindSwitch = 1;
		return addr;
	}
	
    return( addr );  
}


/**********************************************************
 * Function:	StrGetNext
 **********************************************************
 * Description: Get the next stream sector
 * Inputs:		nil
 * Notes:		nil
 * Returns:		nil
 **********************************************************/

static Bool StrGetNext ( 
	long	 timer
	) 
{  
    // Get the next frame from the ring buffer.
	while( StGetNext( &fmv.addr, (u_long**)&fmv.sect ) ) {
		if ( --timer == 0 )
			return( FALSE );
	}

	return( TRUE );
}


/**********************************************************
 * Function:	StrVLCFrame
 **********************************************************
 * Description: VLC decode the frame's BS.
 * Inputs:		frame - Address of frame BS.
 * Notes:		Unlock frame from the ring buffer once 
 *				decoded.
 * Returns:		nil
 **********************************************************/

static void StrVLCFrame ( 
	u_long *frame
	) 
{
	int		result;

	// If no frame to decode return.
	if ( frame == NULL ) {
#if FMV_DEBUG
		printf("ERROR: MOVIE -- No frame to VLC decode!!\n");
#endif
		return;
	}

#if FMV_TESTING
	// Print BS Encoded Format (i.e. Version 2 or 3). 
	printf( "BS Ver = %d\n", *((u_short *) frame + 3) );
	if (DecDCTBufSize(frame) > maxRunLevel)
		maxRunLevel = DecDCTBufSize(frame);
#endif

	//printf( "frame size=%d\n", DecDCTBufSize(frame) );

	// decode 
	dec.vlcId ^= 1;

	if ( DecDCTvlc( frame, dec.vlcBuffer[dec.vlcId] ) != 0 ) {
#if FMV_DEBUG
		printf("ERROR: MOVIE -- VLC decode failed!!\n");
#endif
		return;
	}

	StFreeRing( frame );
}


/**********************************************************
 * Function:	StrDrawDecodeSync
 **********************************************************
 * Description: Wait for current frame's decoding and 
 *				drawing to be finished.
 * Inputs:		nil
 * Notes:		nil
 * Returns:		nil
 **********************************************************/

static void	StrDrawDecodeSync (
	void
	) 
{
	long	timer = WAIT_TIME;

	while (!dec.frameDone) {
		if ( --timer == 0) {
#if FMV_DEBUG
			printf("ERROR: MOVIE -- MDEC decode timeout!!\n");
#endif
			break;
		}
	}

	dec.frameDone = 0;

	dec.rectId ^= 1;
	dec.slice.x = dec.rect[dec.rectId].x;
	dec.slice.y = dec.rect[dec.rectId].y;
}


/**********************************************************
 * Function:	StrFadeOutVol
 **********************************************************
 * Description: Fade out the volume.
 * Inputs:		fadeVol - current volume.
 * Notes:		Update fadeVol
 * Returns:		1 = Volume completely faded (therefore 
 *					finish playing the stream).
 *				0 = Otherwise.
 **********************************************************/

static short StrFadeOutVol (
	short *fadeVol
	) 
{
	if ((*fadeVol -= FADE_VOL_SPD) < 0)
		*fadeVol = 0;

#ifdef FMV_SOUND_ON
	SsSetSerialVol(SS_SERIAL_A, *fadeVol, *fadeVol);
#endif

	if (!*fadeVol)
		return PLAYSTR_USER;			// Flag user exit.
	else
		return 0;
}


/**********************************************************
 * Function:	GetDCToutSize
 **********************************************************
 * Description: Get DecDCT output size.
 * Inputs:		fadeVol - current volume.
 * Notes:		Update fadeVol
 * Returns:		DecDCTout size in longs for current 
 *				playback mode.
 **********************************************************/

static long GetDCToutSize (
	void
	) 
{
	u_short	h = dec.height;
	
	if (dec.is24Bit)
		return (((SLICE_WIDTH * PPW24) * bound16(h)) >> 1);
	else
		return (((SLICE_WIDTH * PPW16) * bound16(h)) >> 1);
}


/**********************************************************
 * Function:	ScalePPW
 **********************************************************
 * Description: scale words per pixel.
 * Inputs:		n - a number.
 * Notes:		Update fadeVol
 * Returns:		n scaled by the correct PPW (words/pixel). 
 *				This depends on the current playback mode.
 **********************************************************/

static u_short ScalePPW (
	u_short n
	) 
{
	if (dec.is24Bit)
		return(n * PPW24);
	else
		return(n * PPW16);
}


/**********************************************************
 * Function:	GetDCT_MODE
 **********************************************************
 * Description: Get the current DCT mode.
 * Inputs:		nil
 * Notes:		nil
 * Returns:		The current DCT mode.	
 **********************************************************/

static u_short GetDCT_MODE (
	void
	) 
{
	return ((dec.is24Bit) ? DCT_MODE24 : DCT_MODE16);
}


#if FMV_TESTING
/**********************************************************
 * Function:	PrintVLCBufSize
 **********************************************************
 * Description: Prints the maximum VLC buffer size to play 
 *				a stream.
 * Inputs:		nil
 * Notes:		nil
 * Returns:		The current DCT mode.	
 **********************************************************/

static void PrintVLCBufSize (
	void
	)
{
	printf("\nMaximum VLC Buffer (RunLevel) Size\n");
	printf("----------------------------------\n");
	printf("Max RunLevel size = %d bytes.\n", maxRunLevel << 2);
	printf("Max size of each VLC buffer = %d bytes.\n\n", (maxRunLevel << 2) + RL_HEADER_SIZE);

	// If frames were skipped warn of possible incorrect result! 
	if (numSkipped) {
		printf("Warning:\n");
		printf("\tSkipped %d frames during playback!\n", numSkipped);
		printf("\tTherefore the maximum VLC buffer size may be incorrect!\n\n"); 
	}
}
#endif

#if 0
/* - Type:	PRIVATE
 * -
 * - Param:	size = (In) No. of bytes to allocate.
 * -
 * - Ret:	Base of allocated mem.
 * -
 * - Usage:	Allocate "size" bytes from the heap.
 */

static u_long AllocMem (
	u_long size
	) 
{
	u_long mem = __heapbase;
	
	if ((size & 3) != 0)
		size = (size + 4) & 0xfffffffc;

#if FMV_DEBUG
	if ((__heapbase + size) >= (0x80000000 + _ramsize - _stacksize))
		printf("ERROR: MOVIE -- Not enough memory, can't allocate %d bytes!\n", size);
	else
		printf("Allocated %d bytes. Free memory = %d bytes.\n",
				size,
				(0x80000000 + _ramsize - _stacksize) - (__heapbase + size)
				);
#endif			

	__heapbase += size;

	return mem; 
}
#endif
#endif	// PSX_FULL_DEV_KIT

