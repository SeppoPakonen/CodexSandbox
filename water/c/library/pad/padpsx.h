/**********************************************************
 * Copyright (C) 1999 Mark Theyer
 * All Rights Reserved
 **********************************************************
 * Project:	PSX game
 **********************************************************
 * File:	padpsx.h
 * Author:	Mark Theyer
 * Created:	28 Oct 1999
 **********************************************************
 * Description:	Interface to PSX controller
 **********************************************************
 * Revision History:
 * 28-Oct-99	Theyer	Initial coding from pad.h and padpsx.c
 **********************************************************/

#ifndef THEYER_PADPSX_H
#define THEYER_PADPSX_H

/*
 * includes
 */

#include <type/datatype.h>

#ifdef PSX_FULL_DEV_KIT
/* full libs can use libapi or libpad */
#define	USE_LIBPAD_API		1
#if USE_LIBPAD_API
#include <stddef.h>
#include <libetc.h>
#include <libpad.h>
#else
#include <libapi.h>
#endif
#else
/* net yaroze lib */
#include <libps.h>
#endif

#include <pad/pad.h>

/*
 * macros
 */

/* limits */
#define MAXPADS			8

/* ports */
#define PAD0			0
#define PAD1			1
#define PAD2			2
#define PAD3			3

/* pad status */
#define PAD_ATTACHED			0x10000000

/* analog margins to allow for inaccuracy of feedback */
#define PAD_LOW_ANALOG_MARGIN	 15
#define PAD_HIGH_ANALOG_MARGIN	110

/* controller attached codes */
#define NO_CONTROLLER  		0xFF
#define HAS_CONTROLLER 		0x00

/* number of pads supported */
#define NPADS 				2

/* controller types */
#define PAD_NOT_THERE		0
#define PAD_TYPE_MOUSE		1
#define PAD_TYPE_NEGCON		2
#define PAD_TYPE_STD		4
#define PAD_TYPE_ANALOG		5
#define PAD_TYPE_DUALSHOCK	7
#define PAD_TYPE_MULTITAP	8

/* pad initialisation */
#define PAD_INIT_NONE		0x00
#define PAD_INIT_STAGE_1	0x01
#define PAD_INIT_STAGE_2	0x02
#define PAD_INIT_FULL		0x04

#if USE_LIBPAD_API
#else
/* constants for the PSX standard controller */
#define PADLup     			0x00001000	/*(1<<12)*/
#define PADLdown   			0x00004000	/*(1<<14)*/
#define PADLleft   			0x00008000	/*(1<<15)*/
#define PADLright  			0x00002000	/*(1<<13)*/
#define PADRup     			0x00000010	/*(1<< 4)*/
#define PADRdown   			0x00000040	/*(1<< 6)*/
#define PADRleft   			0x00000080	/*(1<< 7)*/
#define PADRright  			0x00000020	/*(1<< 5)*/
#define PADi       			0x00000200	/*(1<< 9)*/
#define PADj				0x00000400	/*(1<<10)*/
#define PADk       			0x00000100	/*(1<< 8)*/
#define PADl       			0x00000008	/*(1<< 3)*/
#define PADm      			0x00000002	/*(1<< 1)*/
#define PADn       			0x00000004	/*(1<< 2)*/
#define PADo       			0x00000001	/*(1<< 0)*/
#define PADh       			0x00000800	/*(1<<11)*/
#define PADL1      			PADn
#define PADL2      			PADo
#define PADR1      			PADl
#define PADR2      			PADm
#define PADstart   			PADh
#define PADselect  			PADk
#endif

#define padAbs(n)	((n > 0) ? n : -(n))

/* direct connection port macros */
#define PAD_PORT_DEV(i)					(i<<4)
#define PAD_PORT_TYPE(i)				((PadType *)(pad_info.port[i]+1))
#define PAD_PORT_CONNECTED(i)			(*pad_info.port[i])
#define PAD_PORT_ADDRESS(i)				(pad_info.port[i])
#define PAD_PORT_NUM(i)					(i)

/* multi-tap connection port macros */
#define PAD_MTAP_PORT_DEV(i,m)			((i<<4)+m)
#define PAD_MTAP_PORT_TYPE(i,m)			((PadType *)(pad_info.port[i]+3+(m*8)))
#define PAD_MTAP_PORT_CONNECTED(i,m)	(*(pad_info.port[i]+2+(m*8)))
#define PAD_MTAP_PORT_ADDRESS(i,m)		((pad_info.port[i]+2+(m*8)))
#define PAD_MTAP_PORT_NUM(i,m)			((m==0)?(i):((i*4)+(m+((i==0)?(1):(0)))))

/*
 * typedefs
 */

#if 0
/* pad config */
typedef struct {
    PadState	right;
    PadState	left;
    PadState	up;
    PadState	down;
    PadState	start;
    PadState	select;
    PadState	buttonA; 
    PadState	buttonB;
    PadState	buttonC;
    PadState	buttonD;
    PadState	left1;
    PadState	left2;
    PadState	right1;
    PadState	right2;
} PadConfig;
#endif

/* may need a hasleftright Bool value for each pad? */
typedef struct {
    int			npads;
//  PadConfig	padconfig[MAXPADS];
    Bool		hasanalog[MAXPADS];
	Byte		ctype[MAXPADS];
    Byte		analog[MAXPADS][4];
    PadState	state[MAXPADS];
    PadState	last_state[MAXPADS];
	int			analog_range;
} PadControl;

typedef struct {
    Byte			 size:4,
    				 type:4;
} PadType;

typedef struct {
    volatile unsigned char *port[NPADS];				/* port buffers					*/
    Byte					ctype[NPADS];				/* current controller type 		*/
    Byte					init[NPADS];				/* controller initialised flag 	*/
    Byte					has_shock[NPADS];			/* has shock support flag 		*/
    Byte					mtap_ctype[NPADS][4];		/* current controller type 		*/
    Byte					mtap_init[NPADS][4];		/* controller initialised flag 	*/
    Byte					mtap_has_shock[NPADS][4];	/* has shock support flag 		*/
    Byte					shock[MAXPADS][2];			/* shock actuator data			*/
} PadInfo;

#endif	// THEYER_PADPSX_H

