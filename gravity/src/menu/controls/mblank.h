/*
 *   GRAVITY STRIKE -- menu/controls/mblank.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _MBLANK_
#define _MBLANK_

#include "mcontrol.h"
#include <string.h>

class mBlank : public mControl
{
  private:
    int  field_pos;                                // starting x of field
    char text[40];

  public:
    void set_text(char *n)                         { strncpy(text, n, 40); }
    int  get_int()                                 { return field_pos; }

    // functions
    void create(int x, int y, int dx, int dy, int w, int h, int pfield); // create blank button
    void draw(BITMAP *);                           // draw bar
};

#endif

