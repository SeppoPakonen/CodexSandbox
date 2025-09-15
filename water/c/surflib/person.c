/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		person.c
 * Author:		Mark Theyer
 * Created:		6 Dec 1997
 **********************************************************
 * Description: Surf game person manipulation functions	
 **********************************************************
 * Functions:
 *	personCreate() 
 *				Initialise a person.
 *	personDraw()
 *				Draw a person.
 *	personPutOnVehicle()
 *				Put person on a vehicle (parent/child).
 *	personAssignVehicle()
 *				Person will be using assigned vehicle.
 *	personInPosition()
 *				Is person in position?
 *	personSavePosition()
 *				Save this position.
 *	personRestorePosition()
 *				Restore a position.
 *	personPosition()
 *				Put person into current position.
 *	personUpdate()
 *				Update the positional values to go to new
 *				position at a defined rate.
 *	personSetPosition()
 *				Set the position desired for a person.
 *	personProcessEvents()
 *				Handle events for play mode.
 *	personNextPosition()
 *				Go to next position for person.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 06-Dec-97	Theyer	Initial Coding
 * 15-Jul-99	Theyer	Initial update for new game
 * 31-May-01	Theyer	New version for PS2
 *
 **********************************************************/

/*
 * includes
 */

#include <person.h>

/*
 * typedefs
 */

/*
 * macros
 */

/*
 * prototypes
 */

/*
 * variables
 */

/*
 * functions
 */

/**********************************************************
 * Function:	personCreate
 **********************************************************
 * Description: Create a person dude
 * Inputs:		person - person pointer
 *				weight - weight of person
 * Notes:	
 * Returns:		initialised person data
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

public void personCreate (
    PersonInfo					*person,
	GfxObject					*object,
	void						*veh,
	PersonEventCallback			*processevents,
	PersonNextPositionCallback	*nextposition
    )
{
    /* no error checking! */
	person->object     = object;
    person->state      = 0;
	person->current    = 0;
	person->target     = 0;
	person->interpol   = 0.0f;
	person->onvehicle  = FALSE;
	person->vehicle    = veh;
	person->move_speed = PERSON_MOVESPEED;
	person->crash      = FALSE;
	person->processevents = processevents;
	person->nextposition  = nextposition;
}


/**********************************************************
 * Function:	personPutOnVehicle
 **********************************************************
 * Description: Place rider on a vehicle
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

void personPutOnVehicle (
	PersonInfo		*person,
	GfxObject		*veh_obj	// NULL to take off
    )
{
	if ( veh_obj ) {
		person->onvehicle = TRUE;
		gfxObjectParent( person->object, veh_obj );
	} else {
		person->onvehicle = FALSE;
		gfxObjectParent( person->object, NULL );
	}
}


/**********************************************************
 * Function:	personUpdate
 **********************************************************
 * Description: Update a person's position
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

public void personUpdate (
    PersonInfo 	   *person,
	float			tick
    )
{
	/* update position */
	if ( person->current != person->last ) {
		person->interpol += (person->move_speed * tick);
		if ( person->interpol > 1.0f ) {
			person->interpol = 0.0f;
			person->last = person->current;
		}
		/* set position */
		gfxSetPosition( &person->object, 1, person->last, person->current, person->interpol );
	}
}


#if 1
/**********************************************************
 * Function:	personProcessEvents
 **********************************************************
 * Description: Handle events for play mode
 * Inputs:		person - process events for person
 * Notes:	
 * Returns:
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 07-Sep-99	Theyer	Initial coding.
 *
 **********************************************************/

void personProcessEvents ( 
	PersonInfo	*person
	)
{
	if ( person->processevents )
		person->processevents( person );
}


/**********************************************************
 * Function:	personNextPosition
 **********************************************************
 * Description: Go to next position for person
 * Inputs:		person - person to update
 * Notes:	
 * Returns:
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 07-Sep-99	Theyer	Initial coding.
 *
 **********************************************************/

void personNextPosition ( 
	PersonInfo	*person
	)
{
	if ( person->nextposition )
		person->nextposition( person );
}
#endif

