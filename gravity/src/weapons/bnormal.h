/*
 *   GRAVITY STRIKE -- weapons/bnormal.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _B_NORMAL_
#define _B_NORMAL_

#include "../gsbullet.h"

class bulletNormal : public Bullet
{
  private:
    int mode;                                      // 0=single, 1=double
    int my_by_player;                              // for color
    float dxspd, dyspd;                            // for double blaster upgrade
    float dxspda, dyspda;

  public:
    bulletNormal(class gsPlayer *,  int puser, int pupgrade);   // mode: 0 = middle, 1 = left, 2 = right
    bulletNormal(long psig, int enr, int px, int py, float pxspd, float pyspd, int pstrength, int pshield, int phull, float puser, int pupgrade);
    ~bulletNormal();

    void move();
    void move(int notrail);
    int  check_collisions();
    int  check_map_collisions(class gsMap *);
    void draw(class gsMap *);
    void draw(BITMAP *);
};

#endif

