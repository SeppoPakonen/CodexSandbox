/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		PlayStation2
 **********************************************************
 * File:		padps2.c
 * Author:		Mark Theyer
 * Created:		01 May 1997
 **********************************************************
 * Description:	Interface to PS2 controllers
 **********************************************************
 * Functions:
 *	padInit()
 *				Initialise the controllers.
 *	padRead()
 *				Read the controllers.
 *	padMaxPads()
 *				Maximum controllers supported.
 *	padAttached()
 *				Does this port have a controller attached?
 *	padSupported()
 *				Does this port have a supported controller?
 *	padHasAnalog()
 *				Does this port have an analog controller?
 *	padHasShock()
 *				Does this port have an shock capable controller?
 *	padGet()
 *				Get the values from a controller for saving.
 *	padSet()
 *				Set/override the values for a controller.
 *	padAnyButtonDown()
 *				Has any controller pressed this button?
 *	padButtonDown()
 *				Has this controller pressed this button?
 *	padAnalogX()
 *				Get the X axis analog values from a controller.
 *	padAnalogY()
 *				Get the Y axis analog values from a controller.
 *
 *	Private Functions:
 *
 *	padAnalogToFixed()
 *				Convert an analog value to a fixed value.
 *	padProcessPortData()
 *				Get data for a controller on a port
 **********************************************************
 * Revision History:
 * 01-May-97	Theyer	Initial coding.
 * 17-Nov-99	Theyer	Updated documentation.
 * 11-Feb-00	Theyer	Dual Shock and Multi-Tap support
 * 21-Apr-01	Theyer	Created this file from padpsx.c
 * 21-Apr-01	Theyer	Preliminary port for PS2
 * 22-May-01	Theyer	Handle failed comms via scePadRead()
 **********************************************************/

/*
 * includes
 */

#include <type/datatype.h>
#include <pad/pad.h>
#include <pad/padps2.h>
#include <memory/memory.h>

/*
 * macros
 */

#define PAD_VALIDATE	1

/*
 * typedefs
 */


/*
 * prototypes
 */

/*
 * variables
 */

/* pad DMA buffers for each opened port */
Bit128					 pad_dma0[PAD_DMA_BUFFER_SIZE] ALIGN512;
Bit128					 pad_dma1[PAD_DMA_BUFFER_SIZE] ALIGN512;
Bit128					 pad_dma2[PAD_DMA_BUFFER_SIZE] ALIGN512;
Bit128					 pad_dma3[PAD_DMA_BUFFER_SIZE] ALIGN512;
Bit128					 pad_dma4[PAD_DMA_BUFFER_SIZE] ALIGN512;
Bit128					 pad_dma5[PAD_DMA_BUFFER_SIZE] ALIGN512;
Bit128					 pad_dma6[PAD_DMA_BUFFER_SIZE] ALIGN512;
Bit128					 pad_dma7[PAD_DMA_BUFFER_SIZE] ALIGN512;

private  PadInfo		 pad_info;
private  PadControl		 pad_control_data;
private  PadControl		*pad_control = NULL;
private  Byte			 pad_act_align[6] = { 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF };

/*
 * functions
 */

