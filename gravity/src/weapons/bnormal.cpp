/*
 *   GRAVITY STRIKE -- weapons/bnormal.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "bnormal.h"
#include "../gsbase.h"
#include "../gsobject.h"
#include "../gsmap.h"
#include "../gsclear.h"
#include "../gsplayer.h"
#include "../gshelper.h"
#include "../gseffect.h"
#include "../gssound.h"
#include "../gsenemy.h"
#include "../sin.h"
#include "../cos.h"
#include "../gsglob.h"
#include "../gserror.h"
#include <math.h>

extern gsClear    *clearlist;
extern gsGlobals  *globals;
extern gsPlayer   *player[];
extern gsEffect   *effects;

// spawned with player structure
bulletNormal::bulletNormal(gsPlayer *p,  int pmode, int pupgrade)
{
  // bullet init
  if (pmode) type = W_DOUBLE;
  else type = W_SINGLE;
  by_player = p->get_sig();
  mode      = pmode;
  upgrade   = pupgrade;
  my_by_player = get_player_nr_by_sig(by_player);

  // stat
  if (mode == 0 || pmode == 1) // only for first activation
  {
    if (type == W_SINGLE)
    {
      if (upgrade == 0) p->inc_bullets_shot(1);
      else if (upgrade == 1) p->inc_bullets_shot(2);
    }  
    else if (type == W_DOUBLE)
    {
      if (upgrade == 0) p->inc_bullets_shot(2);
      else if (upgrade == 1) p->inc_bullets_shot(3);
    }  
  }  

  // image
  w = h = 1; 

  // position & speed
  float xp = p->get_width()/2; float yp = p->get_height()/2;
  int a = (int)p->get_head();
  float xrot = sinus[ a ];
  float yrot = cosinus[ a ];

  float dx = xp + xrot*yp;  // middle shot
  float dy = yp - yrot*yp;

  mode = 0;
  if (pmode == 1) { mode = 1; dx = xp - yrot*4; dy = yp - xrot*4; }        // left?
  else if (pmode == 2) { mode = 1; dx = xp + yrot*4; dy = yp + xrot*4; }   // right?

  x = p->get_x() + dx;
  y = p->get_y() + dy;

  xspd = xrot * globals->shoot_speed + p->get_xspd();
  yspd = yrot * globals->shoot_speed - p->get_yspd();

  float xspd2, yspd2;
  xspd2 = -xrot * globals->shoot_speed + p->get_xspd();
  yspd2 = -yrot * globals->shoot_speed - p->get_yspd();

  // max range
  min_x = (int)p->get_x() - globals->playscreen_width * 2;
  max_x = (int)p->get_x() + globals->playscreen_width * 2;
  min_y = (int)p->get_y() - globals->playscreen_height * 2;
  max_y = (int)p->get_y() + globals->playscreen_height * 2;

  // time to live
  orig_ttl = ttl = 60 * 4;

  // strength
  if (type == W_SINGLE)
  {
    strength    = 1;
    hitp_hull   = 1;
    hitp_shield = 1;
  }
  else
  {
    strength    = 2;
    hitp_hull   = 2;
    hitp_shield = 1;
  }

  // temp vars
  teleport_time = 0;
  dxspd = dyspd = dxspda = dyspda = 0;
  
  // upgrades
  if (upgrade == 1)
  {
    if (type == W_SINGLE) // single blaster: backshot
    {
      effects->bullets->add(W_SINGLE, p->get_sig(), 0, x, y, xspd2, yspd2, 1, 1, 1, 0, 0);
    }
    else if (pmode == 2) // double blaster: three shot
    {
      float dx2 = xp + xrot*yp;
      float dy2 = yp - yrot*yp;
      effects->bullets->add(W_SINGLE, p->get_sig(), 0, p->get_x()+dx2, p->get_y()+dy2, xspd, yspd, 2, 2, 1, 0, 0);

      weaponslot *tmpslot = p->get_weapon_slot(p->get_active_weapon());
      tmpslot->bullets--;
      p->set_weapon_slot(p->get_active_weapon(), tmpslot);
      
      statistics *tmpstat = p->get_statistics();
      tmpstat->bullets_shot++;
      p->set_statistics(tmpstat);
    }
  }
  
  // player recoil
  if (mode == 0 || pmode == 1)
  {
    if (p->get_recoil())
    {
      p->set_xspd(p->get_xspd() - (xrot*0.01));
      p->set_yspd(p->get_yspd() + (yrot*0.01));  
    }  
  }
  
  
}

// spawned with coordinates and speeds
bulletNormal::bulletNormal(long psig, int enr, int px, int py, float pxspd, float pyspd, int pstrength, int pshield, int phull, float puser, int pupgrade)
{
 // bullet init
  type = W_SINGLE;
  by_player = psig;
  by_enemy  = enr;
  upgrade   = pupgrade;
  user      = puser;

  // image
  w = h = 1;
  
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
  orig_ttl = ttl = 60 * 4;

  // strength
  strength    = pstrength;
  hitp_hull   = phull;
  hitp_shield = pshield;

  // temp vars
  teleport_time = 0;
  dxspd = dyspd = 0;
  my_by_player = get_player_nr_by_sig(by_player);
}

bulletNormal::~bulletNormal()
{
}

void bulletNormal::move()
{
  Bullet::move();
  
  x += xspd;
  y -= yspd;

  ttl--;

  // draw a little trail
  if (!invisible)
    effects->pixels->add(1, x-1+gsrand()%2, y-1+gsrand()%2, 1, 1,
                         0, -globals->particle_gravity, 0,
                         globals->tbl_white, 9, 100, 2, 10, 10);
}

void bulletNormal::move(int no_trail)
{
  Bullet::move();
  
  x += xspd;
  y -= yspd;

  if (ttl) ttl--;
}

void bulletNormal::draw(gsMap *m)
{
  if (!invisible && m->is_on_screen(x, y, 1, 1))
  {
    if (getpixel(m->get_vscreen(), (int)x-m->get_map_x(), (int)y-m->get_map_y()) == 0)
    {
      int col = (my_by_player == 0) ? globals->col_white : globals->col_yellow;
      putpixel(m->get_vscreen(), (int)x - m->get_map_x(), (int)y - m->get_map_y(), col);
      clearlist->add(m, x, y, 1, 1);
    }
  }
}

void bulletNormal::draw(BITMAP *m)
{
  if (!invisible && getpixel(m, (int)x, (int)y) == 0)
  {
    int col = (my_by_player == 0) ? globals->col_white : globals->col_yellow;
    putpixel(m, (int)x, (int)y, col);
    clearlist->add(m, x, y, 1, 1);
  }
}


int bulletNormal::check_collisions()
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
        player[n]->set_xspd( player[n]->get_xspd() + xspd/50 );
        player[n]->set_yspd( player[n]->get_yspd() - yspd/50 );
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
               just_coll = 2;
             }
             return 0; // don't delete bullet
             break;

        case SPOBJ_TELEPORT1 : case SPOBJ_TELEPORT2 :
             if (teleport_time == 0)
             {
               if (collision( x, y, 1, 1,
                              temp_o->get_x()+1, temp_o->get_y()+1,
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

int bulletNormal::check_map_collisions(gsMap *m)
{
  // collision with landscape?
  if (m->is_on_map(x, y, w, h))
  {
    if  (m->is_pixel(x, y))
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

