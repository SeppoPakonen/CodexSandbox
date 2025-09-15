/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		demo.c
 * Author:		Mark Theyer
 * Created:		18 Aug 1999
 **********************************************************
 * Description:	demonstration code related functions
 **********************************************************
 * Functions:
 *	f() 
 *				func desc.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 18-Aug-99	Theyer	Initial coding.
 * 23-Aug-01	Theyer	PS2 demo port.
 *
 **********************************************************/

/*
 * includes
 */

#include <surfgame.h>

#if SURF_PROTOTYPE_VERSION
//#ifndef PSX_FILEREGISTER_TESTMODE
//#include <../data/intfiles/demo1.dat>
//#endif
#include <../data/intfiles/demo1.h>
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
 * global variables
 */

extern SurfData		surf;

/*
 * functions
 */

/**********************************************************
 * Function:	surfInitDemo
 **********************************************************
 * Description: Initialise demo data
 * Inputs:		
 * Notes:		nil
 * Returns:		
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 18-Aug-99	Theyer	Initial coding.
 *
 **********************************************************/

void surfInitDemo (
	void
	)
{
#if SURF_PROTOTYPE_VERSION
	intfiles_register();	
#endif
}

