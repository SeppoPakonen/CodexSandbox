/*
Omnispeak: A Commander Keen Reimplementation
Copyright (C) 2012 David Gow <david@ingeniumdigital.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef ID_IN_H
#define ID_IN_H

#include <stdbool.h>
#include <stdint.h>

#include "ck_config.h"

// This is how it's done in Wolf3D, even if it's bad practice in modern C++ code
typedef uint8_t IN_ScanCode;

//
// These scancodes match keen (and wolf3d)'s numbering, such that
// patches and config files may be compatible.
//
// Also based on an earlier investigation of DOS scancodes (apparently some
// keys were originally found on the keypad only, although the slash and enter
// keys are exceptions).
//
// TODO: Maybe rearrange these based on the history of keyboards?
// Say, if something was found on the keypad only, use IN_KP.
// More generally, maybe write IN_SC_KP (or similar) rather than IN_KP.

/*typedef*/ enum /*IN_ScanCode*/
{
	IN_SC_None = 0x00,
	IN_SC_Invalid = 0xff,
	IN_SC_Escape = 0x01,
	IN_SC_One = 0x02,
	IN_SC_Two = 0x03,
	IN_SC_Three = 0x04,
	IN_SC_Four = 0x05,
	IN_SC_Five = 0x06,
	IN_SC_Six = 0x07,
	IN_SC_Seven = 0x08,
	IN_SC_Eight = 0x09,
	IN_SC_Nine = 0x0a,
	IN_SC_Zero = 0x0b,
	IN_SC_Minus = 0x0c,
	IN_SC_Equals = 0x0d,
	IN_SC_Backspace = 0x0e,
	IN_SC_Tab = 0x0f,
	IN_SC_LeftBracket = 0x1a,
	IN_SC_RightBracket = 0x1b,
	IN_SC_Enter = 0x1c,
	IN_SC_Control = 0x1d,
	IN_SC_SemiColon = 0x27,
	IN_SC_SingleQuote = 0x28,
	IN_SC_Grave = 0x29,
	IN_SC_LeftShift = 0x2a,
	IN_SC_BackSlash = 0x2b,
	IN_SC_Comma = 0x33,
	IN_SC_Period = 0x34,
	IN_SC_Slash = 0x35,
	IN_SC_RightShift = 0x36,

	IN_SC_Alt = 0x38,
	IN_SC_Space = 0x39,
	IN_SC_CapsLock = 0x3a,
	IN_SC_NumLock = 0x45,
	IN_SC_ScrollLock = 0x46,

	IN_SC_UpArrow = 0x48,
	IN_SC_LeftArrow = 0x4b,
	IN_SC_RightArrow = 0x4d,
	IN_SC_DownArrow = 0x50,

	IN_SC_Home = 0x47,
	IN_SC_End = 0x4f,
	IN_SC_PgUp = 0x49,
	IN_SC_PgDown = 0x51,
	IN_SC_Insert = 0x52,
	IN_SC_Delete = 0x53,

	IN_SC_Pause = 0xe1,

	IN_SC_F1 = 0x3b,
	IN_SC_F2 = 0x3c,
	IN_SC_F3 = 0x3d,
	IN_SC_F4 = 0x3e,
	IN_SC_F5 = 0x3f,
	IN_SC_F6 = 0x40,
	IN_SC_F7 = 0x41,
	IN_SC_F8 = 0x42,
	IN_SC_F9 = 0x43,
	IN_SC_F10 = 0x44,

	IN_SC_F11 = 0x57,
	IN_SC_F12 = 0x58,

	IN_SC_A = 0x1e,
	IN_SC_B = 0x30,
	IN_SC_C = 0x2e,
	IN_SC_D = 0x20,
	IN_SC_E = 0x12,
	IN_SC_F = 0x21,
	IN_SC_G = 0x22,
	IN_SC_H = 0x23,
	IN_SC_I = 0x17,
	IN_SC_J = 0x24,
	IN_SC_K = 0x25,
	IN_SC_L = 0x26,
	IN_SC_M = 0x32,
	IN_SC_N = 0x31,
	IN_SC_O = 0x18,
	IN_SC_P = 0x19,
	IN_SC_Q = 0x10,
	IN_SC_R = 0x13,
	IN_SC_S = 0x1f,
	IN_SC_T = 0x14,
	IN_SC_U = 0x16,
	IN_SC_V = 0x2f,
	IN_SC_W = 0x11,
	IN_SC_X = 0x2d,
	IN_SC_Y = 0x15,
	IN_SC_Z = 0x2c,

	IN_SC_SecondaryBackSlash = 0x56, // Additional key in 102-keys layout

	//IN_KP_Enter = 0x0d,
	//IN_KP_Escape = 0x1b,
	//IN_KP_Space = 0x20,
	//IN_KP_Backspace = 0x
} /*IN_ScanCode*/;

