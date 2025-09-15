/**********************************************************
 * Copyright (C) 1997 Mark Theyer
 * All Rights Reserved
 **********************************************************
 * Project:	PSX game
 **********************************************************
 * File:	padpsx.c
 * Author:	Mark Theyer
 * Created:	01 May 1997
 **********************************************************
 * Description:	Interface to PSX controller
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
 **********************************************************/

/*
 * includes
 */

#include <type/datatype.h>
#include <pad/pad.h>
#include <pad/padpsx.h>
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

private fixed padAnalogToFixed( int value );
private void  padProcessPortData( Bool tap, int port, int n );

/*
 * variables
 */

/* low-level pad buffers: never need to touch */
#ifdef PSX_FULL_DEV_KIT
private  Byte		 pad_buffer_a[36];	// 34 bytes required (+2 for align)
private  Byte		 pad_buffer_b[36];	// 34 bytes required (+2 for align)
#endif

volatile unsigned char	*pad_port0, *pad_port1;
private  PadInfo		 pad_info;
private  PadControl		 pad_control_data;
private  PadControl		*pad_control = NULL;
private  Byte			 pad_act_align[6] = { 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF };

/*
 * functions
 */

 
/**********************************************************
 * Function:	padInit
 **********************************************************
 * Description: Initialise input controllers
 * Inputs:	
 * Notes:		call once only at program initialisation
 * Returns:
 **********************************************************/

public Bool padInit(
    void
    )
{
    PadType	*cont;
    int		 i, j, m;
	int		 state;
	int		 padnum;

	/* init */
	memClear( &pad_info, sizeof(PadInfo) );
	pad_control = &pad_control_data;

    /* get pointers to the PSX controller buffers */
#ifdef PSX_FULL_DEV_KIT
	/* devkit method */
	pad_port0 = pad_buffer_a;
	pad_port1 = pad_buffer_b;
#if USE_LIBPAD_API
	/* libpad init functions */
#ifdef PAD_USE_MULTITAP
	PadInitMtap( pad_buffer_a, pad_buffer_b );
#else
	PadInitDirect( pad_buffer_a, pad_buffer_b );
#endif
	PadStartCom();
#else
	/* libapi init functions */
	InitPAD( (char *)pad_buffer_a, (long)34, (char *)pad_buffer_b, (long)34 );
	StartPAD();
#endif
#else
	/* yaroze method */
    GetPadBuf( &pad_port0, &pad_port1 );
#endif

    /* init */
    pad_control->npads  = NPADS;
    pad_info.port[0] = pad_port0;
    pad_info.port[1] = pad_port1;    
	pad_control->analog_range = (PAD_HIGH_ANALOG_MARGIN - PAD_LOW_ANALOG_MARGIN);

	/* init port data */
    for ( i=0; i<NPADS; i++ ) {
    	/* get the controller type */
    	if ( PAD_PORT_CONNECTED(i) == HAS_CONTROLLER ) {
			/* save controller type */
		    cont = PAD_PORT_TYPE(i);
			pad_info.ctype[i] = cont->type;
#ifdef PSX_FULL_DEV_KIT
#if USE_LIBPAD_API
			/* DUAL SHOCK actuator initialisation */
			if ( (cont->type == PAD_TYPE_DUALSHOCK) || (cont->type == PAD_TYPE_STD) ) {
get_state:
				state = PadGetState( PAD_PORT_DEV(i) );
				if ( state == PadStateStable ) {
					/* does this controller support shock? */
					if ( PadInfoAct( PAD_PORT_DEV(i), -1, 0 ) != 0 ) {
						while( PadGetState( PAD_PORT_DEV(i) ) != PadStateStable );
						PadSetAct( PAD_PORT_DEV(i), pad_info.shock[PAD_PORT_NUM(i)], 2 );
						while( PadGetState( PAD_PORT_DEV(i) ) != PadStateStable );
						while( PadSetActAlign( PAD_PORT_DEV(i), pad_act_align ) == 0 );
						pad_info.has_shock[i] = TRUE;
					}
				} else if ( state != PadStateFindCTP1 )
					goto get_state;
			}
#endif
#endif
			/* MULTI-TAP initialisation */
			if ( cont->type == PAD_TYPE_MULTITAP ) {
				for ( m=0; m<4; m++ ) {
					/* identify controller type */
					cont   = PAD_MTAP_PORT_TYPE(i,m);
					padnum = PAD_MTAP_PORT_NUM(i,m);
					/* set new controller type if necessary */
					if ( PAD_MTAP_PORT_CONNECTED(i,m) == HAS_CONTROLLER ) {
    					pad_info.mtap_ctype[i][m] = cont->type;
					} else
						pad_info.mtap_ctype[i][m]     = PAD_NOT_THERE;
#ifdef PSX_FULL_DEV_KIT
#if USE_LIBPAD_API
					/* DUAL SHOCK actuator initialisation */
					if ( (cont->type == PAD_TYPE_DUALSHOCK) || (cont->type == PAD_TYPE_STD) ) {
get_state_tap:
						state = PadGetState( PAD_MTAP_PORT_DEV(i,m) );
						if ( state == PadStateStable ) {
							/* does this controller support shock? */
							if ( PadInfoAct( PAD_MTAP_PORT_DEV(i,m), -1, 0 ) != 0 ) {
								while( PadGetState( PAD_MTAP_PORT_DEV(i,m) ) != PadStateStable );
								PadSetAct( PAD_MTAP_PORT_DEV(i,m), pad_info.shock[PAD_MTAP_PORT_NUM(i,m)], 2 );
								while( PadGetState( PAD_MTAP_PORT_DEV(i,m) ) != PadStateStable );
								while( PadSetActAlign( PAD_MTAP_PORT_DEV(i,m), pad_act_align ) == 0 );
								pad_info.mtap_has_shock[i][m] = TRUE;
							}
						} else if ( state != PadStateFindCTP1 )
							goto get_state_tap;
					}
#endif
#endif
   					pad_info.mtap_init[i][m] = PAD_INIT_FULL;
				}
			}
			pad_info.init[i]  = PAD_INIT_FULL;
		} else {
			pad_info.ctype[i] = PAD_NOT_THERE;
		}
	}
	
	/* init all pad_control states */
	for ( i=0; i<MAXPADS; i++ ) {
		/* init the pad states */
	    pad_control->state[i] = 0;
	    pad_control->last_state[i] = 0;
	    pad_control->hasanalog[i] = FALSE;
		for ( j=0; j<4; j++ )
		    pad_control->analog[i][j] = 127;
	}

	return( TRUE );
}  


