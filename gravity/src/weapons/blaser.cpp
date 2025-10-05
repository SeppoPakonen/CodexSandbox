/*
 *   GRAVITY STRIKE -- weapons/blaser.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */
 
#include "blaser.h"
#include "../gsbase.h"
#include "../gsobject.h"
#include "../gsmap.h"
#include "../gsclear.h"
#include "../gsplayer.h"
#include "../gshelper.h"
#include "../gseffect.h"
#include "../gssound.h"
#include "../gsenemy.h"
#include "../gserror.h"
#include "../gsglob.h"
#include <math.h>

extern float sinus[];
extern float cosinus[];

extern gsClear    *clearlist;
extern gsGlobals  *globals;
extern gsPlayer   *player[];
extern gsEffect   *effects;

// spawned with player structure
bulletLaser::bulletLaser(gsPlayer *p,  int puser, int pupgrade, Image *spr)
{
  // bullet init
  type = W_LASER;
  by_player = p->get_sig();
  user      = puser;
  upgrade   = pupgrade;

  // stat
  if (upgrade == 0) p->inc_bullets_shot(1);
  else if (upgrade == 1) p->inc_bullets_shot(2);

  // image
  image = spr;
  w = image->get_width();
  h = image->get_height();
  maxframet = 8;
  use_backbuf = 1;
  int mw = w; if (h > w) mw = h;
  mw+=2;
  backbuf = create_bitmap(mw, mw);
  clear(backbuf);

  // position & speed
  float xp = p->get_width()/2; float yp = p->get_height()/2;
  int a = (int)p->get_head();
  float xrot = sinus[ a ];
  float yrot = cosinus[ a ];

  float dx = xp + xrot*yp;  // middle shot
  float dy = yp - yrot*yp;

  x = p->get_x() + dx - w/2;
  y = p->get_y() + dy - h/2;

  xspd = xrot * globals->shoot_speed*2 + p->get_xspd();
  yspd = yrot * globals->shoot_speed*2 - p->get_yspd();

  // upgrade 1: double shot
  if (upgrade == 1)
  {
    float dx2, dy2, xspd2, yspd2;
    dx = xp - yrot*4; dy = yp - xrot*4;
    dx2 = xp + yrot*4; dy2 = yp + xrot*4;

    // new first bullet
    x = p->get_x() + dx - w/2;
    y = p->get_y() + dy - h/2;

    a = (int)p->get_head() + 2;
    if (a > 360) a = 360-a;
    xrot = sinus[ a ]; yrot = cosinus[ a ];
    xspd = xrot * globals->shoot_speed*2 + p->get_xspd();
    yspd = yrot * globals->shoot_speed*2 - p->get_yspd();
    
    // new second bullet
    a = (int)p->get_head() - 2;
    if (a < 0) a = 360+a;
    xrot = sinus[ a ]; yrot = cosinus[ a ];
    xspd2 = xrot * globals->shoot_speed*2 + p->get_xspd();
    yspd2 = yrot * globals->shoot_speed*2 - p->get_yspd();

    effects->bullets->add(W_LASER, p->get_sig(), 0, 
                          p->get_x()+dx2-w/2, p->get_y()+dy2-h/2, xspd2, yspd2,
                          2, 3, 0, 0, 0);

    weaponslot *tmpslot = p->get_weapon_slot(p->get_active_weapon());
    tmpslot->bullets--;
    p->set_weapon_slot(p->get_active_weapon(), tmpslot);
      
    statistics *tmpstat = p->get_statistics();
    tmpstat->bullets_shot++;
    p->set_statistics(tmpstat);
  }

  // max range
  min_x = (int)p->get_x() - globals->playscreen_width * 2;
  max_x = (int)p->get_x() + globals->playscreen_width * 2;
  min_y = (int)p->get_y() - globals->playscreen_height * 2;
  max_y = (int)p->get_y() + globals->playscreen_height * 2;

  // time to live
  orig_ttl = ttl = 60 * 2;

  // strength
  strength    = 2;
  hitp_hull   = 0;
  hitp_shield = 3;

  // temp vars
  teleport_time = 0;
}

