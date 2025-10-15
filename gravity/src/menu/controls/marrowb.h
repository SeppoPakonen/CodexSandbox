/*
 *   GRAVITY STRIKE -- menu/controls/marrowb.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _MARROWBUTTON_
#define _MARROWBUTTON_

#include "mcontrol.h"

enum ARROW_DIRECTION {
  A_LEFT,
  A_RIGHT,
  A_UP,
  A_DOWN,
};

class mArrowButton : public mControl
{
  private:
    int direction;                                 // direction of arrow

  public:
    // functions
    void create(int x, int y, int dx, int dy, int dir);// create arrow button
    void create(int x, int y, int dx, int dy, int w, int h, int dir);// create arrow button
    void draw(BITMAP *);                           // draw button
};

#endif

