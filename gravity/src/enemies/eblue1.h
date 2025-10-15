/*
 *   GRAVITY STRIKE -- enemies/eblue1.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _E_SHIP_BLUE1_
#define _E_SHIP_BLUE1_

#include "../gsenemy.h"

class enemyShipBlue1 : public Enemy
{
  private:
    int f_add;                                    // frame add (update)

  public:
    enemyShipBlue1(int x, int y, float pfreq=80, float pradius=100, float pspeed=3, float pstrength=1, float ptargplay=0, float weaptype=1);
    ~enemyShipBlue1();

    void move();
    void update();
    void dead();
    void shoot();
    int  hit(long frompl, int fromen, float n, int x=0, int y=0, float fxspd=0, float fyspd=0, int weapon=-1);
};

#endif

