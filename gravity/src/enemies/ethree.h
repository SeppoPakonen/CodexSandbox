/*
 *   GRAVITY STRIKE -- enemies/ethree.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _E_TOWER_THREE_
#define _E_TOWER_THREE_

#include "../gsenemy.h"

class enemyThreeTower : public Enemy
{
  private:

  public:
    enemyThreeTower(int x, int y, float pfreq=60, float pradius=200, float pspeed=3, float pstrength=1, float user5=0, float weaptype=1);
    ~enemyThreeTower();

    void move();
    void update();
    void dead();
    void shoot();
    int  hit(long frompl, int fromen, float n, int x=0, int y=0, float fxspd=0, float fyspd=0, int weapon=-1);
};

#endif

