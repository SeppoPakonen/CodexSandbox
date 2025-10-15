/*
 *   GRAVITY STRIKE -- menu/controls/mmbutton.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"
#include "mmbutton.h"
#include "../../gsclear.h"
#include "../../gsglob.h"
#include "../../gshelper.h"
#include "../gsmd.h"
#include "../mpixels.h"

extern mPixelRain *mpixels;

void mMainButton::create(int px, int py, int sx, int sy, int pw, int ph)
{
  mControl::create(px, py, sx, sy);

  w = pw;
  h = ph;
}

void mMainButton::draw(BITMAP *v)
{
  if (!(flags & MC_INVISIBLE))
  {
    int c = color1;
    if (flags & MC_ACTIVE) c = globals->col_green;
    FONT *f = (FONT*)globals->fontdat[FONT_ICE].dat;
//    FONT *f = (FONT*)globals->data[font_impact2].dat;
  
    int tlen = text_length(f, text);
    int theight = text_height(f);

    text_mode(-1);
    aatextout(v, f, text, (int)x+w/2-tlen/2, (int)y+h/2-theight/2, c);
  
    if (flags & MC_ACTIVE)
    {
      mpixels->add(10,fxrand()%tlen + x+w/2-tlen/2,
                      fxrand()%theight + y+h/2-theight/2,
                      0, 0, 1-(fxrand() % 3), 1-(fxrand() % 3),
                      50, globals->tbl_green, 10, 1000, 10);
      rect(v, (int)x, (int)y, (int)x+w, (int)y+h, color2);
    }
  
//    rectfill(v, (int)x+1, (int)y+1, (int)x+w-1, (int)y+h-1, c);
    clearlist->add(v, x, y, w, h);
  }
}

void mMainButton::do_explosion()
{
//  FONT *f = (FONT*)globals->data[verdana12].dat;
//  int tlen = text_length(f, text);
//  int theight = text_height(f);

  mpixels->add(300,x+w/2-w/4, y+h/2, 0, 0, 0, 0, 18, globals->tbl_green, 10, 1000, 100);
  mpixels->add(200,x+w/2, y+h/2, 0, 0, 0, 0, 18, globals->tbl_green, 10, 1000, 100);
  mpixels->add(300,x+w/2+w/4, y+h/2, 0, 0, 0, 0, 18, globals->tbl_green, 10, 1000, 100);
}

