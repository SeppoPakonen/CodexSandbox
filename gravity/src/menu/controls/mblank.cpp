/*
 *   GRAVITY STRIKE -- menu/controls/mblank.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegttf.h"
#include "mblank.h"
#include "../../gsclear.h"
#include "../../gsglob.h"
#include "../gsmd.h"

void mBlank::create(int px, int py, int dx, int dy, int pw, int ph, int pfield)
{
  mControl::create(px, py, dx, dy);
  w = pw;
  h = ph;
  field_pos = pfield;
  strcpy(text, "");
}

void mBlank::draw(BITMAP *v)
{
  if (!(flags & MC_INVISIBLE))
  {
    int c = color1;
    if (flags & MC_ACTIVE) c = color2;
    FONT *f = (FONT*)globals->fontdat[FONT_IMPACT10].dat;

    int theight = text_height(f);

    rect(v, (int)x+field_pos, (int)y, (int)x+w, (int)y+h, c);

    if (strlen(text) > 0)
    {
      aatextout(v, f, text, (int)x, (int)y+h/2-theight/2, c);
    }

    clearlist->add(v, x, y, w, h);
  }
}


