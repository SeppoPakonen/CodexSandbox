/*
 *   GRAVITY STRIKE -- gsexplo.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "gsexplo.h"
#include "gsmap.h"
#include "gsplayer.h"
#include "gserror.h"
#include "gsglob.h"

Explosion::Explosion(long pplayer_sig, int ptype, int pmaxframe, int pmaxframet, int pstarttime, int px, int py, DATAFILE *pdat)
{
  player_sig  = pplayer_sig;
  type        = ptype;
  maxframe    = pmaxframe;
  maxframet   = pmaxframet;
  starttime   = pstarttime;
  x           = px;
  y           = py;
  dat         = pdat;

  curframe    = 1;
  curframet   = 0;

  expl_player = 0;

  if (ptype == effect_shield_1)
  {
    expl_player = get_player_nr_by_sig(player_sig);
    BITMAP *bmpbit = player[expl_player]->get_shield_frame(0);
    w = bmpbit->w;
    h = bmpbit->h;
    bw = w + 10;
    bh = h + 10;
    x = (int)player[expl_player]->get_x() + player[expl_player]->get_width()/2 - w/2;
    y = (int)player[expl_player]->get_y() + player[expl_player]->get_height()/2 - h/2;
  }
  else if (ptype == effect_gravfield_1)
  {
    expl_player = get_player_nr_by_sig(player_sig);
    w = bw = ((BITMAP*)dat[type].dat)->w;
    h = bh = ((BITMAP*)dat[type].dat)->h;
    x = (int)player[expl_player]->get_x() + player[expl_player]->get_width()/2 - w/2;
    y = (int)player[expl_player]->get_y() + player[expl_player]->get_height()/2 - h/2;
  }
  else
  {
    w = bw = ((BITMAP*)dat[type].dat)->w;
    h = bh = ((BITMAP*)dat[type].dat)->h;
  }

  background = create_bitmap(bw,bh);

  clear_bitmap(background);
  back_okay = 0;
}

Explosion::~Explosion()
{
  if (background) destroy_bitmap(background);
}

void Explosion::draw(gsMap *m)
{
  RLE_SPRITE *bit;

  if (m->is_on_screen(x, y, w, h))
  {
    if (type == effect_shield_1)
    {
      if (player[expl_player])
      {
        BITMAP *bmpbit = player[expl_player]->get_shield_frame(curframe-1);
        rotate_sprite(m->get_vscreen(), bmpbit, x - m->get_map_x(), y - m->get_map_y(),
                      ftofix(player[expl_player]->get_head() / 1.4));
      }
    }
    else
    {
      bit = (RLE_SPRITE *)dat[ type + curframe - 1 ].dat;
  
      draw_rle_sprite(m->get_vscreen(), bit,
                      (int)x - m->get_map_x(), (int)y - m->get_map_y());
    } // type
  } // on screen
}

int Explosion::update(class gsMap *m, class gsMap *m2)
{
  if (starttime <= 0)
  {
    if (curframe >= maxframe)
    {
      return 1;
    }
    else
    {
      curframet++;
      if (curframet >= maxframet)
      {
        curframet = 0;
        curframe++;
      }
    }

    // special things
    switch (type)
    {
      case effect_shield_1 :
                        if (player[expl_player] && !player[expl_player]->get_dead())
                        {
                          x = (int)player[expl_player]->get_x() + player[expl_player]->get_width()/2 - w/2;
                          y = (int)player[expl_player]->get_y() + player[expl_player]->get_height()/2 - h/2;
                        }
                        break;
      case effect_gravfield_1 :
                        if (player[expl_player] && !player[expl_player]->get_dead())
                        {
                          x = (int)player[expl_player]->get_x() + player[expl_player]->get_width()/2 - w/2;
                          y = (int)player[expl_player]->get_y() + player[expl_player]->get_height()/2 - h/2;
                        }
                        break;

    } // switch
  } // if starttime <= 0
  else
  {
    starttime--;
  }

  return 0;
}

void Explosion::restore(gsMap *tm)
{
  int t=0;

  if (tm->is_on_map(x, y, w, h))
  {
    if (type == effect_shield_1) t = (bw-w)/2;

    blit(background, tm->get_vscreen(), 0, 0,
         x - t - tm->get_map_x(), y - t - tm->get_map_y(), bw, bh);
  }
}

void Explosion::save_background(gsMap *m, gsMap *m2)
{
  int t = 0;

  if (type == effect_shield_1) t = (bw-w)/2;

  if (m->is_on_map(x, y, w, h))
  {
    blit(m->get_vscreen(), background, x - t - m->get_map_x(), y - t - m->get_map_y(), 0, 0, bw, bh);
    back_okay = 1;
//rect(m->get_vscreen(), x-t-m->get_map_x(), y-t-m->get_map_y(), x-t-m->get_map_x()+bw, y-t-m->get_map_y()+bh, globals->col_green);
  }
  else
  if (m2 && m2->is_on_map(x, y, w, h))
  {
    blit(m2->get_vscreen(), background, x - t - m2->get_map_x(), y - t - m2->get_map_y(), 0, 0, bw, bh);
    back_okay = 1;
  }
}

gsExplosion::gsExplosion(gsMap *pm, gsMap *pm2)
{
  first_exp=NULL;
  last_exp=NULL;
  count = 0;
  m = pm;
  m2 = pm2;
}

gsExplosion::~gsExplosion()
{
  Explosion *temp, *next_exp = NULL;
  temp = first_exp;

  while (temp != NULL)
  {
    next_exp = temp->get_next();
    delete temp;
    temp = next_exp;
  }
}

Explosion *gsExplosion::add(int what, int speed, int pstart, int px, int py)
{
  int ptype;
  int pmaxframe;

  switch (what)
  {
    case EXP_SMALL_1 :
         pmaxframe = 10;
         ptype = expl_sm1_01;
         break;

    case EXP_SMALL_2 :
         pmaxframe = 7;
         ptype = expl_sm2_01;
         break;

    case EXP_SMALL_3 :
         pmaxframe = 5;
         ptype = expl_sm3_01;
         break;

    case EXP_SMALL_4 :
         pmaxframe = 3;
         ptype = expl_sm4_01;
         break;

    case EXP_MED_1 :
         pmaxframe = 7;
         ptype = expl_med1_01;
         break;

    case EXP_BIG_1 :
         pmaxframe = 5;
         ptype = expl_big_1;
         break;

    case EXP_BIG_2 :
         pmaxframe = 14;
         ptype = expl_big2_01;
         break;

    default :
         pmaxframe = 10;
         ptype = expl_sm1_01;

  } // what?

  return add(-1, ptype, pmaxframe, speed, pstart, px, py, globals->gamedat);
}

Explosion *gsExplosion::add(long pplayer_sig, int ptype, int pmaxframe, int pmaxframet, int pstarttime, int px, int py, DATAFILE *dat)
{
  Explosion *TempExp;

  TempExp = new Explosion(pplayer_sig, ptype, pmaxframe, pmaxframet, pstarttime, px, py, dat);

  if (TempExp != NULL)
  {
    if (first_exp==NULL)
    {
      last_exp = first_exp = TempExp;
      first_exp->set_next(NULL); first_exp->set_prev(NULL);
      TempExp->set_next(NULL); TempExp->set_prev(NULL);
    }
    else
    {
      last_exp->set_next(TempExp);
      TempExp->set_prev(last_exp);
      last_exp = TempExp;
      TempExp->set_next(NULL);
    }
  } // TempExp != NULL

  count++;
  return TempExp;
}

void gsExplosion::draw(gsMap *tm)
{
  Explosion *temp = first_exp;

  while (temp != NULL)
  {
    if (temp->get_back_okay() && temp->get_type() != effect_shield_1) temp->draw(tm);
    temp = temp->get_next();
  }
}

void gsExplosion::draw_shield(gsMap *tm)
{
  Explosion *temp = first_exp;

  while (temp != NULL)
  {
    if (temp->get_type() == effect_shield_1 && temp->get_back_okay()) temp->draw(tm);
    temp = temp->get_next();
  }
}

void gsExplosion::update()
{
  Explosion *temp = first_exp, *next_exp;

  while (temp != NULL)
  {
    next_exp = temp->get_next();
    if (temp->update(m, m2)) remove(temp);
    temp = next_exp;
  } // while temp != NULL
}

void gsExplosion::clear(gsMap *tm)
{
  Explosion *temp = first_exp;

  while (temp != NULL)
  {
    if (temp->get_back_okay()) temp->restore(tm);
    temp = temp->get_next();
  } // while temp != NULL
}

void gsExplosion::save_background()
{
  Explosion *temp = first_exp;

  while (temp != NULL)
  {
//    if (!temp->get_back_okay() || temp->get_type() == effect_shield_1)
    temp->save_background(m, m2);
    temp = temp->get_next();
  } // while temp != NULL
}

void gsExplosion::remove(Explosion *temp)
{
  Explosion *next_exp, *prev_exp;

  if (temp != NULL)
  {
    prev_exp = temp->get_prev();
    next_exp = temp->get_next();

    if (prev_exp==NULL) first_exp = next_exp;
    else prev_exp->set_next(next_exp);

    if (next_exp==NULL) last_exp = prev_exp;
    else next_exp->set_prev(prev_exp);

    delete temp;
    temp = NULL;

    count--;
  }
}

