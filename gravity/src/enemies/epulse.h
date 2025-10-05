/*
 *   GRAVITY STRIKE -- enemies/epulse.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _E_PULSE_TOWER_
#define _E_PULSE_TOWER_

#include "../gsenemy.h"

class enemyPulseTower : public Enemy
{
  private:

  public:
    enemyPulseTower(int x, int y, float pfreq=10, float pradius=150, float pspeed=3, float pstrength=1, float user5=0, float user6=0);
    ~enemyPulseTower();

    void move();
    void update();
    void dead();
    void shoot();
    int  hit(long frompl, int fromen, float n, int x=0, int y=0, float fxspd=0, float fyspd=0, int weapon=-1);
};

#endif

