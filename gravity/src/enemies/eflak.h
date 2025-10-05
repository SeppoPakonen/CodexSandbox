/*
 *   GRAVITY STRIKE -- enemies/eflak.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _E_FLAK_
#define _E_FLAK_

#include "../gsenemy.h"

class enemyFlak : public Enemy
{
  private:
    int shoot_random;                             // how long bullet lasts

  public:
    enemyFlak(int x, int y, float pshootrand=60, float pfreq=120, float pradius=100, float pspeed=3, float pstrength=1, float user6=0);
    ~enemyFlak();

    void move();
    void update();
    void dead();
    void shoot();
    int  hit(long frompl, int fromen, float n, int x=0, int y=0, float fxspd=0, float fyspd=0, int weapon=-1);
};

#endif

