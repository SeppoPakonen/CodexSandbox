/*
 *   GRAVITY STRIKE -- menu/mstars.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "mstars.h"
#include "../gspixel.h"
#include "../gsclear.h"
#include "../gsglob.h"
#include "../gshelper.h"

mStar::mStar(int px, int py, float pxspd, float pyspd, int pcol)
{
  x = px;
  y = py;
  xspd = pxspd;
  yspd = pyspd;
  col = pcol;
}

mStar::~mStar()
{
}

void mStar::update()
{
  x -= xspd;

  if (x < 0) { x = 640; y = fxrand() % 480; }
}

void mStar::draw(BITMAP *v)
{
  putpixel(v, (int)x, (int)y, col);
  clearlist->add(v, x, y, 1, 1);
}

mStars::mStars()
{
  first_star = NULL;
  last_star = NULL;
  count = 0;
}

mStars::~mStars()
{
  mStar *temp, *next;

  temp = first_star;

  while (temp)
  {
    next = temp->get_next();
    delete(temp);
    temp = next;
  }
}


void mStars::add(int anz)
{
  mStar *temp_star;

  for (int n=0; n < anz; n++)
  {
    float xspd = (fxrand() % 10) / 1.5;
//    int r = (fxrand() % 100) + 155;

    if (xspd <= 0 || xspd > 10) xspd = 0.1;

    temp_star = new mStar(fxrand() % 640, fxrand() % 480,
                          xspd, 0,
                          globals->tbl_white[int(xspd)+4]);
  
    if (temp_star != NULL)
    {
      if (first_star == NULL)
      {
        last_star = first_star = temp_star;
        first_star->set_next(NULL); first_star->set_prev(NULL);
        temp_star->set_next(NULL); temp_star->set_prev(NULL);
      }
      else
      {
        last_star->set_next(temp_star);
        temp_star->set_prev(last_star);
        last_star = temp_star;
        temp_star->set_next(NULL);
      }
    } // temp_star != NULL
  
    count++;
  }

}

void mStars::update()
{
  mStar *temp = first_star;

  while (temp)
  {
    temp->update();
    temp = temp->get_next();
  }
}

void mStars::draw(BITMAP *v)
{
  mStar *temp = first_star;

  while (temp)
  {
    temp->draw(v);
    temp = temp->get_next();
  }
}

