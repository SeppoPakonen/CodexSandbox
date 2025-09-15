/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		person.h
 * Author:		Mark Theyer
 * Created:		06 Dec 1997
 **********************************************************
 * Description: Human character model management.
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

#ifndef SURF_PERSON_H
#define SURF_PERSON_H

/*
 * includes
 */

#include <compile.h>
#include <type/datatype.h>
#include <gfx/gfx.h>
#include <pad/pad.h>
#include <quat/quat.h>
//#include <vehicle.h>
//#include <font.h>

#include <surfutil.h>

/*
 * macros
 */

#define PERSON_MOVESPEED					0.1f

/*
 * typedefs
 */

// person info 
typedef struct PersonInfoStruct PersonInfo;

// callbacks for a managed person
typedef void *(PersonEventCallback)( PersonInfo *person );
typedef void *(PersonNextPositionCallback)( PersonInfo *person );

// person data
typedef struct PersonInfoStruct {
	GfxObject					*object;				// object pointer
    int							 state;					// state of person
    int							 current;				// current position of person
    int							 last;					// last position
    int							 target;				// target state of person
	float						 interpol;				// interpolation value
	Bool						 onvehicle;				// on vehicle flag 
	void						*vehicle;				// associated vehicle
	float						 move_speed;			// positional move speed to desired
	int							 extrainfo;				// extra person dependent information...
	Bool						 crash;					// crashed flag
	PersonEventCallback			*processevents;			// process events callback
	PersonNextPositionCallback	*nextposition;			// next position function callback
} PersonInfoStruct;

/*
 * prototypes
 */

extern void personCreate( PersonInfo *person, GfxObject *object, void *veh, PersonEventCallback *processevents, PersonNextPositionCallback *nextposition );
extern void personDraw( PersonInfo *person );
extern void personUpdate( PersonInfo *person, float tick );
extern Bool personInPosition( PersonInfo *person );
extern void personPutOnVehicle( PersonInfo *person, GfxObject *veh_obj );
extern void personProcessEvents( PersonInfo *person );
extern void personNextPosition( PersonInfo *person );

#define personOnVehicle(person)			((person)->onvehicle)
#define personObject(person)			((person)->object)
#define personState(person)				((person)->state) 
#define personSetState(person,newstate)	((person)->state = newstate)
#define personMoveSpeed(person,spd)		((person)->move_speed = spd)
#define personDraw(person)				(gfxDrawObject((person)->object))
#define personSetPosition(person,pos)	((person)->current = pos)
#define personCrashed(person)			((person)->crash)
#define personInPosition(person)		(((person)->current == (person)->last) && ((person)->interpol == 0.0f))
#define personInState(person)			((person)->state == (person)->target)
//#define personNextPosition(person)	(((person)->nextposition)?((person)->nextposition(person)))
//#define personProcessEvents(person)	(((person)->processevents)?((person)->processevents(person)))

#endif	// SURF_PERSON_H