/**********************************************************
 * Function:	padRead
 **********************************************************
 * Description: Read the current input controllers
 * Inputs:	
 * Notes:		call once per VSync(0)
 * Returns:		TRUE if read was OK and FALSE if there was 
 *				a change in a controller status
 **********************************************************/

public Bool padRead(
    void
    )
{
    int				 i, j, m;
	int				 padnum;
	int				 state;
    PadType			*cont;
	Bool			 ok;

	/* init ok flag */
	ok = TRUE;

    /* init the pad states */
    for ( i=0; i<MAXPADS; i++ ) {
		pad_control->last_state[i] = pad_control->state[i];
        pad_control->state[i] = 0;
	}
    
    /* update the attached and controller type status of the ports */
    for ( i=0; i<NPADS; i++ ) {
		//printf( "pad %d state = %d\n", i, PadGetState((i<<4)) );
		//printf( "pad %d shock values=%d,%d\n", i, pad_info.shock[i][0], pad_info.shock[i][1] );
		//printf( "connected=%d,0x%X\n", PAD_PORT_CONNECTED(i), *PAD_PORT_TYPE(i) );
    	if ( PAD_PORT_CONNECTED(i) == HAS_CONTROLLER ) {
			/* identify controller type */
            pad_control->state[i] |= PAD_ATTACHED;
		    cont = PAD_PORT_TYPE(i);
			/* set new controller type if necessary */
			if ( (pad_info.ctype[i] != cont->type) || (pad_info.init[i] != PAD_INIT_FULL) ) {
	    	    pad_info.ctype[i]     = cont->type;
				pad_info.has_shock[i] = FALSE;
#ifdef PSX_FULL_DEV_KIT
#if USE_LIBPAD_API
				/* DUAL SHOCK actuator initialisation */
				if ( (cont->type == PAD_TYPE_DUALSHOCK) || (cont->type == PAD_TYPE_STD) ) {
					state = PadGetState( PAD_PORT_DEV(i) );
					if ( state == PadStateStable ) {
						switch( pad_info.init[i] ) {
						case PAD_INIT_NONE:
							/* does this controller support shock? */
							if ( PadInfoAct( PAD_PORT_DEV(i), -1, 0 ) != 0 )
								pad_info.init[i] = PAD_INIT_STAGE_1;
							else
								pad_info.init[i] = PAD_INIT_FULL;
							break;
						case PAD_INIT_STAGE_1:
							/* init actuator stage 1 */
							PadSetAct( PAD_PORT_DEV(i), pad_info.shock[PAD_PORT_NUM(i)], 2 );
							pad_info.init[i] = PAD_INIT_STAGE_2;
							break;
						case PAD_INIT_STAGE_2:
							/* init actuator stage 2 */
							if ( PadSetActAlign( PAD_PORT_DEV(i), pad_act_align ) != 0 ) {
								pad_info.init[i] = PAD_INIT_FULL;
								pad_info.has_shock[i] = TRUE;
							}
							break;
						}
					} else if ( state == PadStateFindCTP1 )
						pad_info.init[i] = PAD_INIT_FULL;
					//printf( "state(%d)=0x%X\n", i, pad_info.init[i] );
				}
#endif
#endif
				/* set 'something has changed' flag */
				ok = FALSE;
			}
			//printf( "controller type=%d\n", cont->type );
			/* MULTI-TAP controller identification */
			if ( cont->type == PAD_TYPE_MULTITAP ) {
				pad_info.init[i] = PAD_INIT_FULL;
				for ( m=0; m<4; m++ ) {
					/* identify controller type */
					cont = (PadType *)PAD_MTAP_PORT_TYPE(i,m);
					padnum = PAD_MTAP_PORT_NUM(i,m);
					//printf( "multitap controller %d type=%d, port=%d\n", m, cont->type, padnum );
					/* set new controller type if necessary */
					if ( PAD_MTAP_PORT_CONNECTED(i,m) == HAS_CONTROLLER ) {
						pad_control->state[padnum] |= PAD_ATTACHED;
						if ( (pad_info.mtap_ctype[i][m] != cont->type) || (pad_info.mtap_init[i][m] != PAD_INIT_FULL) ) {
	    					pad_info.mtap_ctype[i][m] = cont->type;
	    					pad_info.mtap_has_shock[i][m] = FALSE;
#ifdef PSX_FULL_DEV_KIT
#if USE_LIBPAD_API
							/* DUAL SHOCK actuator initialisation */
							if ( (cont->type == PAD_TYPE_DUALSHOCK) || (cont->type == PAD_TYPE_STD) ) {
								state = PadGetState( PAD_MTAP_PORT_DEV(i,m) );
								if ( state == PadStateStable ) {
									switch( pad_info.mtap_init[i][m] ) {
									case PAD_INIT_NONE:
										/* does this controller support shock? */
										if ( PadInfoAct( PAD_MTAP_PORT_DEV(i,m), -1, 0 ) != 0 )
											pad_info.mtap_init[i][m] = PAD_INIT_STAGE_1;
										else
											pad_info.mtap_init[i][m] = PAD_INIT_FULL;
										break;
									case PAD_INIT_STAGE_1:
										/* init actuator stage 1 */
										PadSetAct( PAD_MTAP_PORT_DEV(i,m), pad_info.shock[PAD_MTAP_PORT_NUM(i,m)], 2 );
										pad_info.mtap_init[i][m] = PAD_INIT_STAGE_2;
										break;
									case PAD_INIT_STAGE_2:
										/* init actuator stage 2 */
										if ( PadSetActAlign( PAD_MTAP_PORT_DEV(i,m), pad_act_align ) != 0 ) {
											pad_info.mtap_init[i][m] = PAD_INIT_FULL;
					    					pad_info.mtap_has_shock[i][m] = TRUE;
										}
										break;
									}
								} else if ( state == PadStateFindCTP1 )
									pad_info.mtap_init[i][m] = PAD_INIT_FULL;
								//printf( "state(%d,%d)=0x%X\n", i, m, pad_info.mtap_init[i][m] );
							}
#endif
#endif
							/* set 'something has changed' flag */
							ok = FALSE;
						}
					} else {
						/* identify as no controller attached */
						if ( pad_info.mtap_ctype[i][m] != PAD_NOT_THERE ) {
							pad_info.mtap_ctype[i][m]     = PAD_NOT_THERE;
							pad_info.mtap_init[i][m]      = PAD_INIT_NONE;
							pad_info.mtap_has_shock[i][m] = FALSE;
							ok = FALSE;
						}
					}
				}
			}
		} else {
			/* identify as no controller attached */
			if ( pad_info.ctype[i] != PAD_NOT_THERE ) {
    			pad_info.ctype[i]     = PAD_NOT_THERE;
				pad_info.init[i]      = PAD_INIT_NONE;
				pad_info.has_shock[i] = FALSE;
				for ( m=0; m<4; m++ ) {
					pad_info.mtap_ctype[i][m]     = PAD_NOT_THERE;
					pad_info.mtap_init[i][m]      = PAD_INIT_NONE;
					pad_info.mtap_has_shock[i][m] = FALSE;
				}
				/* clear ctype for control data */
				pad_control->ctype[i] = PAD_NOT_THERE;
				/* clear multi-tap affected port numbers */
				if ( i ) {
					pad_control->ctype[5] = PAD_NOT_THERE;
					pad_control->ctype[6] = PAD_NOT_THERE;
					pad_control->ctype[7] = PAD_NOT_THERE;
				} else {
					pad_control->ctype[2] = PAD_NOT_THERE;
					pad_control->ctype[3] = PAD_NOT_THERE;
					pad_control->ctype[4] = PAD_NOT_THERE;
				}
				ok = FALSE;
			}
		}
    }

#if 0
	printf( "attached info: (%d,%d) ", pad_info.ctype[0], pad_info.ctype[1] );
    for ( i=0; i<MAXPADS; i++ )
		printf( "%d=%s ", i, (((pad_control->state[i] & PAD_ATTACHED)!=0)?("YES"):("NO")) );
	printf( "\n" );
#endif

    /* update the button status of the ports */
    for ( i=0; i<NPADS; i++ )
		padProcessPortData( FALSE, i, 0 );

	/* return status flag */
	return( ok );
}


