/**********************************************************
 * Copyright (C) TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * File:		pad.h
 * Author:		Mark Theyer
 * Created:		01 May 1997
 **********************************************************
 * Description:	Generic Game Controller Interface
 **********************************************************
 * Revision History:
 * 01-May-97	Theyer	Initial Coding
 * 21-Apr-01	Theyer	PlayStation2 port (no changes).
 **********************************************************/

#ifndef THEYER_PAD_H
#define THEYER_PAD_H

/*
 * include
 */

#include <type/datatype.h>

#ifdef OsTypeIsWin95
#include <gui/gui.h>
#endif

/*
 * macros
 */

/* public pad types returned by padType() */
#define PAD_TYPE_UNSUPPORTED	-1
#define PAD_TYPE_NONE			 0
#define PAD_TYPE_PSX_STD	 	 1
#define PAD_TYPE_PSX_ANALOG		 2
#define PAD_TYPE_PSX_DUALSHOCK	 3
#define PAD_TYPE_PSX_MULTITAP	 4

/* pad buttons */
#define PAD_ANY_BUTTON			0x00003FFF
#define PAD_BUTTON_RIGHT		0x00000001
#define PAD_BUTTON_LEFT			0x00000002
#define PAD_BUTTON_UP			0x00000004
#define PAD_BUTTON_DOWN			0x00000008
#define PAD_BUTTON_START		0x00000010
#define PAD_BUTTON_SELECT		0x00000020
#define PAD_BUTTON_A			0x00000040 
#define PAD_BUTTON_B			0x00000080
#define PAD_BUTTON_C			0x00000100
#define PAD_BUTTON_D			0x00000200
#define PAD_BUTTON_LEFT_1		0x00000400
#define PAD_BUTTON_LEFT_2		0x00000800
#define PAD_BUTTON_RIGHT_1		0x00001000
#define PAD_BUTTON_RIGHT_2		0x00002000

/* shock types */
#define PAD_SHOCK_MINOR			0
#define PAD_SHOCK_MAJOR			1

#define PAD_NONE			-1

/*
 * typedefs
 */

/* pad state is 32 bits */
typedef unsigned int	PadState;
typedef unsigned int 	PadAnalog;
typedef Byte		 	PadPressure[12];

typedef struct {
	PadState	state;
	PadAnalog	analog;
} PadInput;

typedef struct {
	PadState	state;
	PadAnalog	analog;
	PadPressure	pressure;
} PadInput2;

/*
 * prototypes
 */

extern Bool padInit( void );
extern Bool padRead( void );

extern int  padMaxPads( void );
extern Bool padAttached( int padnum );
extern Bool padSupported( int padnum );
extern Bool padHasAnalog( int padnum );
extern Bool padHasPressure( int padnum );
extern Bool	padHasShock( int padnum );
extern Bool padHasMultiTap( int padnum );
extern int 	padType( int padnum );		// maybe should not support this...
	
extern PadInput padGet( int padnum );
extern void     padSet( int padnum, PadInput input );

extern PadInput2	padGet2( int padnum );
extern void			padSet2( int padnum, PadInput2 input );

extern int  padAnyButtonUp( int button );
extern int  padAnyButtonDown( int button );
extern int  padAnyButtonPressed( int button );

extern Bool padButtonUp( int padnum, int button );
extern Bool padButtonDown( int padnum, int button );
extern Bool padButtonPressed( int padnum, int button );

#if PAD_FIXEDPOINT_API
extern fixed padAnalogX( int padnum, int sticknum );
extern fixed padAnalogY( int padnum, int sticknum );
#else
extern float padButtonPressure( int padnum, int button );
extern float padAnalogX( int padnum, int sticknum );
extern float padAnalogY( int padnum, int sticknum );
#endif

extern void padSendShock( int padnum, int type, Byte value );
extern void padStopShock( void );

#ifdef OsTypeIsWin95
extern void   padSendKey( GuiEvent *event );
#endif

#endif
