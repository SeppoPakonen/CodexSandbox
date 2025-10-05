/*
 *   GRAVITY STRIKE -- menu/controls/mcontrol.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "mcontrol.h"
#include "../../gshelper.h"
#include "../../gsplayer.h"
#include "../../gssound.h"
#include "../../gsglob.h"
#include <math.h>

#define max(A, B)    ((A) > (B) ? (A) : (B))
#define min(A, B)    ((A) < (B) ? (A) : (B))

mControl::mControl()
{
  x = y = w = h = 0;
  dest_x = dest_y = xspd = yspd = 0;
  xacc = yacc = 1;
  flags = 0;
  color1 = makecol8(58, 64, 191);
  color2 = makecol8(89, 102, 251);
  color3 = makecol8(91, 162, 255);
  ox = oy = 0;

  gspd = 30;
}

mControl::~mControl()
{
}

int mControl::update()
{
  if (dest_x != x || dest_y != y)
  {
    float cxspd=xspd, cyspd=yspd;
    float cdist = distance((int)x, (int)y, (int)dest_x, (int)dest_y);

    cxspd = xspd * ((cdist / idist)+0.1);
    cyspd = yspd * ((cdist / idist)+0.1);

    x += cxspd;
    y += cyspd;

    if (fabs(dest_x - x) < fabs(cxspd)) { x = dest_x; xspd = 0; }
    if (fabs(dest_y - y) < fabs(cyspd)) { y = dest_y; yspd = 0; }

    return 1;
  }

  return 0;
}

void mControl::check()
{
  // check for mouse-over
  if (!(get_flags() & MC_INACTIVE))
  {
     if (mouse_x >= x && mouse_x <= x+w &&
         mouse_y >= y && mouse_y <= y+h)
     {
       set_flag(MC_ACTIVE);
       if (mouse_b & 1) set_flag(MC_PUSHED);
     }
     else
     {
       del_flag(MC_ACTIVE);
 //      del_flag(MC_PUSHED);
     }
  }
}

void mControl::create(int px, int py, int sx, int sy)
{
  x = ox = px;
  y = oy = py;
  dest_x = px;
  dest_y = py;

  if (sx != 0) { x = sx; dest_x = px; }
  else dest_x = x;

  if (sy != 0) { y = sy; dest_y = py; }
  else dest_y = y;

  if (x != dest_x) xspd = (dest_x-x)/gspd;
  if (y != dest_y) yspd = (dest_y-y)/gspd;

  idist = distance((int)x, (int)y, (int)dest_x, (int)dest_y);
  if (idist == 0) idist = 1;
}

void mControl::set_dest_x(int n)
{
  dest_x = n; xspd = (dest_x-x)/gspd;
  idist = distance((int)x, (int)y, (int)dest_x, (int)dest_y);
  if (idist == 0) idist = 1;
}

void mControl::set_dest_y(int n)
{
  dest_y = n; yspd = (dest_y-y)/gspd;
  idist = distance((int)x, (int)y, (int)dest_x, (int)dest_y);
  if (idist == 0) idist = 1;
}

void mControl::create(int x, int y, int sx, int sy, int w, int h)
{
}

void mControl::create(int x, int y, int sx, int sy, int dir)
{
}

void mControl::create(int x, int y, int sx, int sy, int w, int h, int dir)
{
}

void mControl::set_text(char *n)
{
}

void mControl::do_explosion()
{
}

void mControl::reinit()
{
}

class mArrowButton *mControl::get_arrow1()
{
  return NULL;
}

class mArrowButton *mControl::get_arrow2()
{
  return NULL;
}

class mBar *mControl::get_bar()
{
  return NULL;
}

void mControl::set_int(int n) { }
int  mControl::get_int() { return 0; }

void mControl::set_align(int n) { }
int  mControl::get_align() { return 0; }

void mControl::set_float(float n) { }
float  mControl::get_float() { return 0; }

char *mControl::get_text() { return "n/a"; }

void mControl::set_player(gsPlayer *n) { }

mSmallButton *mControl::get_sbutton(int n) { return NULL; }