/**********************************************************
 * Function:	padProcessPortData
 **********************************************************
 * Description: Process port data 
 * Inputs:	
 * Notes:		Reentrant function when using multi-tap
 * Returns:
 **********************************************************/

private void  padProcessPortData (
	Bool	tap,
	int		port,
	int		n
	)
{
    unsigned long	 pad;
	int				 i, j;
	int				 ctype;
	volatile Byte	*cdata;
	PadState		*state;

	/* force port value into range (0 or 1) */
	port &= 0x1;

	/* init */
	if ( tap ) {
		ctype = pad_info.mtap_ctype[port][n];
		i = PAD_MTAP_PORT_NUM(port,n);
		cdata = PAD_MTAP_PORT_ADDRESS(port,n);
	} else {
		ctype = pad_info.ctype[port];
		cdata = PAD_PORT_ADDRESS(port);
		i = port;
	}

	/* init */
	pad_control->ctype[i] = ctype;
	state = &pad_control->state[i];

	/* process controller data */
	switch( ctype ) {
    case PAD_NOT_THERE:
        break;
    case PAD_TYPE_STD:
        /* clear analog values and flag */
        pad_control->hasanalog[i] = FALSE;
        for ( j=0; j<4; j++ ) 
            pad_control->analog[i][j] = 127;
		/* get PSX controller state */
        pad = ~( *(cdata+3) | *(cdata+2) << 8 );
		/* update the state */
    	if ( pad & PADLleft  ) *state |= PAD_BUTTON_LEFT;
    	if ( pad & PADLright ) *state |= PAD_BUTTON_RIGHT;
    	if ( pad & PADLup    ) *state |= PAD_BUTTON_UP;
    	if ( pad & PADLdown  ) *state |= PAD_BUTTON_DOWN;
    	if ( pad & PADstart  ) *state |= PAD_BUTTON_START;
    	if ( pad & PADselect ) *state |= PAD_BUTTON_SELECT;
    	if ( pad & PADRdown  ) *state |= PAD_BUTTON_A; 
    	if ( pad & PADRleft  ) *state |= PAD_BUTTON_B;
    	if ( pad & PADRup    ) *state |= PAD_BUTTON_C;
    	if ( pad & PADRright ) *state |= PAD_BUTTON_D;
    	if ( pad & PADL1     ) *state |= PAD_BUTTON_LEFT_1;  
    	if ( pad & PADL2     ) *state |= PAD_BUTTON_LEFT_2;  
    	if ( pad & PADR1     ) *state |= PAD_BUTTON_RIGHT_1;  
    	if ( pad & PADR2     ) *state |= PAD_BUTTON_RIGHT_2;
        break;
    case PAD_TYPE_MOUSE:
        /* ignore - unsupported */
        break;
    case PAD_TYPE_NEGCON:
        /* to do... */
        break;
    case PAD_TYPE_ANALOG:
    case PAD_TYPE_DUALSHOCK:
        /* set analog values and flag */
        pad_control->hasanalog[i] = TRUE;
        for ( j=0; j<4; j++ )
            pad_control->analog[i][j] = *(cdata+4+j);
		/* get PSX controller state */
        pad = ~( *(cdata+3) | *(cdata+2) << 8 );
		/* update the state */
    	if ( pad & PADLleft  ) *state |= PAD_BUTTON_LEFT;
    	if ( pad & PADLright ) *state |= PAD_BUTTON_RIGHT;
    	if ( pad & PADLup    ) *state |= PAD_BUTTON_UP;
    	if ( pad & PADLdown  ) *state |= PAD_BUTTON_DOWN;
    	if ( pad & PADstart  ) *state |= PAD_BUTTON_START;
    	if ( pad & PADselect ) *state |= PAD_BUTTON_SELECT;
    	if ( pad & PADRdown  ) *state |= PAD_BUTTON_A; 
    	if ( pad & PADRleft  ) *state |= PAD_BUTTON_B;
    	if ( pad & PADRup    ) *state |= PAD_BUTTON_C;
    	if ( pad & PADRright ) *state |= PAD_BUTTON_D;
    	if ( pad & PADL1     ) *state |= PAD_BUTTON_LEFT_1;  
    	if ( pad & PADL2     ) *state |= PAD_BUTTON_LEFT_2;  
    	if ( pad & PADR1     ) *state |= PAD_BUTTON_RIGHT_1;  
    	if ( pad & PADR2     ) *state |= PAD_BUTTON_RIGHT_2;
        break;
	case PAD_TYPE_MULTITAP:
		if ( ! tap ) {
			for ( j=0; j<4; j++ )
				padProcessPortData( TRUE, port, j );
		}
		break;
    }
}


