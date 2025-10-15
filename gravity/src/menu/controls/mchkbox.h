/*
 *   GRAVITY STRIKE -- menu/controls/mchkbox.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _MCHECKBOX_
#define _MCHECKBOX_

#include "mcontrol.h"

class mCheckBox : public mControl
{
  private:
    char text[50];
    int  tlen;
    int  mode;
    int  ow;                                       // original width

  public:
    void set_text(char *n);
    void set_int(int n)                            { mode = n; }

    // functions
    void check();
    void create(int x, int y, int dx, int dy, int w, int h); // create bar
    void draw(BITMAP *);                           // draw bar
};

#endif

