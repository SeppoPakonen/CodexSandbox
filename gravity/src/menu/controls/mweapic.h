/*
 *   GRAVITY STRIKE -- menu/controls/mweapic.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _MWEAPONICON_
#define _MWEAPONICON_

#include "mcontrol.h"

class mWeaponIcon : public mControl
{
  private:
    int  weapon;                                   // weapon to display
    int  slot;                                     // slot weapon is in

  public:
    void set_int(int n)                            { slot = n; }
    int  get_int()                                 { return slot; }

    // functions
    void create(int x, int y, int dx, int dy, int w, int h, int weap); // create blank button
    void draw(BITMAP *);                           // draw bar
};

#endif

