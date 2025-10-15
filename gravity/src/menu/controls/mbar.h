/*
 *   GRAVITY STRIKE -- menu/controls/mbar.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _MBAR_
#define _MBAR_

#include "mcontrol.h"

class mBar : public mControl
{
  private:
    float percent;                                   // 0 (empty), 100 (full)

  public:
    // get/set
    void set_int(int n)                            { if (n >= 0 && n <= 100) percent = (int)n; }
    int  get_int()                                 { return (int)percent; }
    void set_float(float n)                        { if (n >= 0 && n <= 100) percent = n; }
    float get_float()                              { return percent; }

    // functions
    void create(int x, int y, int dx, int dy, int w, int h); // create bar
    void draw(BITMAP *);                           // draw bar
};

#endif

