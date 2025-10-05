/*
 *   GRAVITY STRIKE -- menu/controls/mbar.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "mbar.h"
#include "../../gsclear.h"
#include "../../gsglob.h"
#include "../gsmd.h"

void mBar::create(int px, int py, int dx, int dy, int pw, int ph)
{
  mControl::create(px, py, dx, dy);
  w = pw;
  h = ph;
  percent = 0;
}

void mBar::draw(BITMAP *v)
{
  if (!(flags & MC_INVISIBLE))
  {
    int c = color1;
    if (flags & MC_ACTIVE) c = color2;

    rect(v, (int)x, (int)y, (int)x+w, (int)y+h, c);

    int maxlines = w/2-1;
    int lines = int(maxlines * float(percent/100.0));
    if (lines == 0 && percent > 0) lines = 1;

    for (int n=0; n < lines; n++)
    {
      vline(v, int(x+n*2)+2, int(y+2), int(y+h-2), color3);
    }

    clearlist->add(v, x, y, w, h);
  }
}