/**********************************************************
 * Function:	padMaxPads
 **********************************************************
 * Description: Check if a button is pressed
 * Inputs:	
 * Notes:		
 * Returns:
 **********************************************************/

public int padMaxPads (
	void
	)
{
#ifdef PAD_USE_MULTITAP
	/* 8 controllers supported with multi-tap */
	return( MAXPADS );
#else
	/* 2 direct connection ports */
	return( NPADS );
#endif
}


/**********************************************************
 * Function:	padType
 **********************************************************
 * Description: Get a controller type
 * Inputs:	
 * Notes:		
 * Returns:
 **********************************************************/

public int padType (
	int		padnum
	)
{
	int		type;

#if PAD_VALIDATE
	if ( padnum < 0 || padnum >= MAXPADS ) 
		return( FALSE );
#endif

	/* init */
	type = PAD_TYPE_UNSUPPORTED;

	/* get type */
	if ( pad_control->state[padnum] & PAD_ATTACHED ) {
		switch( pad_control->ctype[padnum] ) {
		case PAD_NOT_THERE:
			type = PAD_TYPE_NONE;
			break;
		case PAD_TYPE_STD:
			type = PAD_TYPE_PSX_STD;
			break;
		case PAD_TYPE_ANALOG:
			type = PAD_TYPE_PSX_ANALOG;
			break;
		case PAD_TYPE_DUALSHOCK:
			type = PAD_TYPE_PSX_DUALSHOCK;
			break;
		case PAD_TYPE_MULTITAP:
			/* will never reach here as control data does not care (pad info does) */
			type = PAD_TYPE_PSX_MULTITAP;
			break;
		default:
		case PAD_TYPE_MOUSE:
		case PAD_TYPE_NEGCON:
			/* ignore - unsupported */
			type = PAD_TYPE_UNSUPPORTED;
			break;
		}
	}

	return( type );
}


