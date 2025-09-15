/**********************************************************
 * Copyright (C) 1999,2000 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:	PlayStation surf game
 **********************************************************
 * File:	sounds.h
 * Author:	Mark Theyer
 * Created:	04 Aug 2000
 **********************************************************
 * Description:	Sound effects id reference numbers
 **********************************************************
 * Revision History:
 * 04-Aug-00	Theyer	Initial coding.
 **********************************************************/

#ifndef THEYER_SOUNDS_H
#define THEYER_SOUNDS_H

/*
 * includes
 */

/*
 * macros
 */

/* id number has program number in upper 8 bits, tone in low 8 bits */
#define SURF_SFX_ID(prog,tone)				((prog<<8)|(tone))

/* quickie macro */
#define SURF_SFX_PLAY(sfx_id)				sndPlay( surf.sound.loadid, sfx_id, 2048, 2048 )
#define SURF_SFX_VPLAY(sfx_id,v1,v2)		sndPlay( surf.sound.loadid, sfx_id, v1, v2 )
#define SURF_SFX_WINNER_CHEER				sndPlay( surf.sound.loadid, SURF_SFX_ID_WINNER_CHEER, FIXED_ONE, FIXED_ONE )
#define SURF_SFX_CHAMP_CHEER				sndPlay( surf.sound.loadid, SURF_SFX_ID_CHAMP_CHEER, FIXED_ONE, FIXED_ONE )

/* instant sound effects for menu selections */
#define SURF_SFX_MENU_DROP					SURF_SFX_PLAY( SURF_SFX_ID_MENU_SCROLL_FWD )
#define SURF_SFX_MENU_RETRACT				SURF_SFX_PLAY( SURF_SFX_ID_MENU_SCROLL_BACK )
#define SURF_SFX_MENU_SELECT				SURF_SFX_PLAY( SURF_SFX_ID_MENU_SELECT )
#define SURF_SFX_MENU_MOVE					SURF_SFX_PLAY( SURF_SFX_ID_MENU_MOVE )
#define SURF_SFX_MENU_ADJUST				SURF_SFX_PLAY( SURF_SFX_ID_MENU_ADJUST )
#define SURF_SFX_MENU_ADJUST_LIMIT			SURF_SFX_VPLAY( SURF_SFX_ID_MENU_ADJUST, 1024, 1024 )
#define SURF_SFX_MENU_GO_BACK				SURF_SFX_PLAY( SURF_SFX_ID_MENU_GO_BACK )
#define SURF_SFX_SESSION_HORN				SURF_SFX_PLAY( SURF_SFX_ID_SESSION_HORN )
#define SURF_SFX_TIMER_BLEEP				SURF_SFX_PLAY( SURF_SFX_ID_TIMER_BLEEP )
#define SURF_SFX_TIMER_ZERO					SURF_SFX_PLAY( SURF_SFX_ID_TIMER_ZERO )
#define SURF_SFX_TIMER_SCROLL				SURF_SFX_PLAY( SURF_SFX_ID_TIMER_SCROLL )
#define SURF_SFX_SUCCESS					SURF_SFX_PLAY( SURF_SFX_ID_SUCCESS )

/* sound values */
#define SURF_SFX_VOL_WAVE_STD				 64
#define SURF_SFX_VOL_ADJUST_INC				205
#define SURF_SOUND_MAX_TRIGGERS				 16

/* crowd response base and number of responses */
#define SURF_SFX_ID_CROWD_CHEER				SURF_SFX_ID(1,0)	// VAG 15
#define SURF_SFX_CROWD_NUM_CHEERS			4

