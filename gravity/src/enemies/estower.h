/*
 *   GRAVITY STRIKE -- enemies/estower.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _E_SINGLE_TOWER_
#define _E_SINGLE_TOWER_

#include "../gsenemy.h"

class enemySingleTower : public Enemy
{
  private:
    int direction;                                // 0=left, 1=right, 2=up, 3=down

  public:
    enemySingleTower(int x, int y, float direction=D_LEFT, float pfreq=20, float pradius=100, float pspeed=3, float pstrength=1, float weaptype=1);
    ~enemySingleTower();

    void move();
    void update();
    void dead();
    void shoot();
    int  hit(long frompl, int fromen, float n, int x=0, int y=0, float fxspd=0, float fyspd=0, int weapon=-1);
};

#endif

