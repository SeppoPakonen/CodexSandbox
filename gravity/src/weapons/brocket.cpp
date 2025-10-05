/*
 *   GRAVITY STRIKE -- weapons/brocket.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "brocket.h"
#include "../gsmap.h"
#include "../gsclear.h"
#include "../gsplayer.h"
#include "../gshelper.h"
#include "../gseffect.h"
#include "../gsobject.h"
#include "../gsbase.h"
#include "../gssound.h"
#include "../gsenemy.h"
#include "../gserror.h"
#include "../gsnetw.h"
#include "../gsglob.h"
#include <math.h>

extern float sinus[];
extern float cosinus[];

// spawned with player structure
bulletRocket::bulletRocket(gsPlayer *p,  int puser, int pupgrade, Image *spr)
{
  // bullet init
  type = W_ROCKET;
  by_player = p->get_sig();
  user      = puser;
  upgrade   = pupgrade;
  maxhitp   = 2;

  // stat
  p->inc_bullets_shot(1);

  // image
  image = spr;
  w = image->get_width();
  h = image->get_height();
  maxframet = 8;

  // position & speed
  float xp = p->get_width()/2; float yp = p->get_height()/2;
  float xrot = sinus[ (int)p->get_head() ];
  float yrot = cosinus[ (int)p->get_head() ];

  float dx = xp + xrot*yp;
  float dy = yp - yrot*yp;

  x = p->get_x() + dx - 2;
  y = p->get_y() + dy;

  xspd = xrot * globals->shoot_speed_bomb/2 + p->get_xspd();
  yspd = yrot * globals->shoot_speed_bomb/2 - p->get_yspd();

  // max range
  min_x = (int)p->get_x() - globals->playscreen_width * 4;
  max_x = (int)p->get_x() + globals->playscreen_width * 4;
  min_y = (int)p->get_y() - globals->playscreen_height * 4;
  max_y = (int)p->get_y() + globals->playscreen_height * 4;

  // time to live
  orig_ttl = ttl = 60 * 10;

  // strength
  strength    = 6;
  hitp_hull   = 3;
  hitp_shield = 6;

  // temp vars
  teleport_time = 0;
  user = 50; // time until target locking
  turn_speed = orig_turn_speed = p->get_rocket_turn_speed();
  time_since_fire = 0;
  shoot_angle = 0;
  target = search_target(x, y, by_player);
  
  // player recoil
  if (p->get_recoil())
  {
    p->set_xspd(p->get_xspd() - (xrot*0.7));
    p->set_yspd(p->get_yspd() + (yrot*0.7));
  }
}

bulletRocket::bulletRocket(long psig, int enr, int px, int py, float pxspd, float pyspd, int pstrength, int pshield, int phull, float puser, int pupgrade, Image *spr)
{
  // bullet init
  type = W_ROCKET;
  by_player = psig;
  by_enemy  = enr;
  upgrade   = pupgrade;
  user      = puser;
  maxhitp   = 2;

  // image
  image = spr;
  w = image->get_width();
  h = image->get_height();
  maxframet = 8;
  
  // position & speed
  x = px;
  y = py;

  xspd = pxspd;
  yspd = pyspd;

  // max range
  min_x = (int)x - globals->playscreen_width * 4;
  max_x = (int)x + globals->playscreen_width * 4;
  min_y = (int)y - globals->playscreen_height * 4;
  max_y = (int)y + globals->playscreen_height * 4;

  // time to live
  orig_ttl = ttl = 60 * 10;

  // strength
  strength    = pstrength;
  hitp_hull   = phull;
  hitp_shield = pshield;

  // temp vars
  teleport_time = 0;
  if (puser == 0) turn_speed = 0.05; else turn_speed = puser;
  orig_turn_speed = turn_speed;
  time_since_fire = 0;
  user = 50;
  shoot_angle = 0;
  target = 0;
}

bulletRocket::~bulletRocket()
{
}

int bulletRocket::search_target(int px, int py, long psig)
{
  int bx, by;
  long ret=0;
  float a=50000, b=0;

  for (int n=0; n < globals->players; n++)
  {
    if (player[n]->get_sig() != psig)
    {
      bx = int(player[n]->get_x() + player[n]->get_width()/2);
      by = int(player[n]->get_y() + player[n]->get_height()/2);
      b = distance( px, py, bx, by );
      if (b <= a) { a = b; ret = n/*player[n]->get_sig()*/; }
    }
  }

  if (enemies->get_count() > 0)
  {
    Enemy *temp = enemies->get_first_enemy();

    while (temp != NULL)
    {
      if (temp->get_sig() != psig)
      {
        bx = int(temp->get_x() + temp->get_width()/2);
        by = int(temp->get_y() + temp->get_height()/2);
        b = distance( px, py, bx, by );
        if (b <= a) { a = b; ret = -temp->get_nr(); }
      }

      temp = temp->get_next();;
    } // while temp != NULL
  }

  return ret;
}

