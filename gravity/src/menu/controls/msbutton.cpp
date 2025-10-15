/*
 *   GRAVITY STRIKE -- menu/controls/msbutton.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"
#include "msbutton.h"
#include "../../gsclear.h"
#include "../../gsglob.h"
#include "../../gshelper.h"
#include "../gsmd.h"
#include "../mpixels.h"

extern mPixelRain *mpixels;

void mSmallButton::create(int px, int py, int sx, int sy, int pw, int ph)
{
  mControl::create(px, py, sx, sy);

  w = pw;
  h = ph;
  
  always_active = 0;
}

void mSmallButton::draw(BITMAP *v)
{
  if (!(flags & MC_INVISIBLE))
  {
    int c = color1;
    if (flags & MC_ACTIVE || always_active) c = globals->col_green;
    FONT *f = (FONT*)globals->fontdat[FONT_IMPACT10].dat;
  
    int tlen = text_length(f, text);
    int theight = text_height(f);
  
    text_mode(-1);
    aatextout(v, f, text, (int)x+w/2-tlen/2, (int)y+h/2-theight/2+1, c);

    rect(v, (int)x, (int)y, (int)x+w, (int)y+h, color2);
    
    if (flags & MC_ACTIVE || always_active)
    {
      mpixels->add(10,fxrand()%tlen + x+w/2-tlen/2,
                      fxrand()%theight + y+h/2-theight/2,
                      0, 0, 1-(fxrand() % 3), 1-(fxrand() % 3),
                      50, globals->tbl_green, 10, 1000, 10);
    }
  
//    rectfill(v, (int)x+1, (int)y+1, (int)x+w-1, (int)y+h-1, c);
    clearlist->add(v, x, y, w, h);
  }
}

void mSmallButton::do_explosion()
{
//  FONT *f = (FONT*)globals->data[verdana12].dat;
//  int tlen = text_length(f, text);
//  int theight = text_height(f);

  mpixels->add(300,x+w/2-w/4, y+h/2, 0, 0, 0, 0, 18, globals->tbl_green, 10, 1000, 100);
  mpixels->add(200,x+w/2, y+h/2, 0, 0, 0, 0, 18, globals->tbl_green, 10, 1000, 100);
  mpixels->add(300,x+w/2+w/4, y+h/2, 0, 0, 0, 0, 18, globals->tbl_green, 10, 1000, 100);
}