/**********************************************************
 * Function:	padHasMultiTap
 **********************************************************
 * Description: Is this controller on a multi tap device?
 * Inputs:	
 * Notes:		
 * Returns:
 **********************************************************/

public Bool padHasMultiTap (
	int		padnum
	)
{
	int		type;

#if PAD_VALIDATE
	if ( padnum < 0 || padnum >= MAXPADS ) 
		return( FALSE );
#endif

	/* init */
	type = PAD_TYPE_UNSUPPORTED;

	/* get type */
	if ( pad_control->state[padnum] & PAD_ATTACHED ) {
		switch( padnum ) {
		case 0:
		case 2:
		case 3:
		case 4:
			/* check for tap on port 1 */
			if ( pad_info.ctype[0] == PAD_TYPE_MULTITAP )
				return( TRUE );
			break;
		default:
			/* check for tap on port 2 */
			if ( pad_info.ctype[1] == PAD_TYPE_MULTITAP )
				return( TRUE );
			break;
		}
	}

	return( FALSE );
}


/**********************************************************
 * Function:	padAttached
 **********************************************************
 * Description: Check if a controller is attached
 * Inputs:	
 * Notes:		
 * Returns:
 **********************************************************/

public Bool padAttached (
	int		padnum
	)
{
#if PAD_VALIDATE
	if ( padnum < 0 || padnum >= MAXPADS ) 
		return( FALSE );
#endif
	if ( pad_control->state[padnum] & PAD_ATTACHED ) {
		//printf( "pad %d: attached\n", padnum );
		return( TRUE );
	}
	//printf( "pad %d: NOT attached\n", padnum );
	return( FALSE );
}


