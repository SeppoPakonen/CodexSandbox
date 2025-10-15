/*
 *   GRAVITY STRIKE -- menu/controls/maoption.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "maoption.h"
#include "allegro5_wrapper.h"
#include "../../gsclear.h"
#include "../../gsglob.h"
#include "../../gshelper.h"
#include "../gsmd.h"
#include "mtypes.h"
#include "../mpixels.h"

extern mPixelRain *mpixels;

mArrowOption::~mArrowOption()
{
  if (arrow1) delete(arrow1);
  if (arrow2) delete(arrow2);
  if (bar)    delete(bar);
}

void mArrowOption::create(int px, int py, int sx, int sy, int pw, int ph, int dir)
{
  int asx, asy;

  mControl::create(px, py, sx, sy);
  w = pw;
  h = ph;
  direction = dir;
  strcpy(text, "");

  arrow1 = new mArrowButton();
  arrow2 = new mArrowButton();
  bar    = new mBar();

  asx = asy = 0;

  switch (direction)
  {
    case A_LEFTRIGHT :
         if (sx) asx = sx+2; if (sy) asy = sy+h-14-2;
         arrow1->create(px+2, py+h-14-2, asx, asy, A_LEFT);

         if (sx) asx = sx+w-14-2; if (sy) asy = sy+h-14-2;
         arrow2->create(px+w-14-2, py+h-14-2, asx, asy, A_RIGHT);

         if (sx) asx = sx+16+2; if (sy) asy = sy+h-14-2;
         bar->create(px+16+2, py+h-14-2, asx, asy, w-36, 14);
         break;

    case A_UPDOWN :
         break;
  }
}

void mArrowOption::draw(BITMAP *v)
{
  if (!(flags & MC_INVISIBLE))
  {
    int c = color1;
    int tc = color1;
    if (flags & MC_ACTIVE) { c = color2; tc = globals->col_green; }

    FONT *f = (FONT*)globals->fontdat[FONT_ICE].dat;

    int tlen = 0, theight = 0;
    if (strlen(text) > 0)
    {
      tlen = text_length(f, text);
      theight = text_height(f);
    }

    switch (direction)
    {
      case A_LEFTRIGHT :
           rect(v, (int)x, (int)y, (int)x+w, (int)y+h, c);

           if (strlen(text) > 0)
           {
             text_mode(-1);
             aatextout(v, f, text, (int)x+w/2-tlen/2, (int)y+(h-16)/2-theight/2, tc);
           
             if (flags & MC_ACTIVE)
             {
               mpixels->add(10,fxrand()%tlen + x+w/2-tlen/2,
                               fxrand()%theight + y+(h-16)/2-theight/2,
                               0, 0, 1-(fxrand() % 3), 1-(fxrand() % 3),
                               50, globals->tbl_green, 10, 1000, 10);
             }
           }

           arrow1->draw(v);
           arrow2->draw(v);
           bar->draw(v);
           break;

      case A_UPDOWN :
           break;
    }

    clearlist->add(v, x, y, w, h);
  }
}

int mArrowOption::update()
{
  arrow1->update();
  arrow2->update();
  bar->update();

  return (mControl::update());
}

void mArrowOption::check()
{
  if (arrow1->get_flags() & MC_PUSHED) arrow1->del_flag(MC_PUSHED);
  if (arrow2->get_flags() & MC_PUSHED) arrow2->del_flag(MC_PUSHED);

  arrow1->check();
  arrow2->check();
  bar->check();

  mControl::check();
}

void mArrowOption::set_dest_x(int n)
{
  mControl::set_dest_x(n);

  arrow1->set_dest_x(n+2);
  arrow2->set_dest_x(n+w-2);
  bar->set_dest_x(n+16);
}

void mArrowOption::set_dest_y(int n)
{
  mControl::set_dest_y(n);

  arrow1->set_dest_y(n+h-14-2);
  arrow2->set_dest_y(n+h-14-2);
  bar->set_dest_y(n+h-14-2);
}