void bulletRocket::move()
{
  Bullet::move();

  time_since_fire++;
  Enemy *tpl_en = NULL;
  gsPlayer *tpl_pl = NULL;

  if (target >= 0)
  {
    tpl_pl = player[target]; //get_player_by_sig(target);
    if (!tpl_pl)
    {
      target = search_target(x, y, by_player);
      tpl_pl = player[target]; //get_player_by_sig(target);
    }
  }
  else if (target < 0)
  {
    tpl_en = enemies->get_enemy(abs(target));
    if (!tpl_en)
    {
      target = search_target(x, y, by_player);
      tpl_en = enemies->get_enemy(abs(target));
    }
  }

  int okay = 1;
  if (tpl_pl && tpl_pl->get_dead()) okay = 0;

  // lock target when rocket is ready
  if (time_since_fire == (int)user)
  {
    target = search_target(x, y, by_player);
  }  

  if ( (tpl_pl || tpl_en) &&
       time_since_fire > (int)user && okay)
  {
     int enx=0, eny=0;
     if (tpl_en)
     {
       enx = fast_ftol(tpl_en->get_x() + tpl_en->get_width()/2);
       eny = fast_ftol(tpl_en->get_y() + tpl_en->get_height()/2);
     }
     else if (tpl_pl)
     {
       enx = fast_ftol(tpl_pl->get_x() + tpl_pl->get_width()/2);
       eny = fast_ftol(tpl_pl->get_y() + tpl_pl->get_height()/2);
     }

     float rot = V_GetAngle(enx,eny,(int)x,(int)y);
     float xrot = sin(rot);
     float yrot = cos(rot);

     float scale_x = 0;

     // target cloaked? if so, decrease turn speed
     if (tpl_pl)
     {
       if ((tpl_pl->get_extra_action() && tpl_pl->get_extra() == EXTRA_CLOAK))
         turn_speed = orig_turn_speed/2;
       else
         turn_speed = orig_turn_speed;
     }

     if (x >= enx)
       scale_x = -xrot;
     else
       scale_x = xrot;

     float scale_y = yrot;

     dest_xspd = scale_x * (time_since_fire / 10) / 3;
     dest_yspd = scale_y * (time_since_fire / 10) / 3;
     
     
     // upgrade 1: shoot circle bullets
     if (upgrade == 1)
     {
       if (shoot_angle < 360) shoot_angle+=10; else shoot_angle = 0;
     }
     if (upgrade == 1 && time_since_fire % 2 == 0)
     {
       float mxspd, myspd;
       mxspd = sin(shoot_angle * 3.14159/180.0) * 2;
       myspd = cos(shoot_angle * 3.14159/180.0) * 2;
       
       Bullet *tmpbull = effects->bullets->add(W_SINGLE, by_player, by_enemy, 
                                               x+2, y+2, mxspd, myspd,
                                               2, 2, 1, 0, 0);
       tmpbull->set_ttl(30);

       tmpbull = effects->bullets->add(W_SINGLE, by_player, by_enemy, 
                                        x+2, y+2, -mxspd, -myspd,
                                        2, 2, 1, 0, 0);
       tmpbull->set_ttl(30);

       gsPlayer *bypl = get_player_by_sig(by_player);
       if (bypl) bypl->inc_bullets_shot(2);
     }
     
  } // target locked? move to target
  else if (time_since_fire > user && target == 0)
  {
    target = search_target(x, y, by_player);
  }
  else
  {
    dest_xspd = xspd;
    dest_yspd = yspd;
  }

  if (xspd > dest_xspd) xspd -= turn_speed;
  else if (xspd < dest_xspd) xspd += turn_speed;
  if (yspd > dest_yspd) yspd -= turn_speed;
  else if (yspd < dest_yspd) yspd += turn_speed;

  x += xspd;
  y -= yspd;

  // draw trail
  if (!invisible)
    effects->pixels->add(1, x+gsrand()%4, y-1+gsrand()%4, 1, 1,
                         0, -globals->particle_gravity, 0,
                         globals->tbl_white, 9, 60*5, 20, 100, 100);

  if (ttl) ttl--;
}