// spawned with coordinates and speeds
bulletLaser::bulletLaser(long psig, int enr, int px, int py, float pxspd, float pyspd, int pstrength, int pshield, int phull, float puser, int pupgrade, Image *spr)
{
  // bullet init
  type = W_LASER;
  by_player = psig;
  by_enemy  = enr;
  upgrade   = pupgrade;
  user      = puser;

  // image
  image = spr;
  w = image->get_width();
  h = image->get_height();
  maxframet = 8;
  use_backbuf = 1;
  int mw = w; if (h > w) mw = h;
  mw+=2;
  backbuf = create_bitmap(mw, mw);
  clear(backbuf);
  
  // position & speed
  x = px;
  y = py;

  xspd = pxspd;
  yspd = pyspd;

  // max range
  min_x = (int)x - globals->playscreen_width * 2;
  max_x = (int)x + globals->playscreen_width * 2;
  min_y = (int)y - globals->playscreen_height * 2;
  max_y = (int)y + globals->playscreen_height * 2;

  // time to live
  orig_ttl = ttl = 60 * 2;

  // strength
  strength    = pstrength;
  hitp_hull   = phull;
  hitp_shield = pshield;

  // temp vars
  teleport_time = 0;
}

bulletLaser::~bulletLaser()
{
  if (backbuf) destroy_bitmap(backbuf);
}

void bulletLaser::move()
{
  Bullet::move();

  x += xspd;
  y -= yspd;

  ttl--;
}

void bulletLaser::draw(gsMap *m)
{
  float head = 0;
  
  // calculate heading from xspd/yspd
  float spd = sqrt(xspd*xspd + yspd*yspd);
  head = acos(xspd/spd) * 180/3.14159;
  if (yspd > 0) head = -head;
  
  if (!invisible && m->is_on_screen(x, y, w, h))
    if (getpixel(m->get_vscreen(), (int)x-m->get_map_x(), (int)y-m->get_map_y()) == 0)
    {
      rotate_sprite(m->get_vscreen(), image->get_bitmap_frame(curframe), (int)x-m->get_map_x(), (int)y-m->get_map_y(), ftofix(head / 1.40625));
    }
}


