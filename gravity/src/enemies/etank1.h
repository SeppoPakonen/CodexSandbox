/*
 *   GRAVITY STRIKE -- enemies/etank1.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _E_TANK_
#define _E_TANK_

#include "../gsenemy.h"

class enemyTank1 : public Enemy
{
  private:
    float user[6];                                // user values of packship
    int etype;                                    // type of packed enemy

  public:
    enemyTank1(int x, int y, float penemy=E_TOWER_SINGLE, float pfreq=20, float pradius=100, float pspeed=3, float pstrength=1, float weaptype=0);
    ~enemyTank1();

    void init();
    void move();
    void update();
    void dead();
    void shoot();
    int  hit(long frompl, int fromen, float n, int x=0, int y=0, float fxspd=0, float fyspd=0, int weapon=-1);
    void set_maxhit(int n);
};

#endif

