#ifndef _DolphinSDL_DolphinSDL_h_
#define _DolphinSDL_DolphinSDL_h_

#include <GL/glew.h>

#include <DolphinRE/DolphinRE.h>
//#include <Core/Core.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


#include "SDL2.h"


#define PAD_BUTTON_LEFT         0x0001
#define PAD_BUTTON_RIGHT        0x0002
#define PAD_BUTTON_DOWN         0x0004
#define PAD_BUTTON_UP           0x0008
#define PAD_TRIGGER_Z           0x0010
#define PAD_TRIGGER_R           0x0020
#define PAD_TRIGGER_L           0x0040
#define PAD_BUTTON_A            0x0100
#define PAD_BUTTON_B            0x0200
#define PAD_BUTTON_X            0x0400
#define PAD_BUTTON_Y            0x0800
#define PAD_BUTTON_MENU         0x1000
#define PAD_BUTTON_START        0x1000



#define WINKEY_PAD_BUTTON_LEFT			PAD_BUTTON_LEFT
#define WINKEY_PAD_BUTTON_RIGHT			PAD_BUTTON_RIGHT
#define WINKEY_PAD_BUTTON_DOWN			PAD_BUTTON_DOWN
#define WINKEY_PAD_BUTTON_UP			PAD_BUTTON_UP
#define WINKEY_PAD_TRIGGER_Z			PAD_TRIGGER_Z
#define WINKEY_PAD_TRIGGER_R			PAD_TRIGGER_R
#define WINKEY_PAD_TRIGGER_L			PAD_TRIGGER_L
#define WINKEY_PAD_BUTTON_A				PAD_BUTTON_A
#define WINKEY_PAD_BUTTON_B				PAD_BUTTON_B
#define WINKEY_PAD_BUTTON_X				PAD_BUTTON_X
#define WINKEY_PAD_BUTTON_Y				PAD_BUTTON_Y
#define WINKEY_PAD_BUTTON_MENU			PAD_BUTTON_MENU
#define WINKEY_PAD_ORIGIN				PAD_ORIGIN


typedef struct WinKey_Str {
	int button;
	int stickX; // -127 <= stickX    <= 127
	int stickY; // -127 <= stickY    <= 127
	int substickX; // -127 <= stickX    <= 127
	int substickY; // -127 <= stickY    <= 127
	int triggerLeft; // 0 - 255
	int triggerRight; // 0 - 255
	unsigned int down[64];
	int downCnt;
	unsigned int up[64];
	int upCnt;
} WinKey;


#endif
