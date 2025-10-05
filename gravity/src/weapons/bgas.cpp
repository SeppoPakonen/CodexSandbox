/*
 *   GRAVITY STRIKE -- weapons/bgas.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "bgas.h"
#include "../gsbullet.h"
#include "../gsmap.h"
#include "../gsclear.h"
#include "../gsplayer.h"
#include "../gshelper.h"
#include "../gseffect.h"
#include "../gsobject.h"
#include "../gsbase.h"
#include "../gssound.h"
#include "../gsglob.h"
#include <math.h>

extern float      sinus[];
extern float      cosinus[];

bulletGas::bulletGas(gsPlayer *p,  int puser, int pupgrade, Image *spr)
{
  // bullet init
  type = W_SHRINKER;
  by_player = p->get_sig();
  user      = puser;
  upgrade   = pupgrade;

  // stats
  p->inc_bullets_shot(1);

  // image
  image = spr;
  w = image->get_width();
  h = image->get_height();
  maxframet = 30;

  // position & speed
  float xp = p->get_width()/2; float yp = p->get_height()/2;

  x = p->get_x()+xp;
  y = p->get_y()+yp;

  xspd = yspd = 0;
  dxspd = dyspd = 0.1;

  // max range
  min_x = (int)p->get_x() - globals->playscreen_width * 4;
  max_x = (int)p->get_x() + globals->playscreen_width * 4;
  min_y = (int)p->get_y() - globals->playscreen_height * 4;
  max_y = (int)p->get_y() + globals->playscreen_height * 4;

  // time to live
  orig_ttl = ttl = 0;
  max_ttl = 60 * 15;

  // strength
  strength    = 0;
  hitp_hull   = 0;
  hitp_shield = 0;
  timeleft    = 60 * 2; // time till explosion
  destruction_radius = 70;

  // temp vars
  teleport_time = 0;
}

bulletGas::bulletGas(long psig, int enr, int px, int py, float pxspd, float pyspd, int pstrength, int pshield, int phull, float puser, int pupgrade, Image *spr)
{
  // bullet init
  type = W_GAS;
  by_player = psig;
  by_enemy  = enr;
  upgrade   = pupgrade;
  user      = puser;

  // image
  image = spr;
  w = image->get_width();
  h = image->get_height();
  maxframet = 30;
  
  // position & speed
  x = px;
  y = py;

  xspd = pxspd;
  yspd = pyspd;
  dxspd = dyspd = 0.1;

  // max range
  min_x = (int)x - globals->playscreen_width * 4;
  max_x = (int)x + globals->playscreen_width * 4;
  min_y = (int)y - globals->playscreen_height * 4;
  max_y = (int)y + globals->playscreen_height * 4;

  // time to live
  orig_ttl = ttl = 0;
  max_ttl = 60 * 15;

  // strength
  strength    = pstrength;
  hitp_hull   = phull;
  hitp_shield = pshield;

  // temp vars
  teleport_time = 0;
  timeleft = 60 * 2; // time till explosion
  destruction_radius = 70;
}


bulletGas::~bulletGas()
{
}

void bulletGas::move()
{
  Bullet::move();

  if (timeleft)
  {
    xspd += dxspd;
    yspd += dyspd;

    if (upgrade == 1)
    {  
      if (xspd >= 0.8 || xspd <= -0.6) dxspd = -dxspd;
      if (yspd >= 1.2 || yspd <= -1.0) dyspd = -dyspd;
    }
    else
    {
      if (xspd >= 0.4 || xspd <= -0.3) dxspd = -dxspd;
      if (yspd >= 0.6 || yspd <= -0.5) dyspd = -dyspd;
    }
  
    x += xspd;
    y -= yspd;
  }

  ttl++;
  if (user > 1) user--;
  
  // upgrade 1: apply gravity
  if (upgrade == 1 && timeleft <= 1)
  {
    int grav = max_ttl - int((float)ttl * float(ttl / max_ttl));  
    grav = int(grav / 60); grav = int(grav * 4.5);
    apply_gravity(3);
  }

  if (timeleft == 1) explode();
  if (timeleft > 0) timeleft--;
}

void bulletGas::draw(gsMap *m)
{
  if (!invisible && timeleft && m->is_on_screen(x, y, w, h))
  {
    draw_rle_sprite(m->get_vscreen(), image->get_rle_frame(curframe), (int)x-m->get_map_x(), (int)y-m->get_map_y());
    clearlist->add(m, x, y, w, h);
  }
}

int bulletGas::check_map_collisions(class gsMap *m)
{
  // collision with landscape?
  if (timeleft && m->is_on_map(x, y, 1, 1))
  {
    if  (m->is_pixel((int)x-10, (int)y) ||
         m->is_pixel((int)x+10, (int)y) ||
         m->is_pixel((int)x, (int)y-10) ||
         m->is_pixel((int)x, (int)y+10))
    {
      explode();

      return 1;
    } // if getpixel
  }

  return 0;
}

int bulletGas::check_collisions()
{
  int n;

  // ttl expired?
  if ( ttl >= max_ttl || user == 1 )
  {
    return 1;
  }

  // collision with players?
  for (n=0; n < globals->players; n++)
  {
    float d = distance( (int)x+w/2, (int)y+w/2,
                        (int)player[n]->get_x() + (int)player[n]->get_width()/2,
                        (int)player[n]->get_y() + (int)player[n]->get_height()/2 );

    if ( !player[n]->get_dead() &&        // is player alive?
         by_player != player[n]->get_sig() &&  // is the bullet shot by another player?
         d <= destruction_radius )
    {
      if (timeleft > 1) timeleft = 1;

      if (d <= destruction_radius)
      {
        int damage = max_ttl - int((float)ttl * float(ttl / max_ttl));
        if (damage > 0 && player[n]->get_delirium() == 0)
        {
          player[n]->hit(by_player, by_enemy, 0, 0, type, xspd, yspd);
          player[n]->set_delirium(damage/2);
        }  
      }

    } // collision?
  } // for players

  // collision with objects?
  Object *temp_o;
  temp_o = objects->get_first_object();
  while (temp_o != NULL)
  {   
    if (!temp_o->get_dont_update() && 
        collision( x, y, 5, 5,
                   temp_o->get_x(), temp_o->get_y(),
                   temp_o->get_width(), temp_o->get_height() ) )
    {
      if (timeleft > 1) timeleft = 1;
      break;
    }

    temp_o = temp_o->get_next();
  }

  return 0;
}

void bulletGas::explode()
{
  gsound->expl_medium1(x, y);
  if (!invisible)
  {
    effects->explosions->add(-1, expl_med1_01, 7, 4, 0, x-5, y-5, globals->gamedat);

    for (int n=0; n < 50; n++)
    effects->pixels->add(6, x+4, y+4, 2, 2, 0, -(globals->particle_gravity*globals->gravity_percent), gsrand()%10+10,
                         globals->tbl_yellow, 10,
                         35*60, gsrand()%350+30, 1500, 1500, 0, NULL, NULL, 0.95);
  
    effects->pixels->add(100, x+4, y+4, 2, 2, 0, -(globals->particle_gravity*globals->gravity_percent), gsrand()%10+10,
                         globals->tbl_red, 10,
                         35*60, gsrand()%350+30, 1500, 1500, 0, NULL, NULL, 0.95);
  }
}


void bulletGas::apply_gravity(int st)
{
  float xs, ys;

  // players
  for (int n=0; n < globals->players; n++)
  {
    if (player[n]->get_sig() != by_player && !player[n]->get_land())
    {
      xs = player[n]->get_xspd();
      ys = player[n]->get_yspd();
      
      apply_gravity_to( player[n]->get_x()+5, player[n]->get_y()+5,
                        &xs, &ys, 0, st );

      player[n]->set_xspd(xs);
      player[n]->set_yspd(ys);
    }
  }
}

void bulletGas::apply_gravity_to(int px, int py, float *pxs, float *pys, int ptype, int str)
{
  float angle,scale_x,scale_y,xrot,yrot;
  int gx,gy;
  float s;
  float pi = 3.1415926;

  gx = (int)x;
  gy = (int)y;

  angle = V_GetAngle(px,py,gx,gy);
  xrot = sinus[int(angle*180.0/pi)];
  yrot = cosinus[int(angle*180.0/pi)];

  if (px >= gx)
    scale_x = -xrot;
  else
    scale_x = xrot;

  scale_y = yrot;

  if (str > 100) str = 100; if (str <= 0) str = 1;
  s = str/100.0;
  
  int maxdist = 300;
  int dist = (int)distance((int)gx, (int)gy, (int)px, (int)py);
  if (dist > maxdist) dist = maxdist;
  
  float d = float(float(dist)/float(maxdist));
  float a = 1-d;
  s = a * s;
  
  if (s != 0)
  {
    *pxs += scale_x*s;
    if (ptype == 0)
      *pys += scale_y*s;
    else
      *pys -= scale_y*s;
  }
}

