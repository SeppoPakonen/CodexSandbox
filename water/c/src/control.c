/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		control.c
 * Author:		Mark Theyer
 * Created:		14 Mar 2000
 **********************************************************
 * Description:	Controller related functions
 **********************************************************
 * Functions:
 *	fname()
 *				func desc.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 14-Mar-00	Theyer	Initial coding.
 * 31-Mar-01	Theyer	Ported to PS2
 *
 **********************************************************/

/*
 * includes
 */

#include <surfgame.h>

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
 * global variables
 */

extern SurfData	surf;

/*
 * functions
 */


/**********************************************************
 * Function:	surfInitControl
 **********************************************************
 * Description: Initialise controllers
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 29-Oct-99	Theyer	Initial coding.
 *
 **********************************************************/

Bool surfInitControl (
	void
	)
{
	int		i;
	Bool	ok;
	
retry:
	/* init */
	memClear( &surf.control, sizeof(SurfControl) );
	ok = FALSE;
	surf.control.master = PAD_NONE;
	surf.control.active = PAD_NONE;

	/* read controller state */
	padRead();

	/* init */
	for ( i=0; i<SURF_NUM_PADS; i++ ) {
		if ( padAttached(i)  ) surf.control.pad_status[i] |= SURF_PAD_ATTACHED;
		if ( padSupported(i) ) surf.control.pad_status[i] |= SURF_PAD_SUPPORTED;
		//printf( "pad %d attached=%d,supported=%d\n", i, padAttached(i), padSupported(i) );
		if ( surfPadOK(i) ) {
			/* set first detected controller as the master */
			if ( surf.control.master == PAD_NONE ) surf.control.master = i;
			/* has analog controls? */
			if ( padHasAnalog(i) ) surf.control.pad_status[i] |= SURF_PAD_HAS_ANALOG;
			/* has shock function? */
			if ( padHasShock(i)  ) surf.control.pad_status[i] |= SURF_PAD_HAS_SHOCK;
			/* update flag */
			ok = TRUE;
		}
	}

	/* complain if no controllers detected */ 
	if ( surf.control.master == PAD_NONE ) {
		/* pause game and wait for new active controller */
		if ( surf.playmode == SURF_PLAYMODE_MENU ) {
			surfLostControl( 0, TRUE );
		} else {
			surfLostControl( 0, FALSE );
		}
		goto retry;
	}

	return( ok );
}


/**********************************************************
 * Function:	surfUpdateControl
 **********************************************************
 * Description: Update controller status
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 29-Oct-99	Theyer	Initial coding.
 *
 **********************************************************/

void surfUpdateControl (
	void
	)
{
	int		active;

	/* save active */
	active = surf.control.active;

	/* another controller was plugged in? */
	if ( padAttached(surf.control.active) ) {
		/* re-examine the connections and status of controllers */
		surfInitControl();
		/* in menu mode select new master controller as active */
		if ( surf.playmode == SURF_PLAYMODE_MENU ) {
			surf.control.active = surf.control.master;
			return;
		}
		/* make sure active is still OK */
		if ( surfPadOK(active) ) {
			surf.control.active = active;
			/* we are OK to continue */
			return;
		}
	}

	/* looks like someone removed the active controller! */
	if ( surf.playmode == SURF_PLAYMODE_MENU ) {
		surfLostControl( 0, TRUE );
	} else {
		/* pause game and wait for new active controller */
		surfLostControl( active, FALSE );
	}

	/* re-examine the connections and status of controllers */
	surfInitControl();

	/* in menu mode select new master controller as active */
	if ( surf.playmode == SURF_PLAYMODE_MENU )
		surf.control.active = surf.control.master;
	else if ( padAttached(active) )
		surf.control.active = active;
	else
		surf.control.active = surfPlayerControlPad(surf.active_player);
}