enum
{

	IN_KP_None = 0,
	//IN_KP_Return = 0x1c,
	//IN_KP_Enter = 0x1c,
	IN_KP_Escape = 0x1b,
	IN_KP_Space = 0x20,
	IN_KP_BackSpace = 0x08,
	IN_KP_Tab = 0x09,
	IN_KP_Multiply = 0x37,
	IN_KP_Minus = 0x4a,
	IN_KP_Center = 0x4c,
	IN_KP_Plus = 0x4e,
	//IN_KP_Delete = 0x53,

};

// See wolf3d ID_IN.H

typedef enum IN_DemoMode
{
	IN_Demo_Off = 0,
	IN_Demo_Record,
	IN_Demo_Playback,
	IN_Demo_PlayDone
} IN_DemoMode;

typedef enum
{
	IN_motion_Left = -1,
	IN_motion_Up = -1,
	IN_motion_None = 0,
	IN_motion_Right = 1,
	IN_motion_Down = 1
} IN_Motion;

typedef enum
{
	IN_dir_North,
	IN_dir_NorthEast,
	IN_dir_East,
	IN_dir_SouthEast,
	IN_dir_South,
	IN_dir_SouthWest,
	IN_dir_West,
	IN_dir_NorthWest,
	IN_dir_None
} IN_Direction;

// NOTE: fire is stored separate from this struct in Keen 5 disasm

typedef struct IN_KeyMapping
{
	IN_ScanCode jump;
	IN_ScanCode pogo;
	IN_ScanCode fire;
#ifdef EXTRA_KEYBOARD_OPTIONS
	IN_ScanCode status; // not saved in CONFIG.CKx
	IN_ScanCode toggleScorebox;
#endif
#ifdef QUICKSAVE_ENABLED
	IN_ScanCode quickSave; // not saved in CONFIG.CKx
	IN_ScanCode quickLoad; // not saved in CONFIG.CKx
#endif
	IN_ScanCode upLeft;
	IN_ScanCode up;
	IN_ScanCode upRight;
	IN_ScanCode left;
	IN_ScanCode right;
	IN_ScanCode downLeft;
	IN_ScanCode down;
	IN_ScanCode downRight;
} IN_KeyMapping;

extern IN_KeyMapping in_kbdControls;

extern IN_ScanCode *in_key_controls[];
extern const int in_key_button_controls;
extern const int in_key_direction_controls;

extern int16_t in_gamepadButtons[4];

typedef enum IN_ControlType
{
	IN_ctrl_Keyboard1,
	IN_ctrl_Keyboard2,
	IN_ctrl_Joystick1,
	IN_ctrl_Joystick2,
	IN_ctrl_Mouse,
	IN_ctrl_All
} IN_ControlType;

extern const char *IN_ControlType_Strings[];

typedef struct IN_ControlFrame
{
	bool jump, pogo, button2, button3;
	int x, y;
	IN_Motion xDirection, yDirection;
	IN_Direction dir;
} IN_ControlFrame;

typedef struct IN_Cursor
{
	bool button0;
	bool button1;
	IN_Motion xMotion;
	IN_Motion yMotion;
} IN_Cursor;

