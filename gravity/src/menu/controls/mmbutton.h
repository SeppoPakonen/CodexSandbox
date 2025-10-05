/*
 *   GRAVITY STRIKE -- menu/controls/mmbutton.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _MMBUTTON_
#define _MMBUTTON_

#include "mcontrol.h"
#include <string.h>

class mMainButton : public mControl
{
  private:
    char  text[40];                                // text to draw on button

  public:
    // get/set
    void set_text(char *n)                         { strncpy(text, n, 40); }

    // functions
    void create(int x, int y, int dx, int dy, int w, int h);// create standard button
    void draw(BITMAP *);                           // draw button
    void do_explosion();                           // do explosion of button
};

#endif

