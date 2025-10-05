/*
 *   GRAVITY STRIKE -- menu/controls/marrowb.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "marrowb.h"
#include "../../gsclear.h"
#include "../../gsglob.h"
#include "../gsmd.h"

void mArrowButton::create(int px, int py, int sx, int sy, int dir)
{
  mControl::create(px, py, sx, sy);
  w = 14;
  h = 14;

  direction = dir;
}

void mArrowButton::create(int px, int py, int sx, int sy, int pw, int ph, int dir)
{
  mControl::create(px, py, sx, sy);
  w = pw;
  h = ph;

  direction = dir;
}

void mArrowButton::draw(BITMAP *v)
{
  if (!(flags & MC_INVISIBLE))
  {
    int c = color1;
    if (flags & MC_ACTIVE) c = color2;
    if (flags & MC_PUSHED) c = color3;

    switch (direction)
    {
      case A_LEFT :
           rect(v, (int)x, (int)y, (int)x+w, (int)y+h, c);
           hline(v, (int)x+w/2+1, (int)y+h/3, (int)x+w, c);
           hline(v, (int)x+w/2+1, (int)y+h-h/3, (int)x+w, c);
           vline(v, (int)x+w/2+1, (int)y, (int)y+h/3, c);
           vline(v, (int)x+w/2+1, (int)y+h-h/3, (int)y+h, c);
           line(v, (int)x+1, (int)y+h/2, (int)x+w/2+1, (int)y, c);
           line(v, (int)x+1, (int)y+h/2, (int)x+w/2+1, (int)y+h, c);
           break;

      case A_RIGHT :
           rect(v, (int)x, (int)y, (int)x+w, (int)y+h, c);
           hline(v, (int)x+w/2-1, (int)y+h/3, (int)x, c);
           hline(v, (int)x+w/2-1, (int)y+h-h/3, (int)x, c);
           vline(v, (int)x+w/2-1, (int)y, (int)y+h/3, c);
           vline(v, (int)x+w/2-1, (int)y+h-h/3, (int)y+h, c);
           line(v, (int)x+w-1, (int)y+h/2, (int)x+w/2-1, (int)y, c);
           line(v, (int)x+w-1, (int)y+h/2, (int)x+w/2-1, (int)y+h, c);
           break;

      case A_UP :
           rect(v, (int)x, (int)y, (int)x+w, (int)y+h, c);
           hline(v, (int)x, (int)y+h/2+1, (int)x+w/3, c);
           hline(v, (int)x+w-w/3, (int)y+h/2+1, (int)x+w, c);
           vline(v, (int)x+w/3, (int)y+h/2+1, (int)y+h, c);
           vline(v, (int)x+w-w/3, (int)y+h/2+1, (int)y+h, c);
           line(v, (int)x+w/2, (int)y+1, (int)x, (int)y+h/2+1, c);
           line(v, (int)x+w/2, (int)y+1, (int)x+w, (int)y+h/2+1, c);
           break;

      case A_DOWN :
           rect(v, (int)x, (int)y, (int)x+w, (int)y+h, c);
           hline(v, (int)x, (int)y+h/2-1, (int)x+w/3, c);
           hline(v, (int)x+w-w/3, (int)y+h/2-1, (int)x+w, c);
           vline(v, (int)x+w/3, (int)y+h/2-1, (int)y, c);
           vline(v, (int)x+w-w/3, (int)y+h/2-1, (int)y, c);
           line(v, (int)x+w/2, (int)y+h-1, (int)x, (int)y+h/2-1, c);
           line(v, (int)x+w/2, (int)y+h-1, (int)x+w, (int)y+h/2-1, c);
           break;
    }

    clearlist->add(v, x, y, w, h);
  }
}