/* reference numbers - menu selection */
#define SURF_SFX_ID_MENU_SELECT				SURF_SFX_ID(0,0)	// VAG 1
#define SURF_SFX_ID_MENU_GO_BACK			SURF_SFX_ID(0,1)	// VAG 2
#define SURF_SFX_ID_MENU_ADJUST				SURF_SFX_ID(0,2)	// VAG 3
#define SURF_SFX_ID_MENU_MOVE				SURF_SFX_ID(0,3)	// VAG 4
#define SURF_SFX_ID_MENU_SCROLL_BACK		SURF_SFX_ID(0,4)	// VAG 5
#define SURF_SFX_ID_MENU_SCROLL_FWD			SURF_SFX_ID(0,5)	// VAG 6
#define SURF_SFX_ID_SUCCESS					SURF_SFX_ID(0,6)	// VAG 7
/* reference numbers - results crowd cheers */
#define SURF_SFX_ID_WINNER_CHEER			SURF_SFX_ID(0,7)	// VAG 8
#define SURF_SFX_ID_CHAMP_CHEER				SURF_SFX_ID(0,7)	// VAG 8
/* reference numbers - timer */
#define SURF_SFX_ID_SESSION_HORN			SURF_SFX_ID(0,7)	// VAG 8
#define SURF_SFX_ID_TIMER_BLEEP				SURF_SFX_ID(0,8)	// VAG 9
#define SURF_SFX_ID_TIMER_ZERO				SURF_SFX_ID(0,9)	// VAG 10
#define SURF_SFX_ID_TIMER_SCROLL			SURF_SFX_ID(0,10)	// VAG 11
/* reference numbers - waves */
#define SURF_SFX_ID_WAVE					SURF_SFX_ID(0,11)	// VAG 12
#define SURF_SFX_ID_WAVE_BARREL				SURF_SFX_ID(0,12)	// VAG 13
#define SURF_SFX_ID_WAVE_CRASH				SURF_SFX_ID(0,13)	// VAG 14
/* reference numbers - crowd and ambient sounds */
#define SURF_SFX_ID_CROWD_OH_YEAH			SURF_SFX_ID(1,0)	// VAG 15
#define SURF_SFX_ID_CROWD_YEAH				SURF_SFX_ID(1,1)	// VAG 16
#define SURF_SFX_ID_CROWD_WOOHOO			SURF_SFX_ID(1,2)	// VAG 17
#define SURF_SFX_ID_CROWD_OOO				SURF_SFX_ID(1,3)	// VAG 18
#define SURF_SFX_ID_SEAGULLS				SURF_SFX_ID(1,4)	// VAG 19
#define SURF_SFX_ID_CHOPPER					SURF_SFX_ID(1,5)	// VAG 20
/* reference numbers - rider responses */
#define SURF_SFX_ID_FEMALE_1_YEAH_1			SURF_SFX_ID(1,6)	// VAG 21
#define SURF_SFX_ID_FEMALE_1_YEAH_2			SURF_SFX_ID(1,7)	// VAG 22
#define SURF_SFX_ID_FEMALE_1_OOF			SURF_SFX_ID(1,8)	// VAG 23
#define SURF_SFX_ID_FEMALE_2_YEAH_1			SURF_SFX_ID(1,9)	// VAG 24
#define SURF_SFX_ID_FEMALE_2_YEAH_2			SURF_SFX_ID(1,10)	// VAG 25
#define SURF_SFX_ID_FEMALE_2_OOF			SURF_SFX_ID(1,11)	// VAG 26
#define SURF_SFX_ID_FEMALE_3_YEAH_1			SURF_SFX_ID(1,12)	// VAG 27
#define SURF_SFX_ID_FEMALE_3_YEAH_2			SURF_SFX_ID(1,13)	// VAG 28
#define SURF_SFX_ID_FEMALE_3_OOF			SURF_SFX_ID(1,14)	// VAG 29
#define SURF_SFX_ID_FEMALE_4_YEAH_1			SURF_SFX_ID(2,0)	// VAG 30
#define SURF_SFX_ID_FEMALE_4_YEAH_2			SURF_SFX_ID(2,1)	// VAG 31
#define SURF_SFX_ID_FEMALE_4_OOF			SURF_SFX_ID(2,2)	// VAG 32
#define SURF_SFX_ID_MALE_1_YEAH_1			SURF_SFX_ID(2,3)	// VAG 33
#define SURF_SFX_ID_MALE_1_YEAH_2			SURF_SFX_ID(2,4)	// VAG 34
#define SURF_SFX_ID_MALE_1_OOF				SURF_SFX_ID(2,5)	// VAG 35
#define SURF_SFX_ID_MALE_2_YEAH_1			SURF_SFX_ID(2,6)	// VAG 36
#define SURF_SFX_ID_MALE_2_YEAH_2			SURF_SFX_ID(2,7)	// VAG 37
#define SURF_SFX_ID_MALE_2_OOF				SURF_SFX_ID(2,8)	// VAG 38
#define SURF_SFX_ID_MALE_3_YEAH_1			SURF_SFX_ID(2,9)	// VAG 39
#define SURF_SFX_ID_MALE_3_YEAH_2			SURF_SFX_ID(2,10)	// VAG 40
#define SURF_SFX_ID_MALE_3_OOF				SURF_SFX_ID(2,11)	// VAG 41
#define SURF_SFX_ID_MALE_4_YEAH_1			SURF_SFX_ID(2,12)	// VAG 42
#define SURF_SFX_ID_MALE_4_YEAH_2			SURF_SFX_ID(2,13)	// VAG 43
#define SURF_SFX_ID_MALE_4_OOF				SURF_SFX_ID(2,14)	// VAG 44
/* reference numbers - generic */
#define SURF_SFX_ID_BOARD_WAKE				SURF_SFX_ID(3,0)	// VAG 45
#define SURF_SFX_ID_BOARD_SPRAY				SURF_SFX_ID(3,1)	// VAG 46
#define SURF_SFX_ID_BOARD_SPRAY_TAIL		SURF_SFX_ID(3,2)	// VAG 47
#define SURF_SFX_ID_BOARD_TAKE_OFF			SURF_SFX_ID(3,3)	// VAG 48
#define SURF_SFX_ID_BOARD_IDLE_LAPS			SURF_SFX_ID(3,4)	// VAG 49
#define SURF_SFX_ID_BOARD_SPLAT				SURF_SFX_ID(3,5)	// VAG 50
#define SURF_SFX_ID_BOARD_LANDING			SURF_SFX_ID(3,6)	// VAG 51
#define SURF_SFX_ID_RIDER_SPLAT				SURF_SFX_ID(3,7)	// VAG 52
/* reference numbers - surfing */
#define SURF_SFX_ID_BOARD_SPIN_WHIP			SURF_SFX_ID(3,8)	// VAG 53
#define SURF_SFX_ID_SURFER_STAND_UP			SURF_SFX_ID(3,9)	// VAG 54
#define SURF_SFX_ID_SURFER_PADDLE_LEFT		SURF_SFX_ID(3,10)	// VAG 55
#define SURF_SFX_ID_SURFER_PADDLE_RIGHT		SURF_SFX_ID(3,11)	// VAG 56
/* reference numbers - sailing */
#define SURF_SFX_ID_SAILOR_WHAP				SURF_SFX_ID(3,8)	// VAG 53
#define SURF_SFX_ID_SAILOR_JIBE				SURF_SFX_ID(3,9)	// VAG 54
#define SURF_SFX_ID_SAILOR_SAIL				SURF_SFX_ID(3,10)	// VAG 55
#define SURF_SFX_ID_SAILOR_CHATTER			SURF_SFX_ID(3,11)	// VAG 56
/* reference numbers - bodyboard riding */
#define SURF_SFX_ID_BODYBOARD_KICK			SURF_SFX_ID(3,8)	// VAG 53
#define SURF_SFX_ID_BODYBOARD_SPIN			SURF_SFX_ID(3,9)	// VAG 54
/* reference numbers - jetskiing */
#define SURF_SFX_ID_JETSKI_ENGINE			SURF_SFX_ID(3,8)	// VAG 53
#define SURF_SFX_ID_JETSKI_IDLE				SURF_SFX_ID(3,9)	// VAG 54
#define SURF_SFX_ID_JETSKI_DRY_CONST		SURF_SFX_ID(3,10)	// VAG 55
#define SURF_SFX_ID_JETSKI_DRY_TAIL			SURF_SFX_ID(3,11)	// VAG 56

#endif	// THEYER_SOUNDS_H

