/*
 *   GRAVITY STRIKE -- weapons/bbomb.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "bbomb.h"
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
#include "../gserror.h"
#include "../gsnetw.h"
#include "../gsglob.h"
#include <math.h>

extern float      sinus[];
extern float      cosinus[];

bulletBomb::bulletBomb(gsPlayer *p,  int puser, int pupgrade, Image *spr)
{
  // bullet init
  type = W_BOMB;
  by_player = p->get_sig();
  user      = puser;
  upgrade   = pupgrade;

  // stats
  p->inc_bullets_shot(8);

  // image
  image = spr;
  w = image->get_width();
  h = image->get_height();
  maxframet = 10;

  // position & speed
  float xp = p->get_width()/2; float yp = p->get_height()/2;
  float xrot = sinus[ (int)p->get_head() ];
  float yrot = cosinus[ (int)p->get_head() ];

  float dx = xp + xrot*yp;
  float dy = yp - yrot*yp;

  x = p->get_x() + dx;
  y = p->get_y() + dy;

  xspd = xrot * globals->shoot_speed_bomb + p->get_xspd();
  yspd = yrot * globals->shoot_speed_bomb - p->get_yspd();

  // max range
  min_x = (int)p->get_x() - globals->playscreen_width * 4;
  max_x = (int)p->get_x() + globals->playscreen_width * 4;
  min_y = (int)p->get_y() - globals->playscreen_height * 4;
  max_y = (int)p->get_y() + globals->playscreen_height * 4;

  // time to live
  orig_ttl = ttl = 60 * 7;

  // strength
  strength    = 2;
  hitp_hull   = 2;
  hitp_shield = 2;

  // temp vars
  teleport_time = 0;
  bumpcount = 0;
  
  // player recoil
  if (p->get_recoil())
  {
    p->set_xspd(p->get_xspd() - (xrot*0.7));
    p->set_yspd(p->get_yspd() + (yrot*0.7));  
  }  
}

bulletBomb::bulletBomb(long psig, int enr, int px, int py, float pxspd, float pyspd, int pstrength, int pshield, int phull, float puser, int pupgrade, Image *spr)
{
  // bullet init
  type = W_BOMB;
  by_player = psig;
  by_enemy  = enr;
  upgrade   = pupgrade;
  user      = puser;

  // image
  image = spr;
  w = image->get_width();
  h = image->get_height();
  maxframet = 10;
  
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
  orig_ttl = ttl = 60 * 7;

  // strength
  strength    = pstrength;
  hitp_hull   = phull;
  hitp_shield = pshield;

  // temp vars
  teleport_time = 0;
  bumpcount = 0;
}


bulletBomb::~bulletBomb()
{
}

void bulletBomb::move()
{
  Bullet::move();

  yacc  = -(globals->bomb_gravity * globals->gravity_percent);
  if (fabs(yspd + yacc) < 6) yspd += yacc;

  x += xspd;
  y -= yspd;

  if (ttl) ttl--;

  // draw a little trail
  if (!invisible)
    effects->pixels->add(1, x-1+gsrand()%4, y-1+gsrand()%4, 1, 1,
                         0, -globals->particle_gravity, 0,
                         globals->tbl_white, 9, 60*3, 5, 100, 100);
}

void bulletBomb::draw(gsMap *m)
{
  if (!invisible && m->is_on_screen(x, y, w, h))
    if (getpixel(m->get_vscreen(), (int)x-m->get_map_x(), (int)y-m->get_map_y()) == 0)
    {
      draw_rle_sprite(m->get_vscreen(), image->get_rle_frame(curframe), (int)x-m->get_map_x(), (int)y-m->get_map_y());
      clearlist->add(m, x, y, w, h);
    }
}

int bulletBomb::check_map_collisions(class gsMap *m)
{
  // collision with landscape?
  if (m->is_on_map(x, y, 1, 1))
  {
    int wx=0;
    if  (m->is_pixel((int)x+xspd-10, (int)y-yspd) && (wx=1) ||
         m->is_pixel((int)x+xspd+10, (int)y-yspd) && (wx=2) ||
         m->is_pixel((int)x+xspd, (int)y-yspd-10) && (wx=3) ||
         m->is_pixel((int)x+xspd, (int)y-yspd+10) && (wx=4) )
    {
      
      // upgrade 1: bomb-bump
      if (upgrade == 1 && bumpcount < 2)
      {
        if (wx <= 2)
          xspd = -xspd/2;
        else
          yspd = -yspd/2;
          
        bumpcount++;
        return 0;
      }
      
      // spawn bomb bullets
      add_bomb_bullets();

      // add a crater
      if (!globals->network_game || net->is_server()) 
        effects->craters->add(x+xspd+1, y-yspd+1, 8, 1);

      gsound->expl_small1(x, y);

      // do a little explosion
      effects->explosions->add(-1, expl_sm1_01, 10, 1, 0, x, y, globals->gamedat);

      return 1;
    } // if getpixel
  }

  return 0;
}

int bulletBomb::check_collisions()
{
  float newx1, newy1, newx2, newy2;
  if (just_coll > 0) just_coll--;

  // min_x/y or max_x/y reached or ttl expired?
  if ( ttl == 0 ||
       x < min_x || x > max_x ||
       y < min_y || y > max_y )
  {
    // spawn bomb bullets
    add_bomb_bullets();

    // do a little explosion
    effects->explosions->add(-1, expl_sm1_01, 10, 1, 0, x, y, globals->gamedat);

    gsound->expl_small1(x, y);

    return 1;
  }

  // collision with players?
  for (int n=0; n < globals->players; n++)
  {
    if ( !player[n]->get_dead() &&        // is player alive?
         by_player != player[n]->get_sig() &&  // is the bullet shot by another player?
         collision( x-5, y-5, 10, 10, // so... is there a collision?
                    player[n]->get_x(), player[n]->get_y(),
                    player[n]->get_width(), player[n]->get_height(),
                    xspd, -yspd, player[n]->get_xspd(), player[n]->get_yspd(),
                    &newx1, &newy1, &newx2, &newy2, 1 ) )
    {
      // adjust position
      x = newx1; y = newy1;
      
      // spawn bomb bullets
      add_bomb_bullets();

      // do a little explosion
      effects->explosions->add(-1, expl_sm1_01, 10, 1, 0, x, y, globals->gamedat);

      if (!player[n]->get_land())
      {
        player[n]->set_xspd( player[n]->get_xspd() + xspd/10 );
        player[n]->set_yspd( player[n]->get_yspd() - yspd/10 );
      }

      gsound->expl_small1(x, y);
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
              if (temp_o->get_maxframe() != 0)
              {
                // spawn bomb bullets
                add_bomb_bullets();
          
                // do a little explosion
                effects->explosions->add(-1, expl_sm1_01, 10, 1, 0, x, y, globals->gamedat);
                gsound->expl_small1(x, y);

                return 1;
              }
              break;

         default :
              // spawn bomb bullets
              add_bomb_bullets();
        
              // do a little explosion
              effects->explosions->add(-1, expl_sm1_01, 10, 1, 0, x, y, globals->gamedat);

              gsound->expl_small1(x, y);

              float tx, ty;
              if ( yspd < 0 ) ty = -0.6;
              else ty = 0.4;
              tx = ( (gsrand() % 4) - 2 ) / 5;
              float pspd = abs(int(sqrt((yspd*yspd) + (xspd*xspd))));
              effects->pixels->add( (gsrand() % (int)((pspd*5)+1)) + 1,
                                     x - xspd/4, y + ty*4, 1, 1,
                                     tx, ty, 400,
                                     globals->tbl_white, 10, 60*10, 50,
                                     600, 600);
              return 1;
              break;
      } // switch type

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
               if (collision( x, y,
                              3, 3,
                              temp_o->get_x()+1, temp_o->get_y()+1,
                              temp_o->get_width()-2, temp_o->get_height()-2) )
               {
                 Object *temp = objects->get_spobject(temp_o->get_user());
                 effects->explosions->add(-1, effect_beam2_01, 10, 1, 0, x, y, globals->gamedat);
                 set_x(temp->get_x()+temp->get_width()/2);
                 set_y(temp->get_y()+temp->get_height()/2);
                 reset_min_max();
                 teleport_time = 20;
                 effects->explosions->add(-1, effect_beam2_01, 10, 1, 0, x, y, globals->gamedat);
                 gsound->beam_bullet(x, y);
               }
             }
             return 0; // don't delete bullet
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
      // adjust position
      x = newx1; y = newy1;
      
      if (temp_e->hit(by_player, by_enemy, 0, x, y, xspd, yspd, W_BOMB))
      {
        // spawn bomb bullets
        add_bomb_bullets();
  
        // do a little explosion
        effects->explosions->add(-1, expl_sm1_01, 10, 1, 0, x, y, globals->gamedat);

        gsound->expl_small1(x, y);
        return 1;
      }
    }

    temp_e = temp_e->get_next();
  }

  return 0;
}

void bulletBomb::add_bomb_bullets()
{
  Bullet *tb[8] = { NULL };
  tb[0] = effects->bullets->add(W_SINGLE, by_player, by_enemy, x, y,    0,   -3,  strength, strength, strength, 0, 0);
  tb[1] = effects->bullets->add(W_SINGLE, by_player, by_enemy, x, y,    3,    0,  strength, strength, strength, 0, 0);
  tb[2] = effects->bullets->add(W_SINGLE, by_player, by_enemy, x, y,    0,    3,  strength, strength, strength, 0, 0);
  tb[3] = effects->bullets->add(W_SINGLE, by_player, by_enemy, x, y,   -3,    0,  strength, strength, strength, 0, 0);
  tb[4] = effects->bullets->add(W_SINGLE, by_player, by_enemy, x, y,  1.5, -1.5,  strength, strength, strength, 0, 0);
  tb[5] = effects->bullets->add(W_SINGLE, by_player, by_enemy, x, y,  1.5,  1.5,  strength, strength, strength, 0, 0);
  tb[6] = effects->bullets->add(W_SINGLE, by_player, by_enemy, x, y, -1.5,  1.5,  strength, strength, strength, 0, 0);
  tb[7] = effects->bullets->add(W_SINGLE, by_player, by_enemy, x, y, -1.5, -1.5,  strength, strength, strength, 0, 0);

  for (int n=0; n < 8; n++)
    tb[n]->set_ttl(80);
}