void bulletRocket::hit(float n)
{
  Bullet::hit(n);
  if (kill) effects->explosions->add(-1, expl_sm1_01, 10, 1, 0, x-2, y-2, globals->gamedat);
}

void bulletRocket::draw(gsMap *m)
{
  if (!invisible && m->is_on_screen(x, y, w, h))
    if (getpixel(m->get_vscreen(), (int)x-m->get_map_x(), (int)y-m->get_map_y()) == 0)
    {
      draw_rle_sprite(m->get_vscreen(), image->get_rle_frame(curframe), (int)x-m->get_map_x(), (int)y-m->get_map_y());
      clearlist->add(m, x, y, w, h);
    }
}

int bulletRocket::check_collisions()
{
  float newx1, newy1, newx2, newy2;
  if (just_coll > 0) just_coll--;

  // min_x/y or max_x/y reached or ttl expired?
  if ( ttl == 0 ||
       x < min_x || x > max_x ||
       y < min_y || y > max_y )
  {
    effects->explosions->add(-1, expl_sm1_01, 10, 1, 0, x-2, y-2, globals->gamedat);
    return 1;
  }

  // collision with players?
  for (int n=0; n < globals->players; n++)
  {
    if ( !player[n]->get_dead() &&        // is player alive?
         by_player != player[n]->get_sig() &&  // is the bullet shot by another player?
         collision( x, y, w, h,
                    player[n]->get_x(), player[n]->get_y(),
                    player[n]->get_width(), player[n]->get_height(),
                    xspd, -yspd, player[n]->get_xspd(), player[n]->get_yspd(),
                    &newx1, &newy1, &newx2, &newy2, 1) )
    {
      // adjust position
      x = newx1; y = newy1;
      
      player[n]->hit(by_player, by_enemy, hitp_hull, hitp_shield, type, xspd, yspd);
      effects->explosions->add(-1, expl_sm1_01, 10, 1, 0, x-2, y-2, globals->gamedat);

      if (!player[n]->get_land() &&
           player[n]->get_time_alive() > globals->player_indestructible)
      {
        player[n]->set_xspd( player[n]->get_xspd() + xspd/5 );
        player[n]->set_yspd( player[n]->get_yspd() - yspd/5 );
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
        temp_o->get_active() &&
        collision( x, y, w, h,
                   temp_o->get_x(), temp_o->get_y(),
                   temp_o->get_width(), temp_o->get_height() ) )
    {
      if (temp_o->add_hitpoints(by_player, strength, type, x, y, xspd, yspd))
        return 1;

      effects->explosions->add(-1, expl_sm1_01, 10, 1, 0, x-2, y-2, globals->gamedat);
      gsound->expl_small1(x, y);

      switch (temp_o->get_type())
      {
         case OBJ_DOORH : case OBJ_DOORV : case OBJ_DOORH2 : case OBJ_DOORV2 :
              if (temp_o->get_maxframe() != 0) return 1;
              break;

         default :
              float tx, ty;
              if ( yspd < 0 ) ty = -0.6;
              else ty = 0.4;        
              tx = ( (gsrand() % 4) - 2 ) / 5;        
              float pspd = abs(int(sqrt(yspd*yspd + xspd*xspd)));
              effects->pixels->add( (gsrand() % ((int)(pspd*8)+1)) + 1,
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
    if (!temp_o->get_invisible() && !(temp_o->get_active() == 0) &&
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
             break;
        case SPOBJ_TELEPORT1 : case SPOBJ_TELEPORT2 :
             if (teleport_time == 0)
             {
               if (collision( x, y,
                              5, 5,
                              temp_o->get_x()+1, temp_o->get_y()+1,
                              temp_o->get_width()-2, temp_o->get_height()-2 ) )
               {
                 Object *temp = objects->get_spobject(temp_o->get_user());
                 effects->explosions->add(-1, effect_beam2_01, 10, 1, 0, x, y, globals->gamedat);
                 set_x(temp->get_x()+temp->get_width()/2);
                 set_y(temp->get_y()+temp->get_height()/2);
                 reset_min_max();
                 teleport_time = 30;
                 effects->explosions->add(-1, effect_beam2_01, 10, 1, 0, x, y, globals->gamedat);
                 gsound->beam_bullet(x, y);
               }
             }
             break;
      }
    }

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
      if (temp_e->hit(by_player, by_enemy, strength, x, y, xspd, yspd, W_ROCKET))
      {
        effects->explosions->add(-1, expl_sm1_01, 10, 1, 0, x-2, y-2, globals->gamedat);
        return 1;
      }
    }

    temp_e = temp_e->get_next();
  }
  
  // collision with probes, mines, rockets?
  Bullet *temp_bu;
  temp_bu = effects->bullets->get_first();
  while (temp_bu && temp_bu->get_by_player() != by_player &&
                    (temp_bu->get_type() == W_ROCKET || 
                     temp_bu->get_type() == W_PROBE ||
                     temp_bu->get_type() == W_MINE) )
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

      temp_bu->hit(3);
      return 1;
    }
    
    temp_bu = temp_bu->get_next();
  }
 

  return 0;
}