extern IN_DemoMode in_demoState;
extern IN_ControlType in_controlType;
extern IN_ControlType in_backupControlType;

extern bool in_Paused;
extern const char *in_PausedMessage;
extern bool in_disableJoysticks;
extern bool in_joyAdvancedMotion;

typedef enum IN_JoyConfItem
{
	IN_joy_jump,
	IN_joy_pogo,
	IN_joy_fire,
	IN_joy_menu,
	IN_joy_status,
#ifdef QUICKSAVE_ENABLED
	IN_joy_quickload,
	IN_joy_quicksave,
#endif
	IN_joy_deadzone,
	IN_joy_modern
} IN_JoyConfItem;

void IN_PumpEvents();
void IN_WaitKey();
void IN_SetupKbdControls();
void IN_SaveKbdControls();
const char *IN_GetScanName(IN_ScanCode scan);
bool IN_GetKeyState(IN_ScanCode scanCode);
IN_ScanCode IN_GetLastScan(void);
void IN_SetLastScan(IN_ScanCode scanCode);
char IN_GetLastASCII(void);
void IN_SetLastASCII(char c);
void IN_StartTextInput(const char *reason, const char *existing);
void IN_StopTextInput();
void IN_SetControlType(int player, IN_ControlType type);
void IN_GetJoyAbs(int joystick, int *x, int *y);
uint16_t IN_GetJoyButtonsDB(int joystick);
bool IN_JoyPresent(int joystick);
void IN_ReadCursor(IN_Cursor *cursor);
void IN_Startup(void);
void IN_Default(bool gotit, int16_t inputChoice);
bool IN_DemoStartRecording(int bufferSize);
void IN_DemoStartPlaying(uint8_t *data, int len);
void IN_DemoStopPlaying();
void IN_DemoFreeBuffer();
#if 0
bool IN_DemoIsPlaying();
#endif
IN_DemoMode IN_DemoGetMode();
void IN_DemoSaveToFile(const char *fileName, uint16_t mapNumber);
void IN_ClearKeysDown();
void IN_ClearKey(IN_ScanCode key);
void IN_ReadControls(int player, IN_ControlFrame *controls);
void IN_WaitButton();
int IN_CheckAck();
bool IN_UserInput(int tics, bool arg4);
int IN_GetJoyConf(IN_JoyConfItem item);
void IN_SetJoyConf(IN_JoyConfItem item, int value);
bool IN_GetJoyButtonFromMask(uint16_t mask, IN_JoyConfItem btn);
bool IN_IsJoyButtonDown(IN_JoyConfItem btn);
const char *IN_GetJoyName(int joystick);
const char *IN_GetJoyButtonName(int joystick, int button);

// Called by the backend.
bool INL_StartJoy(int joystick);
void INL_StopJoy(int joystick);
void IN_HandleKeyUp(IN_ScanCode sc, bool special);
void IN_HandleKeyDown(IN_ScanCode sc, bool special);
void IN_HandleTextEvent(const char *utf8Text);

typedef struct IN_Backend
{
	void (*startup)(bool disableJoysticks);
	void (*shutdown)();
	void (*pumpEvents)();
	void (*waitKey)();
	bool (*joyStart)(int joystick);
	void (*joyStop)(int joystick);
	bool (*joyPresent)(int joystick);
	void (*joyGetAbs)(int joystick, int *x, int *y);
	uint16_t (*joyGetButtons)(int joystick);
	const char *(*joyGetName)(int joystick);
	const char *(*joyGetButtonName)(int joystick, int index);
	void (*startTextInput)(const char *reason, const char *oldText);
	void (*stopTextInput)();

	// minimum and maximum values returned by joyGetAbs();
	// requirement: 0 < (joyAxisMax - joyAxisMin) <= 92681
	int joyAxisMin, joyAxisMax;
	bool supportsTextEvents;
} IN_Backend;

IN_Backend *IN_Impl_GetBackend();
#endif
