/*
 *   GRAVITY STRIKE -- gsbase.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */


#include "gsglob.h"
#include "gsbase.h"
#include "gserror.h"
#include "gsmap.h"
#include "gsplayer.h"
#include "gsclear.h"
#include "gseffect.h"
#include "gsobject.h"
#include "gspanel.h"
#include "gssound.h"

extern gsGlobals *globals;
extern gsError   *errors;


Base::Base(int c, BASE_TYPE ptype, int px, int py, int puser, int pcargo, int pcargo_am)
{
  type   = ptype;
  land_x = px;
  land_y = py;
  xspd = yspd = 0;
  cargo_type  = (CARGO_TYPE)pcargo;
  cargo_object = NULL;
  cargo_weight = 1;
  image = NULL;
  nr = c;
  strcpy(cargo_name, "");
  w = h = 0;
  refuel_percent = 0;
  refuel_player = 0;

  curframe = curframet = maxframe = maxframet = 0;

  switch (type)
  {
    case PLAYER_BASE  :
                         image  = (RLE_SPRITE *)globals->gamedat[base_home].dat;
                         land_w = 65;
                         x      = land_x - 22;
                         y      = land_y - 28;
                         break;
    case CARGO_BASE  :
                         image  = (RLE_SPRITE *)globals->gamedat[base_cargo].dat;
                         land_w = 67;
                         x      = land_x - 6;
                         y      = land_y - 2;
                         break;
    case ENEMY_S_BASE :
                         image  = (RLE_SPRITE *)globals->gamedat[base_senemy].dat;
                         land_w = 35;
                         x      = land_x - 1;
                         y      = land_y - 4;
                         break;
    case ENEMY_L_BASE :
                         image  = (RLE_SPRITE *)globals->gamedat[base_lenemy].dat;
                         land_w = 67;
                         x      = land_x - 3;
                         y      = land_y - 4;
                         break;
    case CUSTOM_BASE  :
                         image  = NULL;
                         land_w = 33;
                         x      = land_x;
                         y      = land_y;
                         break;
    case RELOAD_BASE  :
                         image  = (RLE_SPRITE *)globals->gamedat[base_reload_1].dat;
                         land_w = 49;
                         x      = land_x - 23;
                         y      = land_y - 22;
//                         yspd   = -0.01;
                         maxframe = 2;
                         maxframet = 8;
                         break;
    case ENEMY_BASE_LEFT :
                         image  = (RLE_SPRITE *)globals->gamedat[base_cust_left].dat;
                         land_w = 9;
                         x      = land_x - 1;
                         y      = land_y - 4;
                         break;
    case ENEMY_BASE_RIGHT :
                         image  = (RLE_SPRITE *)globals->gamedat[base_cust_right].dat;
                         land_w = 9;
                         x      = land_x - 1;
                         y      = land_y - 4;
                         break;
    case ENEMY_BASE_MIDDLE :
                         image  = (RLE_SPRITE *)globals->gamedat[base_cust_middle].dat;
                         land_w = 40;
                         x      = land_x;
                         y      = land_y - 4;
                         break;
    default :
                         image  = (RLE_SPRITE *)globals->gamedat[base_home].dat;
                         x      = land_x;
                         y      = land_y;
  }

  if (puser > 0) land_w = puser;

  if (image)
  {
    w = image->w;
    h = image->h;
  }
}

Base::~Base()
{
}

void Base::set_cargo_object(class Object *n)
{
  if (n)
  {
    cargo_object = n;
    cargo_type = CARGO_OBJECT;
  }
  else { cargo_object = NULL; cargo_weight = 0; }
}

void Base::draw_cargo(gsMap *m)
{
  if (m->is_on_screen(x, y, w, h))
  {
    int cx,cy,cnum;
    RLE_SPRITE *img = NULL;
    float scale = 0;

    switch (cargo_type)
    {
      case CARGO_NORMAL  :
                            scale = 4 + (cargo_weight / 3);
                            cnum = cargo1;
                            cx = (int)land_x + land_w/2 - (int)scale/2;
                            cy = (int)land_y - (int)scale - 1;
                            stretch_sprite(m->get_vscreen(), (BITMAP*)globals->gamedat[cnum].dat,
                                           cx - m->get_map_x(),
                                           cy - m->get_map_y() -1,
                                           (int)scale, (int)scale);
                            clearlist->add(m, (int)land_x + land_w/2 - scale/2 - 1, (int)land_y - scale-2, (int)scale+1, (int)scale+1);
                            break;

      case CARGO_OBJECT  :
                            if (cargo_object)
                            {
                              img = cargo_object->get_current_sprite();
                              cx = (int)land_x + land_w/2 - img->w/2;
                              cy = (int)land_y - img->h - 2;
                              draw_rle_sprite(m->get_vscreen(), img, (int)cx - m->get_map_x(), (int)cy - m->get_map_y());
                              clearlist->add(m, cx, cy, img->w, img->h);
                            }
                            else { cargo_object = NULL; cargo_weight = 0; cargo_type = CARGO_NONE; }
                            break;

      case CARGO_NONE : break;

    } // switch
  }
}

void Base::draw(gsMap *m)
{
  if (m->is_on_screen(x, y, w, h))
  {
    if (type != CUSTOM_BASE)
      draw_rle_sprite(m->get_vscreen(), image, (int)x - m->get_map_x(), (int)y - m->get_map_y());

    if (type == RELOAD_BASE)
    {
      clearlist->add(m, x, y, image->w, image->h);
    }
  }
}