/**********************************************************
 * Function:	padSupported
 **********************************************************
 * Description: Check if a controller is supported
 * Inputs:	
 * Notes:		
 * Returns:
 **********************************************************/

public Bool padSupported (
	int		padnum
	)
{
#if PAD_VALIDATE
	if ( padnum < 0 || padnum >= MAXPADS ) 
		return( FALSE );
#endif

	switch( pad_control->ctype[padnum] ) {
	case PAD_TYPE_STD:
	case PAD_TYPE_ANALOG:
	case PAD_TYPE_DUALSHOCK:
		return( TRUE );
		break;
	}

	return( FALSE );
}


/**********************************************************
 * Function:	padHasAnalog
 **********************************************************
 * Description: Check if a controller has analog support
 * Inputs:	
 * Notes:		
 * Returns:
 **********************************************************/

public Bool padHasAnalog (
	int		padnum
	)
{
#if PAD_VALIDATE
	if ( padnum < 0 || padnum >= MAXPADS ) 
		return( FALSE );
#endif

	switch( pad_control->ctype[padnum] ) {
	case PAD_TYPE_ANALOG:
	case PAD_TYPE_DUALSHOCK:
		return( TRUE );
		break;
	}

	return( FALSE );
}


/**********************************************************
 * Function:	padHasShock
 **********************************************************
 * Description: Check if a controller has shock support
 * Inputs:	
 * Notes:		
 * Returns:
 **********************************************************/

public Bool padHasShock (
	int		padnum
	)
{
#if PAD_VALIDATE
	if ( padnum < 0 || padnum >= MAXPADS ) 
		return( FALSE );
#endif

	switch( pad_control->ctype[padnum] ) {
	case PAD_TYPE_STD:
		switch( padnum ) {
		case 0:
			/* check for tap in port 1 */
			if ( pad_info.ctype[0] == PAD_TYPE_MULTITAP ) {
				/* check for shock on controller in port 1-A */
				if ( pad_info.mtap_has_shock[0][0] ) 
					return( TRUE );
			} else {
				/* check for shock on controller in port 1 */
				if ( pad_info.has_shock[0] )
					return( TRUE );
			}
			break;
		case 1:
			/* check for tap in port 2 */
			if ( pad_info.ctype[1] == PAD_TYPE_MULTITAP ) {
				/* check for shock on controller in port 2-A */
				if ( pad_info.mtap_has_shock[1][0] ) 
					return( TRUE );
			} else {
				/* check for shock on controller in port 2 */
				if ( pad_info.has_shock[1] )
					return( TRUE );
			}
			break;
		case 2:
			/* check for shock on controller in port 1-B */
			if ( pad_info.mtap_has_shock[0][1] )
				return( TRUE );
			break;
		case 3:
			/* check for shock on controller in port 1-C */
			if ( pad_info.mtap_has_shock[0][2] )
				return( TRUE );
			break;
		case 4:
			/* check for shock on controller in port 1-D */
			if ( pad_info.mtap_has_shock[0][3] )
				return( TRUE );
			break;
		case 5:
			/* check for shock on controller in port 2-B */
			if ( pad_info.mtap_has_shock[1][1] )
				return( TRUE );
			break;
		case 6:
			/* check for shock on controller in port 2-C */
			if ( pad_info.mtap_has_shock[1][2] )
				return( TRUE );
			break;
		case 7:
			/* check for shock on controller in port 2-D */
			if ( pad_info.mtap_has_shock[1][3] )
				return( TRUE );
			break;
		}
		/* no shock found */
		return( FALSE );
		break;
	case PAD_TYPE_DUALSHOCK:
		/* guarenteed to have shock */
		return( TRUE );
		break;
	}

	return( FALSE );
}


