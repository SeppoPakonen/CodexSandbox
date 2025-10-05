/*
 *   GRAVITY STRIKE -- gsclear.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "gsclear.h"
#include "gsmap.h"


gsRectangle::gsRectangle(gsMap *pscr, int px, int py, int pw, int ph)
{
  scr = pscr;
  bscr = NULL;
  x = px;
  y = py;
  w = pw;
  h = ph;
}

gsRectangle::gsRectangle(BITMAP *pscr, int px, int py, int pw, int ph)
{
  scr = NULL;
  bscr = pscr;
  x = px;
  y = py;
  w = pw;
  h = ph;
}

void gsRectangle::draw()
{
  if (w == 1 && h == 1)
  {
    if (scr)
      putpixel(scr->get_vscreen(), x - scr->get_map_x(), y - scr->get_map_y(), 0);
    else
      putpixel(bscr, x, y, 0);
  }
  else
  {
    if (scr)
      rectfill(scr->get_vscreen(), x - scr->get_map_x(), y - scr->get_map_y(),
               x - scr->get_map_x() + w,
               y - scr->get_map_y() + h, 0);
    else
      rectfill(bscr, x, y, x + w, y + h, 0);
  }
}

gsClear::gsClear()
{
  first_rect = NULL;
  count = 0;
}

gsClear::~gsClear()
{
  clear_list();
}

void gsClear::clear_list()
{
  gsRectangle *temp, *next_rect;
  temp = first_rect;

  while (temp)
  {
    next_rect = temp->next;
    delete temp;
    temp = next_rect;
  }

  first_rect = NULL;
  count = 0;
}

void gsClear::add(gsMap *scr, int x, int y, int w, int h)
{
  gsRectangle *temp_rect;

  temp_rect = new gsRectangle(scr, x, y, w, h);

  if (temp_rect)
  {
    temp_rect->next = first_rect;
    first_rect = temp_rect;
  }
}

void gsClear::add(BITMAP *scr, int x, int y, int w, int h)
{
  gsRectangle *temp_rect;

  temp_rect = new gsRectangle(scr, x, y, w, h);

  if (temp_rect)
  {
    temp_rect->next = first_rect;
    first_rect = temp_rect;
  }
}

void gsClear::clear_screen()
{
  gsRectangle *temp = first_rect, *next_rect;

  while (temp)
  {
    next_rect = temp->next;

    temp->draw();
    delete(temp);
    count--;

    temp = next_rect;
  }

  first_rect = NULL;
}

