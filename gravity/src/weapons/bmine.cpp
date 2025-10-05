/*
 *   GRAVITY STRIKE -- weapons/bmine.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "bmine.h"
#include "../gsbullet.h"
#include "../gsmap.h"
#include "../gsclear.h"
#include "../gsplayer.h"
#include "../gshelper.h"
#include "../gseffect.h"
#include "../gsobject.h"
#include "../gsbase.h"
#include "../gssound.h"
#include "../gsenemy.h"
#include "../gsglob.h"
#include <math.h>

extern float      sinus[];
extern float      cosinus[];

bulletMine::bulletMine(gsPlayer *p,  int puser, int pupgrade, Image *spr)
{
  // bullet init
  type = W_MINE;
  by_player = p->get_sig();
  user      = puser;
  upgrade   = pupgrade;
  maxhitp   = 3;

  // stat
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
  orig_ttl = ttl = 60 * 100;

  // strength
  strength    = 15;
  hitp_hull   = 15;
  hitp_shield = 10;

  // temp vars
  teleport_time = 0;
  activation_radius = 25;
  destruction_radius = 50;
  visibility_radius = 100;
}

bulletMine::bulletMine(long psig, int enr, int px, int py, float pxspd, float pyspd, int pstrength, int pshield, int phull, float puser, int pupgrade, Image *spr)
{
  // bullet init
  type = W_MINE;
  by_player = psig;
  by_enemy  = enr;
  upgrade   = pupgrade;
  user      = puser;
  maxhitp   = 3;

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
  orig_ttl = ttl = 60 * 60;

  // strength
  strength    = pstrength;
  hitp_hull   = phull;
  hitp_shield = pshield;

  // temp vars
  teleport_time = 0;
  if (puser == 0) activation_radius = 25; else activation_radius = (int)puser;
  destruction_radius = 50;
  visibility_radius = 100;
}


bulletMine::~bulletMine()
{
}

void bulletMine::move()
{
  Bullet::move();

  xspd += dxspd;
  yspd += dyspd;

  // upgrade 1: show upgrade by more wobbling, then apply gravity
  if (upgrade == 1) 
  {
    if (xspd >= 1.2 || xspd <= -1.2) dxspd = -dxspd;
    if (yspd >= 1.2 || yspd <= -1.2) dyspd = -dyspd;
    
    apply_gravity();
  }
  else
  {
    if (xspd >= 0.4 || xspd <= -0.3) dxspd = -dxspd;
    if (yspd >= 0.6 || yspd <= -0.5) dyspd = -dyspd;
  }

  x += xspd;
  y -= yspd;

  if (ttl) ttl--;
  if (user > 1) user--;
}

void bulletMine::draw(gsMap *m)
{
  if (!invisible && m->is_on_screen(x, y, w, h))
//    if (getpixel(m->get_vscreen(), (int)x-m->get_map_x(), (int)y-m->get_map_y()) == 0)
    {
      draw_rle_sprite(m->get_vscreen(), image->get_rle_frame(curframe), (int)x-m->get_map_x(), (int)y-m->get_map_y());
      clearlist->add(m, x, y, w, h);
    }
}

int bulletMine::check_map_collisions(class gsMap *m)
{
  // collision with landscape?
  if (m->is_on_map(x, y, 1, 1))
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

int bulletMine::check_collisions()
{
  if (just_coll > 0) just_coll--;

  int n;
  
  invisible = 1; // mine is invisible by default

  // ttl expired?
  if ( ttl == 0 || user == 1 )
  {
    explode();

    for (n=0; n < globals->players; n++)
    {
      if ( !player[n]->get_dead() &&        // is player alive?
           by_player != player[n]->get_sig() ) // is the bullet shot by another player?
      {
        float d = distance( (int)x+w/2, (int)y+w/2,
                            (int)player[n]->get_x() + (int)player[n]->get_width()/2,
                            (int)player[n]->get_y() + (int)player[n]->get_height()/2 );

        if (d <= destruction_radius)
        {
          float damage_hull = strength - (int((float)hitp_hull * float(d / destruction_radius)));
          float damage_shield = strength - (int((float)hitp_shield * float(d / destruction_radius)));
          if (damage_hull+damage_shield > 0) player[n]->hit(by_player, by_enemy, damage_hull, damage_shield, type, xspd, yspd);
        }
      } // collision?
    } // for players

    return 1;
  }

  // collision with players?
  for (n=0; n < globals->players; n++)
  {
    float d = distance( (int)x+w/2, (int)y+w/2,
                        (int)player[n]->get_x() + (int)player[n]->get_width()/2,
                        (int)player[n]->get_y() + (int)player[n]->get_height()/2 );

    if (d < visibility_radius) invisible = 0;
    
    if ( !player[n]->get_dead() &&        // is player alive?
         by_player != player[n]->get_sig() &&  // is the bullet shot by another player?
         d < activation_radius )
    {
      // do a little explosion
      explode();

      if (d <= destruction_radius)
      {
        float damage_hull = strength - (int((float)hitp_hull * float(d / destruction_radius)));
        float damage_shield = strength - (int((float)hitp_shield * float(d / destruction_radius)));
        if (damage_hull+damage_shield > 0) player[n]->hit(by_player, by_enemy, damage_hull, damage_shield, type, xspd, yspd);
      }

      return 1;
    } // collision?
  } // for players

  // collision with objects?
  Object *temp_o;
  temp_o = objects->get_first_object();
  while (temp_o != NULL)
  {
    if (!temp_o->get_dont_update() && 
        collision( x, y, w, h,
                   temp_o->get_x(), temp_o->get_y(),
                   temp_o->get_width(), temp_o->get_height() ) )
    {
      user = 1;
      break;
    }

    temp_o = temp_o->get_next();
  }

  // collision with enemies?
  Enemy *temp_e;
  temp_e = enemies->get_first_enemy();
  while (temp_e != NULL)
  {
    float d = distance( (int)x+w/2, (int)y+w/2,
                        (int)temp_e->get_x() + (int)temp_e->get_width()/2,
                        (int)temp_e->get_y() + (int)temp_e->get_height()/2 );

    if (d < visibility_radius) invisible = 0;
    
    if (collision( x+w/2, y+h/2, w, h,
                   temp_e->get_x(), temp_e->get_y(),
                   temp_e->get_width(), temp_e->get_height() ) )
    {
      if (temp_e->hit(by_player, by_enemy, strength, x, y, xspd, yspd, W_MINE))
      {
        explode();
        return 1;
      }
    }

    temp_e = temp_e->get_next();
  }

  return 0;
}

void bulletMine::hit(float n)
{
  Bullet::hit(n);
  if (kill)
  {
//    explode();

    gsound->expl_medium1(x, y);
    effects->explosions->add(-1, expl_med1_01, 7, 4, 0, x-2, y-2, globals->gamedat);
    effects->pixels->add(8, (int)x+2, (int)y+2, 1, 1, 0, 0.1, 10, PIX_DEBRIS5, 10);
    effects->pixels->add(60, (int)x+3, (int)y+3, 1, 1, 0, 0, 10, PIX_WHITE, 10, 60*30, 50);
  }
}

void bulletMine::explode()
{
  gsound->expl_medium1(x, y);
  effects->explosions->add(-1, expl_med1_01, 7, 4, 0, x-2, y-2, globals->gamedat);
  effects->pixels->add(8, (int)x+2, (int)y+2, 1, 1, 0, 0.1, 10, PIX_DEBRIS5, 10);
  effects->pixels->add(60, (int)x+3, (int)y+3, 1, 1, 0, 0, 10, PIX_WHITE, 10, 60*30, 50);

  check_mine_radius();
}

// check for other mines & players in radius and destroy them
void bulletMine::check_mine_radius()
{
  Bullet *temp = effects->bullets->get_first(), *next_bullet;

  while (temp != NULL)
  {
    next_bullet = temp->get_next();

    if ( distance((int)x, (int)y, (int)temp->get_x(), (int)temp->get_y()) < destruction_radius )
      temp->set_user(10);

    temp = next_bullet;
  }
}


void bulletMine::apply_gravity()
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
                        &xs, &ys, 0, 1 );

      player[n]->set_xspd(xs);
      player[n]->set_yspd(ys);
    }
  }

  /* bullets
  Bullet *bul = effects->bullets->get_first(), *nextbul;
  while (bul)
  {
    nextbul = bul->get_next();

    xs = bul->get_xspd();
    ys = bul->get_yspd();

    apply_gravity_to( bul->get_x(), bul->get_y(),
                      &xs, &ys, 1 );

    bul->set_xspd(xs);
    bul->set_yspd(ys);

    bul = nextbul;
  }
  */
}

void bulletMine::apply_gravity_to(int px, int py, float *pxs, float *pys, int ptype, int str)
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
  
  int maxdist = 250;
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

