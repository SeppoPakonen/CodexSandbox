/*
 *   GRAVITY STRIKE -- weapons/bfreezer.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _B_FREEZER_
#define _B_FREEZER_

#include "../gsbullet.h"

class bulletFreezer : public Bullet
{
  private:
    // variables
//    class BulletSprite *image;
    float orig_turn_speed;                         // original turn_speed
    float dest_xspd;                               // destination speed of bullet
    float dest_yspd;
    float turn_speed;                              // turn speed of rocket

    int search_target(int x, int y, long sig);

  public:
    bulletFreezer(class gsPlayer *,  int puser, int pupgrade);
    bulletFreezer(long psig, int enr, int px, int py, float pxspd, float pyspd, int pstrength, int pshield, int phull, float puser, int pupgrade);
    ~bulletFreezer();

    void move();
    int  check_collisions();
    int  check_map_collisions(class gsMap *);
    void draw(class gsMap *);
};

#endif

