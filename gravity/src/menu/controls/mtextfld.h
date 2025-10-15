/*
 *   GRAVITY STRIKE -- menu/controls/mtextfld.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _MTEXTFIELD_
#define _MTEXTFIELD_

#include "mcontrol.h"

class mTextField : public mControl
{
  private:
    int  maxchars;                                 // max chars for input
    char text[150];                                // text to display/edit (max. 150 chars)
    char caption[50];                              // caption of input field
    int  field_pos;                                // x starting position of input field
    bool enter;                                    // enter pressed?

  public:
    // get/set
    void set_text(char *n);
    char *get_text()                               { return text; }
    void set_int(int n)                            { if (maxchars < 150) maxchars = n; else maxchars = 150; }

    // functions
    void create(int x, int y, int dx, int dy, int w, int h, int fpos); // create field
    int  update();                                 // update field (get text input)
    void draw(BITMAP *);                           // draw field
    int  get_align()                               { if (enter) { enter = false; return true; } return false; }
};

#endif