/* standard PlayStation entry */
int main( void )
{
    PadType	*cont;
    int		 i, m, j;
	Byte	*data;
	u_int	*idata;
	int		 state;
	static u_int	saved[2] = { 0x0, 0x0 };

	/* init */
	memClear( &pad_info, sizeof(PadInfo) );
	memClear( &pad_control_data, sizeof(PadControl) );
	pad_control = &pad_control_data;

    /* init control data */
	pad_control->analog_range = (PAD_HIGH_ANALOG_MARGIN - PAD_LOW_ANALOG_MARGIN);
	for ( i=0; i<MAXPADS; i++ ) {
		/* init the analog values */
		for ( m=0; m<4; m++ )
		    pad_control->analog[i][m] = 127;
	}

	/* init raw info data */
	pad_info.pad_dma_buffer[0] = (u_long128 *)&pad_dma0;		// port 0 or multi-tap port 0, slot 0
	pad_info.pad_dma_buffer[1] = (u_long128 *)&pad_dma1;		// port 1 or multi-tap port 1, slot 0
	pad_info.pad_dma_buffer[2] = (u_long128 *)&pad_dma2;		// multi-tap port 0, slot 1
	pad_info.pad_dma_buffer[3] = (u_long128 *)&pad_dma3;		// multi-tap port 0, slot 2
	pad_info.pad_dma_buffer[4] = (u_long128 *)&pad_dma4;		// multi-tap port 0, slot 3
	pad_info.pad_dma_buffer[5] = (u_long128 *)&pad_dma5;		// multi-tap port 1, slot 1
	pad_info.pad_dma_buffer[6] = (u_long128 *)&pad_dma6;		// multi-tap port 1, slot 2
	pad_info.pad_dma_buffer[7] = (u_long128 *)&pad_dma7;		// multi-tap port 1, slot 3
	pad_info.port[0] = 0;						// port for this pad
	pad_info.port[1] = 1;						// port for this pad
	pad_info.port[2] = 0;						// port for this pad
	pad_info.port[3] = 0;						// port for this pad
	pad_info.port[4] = 0;						// port for this pad
	pad_info.port[5] = 1;						// port for this pad
	pad_info.port[6] = 1;						// port for this pad
	pad_info.port[7] = 1;						// port for this pad
	pad_info.slot[0] = 0;						// slot for this pad
	pad_info.slot[1] = 0;						// slot for this pad
	pad_info.slot[2] = 1;						// slot for this pad
	pad_info.slot[3] = 2;						// slot for this pad
	pad_info.slot[4] = 3;						// slot for this pad
	pad_info.slot[5] = 1;						// slot for this pad
	pad_info.slot[6] = 2;						// slot for this pad
	pad_info.slot[7] = 3;						// slot for this pad

	/* initialise RPC */
    sceSifInitRpc(0);

	/* load IOP modules */
    while( sceSifLoadModule( PAD_MODULE_FILE_SIO, 0, NULL ) < 0 )
        printf( PAD_MSG_RETRYING_MODULE, PAD_MODULE_FILE_SIO );
    //while( sceSifLoadModule( PAD_MODULE_FILE_TAP, 0, NULL ) < 0 )
      //  printf( PAD_MSG_RETRYING_MODULE, PAD_MODULE_FILE_TAP );
    while( sceSifLoadModule( PAD_MODULE_FILE_PAD, 0, NULL ) < 0 )
        printf( PAD_MSG_RETRYING_MODULE, PAD_MODULE_FILE_PAD );

	/* reset */
    sceGsResetPath();
    sceDmaReset(1);

	/* init */
	//sceMtapInit();
    scePadInit(0);

	/* open ports */
    for ( i=0; i<NPORTS; i++ ) {
		/* always open port 0 and 1 */
		while( scePadPortOpen( i, 0, pad_info.pad_dma_buffer[i] ) != 1 );
		printf( "scePadPortOpen( %d, 0, 0x%X )\n", i, (u_int)pad_info.pad_dma_buffer[i] );
		pad_info.open[i] = TRUE;
	}

	while(1) {
		/* init pads */
		for ( i=0; i<MAXPADS; i++ ) {
    		/* port open? */
			if ( pad_info.open[i] ) {
				/* get data from the controller */
				data  = pad_info.pad_buffer[i];
				idata = (u_int *)data;
				memClear( data, PAD_DMA_BUFFER_SIZE );
				/* read controller state (wait if necessary) and test if connected */
				if ( scePadRead( PAD_PORT_NUM(i), PAD_SLOT_NUM(i), data ) ) {
					if ( *idata != saved[i] ) {
						printf( "scePadRead( %d, %d, 0x%X ) = 0x%X\n", PAD_PORT_NUM(i), PAD_SLOT_NUM(i), data, *idata );
						saved[i] = *idata;
					}
				}
			}
		}
	}

	return( TRUE );
}  
