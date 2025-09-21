#include "DolphinSDL.h"

extern WinKey winKey;



BOOL PADInit() {
    // Initialize the winKey structure
    winKey.button = 0;
    winKey.stickX = 0;
    winKey.stickY = 0;
    winKey.substickX = 0;
    winKey.substickY = 0;
    winKey.triggerLeft = 0;
    winKey.triggerRight = 0;
    memset(winKey.down, 0, sizeof(winKey.down));
    winKey.downCnt = 0;
    memset(winKey.up, 0, sizeof(winKey.up));
    winKey.upCnt = 0;
    return TRUE;
}


void PADRead(PADStatus* status) {
	TODO
	#if 0
	GetScreen().Update(0);
	
	memset(status, 0, sizeof(PADStatus));
	status->stickX			= winKey.stickX;
	status->stickY			= winKey.stickY;
	status->substickX		= winKey.substickX;
	status->substickY		= winKey.substickY;
	status->triggerLeft		= winKey.triggerLeft;
	status->triggerRight	= winKey.triggerRight;
	status->button			= winKey.button;
	#endif
}


BOOL PADReset(u32 mask) {
	
	return TRUE;
}

u32 OSGetConsoleType() {
	TODO
	return OS_CONSOLE_EMULATOR;
}




void PADControlMotor(int chan, u32 command) {
	TODO
}

BOOL PADRecalibrate(u32 mask) {
	TODO
}

void PADSetAnalogMode(u32 mode) {
	TODO
}




















// Parameters for dolphin controller

#ifndef MAC
#define PLAY        15          // the play of the stick
#else
#define PLAY        35          // the play of the stick
#endif
#define XMAX        74
#define YMAX        74
#define XMIDDLE     52
#define YMIDDLE     52

#define TRIGGER_MIN 40          // the play of the trigger
#define TRIGGER_MAX 210

/*---------------------------------------------------------------------------*
  Name:         ClampStick

  Description:  Adjusts stick movement data within the octagon, or clamps
                the data to the origin if stick is close to the origin as
                the play.

  Arguments:    px          pointer to movement data in terms of x-axis
                py          pointer to movement data in terms of y-axis

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ClampStick(s8* px, s8* py)
{
    int x = *px;
    int y = *py;
    int signX;
    int signY;
    int d;

    if (0 <= x)
    {
        signX = 1;
    }
    else
    {
        signX = -1;
        x = -x;
    }

    if (0 <= y)
    {
        signY = 1;
    }
    else
    {
        signY = -1;
        y = -y;
    }

    // Clamp dead zone
    if (x <= PLAY)
    {
        x = 0;
    }
    else
    {
        x -= PLAY;
    }
    if (y <= PLAY)
    {
        y = 0;
    }
    else
    {
        y -= PLAY;
    }

    if (x == 0 && y == 0)
    {
        *px = *py = 0;
        return;
        // NOT REACHED HERE
    }

    // Clamp outer octagon
    if (XMIDDLE * y <= YMIDDLE * x)
    {
        d = YMIDDLE * x + (XMAX - XMIDDLE) * y;
        if (YMIDDLE * XMAX < d)
        {
            x = (s8) (YMIDDLE * XMAX * x / d);
            y = (s8) (YMIDDLE * XMAX * y / d);
        }
    }
    else
    {
        d = XMIDDLE * y + (YMAX - YMIDDLE) * x;
        if (XMIDDLE * YMAX < d)
        {
            x = (s8) (XMIDDLE * YMAX * x / d);
            y = (s8) (XMIDDLE * YMAX * y / d);
        }
    }

    *px = (s8) (signX * x);
    *py = (s8) (signY * y);
}

/*---------------------------------------------------------------------------*
  Name:         ClampTrigger

  Description:  Adjusts trigger movement data

  Arguments:    px          pointer to movement data in terms of x-axis
                py          pointer to movement data in terms of y-axis

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ClampTrigger(u8* trigger)
{
    if (*trigger <= TRIGGER_MIN)
    {
        *trigger = 0;
    }
    else
    {
        if (TRIGGER_MAX < *trigger)
        {
            *trigger = TRIGGER_MAX;
        }
        *trigger -= TRIGGER_MIN;
    }
}

/*---------------------------------------------------------------------------*
  Name:         PADClamp

  Description:  Clamps game pad status.

  Arguments:    status      array[PAD_MAX_CONTROLLERS] of PADStatus to be
                            clamped.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void PADClamp(PADStatus* status)
{
    int i;

    for (i = 0; i < PAD_MAX_CONTROLLERS; i++, status++)
    {
        if (status->err != PAD_ERR_NONE)
            continue;
        ClampStick(&status->stickX,    &status->stickY);
        ClampStick(&status->substickX, &status->substickY);
        ClampTrigger(&status->triggerLeft);
        ClampTrigger(&status->triggerRight);
    }
}
