/*
 *   GRAVITY STRIKE -- gswaypnt.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "gswaypnt.h"
#include "gserror.h"
#include <stdio.h>

Waypoint::Waypoint(int px, int py, float pspd, int ppause)
{
  x = px;
  y = py;
  speed = pspd;
  if (speed == 0) speed = 1;
  pause = ppause;
  curpause = 0;
  next = NULL;
}

Waypoint::~Waypoint()
{
}

gsWaypoint::gsWaypoint()
{
  first = NULL;
  count = 0;
  current = 0;
  direction = 1;
  mode = 0;
}

gsWaypoint::~gsWaypoint()
{
  remove_all();
}

void gsWaypoint::remove_all()
{
  Waypoint *temp, *next;
  temp = first;

  while (temp)
  {
    next = temp->get_next();
    delete temp;
    temp = next;
  }

  count = 0;
  current = 0;
  first = NULL;
  direction = 1;
}

Waypoint *gsWaypoint::add(int x, int y, float spd, int pause)
{
  Waypoint *temp, *cur;

  temp = new Waypoint(x, y, spd, pause);

  if (!first) first = temp;
  else
  {
    cur = first;
    while (cur->get_next()) cur = cur->get_next();
    cur->set_next(temp);
  }

  count++;

  return temp;
}

Waypoint *gsWaypoint::get(int n)
{
  Waypoint *cur = first;
  int num = 0;

  cur = first;
  while (cur)
  {
    if (num == n) return cur;
    cur = cur->get_next();
    num++;
  }

  return NULL;
}

void gsWaypoint::remove(Waypoint *w)
{
  Waypoint *cur = first, *last = NULL, *next;

  while (cur)
  {
    next = cur->get_next();

    if (cur == w)
    {
      if (last) last->set_next(cur->get_next());
      else first = cur->get_next();
      delete cur;
      break;
    }

    last = cur;
    cur = next;
  }
}

void gsWaypoint::set_next()
{
  if (count > 0)
  switch (mode)
  {
    case 0  :  if (current < count-1) current++; else current = 0;
               break;

    case 1  :  if (current == count-1 || (current == 0 && direction < 0)) direction = -direction; 
               current += direction;
               break;

    case 2  :  if (current < count-1) current++; else current = count-1;
               break;
  }
}

