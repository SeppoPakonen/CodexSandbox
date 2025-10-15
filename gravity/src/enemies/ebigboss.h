/*
 *   GRAVITY STRIKE -- enemies/ebigboss.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _E_SHIP_BIGBOSS_
#define _E_SHIP_BIGBOSS_

#include "../gsenemy.h"

class enemyShipBigBoss : public Enemy
{
  private:
    float rockturn_speed;                         // speed of rocket turn
    int f_add;                                    // frame add (update)

  public:
    enemyShipBigBoss(int x, int y, float pfreq=60, float pradius=800, float pspeed=3, float pstrength=5, float prockturn=0.03, float user6=0);
    ~enemyShipBigBoss();

    void move();
    void update();
    void dead();
    void shoot();
    int  hit(long frompl, int fromen, float n, int x=0, int y=0, float fxspd=0, float fyspd=0, int weapon=-1);
};

#endif

