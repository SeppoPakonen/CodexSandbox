/*
 *   GRAVITY STRIKE -- menu/controls/mweapic.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"
#include "mweapic.h"
#include "../../gsclear.h"
#include "../../gsbullet.h"
#include "../../gsglob.h"
#include "../../gslng.h"
#include "../../gshelper.h"
#include "../gsmd.h"

void mWeaponIcon::create(int px, int py, int dx, int dy, int pw, int ph, int weap)
{
  mControl::create(px, py, dx, dy);
  w = pw;
  h = ph;
  weapon = weap;
  slot = -1;
}

void mWeaponIcon::draw(BITMAP *v)
{
  if (!(flags & MC_INVISIBLE))
  {
    int c = color1;
    if (flags & MC_ACTIVE) c = color2;

//    rect(v, (int)x, (int)y, (int)x+w, (int)y+h, c);

    BITMAP *tmp;

    switch (weapon)
    {
      case W_SINGLE   : tmp = (BITMAP *)globals->menudat[w_sblaster].dat; break;
      case W_DOUBLE   : tmp = (BITMAP *)globals->menudat[w_dblaster].dat; break;
      case W_BOMB     : tmp = (BITMAP *)globals->menudat[w_bomb].dat; break;
      case W_BIGBOMB  : tmp = (BITMAP *)globals->menudat[w_bigbomb].dat; break;
      case W_ROCKET   : tmp = (BITMAP *)globals->menudat[w_rockets].dat; break;
      case W_MINE     : tmp = (BITMAP *)globals->menudat[w_mines].dat; break;
      case W_SHRINKER : tmp = (BITMAP *)globals->menudat[w_shrinker].dat; break;
      case W_FREEZER  : tmp = (BITMAP *)globals->menudat[w_freezer].dat; break;
      case W_PROBE    : tmp = (BITMAP *)globals->menudat[w_probe].dat; break;
      case W_GAS      : tmp = (BITMAP *)globals->menudat[w_gas].dat; break;
      case W_LASER    : tmp = (BITMAP *)globals->menudat[w_laser].dat; break;
      case W_BUMPER   : tmp = (BITMAP *)globals->menudat[w_bumper].dat; break;
      case W_SHOTGUN  : tmp = (BITMAP *)globals->menudat[w_shotgun].dat; break;
      case W_ACID     : tmp = (BITMAP *)globals->menudat[w_acid].dat; break;
      default         : tmp = (BITMAP *)globals->menudat[w_sblaster].dat;
    }

    draw_sprite( v, tmp, (int)x, (int)y );

    FONT *f = (FONT*)globals->fontdat[FONT_IMPACT10].dat;

    // draw weapon stats
    if (get_flags() & MC_ACTIVE && weapon >= 0)
    {
      char otext[500], wname[50], wshield[20], whull[20], wsp[20];
      char *text = language->get_string(TEXT_STRINGS(T_MENU_SHIP_WI_SBLASTER+weapon));
      strncpy(otext, text, 499);
      
      char *curtok = NULL;

      memset(wname, 0, sizeof(wname));
      memset(wshield, 0, sizeof(wshield));
      memset(whull, 0, sizeof(whull));
      memset(wsp, 0, sizeof(wsp));

      // name
      curtok = strtok(otext, "#");
      if (curtok) strncpy(wname, curtok, 49);
      mytextout_left(v, f, wname, 150, 300, 330-15, globals->col_yellow);

      // strength
      curtok = strtok(NULL, "#");
      if (curtok) strncpy(wsp, curtok, 19);
      mytextout_left(v, f, wsp, 505, 600, 360-15, globals->col_yellow);

      // shield
      curtok = strtok(NULL, "#");
      if (curtok) strncpy(wshield, curtok, 19);
      mytextout_left(v, f, wshield, 200, 300, 360-15, globals->col_yellow);
      
      // hull
      curtok = strtok(NULL, "#");
      if (curtok) strncpy(whull, curtok, 19);
      mytextout_left(v, f, whull, 330, 400, 360-15, globals->col_yellow);
      
      // description
      curtok = strtok(NULL, "#");
//      if (curtok) mytextout_center(v, f, curtok, 156, 575, 377, globals->col_yellow);
      if (curtok) mytextout_left(v, f, curtok, 150, 600, 377, globals->col_yellow);
    }

    clearlist->add(v, (int)x, (int)y, w, h);
  }
}


