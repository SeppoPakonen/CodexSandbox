/*
 *   GRAVITY STRIKE -- menu/controls/makeyb.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _MKEYCONTROL_
#define _MKEYCONTROL_

#include "mcontrol.h"
#include <string.h>

class mKeyboardOption : public mControl
{
  private:
    char text[40];                                 // text to display
    int  dispkey;                                  // key to display

  public:
    // get / set (overloaded)
    void  set_text(char *n)                        { strncpy(text, n, 40); }
    void  set_int(int n)                           { dispkey = n; }

    // functions
    void create(int x, int y, int dx, int dy, int w, int h);// create keyboard button
    void draw(BITMAP *);                           // draw button
};

#endif