/**********************************************************
 * Function:	surfPlayerControlString
 **********************************************************
 * Description: Return string name for this controller number
 * Inputs:		
 * Notes:		nil
 * Returns:		
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

Text surfControlPadString (
	int		pad_num
	)
{
	Text	str;

	switch( pad_num ) {
	case 0:
		str = "1";
		if ( padHasMultiTap(pad_num) )
			str = "1-A";
		break;
	case 1:
		str = "2";
		if ( padHasMultiTap(pad_num) )
			str = "2-A";
		break;
	case 2:
		str = "1-B";
		break;
	case 3:
		str = "1-C";
		break;
	case 4:
		str = "1-D";
		break;
	case 5:
		str = "2-B";
		break;
	case 6:
		str = "2-C";
		break;
	case 7:
		str = "2-D";
		break;
	default:
		str = " ";
		break;
	}

	return( str );
}


/**********************************************************
 * Function:	surfPlayerControlPad
 **********************************************************
 * Description: Reassign active controller for this player
 * Inputs:		
 * Notes:		nil
 * Returns:		
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

int surfPlayerControlPad (
	int		player_num
	)
{
	/* no validation! */
	switch( player_num ) {
	case 1:
		/* controller on port 1 or multi-tap port 1-A */
		if ( bitOn(surf.control.pad_status[0],(SURF_PAD_ATTACHED|SURF_PAD_SUPPORTED)) )
			return(0);
		/* controller on port 2 or multi-tap port 2-A */
		if ( bitOn(surf.control.pad_status[1],(SURF_PAD_ATTACHED|SURF_PAD_SUPPORTED)) )
			return(1);
		break;
	case 2:
		/* controller on multi-tap port 1-B */
		if ( bitOn(surf.control.pad_status[2],(SURF_PAD_ATTACHED|SURF_PAD_SUPPORTED)) )
			return(2);
		/* controller on multi-tap port 2-B */
		if ( bitOn(surf.control.pad_status[5],(SURF_PAD_ATTACHED|SURF_PAD_SUPPORTED)) )
			return(5);
		/* controller on port 2 or multi-tap port 2-A */
		if ( bitOn(surf.control.pad_status[1],(SURF_PAD_ATTACHED|SURF_PAD_SUPPORTED)) )
			return(1);
		/* controller on port 1 or multi-tap port 1-A */
		if ( bitOn(surf.control.pad_status[0],(SURF_PAD_ATTACHED|SURF_PAD_SUPPORTED)) )
			return(0);
		break;
	case 3:
		/* controller on multi-tap port 1-C */
		if ( bitOn(surf.control.pad_status[3],(SURF_PAD_ATTACHED|SURF_PAD_SUPPORTED)) )
			return(3);
		/* controller on multi-tap port 2-C */
		if ( bitOn(surf.control.pad_status[6],(SURF_PAD_ATTACHED|SURF_PAD_SUPPORTED)) )
			return(6);
		/* controller on port 1 or multi-tap port 1-A */
		if ( bitOn(surf.control.pad_status[0],(SURF_PAD_ATTACHED|SURF_PAD_SUPPORTED)) )
			return(0);
		break;
	case 4:
		/* controller on multi-tap port 1-D */
		if ( bitOn(surf.control.pad_status[4],(SURF_PAD_ATTACHED|SURF_PAD_SUPPORTED)) )
			return(4);
		/* controller on multi-tap port 2-D */
		if ( bitOn(surf.control.pad_status[7],(SURF_PAD_ATTACHED|SURF_PAD_SUPPORTED)) )
			return(7);
		/* controller on port 2 or multi-tap port 2-A */
		if ( bitOn(surf.control.pad_status[1],(SURF_PAD_ATTACHED|SURF_PAD_SUPPORTED)) )
			return(1);
		/* controller on port 1 or multi-tap port 1-A */
		if ( bitOn(surf.control.pad_status[0],(SURF_PAD_ATTACHED|SURF_PAD_SUPPORTED)) )
			return(0);
		break;
	}

	/* all fail? use the master controller... */
	return( surf.control.master );
}

    
/**********************************************************
 * Function:	surfLostControl
 **********************************************************
 * Description: Pause until the active controller is re-attached
 * Inputs:		
 * Notes:		nil
 * Returns:		
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

void surfLostControl (
	int		pad_num,
    Bool	frontend
    )
{
	;
}


