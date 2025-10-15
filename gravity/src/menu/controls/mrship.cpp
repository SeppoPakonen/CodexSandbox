/*
 *   GRAVITY STRIKE -- menu/controls/mrship.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "mrship.h"
#include "allegro5_wrapper.h"
#include "../../gsplayer.h"
#include "../../gsclear.h"
#include "../../gsglob.h"
#include "../../gslng.h"
#include "../../gshelper.h"
#include "../gsmd.h"
#include "mtypes.h"
#include "../mpixels.h"

extern mPixelRain *mpixels;

mShipIcon2::~mShipIcon2()
{
}

void mShipIcon2::create(int px, int py, int sx, int sy, int pw, int ph)
{
  mControl::create(px, py, sx, sy);
  w = pw;
  h = ph;
  error = 0;
  rot = 0;
  dead = 0;
  pl = NULL;
  mode = 0;
}

void mShipIcon2::set_player(gsPlayer *p)
{
  pl = p;
}

void mShipIcon2::draw(BITMAP *v)
{
  if (!(flags & MC_INVISIBLE))
  {
    int c = color1;
    int tc = color2;
    BITMAP *shipimage;
    FONT *f = (FONT*)globals->fontdat[FONT_IMPACT10].dat;
    int theight;
    if (mode == 1)
      theight = text_height(f);
    else
      theight = 0;

    if (pl)
    {
      if (flags & MC_ACTIVE && !(flags & MC_INACTIVE))
      {
        c = color2; tc = globals->col_green;
        shipimage = pl->get_ship(1);
      }
      else
      {
        shipimage = pl->get_ship(0);
      }
    }
    else shipimage = NULL;

    rect(v, (int)x, (int)y, (int)x+w, (int)y+h-theight-1, c);

    if (shipimage && !dead)
      rotate_sprite(v, shipimage, int(x+w/2-shipimage->w/2), int(y+(h-theight-1)/2-shipimage->h/2), ftofix(rot));

    if (mode == 1)
    {
      text_mode(-1);

      int l;
      if (pl) l = strlen(pl->get_ship_name());
      else l = 3;
      int tlen = 0;
  
      if (l > 0)
      {
        char text2[10]; strcpy(text2, "");
        if (l > 8)
        {
          strncpy(text2, pl->get_ship_name(), 8);
          text2[8] = '.';
          text2[9] = '\0';
        }
        else
        {
          if (pl) strcpy(text2, pl->get_ship_name());
          else strcpy(text2, "n/a");
        }
  
        tlen = text_length(f, text2);
        aatextout(v, f, text2, (int)x+w/2-tlen/2, (int)y+h-theight+1, tc);
      }
    }

    clearlist->add(v, x, y, w, h);
  }
}

int mShipIcon2::update()
{
  rot++;
  if (rot > 256) rot = 1;

  return (mControl::update());
}

void mShipIcon2::do_explosion()
{
  dead = 1;
  del_flag(MC_ACTIVE);
  set_flag(MC_INACTIVE);

  mpixels->add(400, int(x+w/2), int(y+(h-22)/2),
               4, 4, 0, 0, 25,
               globals->tbl_all, 256);
}