void Base::kill_landscape(gsMap *m)
{
  if (m->is_on_map(x, y, w, h))
  {
    if (type != CUSTOM_BASE)
      rectfill(m->get_vscreen(), (int)land_x - m->get_map_x(), (int)land_y - m->get_map_y() - 20,
               (int)land_x + land_w - m->get_map_x(), (int)land_y - m->get_map_y() - 1, 0);
  }
}

void Base::update()
{

  if (maxframe && maxframet)
  {
    curframet++;
    if (curframet >= maxframet) { curframe++; curframet = 0; }
    if (curframe  >= maxframe)  { curframe = 0; }

    switch (type)
    {
      case RELOAD_BASE :
                          image  = (RLE_SPRITE *)globals->gamedat[base_reload_1 + curframe].dat;
                          break;
      default : break;
    }

  }

  if (xspd || yspd)
  {
    x += xspd;
    y += yspd;
    land_x += xspd;
    land_y += yspd;
  }
}

void Base::hit(int strength)
{
  switch (type)
  {
    case RELOAD_BASE :
            effects->explosions->add(-1, effect_shield_base_1, 3, 5, 0, x-15, y-12, globals->gamedat);
            break;

    default : break;
  }
}

gsBase::gsBase()
{
  first_base = last_base = 0;
  count = 0;
}

gsBase::~gsBase()
{
  Base *temp, *next_base;
  temp = first_base;

  while (temp != NULL)
  {
    next_base = temp->get_next();
    delete temp;
    temp = next_base;
  }
}

Base *gsBase::add(int num, int ptype, int px, int py, int user, int pcargo)
{
  Base *temp_base;

  temp_base = new Base(num, (BASE_TYPE)ptype, px, py, user, pcargo);

  if (temp_base != NULL)
  {
    if (first_base==NULL)
    {
      last_base = first_base = temp_base;
      first_base->set_next(NULL); first_base->set_prev(NULL);
      temp_base->set_next(NULL); temp_base->set_prev(NULL);
    }
    else
    {
      last_base->set_next(temp_base);
      temp_base->set_prev(last_base);
      last_base = temp_base;
      temp_base->set_next(NULL);
    }
  } // temp_base != NULL

  count++;
  return temp_base;
}

void gsBase::draw(gsMap *m)
{
  Base *temp;
  temp = first_base;

  while (temp != NULL)
  {
    temp->draw(m);
    temp = temp->get_next();
  }
}

void gsBase::draw_cargo(gsMap *m)
{
  Base *temp;
  temp = first_base;

  while (temp != NULL)
  {
    temp->draw_cargo(m);
    temp = temp->get_next();
  }
}

void gsBase::kill_landscape(gsMap *m)
{
  Base *temp;
  temp = first_base;

  while (temp != NULL)
  {
    temp->kill_landscape(m);
    temp = temp->get_next();
  }
}

void gsBase::update()
{
  Base *temp;
  temp = first_base;

  while (temp != NULL)
  {
    temp->update();
    temp = temp->get_next();
  }
}

void gsBase::check_if_landed(gsPlayer *p)
{
  Base *temp;
  temp = first_base;

  int xp = (int)p->get_x();
  float yp = (int)p->get_y() + p->get_height() - 2;
  float yp2 = p->get_y() + p->get_height() + p->get_yspd() + 2;

  while (temp != NULL)
  {
    if (yp < temp->get_land_y() && yp2 > temp->get_land_y())
      yp = temp->get_land_y();

    // landing pad hit?
    if ( yp == temp->get_land_y() && xp >= temp->get_land_x() &&
         xp + p->get_width() <= temp->get_land_x() + temp->get_land_w())
    {
      // angle correct? speed acceptable?
      if ( (p->get_head() >= 345 || p->get_head() <= 15) && p->get_yspd() < 3 )
      {
        // too much speed in either direction? -> bounce to land...
        if ( p->get_xspd() < -0.02 || p->get_xspd() > 0.02 || p->get_yspd() > 0.1 )
        {
          p->set_head(0);
          p->set_land(0);
          p->set_yspd( -p->get_yspd() + (p->get_yspd() / 2) );
          p->move();

          gsound->play_sound(SOUND_BASE_JUMP, p->get_x(), p->get_y());

          if (p->get_xspd() > 0)
          {
            if (p->get_xspd() > 0.05) p->set_xspd( p->get_xspd() - 0.05 );
              else p->set_xspd(0);
          }
          else if (p->get_xspd() < 0)
          {
            if (p->get_xspd() < -0.05) p->set_xspd( p->get_xspd() + 0.05 );
              else p->set_xspd(0);
          }
        } // bounce?
        else
        {
          p->set_land(1);
          p->set_land_base( temp->get_nr() );
          p->set_y( temp->get_land_y() - p->get_height() - 2 );
          p->set_head(0);
          p->set_xspd(0);
          p->set_yspd(0);
          temp->set_player_on_base(p->get_sig());
        } // else land...

      } // if angle correct
    } // if land_hit

    temp = temp->get_next();
  } // while bases...
}

Base *gsBase::get_base(int n)
{
  Base *temp;
  temp = first_base;

  while (temp != NULL)
  {
    if (temp->get_nr() == n) return temp;
    temp = temp->get_next();
  }

  return NULL;
}

