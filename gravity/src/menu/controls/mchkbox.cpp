/*
 *   GRAVITY STRIKE -- menu/controls/mchkbox.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "mchkbox.h"
#include "../../gsclear.h"
#include "../../gsglob.h"
#include "../gsmd.h"
#include "allegro5_wrapper.h"

void mCheckBox::create(int px, int py, int dx, int dy, int pw, int ph)
{
  mControl::create(px, py, dx, dy);
  w = ow = pw;
  h = ph;
  strcpy(text, "");
  tlen = 0;
  mode = 0;
}

void mCheckBox::set_text(char *n)
{
  strncpy(text, n, 50);
  tlen = text_length((FONT*)globals->fontdat[FONT_IMPACT10].dat, text);
  w = ow + tlen + 5;
}

void mCheckBox::check()
{
  // check for mouse-over
  if (!(get_flags() & MC_INACTIVE))
  {
    if (mouse_x >= x && mouse_x <= x+w &&
        mouse_y >= y && mouse_y <= y+h)
    {
      set_flag(MC_ACTIVE);
      if ((mouse_b & 1) && !(flags & MC_PUSHED)) { set_flag(MC_PUSHED); mouse_b = 0; }
      else if ((mouse_b & 1) && (flags & MC_PUSHED)) { del_flag(MC_PUSHED); mouse_b = 0; }
    }
    else
    {
      del_flag(MC_ACTIVE);
    }
  }
}

void mCheckBox::draw(BITMAP *v)
{
  if (!(flags & MC_INVISIBLE))
  {
    int c = color1, tc = color1;
    if (flags & MC_ACTIVE) { c = color2; tc = globals->col_green; }
    FONT *f = (FONT*)globals->fontdat[FONT_IMPACT10].dat;
    int tlen = text_length(f, text);

    if (mode >= 0)
      aatextout(v, f, text, (int)x, (int)y+h/2-text_height(f)/2, tc);
    else
      aatextout(v, f, text, (int)x+w-tlen, (int)y+h/2-text_height(f)/2, tc);

    if (mode >= 0)
      rect(v, (int)x+w-(w-tlen)+10, (int)y, (int)x+w, (int)y+h, c);
    else
      rect(v, (int)x, (int)y, (int)x+w-tlen-10, (int)y+h, c);

    if (flags & MC_PUSHED)
    {
      if (mode >= 0)
      {
        line(v, (int)x+1+w-(w-tlen)+10, (int)y+1, (int)x+w-1, (int)y+h-1, c);
        line(v, (int)x+w-1, (int)y+1, (int)x+1+w-(w-tlen)+10, (int)y+h-1, c);
      }
      else
      {
        line(v, (int)x+1, (int)y+1, (int)x+w-tlen-10-1, (int)y+h-1, c);
        line(v, (int)x+w-tlen-10-1, (int)y+1, (int)x+1, (int)y+h-1, c);
      }
    }

    clearlist->add(v, x, y, w, h);
  }
}