int bulletLaser::check_collisions()
{
  float newx1, newy1, newx2, newy2;
  if (just_coll > 0) just_coll--;

  // min_x/y or max_x/y reached or ttl expired?
  if ( ttl == 0 ||
       x < min_x || x > max_x ||
       y < min_y || y > max_y )
  {
    return 1;
  }

  // collision with players?
  for (int n=0; n < globals->players; n++)
  {
    if ( !player[n]->get_dead() &&        // is player alive?
         by_player != player[n]->get_sig() && // is the bullet shot by another player?
         collision( x, y, w, h, // so... is there a collision?
                    player[n]->get_x(), player[n]->get_y(),
                    player[n]->get_width(), player[n]->get_height(),
                    xspd, -yspd, player[n]->get_xspd(), player[n]->get_yspd(),
                    &newx1, &newy1, &newx2, &newy2, 1 ) )
    {
      // adjust position
      x = newx1; y = newy1;
      
      player[n]->hit(by_player, by_enemy, hitp_hull, hitp_shield, type, xspd, yspd);

      if (!player[n]->get_land() &&
           player[n]->get_time_alive() > globals->player_indestructible)
      {
        player[n]->set_xspd( player[n]->get_xspd() + xspd/70 );
        player[n]->set_yspd( player[n]->get_yspd() - yspd/70 );
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
        collision( x-1, y-1, w+2, h+2,
                   temp_o->get_x(), temp_o->get_y(),
                   temp_o->get_width(), temp_o->get_height() ) )
    {
      switch (temp_o->get_type())
      {
         case OBJ_DOORH : case OBJ_DOORV : case OBJ_DOORH2 : case OBJ_DOORV2 :
              if (temp_o->get_maxframe() != 0) return 1;
              break;

         default :
              if (temp_o->add_hitpoints(by_player, strength, type, x, y, xspd, yspd))
                return 1;

              float tx, ty;
              if ( yspd < 0 ) ty = -0.6;
              else ty = 0.4;        
              tx = ( (gsrand() % 4) - 2 ) / 5;        
              float pspd = abs(int(sqrt(yspd*yspd + xspd*xspd)));
              effects->pixels->add( (gsrand() % ((int)(pspd*2)+1)) + 1,
                                     x - xspd/4, y + ty*4, 1, 1,
                                     tx, ty, 400,
                                     globals->tbl_white, 10, 60*10, 50,
                                     600, 600);

              return 1;
              break;
      }

    }

    temp_o = temp_o->get_next();
  }

  // collision with special objects?
  temp_o = objects->get_first_spobject();
  int bumped = 0;
  if (!just_coll)
  while (temp_o != NULL)
  {
    if (temp_o->get_active() &&
        collision( x, y, w, h,
                   temp_o->get_x(), temp_o->get_y(),
                   temp_o->get_width(), temp_o->get_height(),
                   xspd, -yspd, temp_o->get_xspd(), temp_o->get_yspd(),
                   &newx1, &newy1, &newx2, &newy2, 1 ) )
    {
      // hit spobject
      if (temp_o->add_hitpoints(by_player, 0, type, x, y, xspd, yspd))
        return 1;

      switch (temp_o->get_type())
      {
        case SPOBJ_BUMPU : case SPOBJ_BUMPD :
             // adjust position
             x = newx1; y = newy1;
             if (!bumped)
             {
               temp_o->set_curframe(1);
               yspd = -yspd;
               bumped = 1;
               just_coll = 2; // double-bump-prevention-hack (consecutive frames)
             }
             return 0; // don't delete bullet
             break;
        case SPOBJ_BUMPL : case SPOBJ_BUMPR :
             // adjust position
             x = newx1; y = newy1;
             if (!bumped)
             {
               temp_o->set_curframe(1);
               xspd = -xspd;
               bumped = 1;
               just_coll = 2; // double-bump-prevention-hack (consecutive frames)              
             }
             return 0; // don't delete bullet
             break;

        case SPOBJ_TELEPORT1 : case SPOBJ_TELEPORT2 :
             if (teleport_time == 0)
             {
               if (collision( (int)x, (int)y,
                              1, 1,
                              (int)temp_o->get_x()+1, (int)temp_o->get_y()+1,
                              temp_o->get_width()-2, temp_o->get_height()-2) )
               {
                 Object *temp = objects->get_spobject(temp_o->get_user());
                 effects->explosions->add(-1, effect_beam2_01, 10, 1, 0, x, y, globals->gamedat);
                 set_x(temp->get_x()+temp->get_width()/2);
                 set_y(temp->get_y()+temp->get_height()/2);
                 reset_min_max();
                 teleport_time = 15;
                 effects->explosions->add(-1, effect_beam2_01, 10, 1, 0, x, y, globals->gamedat);
                 gsound->beam_bullet(x, y);
               }
             }
             return 0; // don't delete bullet
             break;

      } // type of special object?
    } // collision?

    temp_o = temp_o->get_next();
  }

  // collision with enemies?
  Enemy *temp_e;
  temp_e = enemies->get_first_enemy();
  while (temp_e != NULL)
  {
    if (collision( x, y, w, h,
                   temp_e->get_x(), temp_e->get_y(),
                   temp_e->get_width(), temp_e->get_height(),
                   xspd, -yspd, temp_e->get_xspd(), temp_e->get_yspd(),
                   &newx1, &newy1, &newx2, &newy2, 1 ) )
    {
      // adjust position
      x = newx1; y = newy1;

      if (temp_e->hit(by_player, by_enemy, strength, x, y, xspd, yspd, W_SINGLE))
        return 1;
    }

    temp_e = temp_e->get_next();
  }

  // collision with probes, mines, rockets?
  Bullet *temp_bu;
  temp_bu = effects->bullets->get_first();
  while (temp_bu && temp_bu->get_by_player() != by_player &&
                    (temp_bu->get_type() == W_ROCKET || 
                     temp_bu->get_type() == W_PROBE ||
                     temp_bu->get_type() == W_MINE ||
                     temp_bu->get_type() == W_BUMPER) )
  {
    if (collision( x, y, w, h,
                   temp_bu->get_x(), temp_bu->get_y(),
                   temp_bu->get_width(), temp_bu->get_height(),
                   xspd, -yspd, temp_bu->get_xspd(), temp_bu->get_yspd(),
                   &newx1, &newy1, &newx2, &newy2, 1 ) )
    {
      // adjust position
      x = newx1; y = newy1;
      temp_bu->set_x(newx2);
      temp_bu->set_y(newy2);
      
      temp_bu->hit(1);
      return 1;
    }
    
    temp_bu = temp_bu->get_next();
  }


  return 0;
}

int bulletLaser::check_map_collisions(gsMap *m)
{
  // collision with landscape?
  if (m->is_on_map(x, y, w, h))
  {
    if (m->is_pixel(x+w/2, y+h/2))
    {
      // add pixels when bullet hits the wall
      float tx, ty;

      if ( yspd < 0 ) ty = -0.6;
      else ty = 0.4;

      tx = ( (gsrand() % 4) - 2 ) / 5;

      float pspd = abs(int(sqrt(yspd*yspd + xspd*xspd)));

      int *coltbl = globals->tbl_green;

      if (!strcmp(m->get_style(), "gfw1.fmp"))
        coltbl = globals->tbl_green;
      else
        coltbl = globals->tbl_white;

      effects->pixels->add( (gsrand() % ((int)(pspd*2)+1)) + 1,
                             x - xspd/4, y + ty*4, 1, 1,
                             tx, ty, 400,
                             coltbl, 10, 60*10, 50,
                             600, 600);

      return 1;
    } // if getpixel
  }

  return 0;
}

