/*
 *   GRAVITY STRIKE -- menu/mplayer.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include <math.h>
#include "mplayer.h"
#include "../gsglob.h"
#include "../gsclear.h"
#include "../gshelper.h"
#include "mbullet.h"
#include "../gssound.h"
#include "../gserror.h"
#include "gsmd.h"

extern float sinus[];
extern float cosinus[];
extern mBullet *mbullets;

#ifndef PI
#define PI 3.14159
#endif

mPlayer::mPlayer(int num, int t, int px, int py)
{
  mship1 = (BITMAP *)globals->menudat[ship1_1+t*3].dat;
  mship2 = (BITMAP *)globals->menudat[ship1_2+t*3].dat;
  mship3 = (BITMAP *)globals->menudat[ship1_3+t*3].dat;

  frags = 0;
  sig = fxrand()%10000 + 500;
  x = px;
  y = py;
  xspd = yspd = xacc = yacc = head = 0;
  move_to_x = move_to_y = head_to = 0;
  to_dist = 1;
  target_angle = head = 0;

  turn_speed   = 3;
  acceleration = 0.037;
}

mPlayer::~mPlayer()
{
}

void mPlayer::move_to(int px, int py, int ptangle)
{
  if (x != px || y != py)
  {
    move_to_x = px;
    move_to_y = py;
    target_angle = ptangle;
    turn_to(px, py);
    to_dist = distance((int)x, (int)y, (int)move_to_x, (int)move_to_y);
    if (to_dist == 0) to_dist = 1;
  }
}

void mPlayer::turn_to(int px, int py)
{
  if (x != px || y != py)
  {
    head_to = (V_GetAngle(px, py, (int)x, (int)y)) / PI * 180;
    if (x >= px) head_to = 360-head_to;
  }
}

void mPlayer::move()
{
  float scale_x;
  float scale_y;
  float dist;

  dist = distance((int)x, (int)y, (int)move_to_x, (int)move_to_y);
  if (dist > to_dist/3) turn_to(move_to_x, move_to_y);
  else if (target_angle > 0) head_to = target_angle;

  if (head != head_to)
  {
    if (head-head_to < -180) head -= turn_speed;
    else if (head-head_to > -180 && head-head_to < 0) head += turn_speed;
    else if (head-head_to < 180 && head-head_to > 0) head -= turn_speed;
    else if (head-head_to > 180) head += turn_speed;

    if (head > 360) head = 1;
    if (head < 0) head = 359;
  }

  scale_x = sinus[ (int)head ];
  scale_y = cosinus[ (int)head ];

  xacc = scale_x * acceleration * (dist/to_dist);
  yacc = -scale_y * acceleration * (dist/to_dist);

  if (abs(int(yspd+yacc)) < 1.5)
    yspd += yacc;
  if (abs(int(xspd+xacc)) < 1.5)
    xspd += xacc;

  // autodec
  float autodec = 0.015;
  xspd -= xspd > 0 ? autodec : -autodec;
  yspd -= yspd > 0 ? autodec : -autodec;

  x += xspd;
  y += yspd;
}

void mPlayer::shoot()
{
  float xp = mship1->w/2; float yp = mship1->h/2;
  float xrot = sinus[ (int)head ];
  float yrot = cosinus[ (int)head ];

  float dx = xp + xrot*yp;
  float dy = yp - yrot*yp;

  float px = x + dx;
  float py = y + dy;

  float pxspd = xrot * globals->shoot_speed + xspd;
  float pyspd = yrot * globals->shoot_speed - yspd;

  mbullets->add(W_SINGLE, sig, 0, px, py, pxspd, pyspd, 2, 2, 2);
  gsound->menu_shoot();
}

void mPlayer::draw(BITMAP *v)
{
  rotate_sprite(v, mship2, (int)x, (int)y, ftofix(head / 1.4));
  clearlist->add(v, x-4, y-4, mship2->w+8, mship2->h+8);
/*
  char tmpstr[50];
  textout(globals->vscreen, font, ftoa(tmpstr, "head", head), 10, 400, makecol8(255,255,255));
  textout(globals->vscreen, font, ftoa(tmpstr, "head_to", head_to), 10, 410, makecol8(255,255,255));
  textout(globals->vscreen, font, ftoa(tmpstr, "xpos", x), 10, 420, makecol8(255,255,255));
  textout(globals->vscreen, font, ftoa(tmpstr, "ypos", y), 10, 430, makecol8(255,255,255));
  textout(globals->vscreen, font, ftoa(tmpstr, "m_to_x", move_to_x), 150, 400, makecol8(255,255,255));
  textout(globals->vscreen, font, ftoa(tmpstr, "m_to_y", move_to_y), 150, 410, makecol8(255,255,255));
  textout(globals->vscreen, font, ftoa(tmpstr, "xspd", xspd), 150, 420, makecol8(255,255,255));
  textout(globals->vscreen, font, ftoa(tmpstr, "fps", globals->fpst), 150, 430, makecol8(255,255,255));

  clearlist->add(v, 0, 400, 300, 50);
*/
}

void mPlayer::set_position(int px, int py)
{
  x = px;
  y = py;
  xspd = yspd = xacc = yacc = head = 0;
  move_to_x = move_to_y = head_to = 0;
  to_dist = 1;
  target_angle = head = 0;
}