/**********************************************************
 * Function:	padSendShock
 **********************************************************
 * Description: Send a shock to a controller
 * Inputs:	
 * Notes:		
 * Returns:
 **********************************************************/

public void padSendShock (
	int		padnum,
	int		type,
	Byte	value
	)
{
#if PAD_VALIDATE
	if ( padnum < 0 || padnum >= MAXPADS ) 
		return;
#endif

#if 0 // disable validation as DUAL SHOCK can be sending controller type PAD_TYPE_STD
	/* validate pad supports shock treatment */
	if ( pad_control->ctype[padnum] != PAD_TYPE_DUALSHOCK )
		return;
#endif

	/* force type into range */
	type &= 0x1;

	/* set shock value (0-255 or 0/1 depending on type) */
	pad_info.shock[padnum][type] = value;
}


/**********************************************************
 * Function:	padStopShock
 **********************************************************
 * Description: Stop all shock inputs to all controllers
 * Inputs:	
 * Notes:		
 * Returns:
 **********************************************************/

public void padStopShock (
	void
	)
{
	memClear( pad_info.shock, sizeof(pad_info.shock) );
}


/**********************************************************
 * Function:	padGet
 **********************************************************
 * Description: Get the data values for a controller
 * Inputs:	
 * Notes:		
 * Returns:
 **********************************************************/

public PadInput padGet (
	int		 padnum
	)
{
	PadInput	input;

#if PAD_VALIDATE
	if ( padnum < 0 || padnum >= MAXPADS ) {
		input.state  = 0;
		input.analog = 0;
		return( input );
	}
#endif

	input.state  = pad_control->state[padnum];
	memCopy( pad_control->analog[padnum], &input.analog, sizeof(PadAnalog) );

	return( input );
}


/**********************************************************
 * Function:	padSet
 **********************************************************
 * Description: Set the data values for a controller
 * Inputs:	
 * Notes:		
 * Returns:
 **********************************************************/

public void padSet (
	int		 padnum,
	PadInput input
	)
{
#if PAD_VALIDATE
	if ( padnum < 0 || padnum >= MAXPADS ) 
		return;
#endif
	pad_control->state[padnum] = input.state;
	memCopy( &input.analog, pad_control->analog[padnum], sizeof(PadAnalog) );
}


/**********************************************************
 * Function:	padAnyButtonDown
 **********************************************************
 * Description: Check if a button is pressed on any available
 *				and supported controller
 * Inputs:	
 * Notes:		
 * Returns:		0 if no buttons down or first controller 
 *				detected
 **********************************************************/

public int padAnyButtonDown (
	int		button
	)
{
	int		i;

    for ( i=0; i<MAXPADS; i++ ) {
		if ( (pad_control->state[i] & PAD_ATTACHED) &&
			 (pad_control->state[i] & button) )
			return( i );
	}

	return( PAD_NONE );
}


/**********************************************************
 * Function:	padButtonDown
 **********************************************************
 * Description: Check if a button is pressed
 * Inputs:	
 * Notes:		
 * Returns:
 **********************************************************/

public Bool padButtonDown (
	int		padnum,
	int		button
	)
{
#if PAD_VALIDATE
	if ( padnum < 0 || padnum >= MAXPADS ) 
		return( FALSE );
#endif
	if ( (pad_control->state[padnum] & PAD_ATTACHED) &&
		 (pad_control->state[padnum] & button) )
		return( TRUE );

	return( FALSE );
}


/**********************************************************
 * Function:	padAnyButtonPressed
 **********************************************************
 * Description: Check if a button is pressed on any available
 *				and supported controller
 * Inputs:	
 * Notes:		Checks transition from 0 to 1 state only
 * Returns:		0 if no buttons pressed or first controller 
 *				detected
 **********************************************************/

public int padAnyButtonPressed (
	int		button
	)
{
	int		i;

    for ( i=0; i<MAXPADS; i++ ) {
		if ( (pad_control->state[i] & PAD_ATTACHED) &&
			 ((pad_control->last_state[i] & button) == 0) &&
			 (pad_control->state[i] & button) )
			return( i );
	}

	return( PAD_NONE );
}


