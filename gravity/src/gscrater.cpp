/*
 *   GRAVITY STRIKE -- gscrater.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "gscrater.h"
#include "gseffect.h"
#include "gserror.h"
#include "gsglob.h"
#include "gshelper.h"
#include "gsmap.h"
#include "gsnetw.h"

int tmp_flag = 0;

Crater::Crater(int px, int py, int psize)
{
  set_in_wall(-1);
  set_x(px);
  set_y(py);
  set_size(psize);
}

Crater::~Crater()
{
}

void make_crater_wall(BITMAP *bmp, int x, int y, int d)
{
  if (getpixel(bmp,x,y) > 0)
  {
    putpixel(bmp,x,y+gsrand()%5-2,0);
    putpixel(bmp,x,y+gsrand()%5-2,0);
    putpixel(bmp,x,y+gsrand()%5-2,globals->tbl_red[gsrand() % 6]);
    tmp_flag = 1;
  }
}

void Crater::draw(gsMap *m)
{
  if (m->is_on_map(get_x()-get_size(), get_y()-get_size(), get_size()*2, get_size()*2))
  {
    circlefill(m->get_vscreen(), get_x() - m->get_map_x(), get_y() - m->get_map_y(), get_size(), 0);
    tmp_flag = -1;
    do_circle(m->get_vscreen(), get_x() - m->get_map_x(), get_y() - m->get_map_y(), get_size()+1, 0, make_crater_wall);
    if (get_in_wall() == -1)
      if (tmp_flag == 1) set_in_wall(1);
        else set_in_wall(0);

    drawn = 1;
  }
}

gsCrater::gsCrater()
{
  first_crater = NULL;
  last_crater  = NULL;
  count = 0;
}

gsCrater::~gsCrater()
{
  Crater *temp, *next_crater;
  temp = first_crater;

  while (temp != NULL)
  {
    next_crater = temp->get_next();
    delete temp;
    temp = next_crater;
  }
}

void gsCrater::add(int x, int y, int size, int instant_draw, int net_send)
{
  Crater *temp_crater;

  update();
  temp_crater = new Crater(x, y, size);

  if (temp_crater != NULL)
  {
    if (first_crater==NULL)
    {
      last_crater = first_crater = temp_crater;
      first_crater->set_next(NULL); first_crater->set_prev(NULL);
      temp_crater->set_next(NULL); temp_crater->set_prev(NULL);
    }
    else
    {
      last_crater->set_next(temp_crater);
      temp_crater->set_prev(last_crater);
      last_crater = temp_crater;
      temp_crater->set_next(NULL);
    }
  } // temp_crater != NULL

  count++;

  if (instant_draw)
  {
    temp_crater->draw(playmap[0]);
    if (globals->splitscreen) temp_crater->draw(playmap[1]);
  }

  if (globals->network_game && net_send)
  {
    net->game_send_crater(x, y, size);
  }
}

void gsCrater::remove(Crater *temp)
{
  Crater *next_crater, *prev_crater;

  if (temp != NULL)
  {
    prev_crater = temp->get_prev();
    next_crater = temp->get_next();

    if (prev_crater==NULL) first_crater = next_crater;
    else prev_crater->set_next(next_crater);

    if (next_crater==NULL) last_crater = prev_crater;
    else next_crater->set_prev(prev_crater);

    delete temp;
    temp = NULL;

    count--;
  }
}

void gsCrater::draw(gsMap *m)
{
  Crater *temp;
  temp = first_crater;
  bool d = false;

  while (temp != NULL)
  {
    temp->draw(m);
    temp = temp->get_next();
  }
  /* draw only one crater per call
  while (temp != NULL && !d)
  {
    if (!temp->get_drawn())
    {
      temp->draw(m);
      d = true;
    }
    else
    {
      temp = temp->get_next();
    }
  }

  if (temp == NULL) all_drawn = true;
  */
}


void gsCrater::reset(gsMap *m)
{
  Crater *temp;
  temp = first_crater;

  while (temp != NULL)
  {
    temp->set_drawn(false);
    if (m->is_on_screen(temp->get_x() - 100, 
                        temp->get_y() - 100, 
                        200, 200))
      temp->draw(m);
    temp = temp->get_next();
  }


  all_drawn = false;
}

void gsCrater::update()
{
  int small_removed = 0;
  Crater *temp, *next_crater;
  temp = first_crater;

  while (temp != NULL)
  {
    next_crater = temp->get_next();

    if (temp->get_in_wall() == 0)
      remove(temp);
    /*
    else
    if (count >= globals->max_craters)
      if (temp->get_size() < 10)
      {
        remove(temp);
        small_removed = 1;
      }
    */

    temp = next_crater;
  }

  //if (count >= globals->max_craters && !small_removed)
    //remove(first_crater);
}

