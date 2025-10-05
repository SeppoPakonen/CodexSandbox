/*
 *   GRAVITY STRIKE -- weapons/bshotgun.h
 *
 *
 *   (c) Copyright 2001/2004 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _B_SHOTGUN_
#define _B_SHOTGUN_

#include "../gsbullet.h"

class bulletShotgun : public Bullet
{
  private:
    int mode;                                      // 0=single, 1=double
    long my_by_player;                            // for color
    float dxspd, dyspd;                            // for double blaster upgrade
    float dxspda, dyspda;

  public:
    bulletShotgun(class gsPlayer *,  int puser, int pupgrade, int pspawn);
    bulletShotgun(long psig, int enr, int px, int py, float pxspd, float pyspd, float pstrength, float pshield, float phull, float puser, int pupgrade, int pspawn);
    ~bulletShotgun();

    void move();
    void move(int notrail);
    int  check_collisions();
    int  check_map_collisions(class gsMap *);
    void draw(class gsMap *);
    void draw(BITMAP *);
};

#endif

