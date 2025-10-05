/*
 *   GRAVITY STRIKE -- menu/controls/msbutton.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _MSBUTTON_
#define _MSBUTTON_

#include "mcontrol.h"
#include <string.h>

class mSmallButton : public mControl
{
  private:
    char  text[40];                                // text to draw on button
    int   always_active;                           // always in "active" drawing state?

  public:
    // get/set
    void set_text(char *n)                         { strncpy(text, n, 40); }
    void set_int(int n)                            { always_active = n; }
    int  get_int()                                 { return always_active; }

    // functions
    void create(int x, int y, int dx, int dy, int w, int h);// create standard button
    void draw(BITMAP *);                           // draw button
    void do_explosion();                           // do explosion of button
};

#endif

