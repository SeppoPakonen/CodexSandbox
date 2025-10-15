/*
 *   GRAVITY STRIKE -- menu/controls/mtslider.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "mtslider.h"
#include "allegro5_wrapper.h"
#include "../../gsclear.h"
#include "../../gsglob.h"
#include "../gsmd.h"
#include "mtypes.h"
#include "../mpixels.h"

extern mPixelRain *mpixels;

mTextSlider::~mTextSlider()
{
  if (arrow1) delete(arrow1);
  if (arrow2) delete(arrow2);
  if (bar)    delete(bar);
}

void mTextSlider::create(int px, int py, int sx, int sy, int pw, int ph, int pfield)
{
  int asx, asy;

  mControl::create(px, py, sx, sy);
  w = pw;
  h = ph;
  strcpy(text, "");
  field_pos = pfield;
  arrow1 = arrow2 = NULL;
  bar    = new mBar();
  align = 0;

  asx = asy = 0;

  if (mode)
  {
    arrow1 = new mArrowButton();
    arrow2 = new mArrowButton();

    if (sx) asx = sx+field_pos; if (sy) asy = sy;
    arrow1->create(px+field_pos, py, asx, asy, A_LEFT);

    if (sx) asx = sx+w-14; if (sy) asy = sy;
    arrow2->create(px+w-14, py, asx, asy, A_RIGHT);

    if (sx) asx = sx+field_pos+16; if (sy) asy = sy;
    bar->create(px+field_pos+16, py, asx, asy, w-field_pos-32, ph);
  }
  else
  {
    if (sx) asx = sx+field_pos; if (sy) asy = sy;
    bar->create(px+field_pos, py, asx, asy, w-field_pos, ph);
  }

}

void mTextSlider::draw(BITMAP *v)
{
  if (!(flags & MC_INVISIBLE))
  {
    int c = color1;
    int tc = color1;
    if (flags & MC_ACTIVE) { c = color2; tc = globals->col_green; }

    FONT *f = (FONT*)globals->fontdat[FONT_IMPACT10].dat;

    int tlen = 0, theight = 0;
    if (strlen(text) > 0)
    {
      tlen = text_length(f, text);
      theight = text_height(f);
    }

//    rect(v, (int)x, (int)y, (int)x+w, (int)y+h, c);

    if (strlen(text) > 0)
    {
      text_mode(-1);
      aatextout(v, f, text, (int)x, (int)y+h/2-theight/2, tc);
    }

    if (mode)
    {
      arrow1->draw(v);
      arrow2->draw(v);
    }
    bar->draw(v);

    clearlist->add(v, x, y, w, h);
  }
}

int mTextSlider::update()
{
  if (mode)
  {
    arrow1->update();
    arrow2->update();
  }
  bar->update();

  return (mControl::update());
}

void mTextSlider::check()
{
  if (mode)
  {
    if (arrow1->get_flags() & MC_PUSHED) arrow1->del_flag(MC_PUSHED);
    if (arrow2->get_flags() & MC_PUSHED) arrow2->del_flag(MC_PUSHED);
    arrow1->check();
    arrow2->check();
  }

  bar->check();

  mControl::check();
}

void mTextSlider::set_dest_x(int n)
{
  mControl::set_dest_x(n);

  if (mode)
  {
    arrow1->set_dest_x(n+field_pos);
    arrow2->set_dest_x(n+w-14);
    bar->set_dest_x(n+field_pos+16);
  }
  else
    bar->set_dest_x(n+field_pos);
}

void mTextSlider::set_dest_y(int n)
{
  mControl::set_dest_y(n);

  if (mode)
  {
    arrow1->set_dest_y(n);
    arrow2->set_dest_y(n);
    bar->set_dest_y(n);
  }
  else
    bar->set_dest_y(n);
}

void mTextSlider::set_text(char *n)
{
  strncpy(text, n, 40);
  
  if (align)
  {
    FONT *f = (FONT*)globals->fontdat[FONT_IMPACT10].dat;
    int tlen = text_length(f, text);
  
    field_pos = tlen+5;
    arrow1->set_dest_x(dest_x+field_pos);
    arrow2->set_dest_x(dest_x+field_pos+16+bar->get_w()+2);
    bar->set_dest_x(dest_x+field_pos+16);
    
    w = field_pos + 32 + bar->get_w() + 2;
  }
}