int bulletRocket::check_map_collisions(gsMap *m)
{
  // collision with landscape?
  if (m->is_on_map(x, y, 10, 10))
  {
    if  (m->is_pixel((int)x, (int)y) ||
         m->is_pixel((int)x-2, (int)y) ||
         m->is_pixel((int)x+6, (int)y) ||
         m->is_pixel((int)x, (int)y-5) ||
         m->is_pixel((int)x, (int)y+5))
    {
      // add pixels when bullet hits the wall
      float tx, ty;

      if ( yspd < 0 ) ty = -0.6;
      else ty = 0.4;

      tx = ( (gsrand() % 4) - 2 ) / 5;

      float pspd = abs(int(sqrt(yspd*yspd + xspd*xspd)));

      int *coltbl = NULL;
      if (!strcmp(m->get_style(), "gfw1.fmp"))
        coltbl = globals->tbl_green;
      else
        coltbl = globals->tbl_white;

      effects->pixels->add( (gsrand() % ((int)(pspd*2)+1)) + 50,
                             x - xspd/4, y + ty*4, 1, 1,
                             tx, ty, 400,
                             coltbl, 10, 60*10, 50,
                             600, 600);

      // add a crater
      if (!globals->network_game || net->is_server()) 
        effects->craters->add(x+1, y+1, 6, 1);

      gsound->expl_small1(x, y);

      // do a little explosion
      effects->explosions->add(-1, expl_sm1_01, 10, 1, 0, x-2, y-2, globals->gamedat);

      return 1;
    } // if getpixel
  }

  return 0;
}