/**********************************************************
 * Function:	padButtonPressed
 **********************************************************
 * Description: Check if a button is pressed
 * Inputs:	
 * Notes:		
 * Returns:
 **********************************************************/

public Bool padButtonPressed (
	int		padnum,
	int		button
	)
{
#if PAD_VALIDATE
	if ( padnum < 0 || padnum >= MAXPADS ) 
		return( FALSE );
#endif
	if ( (pad_control->state[padnum] & PAD_ATTACHED) &&
		 ((pad_control->last_state[padnum] & button) == 0) &&
		 (pad_control->state[padnum] & button) )
		return( TRUE );

	return( FALSE );
}


#if PAD_FIXEDPOINT_API
/**********************************************************
 * Function:	padAnalogX
 **********************************************************
 * Description: Get an analog value from the controller
 * Inputs:	
 * Notes:		
 * Returns:
 **********************************************************/

public fixed padAnalogX (
	int		padnum,
	int		sticknum
	)
{
	int		xvalue;

#if PAD_VALIDATE
	if ( padnum < 0 || padnum >= MAXPADS ) 
		return( 0 );
	if ( sticknum < 0 || sticknum > 1 ) 
		return( 0 );
#endif

	if ( ! (pad_control->state[padnum] & PAD_ATTACHED) )
		return( 0 );

#if 0
	switch( pad_control->ctype[padnum] ) {
        case PAD_TYPE_ANALOG:
        case PAD_TYPE_DUALSHOCK:
			if ( sticknum )
				xvalue = (int)pad_control->analog[padnum][0];
			else
				xvalue = (int)pad_control->analog[padnum][2];
            break;
		default:
			return( 0 );
    }
#else
	if ( sticknum )
		xvalue = (int)pad_control->analog[padnum][0];
	else
		xvalue = (int)pad_control->analog[padnum][2];
#endif

	return( padAnalogToFixed( xvalue ) );
}


/**********************************************************
 * Function:	padAnalogY
 **********************************************************
 * Description: Get an analog value from the controller
 * Inputs:	
 * Notes:		
 * Returns:
 **********************************************************/

public fixed padAnalogY (
	int		padnum,
	int		sticknum
	)
{
	int		yvalue;

#if PAD_VALIDATE
	if ( padnum < 0 || padnum >= MAXPADS ) 
		return( 0 );
	if ( sticknum < 0 || sticknum > 1 ) 
		return( 0 );
#endif
	
	if ( ! (pad_control->state[padnum] & PAD_ATTACHED) )
		return( 0 );

#if 0
	switch( pad_control->ctype[padnum] ) {
	    case PAD_TYPE_ANALOG:
        case PAD_TYPE_DUALSHOCK:
			if ( sticknum )
				yvalue = (int)pad_control->analog[padnum][1];
			else
				yvalue = (int)pad_control->analog[padnum][3];
            break;
		default:
			return( 0 );
    }
#else
	if ( sticknum )
		yvalue = (int)pad_control->analog[padnum][1];
	else
		yvalue = (int)pad_control->analog[padnum][3];
#endif

	return( -(padAnalogToFixed( yvalue )) );
}
#endif


/**********************************************************
 * Function:	padAnalogToFixed
 **********************************************************
 * Description: Convert analog value to fixed
 * Inputs:	
 * Notes:		
 * Returns:
 **********************************************************/

private fixed padAnalogToFixed (
	int		value
	)
{
	/* 
	 * Convert 0..255 to -4096..4096 but use formula:
	 *
	 *		range  15..105  = -4096..0
	 *		range 105..149  =     0
	 *		range 149..239  =     0..4096
	 *
	 * to compensate for controller inaccuracy in feedback
	 *
	 */

	/* minimum value range */
	if ( value < 15 )
		return( -(FIXED_ONE) );

	/* maximum value range */
	if ( value > 239 )
		return( FIXED_ONE );

	/* zero value range */
	if ( value >= 105 && value <= 149 )
		return( 0 );

	if ( value < 105 ) {
		value -= 15;
		return( -((FIXED_ONE - FIXED_NORMALISE( value, 90 ))) );
	} else {
		value -= 149;
		return( FIXED_NORMALISE( value, 90 ) );
	}

	/* will never get here */
	return( 0 );
}

