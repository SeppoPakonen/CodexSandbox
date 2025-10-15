/*
 *   GRAVITY STRIKE -- menu/controls/mtext.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "mtext.h"
#include "allegro5_wrapper.h"
#include "../../gsclear.h"
#include "../../gsglob.h"
#include "../gsmd.h"

void mNormalText::create(int px, int py, int dx, int dy)
{
  mControl::create(px, py, dx, dy);

  strcpy(text, "");
  font_num = FONT_ICE;
  color = globals->col_white;
}

void mNormalText::draw(BITMAP *v)
{
  if (!(flags & MC_INVISIBLE))
  {
    FONT *f = (FONT*)globals->fontdat[font_num].dat;

    int tlen = text_length(f, text);
    int theight = text_height(f);
  
    text_mode(-1);
    aatextout(v, f, text, (int)x, (int)y, color);

    clearlist->add(v, x, y, tlen, theight);
  }
}

